/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 19:26:03 by danz              #+#    #+#             */
/*   Updated: 2026/03/28 19:34:00 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_env(char **env, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strncmp(env[j], env[j + 1], envlen(env[j])) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_line(char *env)
{
	int		i;
	char	*equal;

	i = 0;
	equal = ft_strchr(env, '=');
	ft_printf("declare -x ");
	if (equal)
	{
		while (env[i] && env[i] != '=')
		{
			ft_putchar_fd(env[i], 1);
			i++;
		}
		ft_printf("=\"%s\"\n", equal + 1);
	}
	else
		ft_printf("%s\n", env);
}

void export_print(t_list *envp)
{
	int		size;
	int		i;
	char	**env;

	size = ft_lstsize(envp);
	env = malloc(sizeof(char *) * size);
	if (!env)
		return ;
	i = 0;
	while (envp)
	{
		env[i] = envp->content;
		envp = envp->next;
		i++;
	}
	sort_env(env, size);
	i = 0;
	while (i < size)
	{
		print_export_line(env[i]);
		i++;
	}
	free(env);
}
