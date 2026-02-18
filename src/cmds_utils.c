/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 20:14:28 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/18 20:26:18 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void free_pipes(int **pipes, int total)
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
	if (!cont && err)
	{
		perror(err);
		exit(EXIT_FAILURE);
	}
	if (!cont)
		exit(0);
}