/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 20:14:28 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/08 18:44:00 by dplazas-         ###   ########.fr       */
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
