/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/02/18 19:07:38 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_cmd(char *line, t_command *cmd, int cont, char *err)
{
	if (line != NULL)
		free(line);
	if (cmd != NULL)
		t_command_free(cmd);
	if (!cont && err)
	{
		perror(err);
		exit(EXIT_FAILURE);
	}
	if (!cont)
		exit(0);
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
