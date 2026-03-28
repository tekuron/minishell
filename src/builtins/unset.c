/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:31:26 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/28 13:37:51 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_args(t_command *cmd)
{
	if (!cmd->command || !*cmd->command)
		return (0);
	if (ft_isdigit(cmd->command[0][0]))
		return (0);
	if (ft_strchr(cmd->command[0], '='))
		return (0);
	return 1;
}

static void	remove_from_env(t_list **envp, char *str)
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
	if (!validate_args(cmd))
	{
		printf("minishell: unset: invalid parameter name");
		return (1);
	}
	if (args <= 1)
	{
		printf("minishell: unset: not enough arguments");
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