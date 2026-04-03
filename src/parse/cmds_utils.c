/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 20:14:28 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/03 18:20:23 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*t_command_index(t_command *cmd, int index)
{
	int	i;

	i = 0;
	while (cmd && i < index)
	{
		cmd = cmd->next;
		i++;
	}
	return (cmd);
}

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

void	free_cmd(char **line, t_command *cmd, int cont, char *err)
{
	if (line != NULL)
	{
		if (line && *line)
		{
			free(*line);
			*line = NULL;
		}
	}
	if (cmd != NULL)
		t_command_free(cmd);
	if (err && cont == STOP)
	{
		perror(err);
		rl_clear_history();
		prompt(EXIT_SUCCESS, EXIT_SHELL);
		exit(EXIT_FAILURE);
	}
	else if (cont == STOP)
	{
		rl_clear_history();
		prompt(EXIT_SUCCESS, EXIT_SHELL);
		exit(EXIT_SUCCESS);
	}
	else if (err)
		perror(err);
}
