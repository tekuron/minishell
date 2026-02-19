/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:46:29 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/19 23:23:14 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirecting(t_command *cmd)
{
	t_io	*aux;
	int		fd;
	int		dup_out;
	
	aux = cmd->redirs;
	while(aux)
	{
		if (aux->rd == REDIR_IN)
			fd = open(aux->path, O_RDONLY);
		else if (aux->rd == REDIR_OUT)
			fd = open(aux->path, O_CREAT | O_WRONLY | O_TRUNC);
		else if (aux->rd == REDIR_APPEND)
			fd = open (aux->path, O_CREAT | O_WRONLY | O_APPEND);
		if (fd < 0)
		{
			//handle
		}
		if (aux->rd != REDIR_HEREDOC)
		{
			dup_out = dup2(fd, (aux->rd == REDIR_OUT || aux->rd == REDIR_APPEND));
			close(fd);
		}
		if (dup_out == -1)
		{
			//handle
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
