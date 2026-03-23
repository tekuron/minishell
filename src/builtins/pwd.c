/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:31:48 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/23 19:30:10 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin(t_command *cmd, t_list *envp)
{
	char *path;
	
	(void) cmd;
	(void) envp;
	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}