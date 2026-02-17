/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:18:02 by danzamor          #+#    #+#             */
/*   Updated: 2026/02/17 12:47:50 by danz             ###   ########.fr       */
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
	if (env[0] != '\"')
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
	free(split_env);
	return (ret);
}

// static void	insert_env_mod()
// {
	
// }

void	insert_env(t_list **lst, t_list *envp)
{
	t_list *prev;
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
			prev = ft_lstlast(new);
			prev->next = next;
			ft_lstdelone(cur, free);
			cur = prev;
		}
		else
		{
			prev = cur;
			cur = next;
		}
	}
}
