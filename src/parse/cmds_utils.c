/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 20:14:28 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/24 10:32:03 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_pipes(int **pipes, int total)
{
	int	i;

	i = 0;
	if (total != -1)
	{
		while (i < total)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
	}
	i = 0;
	while (pipes && pipes[i])
		free(pipes[i++]);
	free(pipes);
}

void	free_cmd(char *line, t_command *cmd, int cont, char *err)
{
	if (line != NULL)
		free(line);
	if (cmd != NULL)
		t_command_free(cmd);
	if (err && cont == STOP)
	{
		perror(err);
		exit(EXIT_FAILURE);
	}
	else if (cont == STOP)
		exit(EXIT_SUCCESS);
}
