/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/02/28 13:13:48 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sig;

void	handle_child(t_process *data, t_list *envp, int total)
{
	char	*route;
	char	**real_envp;

	if (!piping(data->pipes, total, data->process) || !redirecting(data->cmd))
	{
		free_pipes(data->pipes, total);
		free(data->ids);
		ft_lstclear(&envp, free);
		free_cmd(NULL, data->cmd, STOP, "minishell");
	}
	free_pipes(data->pipes, total - 1);
	route = try_access(data->cmd, envp->next);
	if (!route)
	{
		printf("minishell: %s: command not found...\n", data->cmd->command[0]);
		free_cmd(NULL, data->cmd, CONT, NULL);
		ft_lstclear(&envp, free);
		exit(127);
	}
	real_envp = t_list_to_char(envp->next);
	if (execve(route, data->cmd->command, real_envp) == -1)
	{
		free_strs(real_envp);
		ft_lstclear(&envp, free);
		free_cmd(route, data->cmd, CONT, "execve");
		exit(127);
	}
}

int	forking(t_list *envp, t_process *data, int total)
{
	int	i;

	i = 0;
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
	ft_memmove((void *)((char *)envp->content + 2), (void *)last_exit, i + 1); // ?= +2 => Status\0
}

int	wait_for_children(t_process *data, t_list *envp)
{
	int	i;
	int	status;
	int	last_exit;

	i = -1;
	last_exit = -1;
	while (++i < data->process)
		waitpid(data->ids[i], &status, 0);
	free(data->ids);
	if (WIFEXITED(status))
		last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		last_exit = 128 + WTERMSIG(status);
	change_exit(envp, last_exit);
	return (last_exit);
}

int	exec_command(t_command *cmd, t_list *envp)
{
	t_process	data;
	int			ret_value;
	
	ret_value = heredoc_handling(cmd, envp);
	if (!ret_value)
		return (130);
	ret_value = try_builtin(cmd, envp);
	if (ret_value)
		return (ret_value);
	data = (t_process){0};
	data.cmd = cmd;
	data.process = t_command_size(cmd);
	data.pipes = create_pipes(data.process - 1);
	if (!data.pipes)
		return (-1); //handle with perror
	data.ids = malloc(sizeof(pid_t) * data.process);
	if (!data.ids)
		return (-2); //handle with perror and free pipes
	forking(envp, &data, data.process);
	return (wait_for_children(&data, envp));
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
