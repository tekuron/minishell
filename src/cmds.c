/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/02/22 21:48:36 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
extern volatile sig_atomic_t g_sig;

void	handle_child(t_process *data, t_list *envp, int total) //pair (total and id)
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
	route = try_access(data->cmd);
	if (!route)
		free_cmd(NULL, data->cmd, STOP, "");
	real_envp = t_list_to_char(envp->next);
	if (execve(route, data->cmd->command, real_envp) == -1)
	{
		free_strs(real_envp);
		free_cmd(route, data->cmd, STOP, "execve"); //Check bash for convention
		exit(127);
	}
}

int forking(t_list *envp, t_process *data, int total)
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
			sigaction(SIGINT, &data->old_sigint, NULL);
			sigaction(SIGQUIT, &data->old_sigquit, NULL);
			handle_child(data, envp, total);
		}
		data->cmd = data->cmd->next;
		i++;
	}
	data->process = total;
	free_pipes(data->pipes, data->process - 1);
	return (1);
}

int	wait_for_children(t_process *data)
{
	int	i;
	int	status;
	
	i = 0;
	while (i < data->process)
	{
		waitpid(data->ids[i], &status, 0);
		i++;
	}
	free(data->ids);
	if(WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (-1);

}

int	exec_command(t_command *cmd, t_list *envp, struct sigaction sa[4])
{
	t_process	data;
	
	data = (t_process){0};
	data.old_sigint = sa[1];
	data.old_sigquit = sa[3];
	data.cmd = cmd;
	data.process = t_command_size(cmd);
	data.pipes = create_pipes(data.process - 1);
	if (!data.pipes)
		return (-1); //handle with perror
	data.ids = malloc(sizeof(pid_t) * data.process);
	if (!data.ids)
		return (-2); //handle with perror and free pipes
	heredoc_handling(data.cmd);
	try_builtin(data.cmd, envp);
	forking(envp, &data, data.process);
	return (wait_for_children(&data));
}

/*
pipes()
Fork:
	Child:
		-Set signals (SIGINT and SIGQUT) to default
		-Handle the pipes
		-Apply proper redirections
		-Execve
	Parent:
		-Close fds
Wait
Restore signals	
*/

int	append_to_history(char *line)
{
	if (!line)
	{
		printf("exit\n");
		exit(0);
	}
	else if (!*line)
	{
		free(line);
		return (0);
	}
	add_history(line);
	return (1);
}
