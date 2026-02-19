/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/02/19 23:51:44 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(t_command *cmd, int *ptr_total)
{
	int	**pipes;
	int	total;
	int	i;

	i = -1;
	total = 0;
	while (cmd)
	{
		cmd = cmd->next;
		total++;
	}
	pipes = malloc(sizeof(int *) * total);
	if (!pipes)
		return (NULL);
	while (++i < total - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			free_pipes(pipes, -1);
			return (NULL); // Handle perror's string
		}
	}
	pipes[total - 1] = NULL;
	*ptr_total = total;
	return (pipes);
}

char	*try_access(t_command *cmd)
{
	char 	*path;
	char	*partial_path;
	char	**paths;
	int		i;

	i = 0;
	if (access(cmd->command[0], X_OK))
		return (cmd->command[0]);
	paths = ft_split(getenv("PATH"), ':');
	while (paths && path[i])
	{
		partial_path = ft_strjoin(paths[i], "/");
		if (!partial_path)
			return (free_strs(paths), NULL);
		path = ft_strjoin(partial_path, cmd->command[0]);
		free(partial_path);
		if (!path)
			return (free_strs(paths), NULL);
		if (access(path, X_OK) == 0)
			return (free_strs(paths), path);
		free(path);
		i++;
	}
	return (NULL);
}

void	prepare_and_execute(t_command *cmd, t_list **envp, int **pipes, int pair[2]) //pair (total and id)
{
	char	*route;
	char	**real_envp;
	(void) envp;
	real_envp = NULL;
	piping(pipes, pair[0], pair[1]);
	free_pipes(pipes, pair[1] - 1);
	redirecting(cmd);
	route = try_access(cmd);
	if (!route)
		free_cmd(NULL, cmd, STOP, "");
	//real_envp = t_list_to_char(envp);
	if (execve(route, cmd->command, real_envp) == -1)
	{
		free_cmd(NULL, cmd, STOP, "execve"); //Check bash for convention
		exit(127);
	}
}

int	exec_command(t_command *cmd, t_list **envp, struct sigaction sa[4])
{
	int		**pipes;
	pid_t	*ids;
	int		curr_and_total[2];

	pipes = create_pipes(cmd, &curr_and_total[0]);
	if (!pipes)
		return (-1); // perror pipe failure
	ids = malloc(sizeof(pid_t) * curr_and_total[0]);
	if (!ids)
		return (-2); // perror malloc failure
	curr_and_total[1] = 0;
	while (curr_and_total[1] < curr_and_total[0])
	{
		ids[curr_and_total[1]] = fork();
		if (ids[curr_and_total[1]] < 0)
		{
			// handle_failure
		}
		else if (ids[curr_and_total[1]] == 0)
		{
			free(ids);
			sigaction(SIGINT, &sa[1], NULL);
			sigaction(SIGQUIT, &sa[3], NULL);
			prepare_and_execute(cmd, envp, pipes, curr_and_total); // handle pipes then redirections and exec.
			break ;
		}
		curr_and_total[1]++;
	}
	return (1);
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
	if (!*line)
	{
		free(line);
		return (0);
	}
	add_history(line);
	return (1);
}
