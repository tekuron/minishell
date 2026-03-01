/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:24:17 by danzamor          #+#    #+#             */
/*   Updated: 2026/02/28 12:42:09 by dplazas-         ###   ########.fr       */
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

char	**t_list_to_char(t_list *envp)
{
	char	**env;
	int		i;

	env = malloc(sizeof(char *) * (ft_lstsize(envp) + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp)
	{
		env[i] = ft_strdup((char *)envp->content);
		if (!env[i])
			return ((void) free_strs(env), NULL);
		i++;
		envp = envp->next;
	}
	env[i] = NULL;
	return (env);
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
	int	i;

	i = 0;
	if (!strs)
		return ;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}

