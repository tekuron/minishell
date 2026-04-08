/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:46:29 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/08 17:13:48 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(int total)
{
	int	**pipes;
	int	i;

	i = -1;
	pipes = malloc(sizeof(int *) * (total + 1));
	if (!pipes)
		return (NULL);
	while (++i < total)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			free_pipes(pipes, i);
			return (NULL);
		}
	}
	pipes[total] = NULL;
	return (pipes);
}

int	redirecting(t_command *cmd)
{
	t_io	*aux;
	int		fd;
	int		dup_out;

	aux = cmd->redirs;
	while (aux)
	{
		if (aux->rd == REDIR_IN)
			fd = open(aux->path, O_RDONLY);
		else if (aux->rd == REDIR_OUT)
			fd = open(aux->path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (aux->rd == REDIR_APPEND)
			fd = open (aux->path, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else if (aux->rd == REDIR_HEREDOC)
			fd = aux->heredoc_fd;
		if (fd < 0)
			return (0);
		dup_out = dup2(fd, (aux->rd == REDIR_OUT || aux->rd == REDIR_APPEND));
		close(fd);
		if (dup_out < 0)
			return (0);
		aux = aux->next;
	}
	return (1);
}

int	piping(int **pipes, int total, int id)
{
	if (total < 1)
		return (-1);
	if (id != 0)
	{
		if (dup2(pipes[id - 1][0], STDIN_FILENO) == -1)
			return (0);
	}
	if (id != total - 1)
	{
		if (dup2(pipes[id][1], STDOUT_FILENO) == -1)
			return (0);
	}
	return (1);
}
