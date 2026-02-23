/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:46:29 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/23 16:50:17 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
extern volatile sig_atomic_t g_sig;

int	write_to_pipe(int pipes[2], t_command *cmd, int lines_num)
{
	char	*line;
	int		length;
	
	line = readline(">");
	if (!line)
	{
		printf("warning: here-document at line\
		%i delimited by end-of-file (wanted '%s')\n", lines_num, cmd->redirs->path);
		close(pipes[1]);
		return (0);
	}
	length = ft_strlen(cmd->redirs->path);
	if (ft_strncmp(cmd->redirs->path, line, length + 1) == 0)
	{
		close(pipes[1]);
		free(line);
		return (0);
	}
	write(pipes[1], line, ft_strlen(line));
	write(pipes[1], "\n", 1);
	free(line);
	close(pipes[1]);
	return (1);
}

int	heredoc_handling(t_command *cmd)
{
	struct sigaction	sa[2];
	int					pipes[2];
	int					line;

	initialize_signals(sa, 2);
	sa[0].sa_handler = s_int_handler_heredoc;
	while (cmd)
	{
		if (!cmd->redirs)
		{
			cmd = cmd->next;
			continue ;
		}
		cmd->redirs->heredoc_fd = -1;
		line = 0;
		if (cmd->redirs->rd == REDIR_HEREDOC)
		{
			sigaction(SIGINT, &sa[0], &sa[1]);
			pipe(pipes);
			cmd->redirs->heredoc_fd = pipes[0];
			while(!g_sig && write_to_pipe(pipes, cmd, ++line));
			sigaction(SIGINT, &sa[1], NULL);
			if (g_sig)
			{
				g_sig = 0;
				//close_pipes
				return (0);
			}
		}
		cmd = cmd->next;
	}
	return (1);
}

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
			free_pipes(pipes, -1);
			return (NULL); // Handle perror's string
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
	while(aux)
	{
		if (aux->rd == REDIR_IN)
			fd = open(aux->path, O_RDONLY);
		else if (aux->rd == REDIR_OUT)
			fd = open(aux->path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (aux->rd == REDIR_APPEND)
			fd = open (aux->path, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else if (aux->rd == REDIR_HEREDOC)
			fd = cmd->redirs->heredoc_fd;
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
