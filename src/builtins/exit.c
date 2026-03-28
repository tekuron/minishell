/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:31:02 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/28 09:36:43 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	contains_non_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (1);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}
// Change fds and overflow
int	exit_builtin(t_command *cmd, t_list **envp)
{
	int args;
	int status;

	status = EXIT_SUCCESS;
	args = arr_len(cmd->command);
	write(1, "exit\n", 5);
	if (args > 2)
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else if (args == 2 && contains_non_digits(cmd->command[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", cmd->command[1]);
		status = 255; // Check if it is actually 2 or 255.
	}
	else if (args == 2)
		status = ft_atoi(cmd->command[1]); //Check modulo
	free_cmd(NULL, cmd, CONT, NULL);
	ft_lstclear(envp, free);
	exit((unsigned char) status);
	return (status);
}