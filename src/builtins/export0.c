/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 13:33:10 by danz              #+#    #+#             */
/*   Updated: 2026/03/28 19:35:42 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

int	env_is_valid(char *env)
{
	int	i;

	i = 0;
	if (!env || (!ft_isalpha(env[i]) && env[i] != '_'))
		return (0);
	while (env[i] && env[i] != '=')
	{
		if (env[i] == '+' && env[i + 1] == '=')
			return (1);
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_list	*env_find(t_list *lst, char *env)
{
	size_t	len;

	len = envlen(env);
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

static char	*get_new_content(char *current, char *input)
{
	char	*plus;
	char	*res;
	char	*tmp;

	plus = ft_strchr(input, '+');
	if (current && plus)
	{
		if (!ft_strchr(current, '='))
			tmp = ft_strjoin(current, "=");
		else
			tmp = ft_strdup(current);
		res = ft_strjoin(tmp, plus + 2);
		free(tmp);
		return (res);
	}
	if (!current && plus)
	{
		tmp = ft_substr(input, 0, plus - input);
		res = ft_strjoin(tmp, plus + 1);
		free(tmp);
		return (res);
	}
	return (ft_strdup(input));
}

int	env_exp(t_list **envp, t_list *dst, char *new)
{
	char	*new_content;
	char	*current_val;
	t_list	*new_node;

	if (dst && !ft_strchr(new, '='))
		return (0);
	current_val = NULL;
	if (dst)
		current_val = dst->content;
	new_content = get_new_content(current_val, new);
	if (!new_content)
		return (1);
	if (dst)
	{
		free(dst->content);
		dst->content = new_content;
	}
	else
	{
		new_node = ft_lstnew(new_content);
		if (!new_node)
			return (free(new_content), 1);
		ft_lstadd_back(envp, new_node);
	}
	return (0);
}

int	export_builtin(t_command *cmd, t_list **envp)
{
	char	**envs;
	int		ret;

	envs = cmd->command + 1;
	if (!*envs)
		return (export_print(*envp), 0);
	ret = 0;
	while (*envs)
	{
		if (!env_is_valid(*envs))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n",
				*envs);
			ret = 1;
		}
		else if (env_exp(envp, env_find(*envp, *envs), *envs))
			return (-1);
		envs++;
	}
	return (ret);
}
