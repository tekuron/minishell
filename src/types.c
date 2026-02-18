/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:27:15 by danz              #+#    #+#             */
/*   Updated: 2026/02/18 18:52:42 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	t_command_free(t_command *cmd)
{
	t_command	*cur;
	t_io		*cur_io;

	cur = cmd;
	while (cur)
	{
		free_strs(cmd->command);
		cur_io = cmd->redirs;
		while (cur_io)
		{
			free(cur_io->path);
			cur_io = cur_io->next;
			free(cmd->redirs);
			cmd->redirs = cur_io;
		}
		cur = cur->next;
		free(cmd);
		cmd = cur;
	}
}

void	t_command_append(t_command *top, t_command *new)
{
	while (top->next)
		top = top->next;
	top->next = new;
	top->pipe = TRUE;
}

t_command	*t_command_new(char **cmd)
{
	t_command	*ret;

	ret = malloc(sizeof(t_command));
	if (!ret)
		return (NULL);
	ret->command = cmd;
	ret->redirs = NULL;
	ret->next = NULL;
	ret->pipe = FALSE;
	return (ret);
}

void	t_io_append(t_io *top, t_io *new)
{
	while (top->next)
		top = top->next;
	top->next = new;
}

t_io	*t_io_new(char *path, t_redir rd)
{
	t_io	*ret;

	ret = malloc(sizeof(t_io));
	if (!ret)
		return (NULL);
	ret->next = NULL;
	ret->path = path;
	ret->rd = rd;
	return (ret);
}
