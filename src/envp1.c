/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 11:39:33 by danz              #+#    #+#             */
/*   Updated: 2026/02/17 17:22:05 by danz             ###   ########.fr       */
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

t_list	*prep_cmd(char **wds, t_list *envp)
{
	t_list	*ret;

	ret = lst_from_char(wds);
	if (!ret)
		return (NULL);
	insert_env(&ret, envp);
	remove_quotes(ret);
	return (ret);
}

char	*ft_getenv(char *var, t_list *envp)
{
	size_t len;

	len = 0;
	while (*var && *var != '$')
		var++;
	if (!(*var))
		return (NULL);
	var++;
	while (ft_isalnum(var[len]) || var[len] == '_')
		len++;
	while (envp)
	{
		if (!ft_strncmp(var, (char *)envp->content, len)
				&& ((char *)envp->content)[len] == '=')
			return ((char *)(envp->content) + len + 1);
		envp = envp->next;
	}
	return (NULL);
}
