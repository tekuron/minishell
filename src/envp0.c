/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:18:02 by danzamor          #+#    #+#             */
/*   Updated: 2026/02/19 16:18:24 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_env(t_list *node)
{
	char	*str;
	int		db_qt;

	str = node->content;
	db_qt = 0;
	while (*str)
	{
		if (*str == '\"')
			db_qt = !db_qt;
		if (!db_qt && *str == '\'')
		{
			str++;
			while (*str != '\'')
				str++;
		}
		if (*str++ == '$' && *(str))
			return (1);
	}
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
	slen = len + (str[len] == '$');
	while (str[slen] && (ft_isalnum(str[slen]) || str[slen] == '_'))
		slen++;
	if (str[slen] == '?')
		slen++;
	ret = malloc(len + ft_strlen(str + slen) + ft_strlen(env) + 1);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, str, len + 1);
	tlen = len + ft_strlen(str + slen) + ft_strlen(env) + 1;
	ft_strlcat(ret, env, tlen);
	ft_strlcat(ret, str + slen, tlen);
	return (ret);
}

static t_list	*lst_env(t_list *node, t_list *envp)
{
	char	**split_env;
	char	*env;
	t_list	*ret;

	env = envcat(node->content, ft_getenv(node->content, envp));
	if (!env)
		return (NULL);
	if (env[0] != '\"' && env[0])
		split_env = ft_split(env, ' ');
	else
	{
		split_env = malloc(2 * sizeof(char *));
		if (!split_env)
			return (NULL);
		split_env[0] = ft_strdup(env);
		split_env[1] = NULL;
	}
	free(env);
	if (!split_env)
		return (NULL);
	ret = lst_from_char(split_env);
	free_strs(split_env);
	return (ret);
}

static void	insert_env_mod(t_list **prev, t_list **next, t_list **cur)
{
	*prev = *cur;
	*cur = *next;
}

void	insert_env(t_list **lst, t_list *envp)
{
	t_list	*prev;
	t_list	*next;
	t_list	*new;
	t_list	*cur;

	cur = *lst;
	prev = 0;
	while (cur)
	{
		next = cur->next;
		if (has_env(cur))
		{
			new = lst_env(cur, envp);
			if (prev)
				prev->next = new;
			else
				*lst = new;
			ft_lstdelone(cur, free);
			cur = new;
			new = ft_lstlast(new);
			new->next = next;
		}
		else
			insert_env_mod(&prev, &next, &cur);
	}
}
