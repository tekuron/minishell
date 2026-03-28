/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:46:29 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/28 12:55:01 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_line(char *content, int pipe_fd, int expands, t_list *envp)
{
	int	i;
	char	*expansion;

	i = 0;
	if (!expands)
		write(pipe_fd, content, ft_strlen(content));
	else
	{
		while (content[i])
		{
			while (content[i] && content[i] != '$')
				write(pipe_fd, &content[i++], 1);
			if (content[i] == '$')
			{
				expansion = ft_getenv(content + i, envp);
				if (expansion)
					write(pipe_fd, expansion, ft_strlen(expansion));
			}
			while (content[i] && content[i] != ' ' 
					&& !(content[i] >= 9 && content[i] <= 13))
				i++;
		}
	}
	write(pipe_fd, "\n", 1);
}

int	write_to_pipe(int pipes[2], t_io *redir, int lines_num, t_list *envp)
{
	char	*line;
	int		length;

	line = readline(">");
	if (!line)
	{
		printf("warning: here-document at line\
 %i delimited by end-of-file (wanted '%s')\n", lines_num, redir->path);
		return (0);
	}
	length = ft_strlen(redir->path);
	if (ft_strncmp(redir->path, line, length + 1) == 0)
	{
		free(line);
		return (0);
	}
	write_line(line, pipes[1], !redir->has_qts, envp);
	free(line);
	return (1);
}


int	prepare_heredoc(int	pipes[2], t_io *redir, t_list *envp)
{
	int	line;

	line = 0;
	set_signals(HEREDOC);
	pipe(pipes);
	redir->heredoc_fd = pipes[0];
	while (!get_signal_status() && write_to_pipe(pipes, redir, line++, envp));
	if (get_signal_status())
	{
		set_signal_status(0);
		close(pipes[1]);
		set_signals(SHELL);
		return (0);
	}
	close(pipes[1]);
	set_signals(SHELL);
	return (1);
}

int	heredoc_handling(t_command *cmd, t_list *envp)
{
	int		pipes[2];
	t_io	*redir;

	while (cmd)
	{
		redir = cmd->redirs;
		if (!redir)
		{
			cmd = cmd->next;
			continue ;
		}
		redir->heredoc_fd = -1;
		while (redir)
		{
			if (redir->rd == REDIR_HEREDOC)
			{
				if (!prepare_heredoc(pipes, redir, envp))
					return (0);
			}
			redir = redir->next;
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
