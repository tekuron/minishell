/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_save.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 18:25:59 by danz              #+#    #+#             */
/*   Updated: 2026/02/18 22:57:53 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	get_redir(char *str)
{
	if (!ft_strncmp(str, "<", 2))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 2))
		return (REDIR_OUT);
	if (!ft_strncmp(str, "<<", 3))
		return (REDIR_HEREDOC);
	if (!ft_strncmp(str, ">>", 3))
		return (REDIR_APPEND);
	return (ERROR);
}

static int	get_redirs(t_list *cmd, t_command *ret)
{
	t_io	*new;
	t_redir	rd;

	rd = get_redir(cmd->content);
	if (rd == ERROR)
		return (1);
	cmd = cmd->next;
	new = t_io_new(ft_strdup(cmd->content), rd);
	if (ret->redirs)
		t_io_append(ret->redirs, new);
	else
		ret->redirs = new;
	return (0);
}

static t_command	*get_cmds(t_list *cmd, int i)
{
	t_command	*ret;
	char		**cur_content;
	int			j;

	cur_content = malloc((i + 1) * sizeof(char *));
	if (!cur_content)
		return (NULL);
	j = i;
	while (i && cmd)
	{
		if (is_redir(cmd->content) && cmd->next)
		{
			cmd = cmd->next->next;
			continue ;
		}
		if (!cmd || !strncmp(cmd->content, "|", 2))
			break ;
		cur_content[j - i] = ft_strdup(cmd->content);
		cmd = cmd->next;
		i--;
	}
	cur_content[j - i] = 0;
	ret = t_command_new(cur_content);
	return (ret);
}

int	save_cmds_mod(t_list *cmd, int i, t_list *cur, t_command *new)
{
	while (cmd && !is_redir(cmd->content) && i)
	{
		cmd = cmd->next;
		i--;
	}
	while (cmd && cmd != cur && i--)
	{
		if (cmd && is_redir(cmd->content))
		{
			if (get_redirs(cmd, new))
				return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}

t_command	*save_cmds(t_list *cmd, t_command *ret)
{
	t_command	*new;
	t_list		*cur;
	int			i;

	cur = cmd;
	i = 0;
	ret = 0;
	while (cur)
	{
		if (!ft_strncmp(cur->content, "|", 2) || cur->next == NULL)
		{
			new = get_cmds(cmd, i + (cur->next == NULL));
			if (ret)
				t_command_append(ret, new);
			else
				ret = new;
			if (save_cmds_mod(cmd, i + (cur->next == NULL), cur, new))
				return (t_command_free(ret), NULL);
			cmd = cur->next;
		}
		else
			i++;
		cur = cur->next;
	}
	return (ret);
}
