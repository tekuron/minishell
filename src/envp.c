/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:18:02 by danzamor          #+#    #+#             */
/*   Updated: 2026/02/16 17:26:27 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_env(t_list *node)
{
	char	*str;

	str = node->content;
	if (*str == '\'')
		return (0);
	while (*str)
		if (*str++ == '$' && *(str + 1))
			return (1);
	return (0);
}

char	*envcat(char *str, char *env)
{
	size_t	slen;
	size_t	tlen;
	size_t	len;
	char	*ret;

	if (!str)
		return (NULL);
	if (!env)
		env = "";
	len = 0;
	while (str[len] && str[len] != '$')
		len++;
	slen = len + 1;
	while (str[slen] && (ft_isalnum(str[slen]) || str[slen] == '_'))
		slen++;
	ret = malloc(len + ft_strlen(str + slen) + ft_strlen(env) + 1);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, str, len + 1);
	tlen = len + ft_strlen(str + slen) + ft_strlen(env) + 1;
	ft_strlcat(ret, env, len);
	ft_strlcat(ret, str + slen, len);
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
		if (!ft_strncmp(var, (char *)envp->content, len) && ((char *)envp->content)[len] == '=')
			return ((char *)(envp->content) + len + 1);
		envp = envp->next;
	}
	return (NULL);
}

static t_list	*lst_env(t_list *node, t_list *envp)
{
	char	**split_env;
	char	*env;
	t_list	*ret;

	env = envcat(node->content, ft_getenv(node->content, envp));
	if (!env)
		return (NULL);
	if (env[0] != '\"')
		split_env = ft_split(env, ' ');
	else
	{
		split_env = malloc(2 * sizeof(char *));
		split_env[0] = ft_strdup(env);
		split_env[1] = NULL;
	}
	free(env);
	if (!split_env)
		return (NULL);
	ret = lst_from_char(split_env);
	free(split_env);
	return (ret);
}

void	insert_env(t_list *lst, t_list *envp)
{
	t_list *aux;
	t_list *new;

	while (lst->next)
	{
		if (has_env(lst->next))
		{
			aux = lst->next->next;
			new = lst_env(lst, envp);
			ft_lstdelone(lst->next, free);
			lst->next = new;
			new = ft_lstlast(new);
			new->next = aux;
		}
		lst = lst->next;
	}
}
