/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/04/03 10:58:28 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dir(char *path)
{
	struct stat	sb;

	if (!path)
		return (0);
	if (stat(path, &sb) == -1)
		return (-1);
	return ((sb.st_mode & S_IFMT) == S_IFDIR);
}

void	free_and_exit(t_list **envp, t_command *cmd, int err, int exit_code)
{
	ft_lstclear(envp, free);
	free_cmd(NULL, cmd, CONT, NULL);
	if (err)
		perror("minishell");
	rl_clear_history();
	prompt(exit_code, EXIT_SHELL);
	exit(exit_code);
}

void	execution_message(t_list **envp, t_process *data, t_command *cmd)
{
	if (!ft_strncmp(cmd->command[0], ".", 2))
	{
		printf("minishell: .: filename argument required\n");
		printf(".: usage . filename [arguments]\n");
		free_and_exit(envp, data->cmd, 0, 2);
	}
	else if (!ft_strncmp(cmd->command[0], "..", 3))
		free_and_exit(envp, data->cmd, 0, 127);
	else if (is_dir(cmd->command[0]) == 1)
	{
		write(1, "minishell: ", 12);
		write(1, cmd->command[0], ft_strlen(cmd->command[0]));
		write(1, ": Is a directory\n", 18);
	}
	else if (access(cmd->command[0], F_OK) == 0 && access(cmd->command[0], X_OK) < 0)
	{
		perror(cmd->command[0]);
		free_and_exit(envp, data->cmd, 0, 126);
	}
	else
		printf("minishell: %s: command not found\n", cmd->command[0]);
	free_and_exit(envp, data->cmd, 0, 127);
}

void	run_command_bi(t_process *data, t_shell *shell, t_command *cmd)
{
	t_pair		pair;

	pair.cont = try_builtin_child(cmd ,shell, &pair.status);
	if (pair.cont > 0)
		free_and_exit(shell->envp, data->cmd, 0, pair.status);
}

void	handle_child(t_process *data, t_shell *shell, int total)
{
	char	*route;
	char	**real_envp;
	int		err;
	t_command *cmd;

	cmd = t_command_index(data->cmd, data->process);
	err = !piping(data->pipes, total, data->process) || !redirecting(cmd);
	free_pipes(data->pipes, total - 1);
	if (err || !data->cmd->command || !*data->cmd->command)
		free_and_exit(shell->envp, data->cmd, err, err);
	run_command_bi(data, shell, cmd);
	route = try_access(cmd, shell);
	if (!route)
		execution_message(shell->envp, data, cmd);
	real_envp = t_list_to_char(*shell->envp);
	if (execve(route, cmd->command, real_envp) == -1)
	{
		free_strs(real_envp);
		free(route);
		free_and_exit(shell->envp, data->cmd, 1, 127);
	}
}

void	close_heredocs(t_command *cmd, int number)
{
	t_io	*redir;
	int		i;

	i = 0;
	while (cmd)
	{
		if (cmd->redirs && number != i)
		{
			redir = cmd->redirs;
			while (redir)
			{
				if (redir->rd == REDIR_HEREDOC)
					close(redir->heredoc_fd);
				redir = redir->next;
			}
		}
		i++;
		cmd = cmd->next;
	}
}

int	forking(t_shell *shell, t_process *data, int total)
{
	int	i;

	i = 0;
	set_signals(IGNORE);
	while (i < total)
	{
		data->process = i;
		data->ids[i] = fork();
		if (data->ids[i] < 0)
			return (0);
		if (data->ids[i] == 0)
		{
			free(data->ids);
			close_heredocs(data->cmd, i);
			set_signals(EXECUTION);
			//data->cmd = find_command(data->cmd, i);
			handle_child(data, shell, total);
		}
		i++;
	}
	data->process = total;
	free_pipes(data->pipes, total - 1);
	return (1);
}

int	wait_for_children(t_process *data)
{
	int	i;
	int	status;
	int	last_exit;

	i = -1;
	last_exit = -1;
	while (++i < data->process)
	{
		while (waitpid(data->ids[i], &status, 0) == -1 && errno == EINTR);
	}
	free(data->ids);
	if (WIFEXITED(status))
		last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		last_exit = 128 + WTERMSIG(status);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(1, "Quit\n", 5);
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	set_signals(SHELL);
	return (last_exit);
}

int	errors_detected(t_process data)
{
	if (!data.pipes)
	{
		if (data.ids)
			free(data.ids);
		return (-1);
	}
	if (!data.ids)
	{
		if (data.pipes)
			free(data.pipes);
		return (-2);
	}
	return (1);
}

int	initialize_data(t_process *data, t_command *cmd)
{
	data->cmd = cmd;
	data->process = t_command_size(cmd);
	data->pipes = create_pipes(data->process - 1);
	if (!data->pipes)
	{
		close_heredocs(cmd, -1);
		return (-1);
	}
	data->ids = malloc(sizeof(pid_t) * data->process);
	if (!data->ids)
	{
		close_heredocs(cmd, -1);
		free_pipes(data->pipes, data->process - 1);
		return (-2);
	}
	return (1);
}


int	exec_command(t_command *cmd, t_shell *shell)
{
	t_process	data;
	t_pair		pair;
	
	pair.cont = heredoc_handling(cmd, shell);
	if (!pair.cont)
		return (130);
	pair.cont = try_builtin_parent(cmd, shell, &pair.status);
	if (pair.cont > 0)
		return (pair.status);
	data = (t_process){0};
	pair.status = initialize_data(&data, cmd);
	if (pair.status < 0)
		return (pair.status); //handle with perror
	pair.status = forking(shell, &data, data.process);
	close_heredocs(cmd, -1);
	if (!pair.status)
	{
		set_signals(SHELL);
		return (-3);
	}
	return (wait_for_children(&data));
}

int	append_to_history(char *line)
{
	if (!*line)
	{
		free(line);
		return (0);
	}
	add_history(line);
	return (1);
}
