/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:24:17 by danzamor          #+#    #+#             */
/*   Updated: 2026/02/20 16:48:20 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*lst_from_char(char **wds)
{
	t_list	*ret;
	t_list	*cur;

	ret = ft_lstnew(ft_strdup(*wds));
	wds++;
	if (!ret)
		return (NULL);
	cur = ret;
	while (*wds)
	{
		cur->next = ft_lstnew(ft_strdup(*wds));
		if (!cur->next)
			return (ft_lstclear(&ret, free), NULL);
		cur = cur->next;
		wds++;
	}
	return (ret);
}

int	is_redir(char *str)
{
	if ((!ft_strncmp(str, "<", 2))
		|| (!ft_strncmp(str, ">", 2))
		|| (!ft_strncmp(str, "<<", 3))
		|| (!ft_strncmp(str, ">>", 3)))
		return (1);
	return (0);
}

void	free_strs(char **strs)
{
	int i;

	i = 0;
	if (!strs)
		return ;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}
