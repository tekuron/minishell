/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:46:29 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/18 23:35:26 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	redirecting(t_command *cmd)
{
	t_io	*aux;
	
	aux = cmd->redirs;
	while(aux)
	{
		if (aux->rd == REDIR_IN)
		{
			//Handle all cases
		}
		else if(aux->rd == REDIR_OUT)
		{

		}
		else if (aux->rd == REDIR_APPEND)
		{

		}
		else if (aux->rd == REDIR_HEREDOC)
		{
			
		}
		aux = aux->next;
	}
}

void	piping(int **pipes, int total, int id)
{
	if (id != 0)
	{
		if (dup2(pipes[id - 1][0], STDIN_FILENO) == -1)
		{
			//handle_failure();
		}
	}
	if (id != total - 1)
	{
		if (dup2(pipes[id - 1][1], STDOUT_FILENO) == -1)
		{
			//handle_failure();
		}
	}
}
