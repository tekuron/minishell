/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_prep.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:24:40 by danz              #+#    #+#             */
/*   Updated: 2026/02/23 13:21:49 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes(char **text)
{
	size_t	i;
	size_t	j;
	char	*str;

	while (*text)
	{
		i = 0;
		str = *text;
		while (str[i])
		{
			if (str[i] == '\'' || str[i] == '\"')
			{
				j = i + 1;
				while (str[j] != str[i])
					j++;
				ft_memmove(&str[j], &str[j + 1], ft_strlen(&str[j]));
				ft_memmove(&str[i], &str[i + 1], ft_strlen(&str[i]));
				i = j - 1;
			}
			else
				i++;
		}
		text++;
	}
}

static t_list	*prep_cmd(char **wds, t_list *envp)
{
	t_list	*ret;

	ret = lst_from_char(wds);
	if (!ret)
		return (NULL);
	insert_env(&ret, envp);
	return (ret);
}

static void	all_quotes(t_command *lst)
{
	t_io 	*cur_io;
	char	*path_qts[2];

	path_qts[1] = NULL;
	while (lst)
	{
		remove_quotes(lst->command);
		cur_io = lst->redirs;
		while (cur_io)
		{
			path_qts[0] = cur_io->path;
			if (ft_strchr(path_qts[0], '\"') || ft_strchr(path_qts[0], '\''))
				cur_io->has_qts = TRUE;
			remove_quotes(path_qts);
			cur_io = cur_io->next;
		}
		lst = lst->next;
	}
}

t_command	*get_cmd(char *line, t_list *envp)
{
	t_command	*ret;
	char		**wds;
	t_list		*cmd;

	wds = split_cmd(line);
	if (!wds)
		return (NULL);
	cmd = prep_cmd(wds, envp);
	free_strs(wds);
	if (!cmd)
		return (NULL);
	ret = save_cmds(cmd, 0);
	if (!ret)
		return (NULL);
	all_quotes(ret);
	ft_lstclear(&cmd, free);
	return (ret);
}
