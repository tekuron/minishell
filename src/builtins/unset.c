/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:31:26 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/28 10:04:15 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fix_list(t_list **envp, char *str)
{
	t_list	*prev;
	t_list	*curr;

	curr = *envp;
	prev = NULL;
	while (curr)
	{
		if (curr->content 
			&& ft_strncmp(curr->content, str, ft_strlen(str)) == 0
			&& ((char *)curr->content)[ft_strlen(str)] == '=')
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
		printf("minishell: unset: not enough arguments");
		return (1);
	}
	i = 1;
	while (i < args)
	{
		fix_list(envp, cmd->command[i]);
		i++;
	}
	return (0); //Finish this builtin
}