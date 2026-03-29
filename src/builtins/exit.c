/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:31:02 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/29 12:36:09 by dplazas-         ###   ########.fr       */
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

long long	check_arguments(t_command *cmd, int	args)
{
	long long	res;
	int			safe;
	
	res = 0;
	safe = 1;
	if (args > 2)
	{
		write(1, "minishell: exit: too many arguments\n", 37);
		return (1);
	}
	if (args == 2)
	{
		safe = atoll_safe(cmd->command[1], &res);
		if (contains_non_digits(cmd->command[1]) || !safe)
		{
			printf("minishell: exit: %s: numeric argument required\n", cmd->command[1]);
			return (2);
		}
		else
			return (res);
	}
	else
		return (0);
}

int	exit_builtin(t_command *cmd, t_list **envp)
{
	int			args;
	long long	exit_status;
	
	args = arr_len(cmd->command);
	write(1, "exit\n", 5);
	exit_status = check_arguments(cmd, args);
	free_cmd(NULL, cmd, CONT, NULL);
	ft_lstclear(envp, free);
	exit((unsigned char) exit_status);
	return (exit_status);
}