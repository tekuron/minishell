/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/02/18 17:03:56 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_cmd(char *line, t_command *cmd, int cont, char *err)
{
	if (line != NULL)
		free(line);
	if (line != NULL)
		t_command_free(cmd);
	if (!cont)
	{
		perror(err);
		exit(EXIT_FAILURE);
	}
}


int	exec_command(t_command *cmd, char *envp)
{
	pid_t	*ids;
	int		*pipes;

	(void) cmd;
	(void) envp;
	(void) pipes;
	(void) ids;
	/*
	Create pipes (n - 1)
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
	return (1);
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
