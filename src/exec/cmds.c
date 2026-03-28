/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/03/28 10:03:35 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sig;

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
			printf("minishell: %s: command not found...\n", cmd->command[0]);
	}
	free_cmd(route, cmd, CONT, NULL);
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

int	run_command(char *route, t_command *cmd, t_list **envp, char **real_envp)
{
	t_pair	pair;

	pair.cont = try_builtin(cmd, envp, &pair.status, 2);
	if (pair.cont >= 0)
	{
		ft_lstclear(envp, free);
		free_cmd(route, cmd, CONT, NULL);
		free_strs(real_envp);
		exit(pair.status);
	}
	return (execve(route, cmd->command, real_envp));
}

void	handle_child(t_process *data, t_list **envp, int total)
{
	char	*route;
	char	**real_envp;

	if (!piping(data->pipes, total, data->process) || !redirecting(data->cmd)
		|| !data->cmd->command || !*data->cmd->command)
	{
		free_pipes(data->pipes, total - 1);
		failure_handling(envp, data->cmd, NULL, 1);
	}
	free_pipes(data->pipes, total - 1);
	route = try_access(data->cmd, (*envp)->next);
	if (!route)
		failure_handling(envp, data->cmd, NULL, 2);
	real_envp = t_list_to_char((*envp)->next);
	if (run_command(route, data->cmd, envp, real_envp) == -1)
	{
		free_strs(real_envp);
		failure_handling(envp, data->cmd, route, 3);
	}
}

int	forking(t_list **envp, t_process *data, int total)
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
			handle_child(data, envp, total);
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

int	wait_for_children(t_process *data, t_list *envp)
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
	change_exit(envp, last_exit);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(1, "Quit\n", 5);
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	set_signals(SHELL);
	return (last_exit);
}

int	exec_command(t_command *cmd, t_list **envp)
{
	t_process	data;
	t_pair		pair;
	
	pair.cont = heredoc_handling(cmd, *envp);
	if (!pair.cont)
		return (130);
	pair.cont = try_builtin(cmd, envp, &pair.status, 1);
	if (pair.cont >= 0)
		return (pair.status);
	data = (t_process){0};
	data.cmd = cmd;
	data.process = t_command_size(cmd);
	data.pipes = create_pipes(data.process - 1);
	if (!data.pipes)
		return (-1); //handle with perror
	data.ids = malloc(sizeof(pid_t) * data.process);
	if (!data.ids)
	{
		free_pipes(data.pipes, data.process - 1);
		return (-2);
	} //handle with perror and free pipes
	if (!forking(envp, &data, data.process))
	{
		set_signals(SHELL);
		return (-3);
	}
	return (wait_for_children(&data, *envp));
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
