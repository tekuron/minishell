/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_prep.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:24:40 by danz              #+#    #+#             */
/*   Updated: 2026/02/18 12:29:05 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes(t_list *lst)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*str;

	while (lst)
	{
		i = 0;
		str = (char *)lst->content;
		len = ft_strlen(str);
		while (str[i])
		{
			if (str[i] == '\'' || str[i] == '\"')
			{
				j = i + 1;
				while (str[j] != str[i])
					j++;
				ft_memmove(&str[j], &str[j + 1], len - j);
				ft_memmove(&str[i], &str[i + 1], len - i - 1);
				lst->content = ft_realloc(lst->content, len, len - 2);
			}
			i++;
		}
		lst = lst->next;
	}
}

static t_list	*prep_cmd(char **wds, t_list *envp)
{
	t_list	*ret;

	ret = lst_from_char(wds);
	if (!ret)
		return (NULL);
	insert_env(&ret, envp);
	remove_quotes(ret);
	return (ret);
}

t_command	*get_cmd(char *line, t_list *envp)
{
	t_command	*ret;
	char		**wds;
	t_list		*cmd;
	int i;

	i = 0;
	wds = split_cmd(line);
	cmd = prep_cmd(wds, envp);
	ret = save_cmds(cmd);
	while (wds[i])
		free(wds[i++]);
	free(wds);
	ft_lstclear(&cmd, free);
	return (ret);
}
