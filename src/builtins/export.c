/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:11:43 by danz              #+#    #+#             */
/*   Updated: 2026/02/25 12:43:44 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_is_valid(char *env)
{
	if (!env)
		return (0);
	if ((*env <= '9' && *env >= '0') || *env == '=')
		return (0);
	while (*env && *env != '=')
	{
		if (!ft_isalnum(*env) && *env != '_')
			return (0);
		env++;
	}
	return (1);
}

t_list	*env_find(t_list *lst, char *env)
{
	size_t	len;

	len = 0;
	while (env[len] && env[len] != '=')
		len++;
	while (lst)
	{
		if (ft_strncmp(lst->content, env, len + 1))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

int	env_exp(t_list *envp, t_list *dst, char *new)
{
	void	*new_ptr;

	if (dst)
	{
		new_ptr = ft_realloc(dst->content, ft_strlen((char *)dst->content), ft_strlen(new));
		if (!new_ptr)
			return (1);
		dst->content = new_ptr;
	}
	else
		ft_lstadd_front(envp, ft_lstnew(ft_strdup(new)));
	return (0);
}

int	ft_export(t_command *cmd, t_list *envp)
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
			cur = env_find(envp, *envs);
			if (env_exp(envp, cur, *envs))
				return (-1);
		}
		envs++;
	}
	return (ret);
}
