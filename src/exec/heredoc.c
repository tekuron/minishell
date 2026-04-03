/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 12:45:29 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/03 18:01:46 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	calculate_chars(char *str)
{
	int	i;

	i = 0;
	if (str[i + 1] && str[i + 1] == '?')
		return (2);
	else
	{
		while (str[i] && !ft_isspace(str[i]) && str[i] != '$')
			i++;
	}
	return (i);
}

static void	write_expansion(char *content, int pipe_fd, t_shell *shell)
{
	char	*expansion;
	int		i;

	i = 0;
	while (content[i])
	{
		while (content[i] && content[i] != '$')
			write(pipe_fd, &content[i++], 1);
		if (content[i] == '$')
		{
			if ((content[i + 1] && ft_isspace(content[i + 1]))
				|| !content[i + 1])
				write(pipe_fd, &content[i++], 1);
			else
			{
				expansion = ft_getenv(content + i, shell);
				if (expansion)
					write(pipe_fd, expansion, ft_strlen(expansion));
				i += calculate_chars(content + i + 1) + 1;
			}
		}
	}
}

static int	write_to_pipe(int pipes[2], t_io *redir, int curr, t_shell *shell)
{
	char	*line;
	int		length;

	line = readline(">");
	if (!line)
	{
		printf("warning: here-document at line\
 %i delimited by end-of-file (wanted '%s')\n", curr, redir->path);
		return (0);
	}
	length = ft_strlen(redir->path);
	if (ft_strncmp(redir->path, line, length + 1) == 0)
	{
		free(line);
		return (0);
	}
	if (redir->has_qts)
		write(pipes[1], line, ft_strlen(line));
	else
		write_expansion(line, pipes[1], shell);
	write(pipes[1], "\n", 1);
	free(line);
	return (1);
}

static int	prepare_heredoc(t_io *redir, t_shell *shell)
{
	int	line;
	int	pipes[2];

	line = 0;
	set_signals(HEREDOC);
	pipe(pipes);
	redir->heredoc_fd = pipes[0];
	while (!get_signal_status() && write_to_pipe(pipes, redir, line++, shell))
		(void) line;
	close(pipes[1]);
	if (get_signal_status())
	{
		set_signal_status(0);
		set_signals(SHELL);
		close(pipes[0]);
		return (0);
	}
	set_signals(SHELL);
	return (1);
}

int	heredoc_handling(t_command *cmd, t_shell *shell)
{
	t_io	*redir;

	while (cmd)
	{
		redir = cmd->redirs;
		if (!redir)
		{
			cmd = cmd->next;
			continue ;
		}
		while (redir)
		{
			if (redir->rd == REDIR_HEREDOC)
			{
				if (!prepare_heredoc(redir, shell))
					return (0);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (1);
}
