/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/03/29 10:59:16 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dir(char *path)
{
	struct stat sb;

	if (!path)
		return (0);
	if (stat(path, &sb) == -1)
		return (-1);
	return ((sb.st_mode & S_IFMT) == S_IFDIR);
}

void	failure_handling(t_list **envp, t_command *cmd, char *route, int phase)
{
	int	eof_case;

	eof_case = !cmd->command || !*cmd->command;
	if (phase == 2)
	{
		if (is_dir(cmd->command[0]) == 1 || !ft_strncmp(cmd->command[0], ".", 2)
			|| !ft_strncmp(cmd->command[0], "..", 3))
			printf("Permision denied: %s\n", cmd->command[0]);
		else
			printf("minishell: %s: command not found...\n", cmd->command[0]); //Fix permission issues
	}
	free_cmd(&route, cmd, CONT, NULL);
	ft_lstclear(envp, free);
	if (phase == 1)
	{
		if (eof_case)
			exit (EXIT_SUCCESS);
		else
			exit(EXIT_FAILURE);
	}
	else
		exit(127);
}

void	run_command_bi(t_command *cmd, t_shell *shell)
{
	t_pair	pair;

	pair.cont = try_builtin(cmd, shell, &pair.status, MULTIPLE);
	if (pair.cont > 0)
	{
		ft_lstclear(shell->envp, free);
		free_cmd(NULL, cmd, CONT, NULL);
		exit(pair.status);
	}
}

void	handle_child(t_process *data, t_shell *shell, int total)
{
	char	*route;
	char	**real_envp;

	if (!piping(data->pipes, total, data->process) || !redirecting(data->cmd)
		|| !data->cmd->command || !*data->cmd->command)
	{
		free_pipes(data->pipes, total - 1);
		failure_handling(shell->envp, data->cmd, NULL, 1);
	}
	free_pipes(data->pipes, total - 1);
	run_command_bi(data->cmd, shell);
	route = try_access(data->cmd, shell);
	if (!route)
		failure_handling(shell->envp, data->cmd, NULL, 2);
	real_envp = t_list_to_char(*shell->envp);
	if (execve(route, data->cmd->command, real_envp) == -1)
	{
		free_strs(real_envp);
		failure_handling(shell->envp, data->cmd, route, 3);
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
			set_signals(EXECUTION);
			handle_child(data, shell, total);
		}
		data->cmd = data->cmd->next;
		i++;
	}
	data->process = total;
	free_pipes(data->pipes, total - 1);
	return (1);
}

void	change_exit(t_list *envp, int exit_status)
{
	char	last_exit[10];
	int		i;

	i = 0;
	if (!envp)
		return ;
	if (exit_status == 0)
	{
		ft_memmove(envp->content, (void *)"?=0\0", 4);
		return ;
	}
	while (exit_status != 0 && i < 10)
	{
		last_exit[i] = exit_status % 10 + '0';
		exit_status /= 10;
		i++;
	}
	last_exit[i] = '\0';
	ft_swap(last_exit, i);
	ft_memmove((void *)((char *)envp->content + 2), (void *)last_exit, i);
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

int	exec_command(t_command *cmd, t_shell *shell)
{
	t_process	data;
	t_pair		pair;
	
	pair.cont = heredoc_handling(cmd, shell);
	if (!pair.cont)
		return (130);
	pair.cont = try_builtin(cmd, shell, &pair.status, SINGLE);
	if (pair.cont > 0)
		return (pair.status);
	data = (t_process){0};
	data.cmd = cmd;
	data.process = t_command_size(cmd);
	data.pipes = create_pipes(data.process - 1);
	data.ids = malloc(sizeof(pid_t) * data.process);
	pair.status = errors_detected(data);
	if (pair.status < 0)
		return (pair.status); //handle with perror
	if (!forking(shell, &data, data.process))
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
