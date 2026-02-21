/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/02/21 18:53:33 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
extern volatile sig_atomic_t g_sig;

void	handle_child(t_command *cmd, t_list *envp, int **pipes, int pair[2]) //pair (total and id)
{
	char	*route;
	char	**real_envp;
	(void) envp;

	piping(pipes, pair[0], pair[1]);
	free_pipes(pipes, pair[1] - 1);
	redirecting(cmd);
	route = try_access(cmd);
	if (!route)
		free_cmd(NULL, cmd, STOP, "");
	real_envp = t_list_to_char(envp);
	if (execve(route, cmd->command, real_envp) == -1)
	{
		free_cmd(NULL, cmd, STOP, "execve"); //Check bash for convention
		exit(127);
	}
}

pid_t	*forking(t_command *cmd, t_list *envp, struct sigaction sa[4], int *total)
{
	pid_t	*ids;
	int		**pipes;
	int		total_and_id[2];
	
	pipes = create_pipes(cmd, &total_and_id[0]);
	if (!pipes)
		return (NULL); //handle faillure and perror pipes
	ids = malloc(sizeof(pid_t) * total_and_id[0]);
	if (!ids)
		return (NULL); //free and close pipes and handle malloc failure
	total_and_id[1] = -1;
	while (++total_and_id[1] < total_and_id[0])
	{
		ids[total_and_id[1]] = fork();
		if (ids[total_and_id[1]] < 0)
		{
			//return (NULL, handle_failure(ids, total_and_id, total, pipes));
		}
		else if (ids[total_and_id[1]] == 0)
		{
			clean_and_set(ids, sa);
			handle_child(cmd, envp, pipes, total_and_id);
		}
	}
	free_pipes(pipes, total_and_id[0] - 1);
	*total = total_and_id[0];
	return (ids);
}


int	exec_command(t_command *cmd, t_list *envp, struct sigaction sa[4])
{
	pid_t	*ids;
	int		i;
	int		total;
	int		status;
	
	heredoc_handling(cmd);
	try_builtin(cmd, envp);
	ids = forking(cmd, envp, sa, &total);
	if (!ids)
	{
		//handle_failure, use total to set err_code
	}
	i = 0;
	while (i < total)
	{
		waitpid(ids[i], &status, 0);
		i++;
	}
	free(ids);
	if(WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (-1);
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
