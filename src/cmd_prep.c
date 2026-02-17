/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_prep.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:24:40 by danz              #+#    #+#             */
/*   Updated: 2026/02/17 17:36:10 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes(t_list *lst)
{
	size_t	len;

	while (lst)
	{
		if (((char *)lst->content)[0] == '\''
			|| ((char *)lst->content)[0] == '\"')
		{
			len = ft_strlen((char *)lst->content);
			((char *)lst->content)[len - 1] = 0;
			ft_memmove(lst->content, (char *)lst->content + 1, len - 1);
			lst->content = ft_realloc(lst->content, len, len - 2);
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

t_command	*save_cmd(char **cmd)
{
	t_command	*ret;

	
}

t_command	*get_cmd(char *line, t_list *envp)
{
	t_command	*ret;
	char		**wds;
	t_list		*cmd;

	(void)envp;
	wds = split_cmd(line);
	cmd = prep_cmd(wds, envp);
	ret = save_cmd(cmd);
	free(wds);
	return (NULL);
}
