/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:32:02 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/29 15:40:48 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_builtin(t_command *cmd, t_list *envp)
{
	(void) cmd;
	while (envp)
	{
		if (envp->content && ft_strchr((char *) envp->content, '='))
			printf("%s\n", (char *) envp->content);
		envp = envp->next;
	}
	return (0);
}
