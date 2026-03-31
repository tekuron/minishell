/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:31:26 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/31 15:38:04 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_identifier(char *str)
{
	if (!*str)
		return (0);
	if (ft_isdigit(str[0]))
		return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

static int	validate_args(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->command && cmd->command[i])
	{
		if (!validate_identifier(cmd->command[i]))
			return (0);
		i++;
	}
	return (1);
}

void	remove_from_env(t_list **envp, char *str)
{
	t_list	*prev;
	t_list	*curr;
	int		length;

	curr = *envp;
	prev = NULL;
	while (curr)
	{
		length = ft_strlen(str);
		if (curr->content
			&& ft_strncmp((char *)curr->content, str, length) == 0
			&& ((char *)curr->content)[length] == '=')
		{
			if (prev == NULL)
				*envp = curr->next;
			else
				prev->next = curr->next;
			ft_lstdelone(curr, free);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	unset_builtin(t_command *cmd, t_list **envp)
{
	int		i;
	int		args;

	args = arr_len(cmd->command);
	if (args <= 1)
	{
		printf("minishell: unset: not enough arguments\n");
		return (1);
	}
	if (!validate_args(cmd))
	{
		printf("minishell: unset: invalid parameter name\n");
		return (1);
	}
	i = 1;
	while (i < args)
	{
		remove_from_env(envp, cmd->command[i]);
		i++;
	}
	return (0);
}
