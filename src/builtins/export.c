/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:11:43 by danz              #+#    #+#             */
/*   Updated: 2026/03/11 12:29:28 by danz             ###   ########.fr       */
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

int	env_exp(t_list **envp, t_list *dst, char *new)
{
	char	*dup;
	t_list	*new_node;

	if (dst && new[envlen(new)])
	{
		dup = ft_strchr(new, '+');
		if (dup)
			new = envcat(dst->content, dup + 2);
		free(dst->content);
		dst->content = ft_strdup(new); //Arreglar esto
		if (!dst->content)
			return (1);
	}
	else if (!dst)
	{
		dup = ft_strdup(new);
		if (!dup)
			return (1);
		new_node = ft_lstnew(dup);
		if (!new_node)
			return (free(dup), 1);
		ft_lstadd_back(envp, new_node);
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
