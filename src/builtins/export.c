/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:11:43 by danz              #+#    #+#             */
/*   Updated: 2026/03/11 14:24:14 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_is_valid(char *env)
{
	int	ok;

	ok = 1;
	if (!env)
		return (0);
	if ((*env <= '9' && *env >= '0') || *env == '=')
		return (0);
	while (*env && *env != '=')
	{
		if (!ft_isalnum(*env) && *env != '_')
		{
			ok = 0;
			break ;
		}
		env++;
	}
	if (*env == '+' && *(env + 1) == '=')
		ok = 1;
	return (ok);
}

t_list	*env_find(t_list *lst, char *env)
{
	size_t	len;

	len = envlen(env);;
	while (lst)
	{
		if (!ft_strncmp(lst->content, env, len)
			&& (((char *)lst->content)[len] == '='
			|| !((char *)lst->content)[len]))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

char *env_append(char *dst, char *new)
{
	char	*ret;
	int		i;

	ret = NULL;
	i = 0;
	if (dst)
	{
		if (!ft_strchr(dst, '='))
			new = ft_strjoin("=", new);
		ret = ft_strjoin(dst, new);
		free(new);
	}
	if (ret)
		return (free(dst), ret);
	ret = strdup(new);
	if (!ret)
		return (NULL);
	while (ret[i] && ret[i] != '+')
		i++;
	while (ret[i])
		ret[i] = ret[1 + i++];
	return (ret);
}

int	env_exp(t_list **envp, t_list *dst, char *new)
{
	char *dup;

	if (dst && new[envlen(new)])
	{
		if (ft_strchr(new, '+'))
			dst->content = env_append(dst->content, ft_strchr(new, '+') + 2);
		else
		{
			free(dst->content);
			dst->content = ft_strdup(new);
		}
		if (!dst->content)
			return (1);
	}
	else if (!dst)
	{
		dup = env_append(NULL, new);
		if (!dup)
			return (1);
		dst = ft_lstnew(dup);
		if (!dst)
			return (free(dup), 1);
		ft_lstadd_back(envp, dst);
	}
	return (0);
}

int	export_builtin(t_command *cmd, t_list **envp)
{
	char	**envs;
	t_list	*cur;
	int		ret;

	envs = cmd->command + 1;
	ret = 0;
	while (*envs)
	{
		if (!env_is_valid(*envs))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n",
				*envs);
			ret = 1;
		}
		else
		{
			cur = env_find(*envp, *envs);
			if (env_exp(envp, cur, *envs))
				return (-1);
		}
		envs++;
	}
	return (ret);
}
