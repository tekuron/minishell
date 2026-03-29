/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 19:09:09 by danz              #+#    #+#             */
/*   Updated: 2026/03/29 15:37:27 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	echo_print(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (write(STDOUT_FILENO, str, len) < 0)
	{
		perror("minishell: echo");
		return (1);
	}
	return (0);
}

int	echo_builtin(t_command *cmd)
{
	int		opt;
	int		i;

	opt = (cmd->command[1] && !ft_strncmp(cmd->command[1], "-n", 3));
	i = 1 + opt;
	while (cmd->command[i])
	{
		if (echo_print(cmd->command[i]))
			return (1);
		if (cmd->command[i + 1])
		{
			if (echo_print(" "))
				return (1);
		}
		i++;
	}
	if (!opt)
	{
		if (echo_print("\n"))
			return (1);
	}
	return (0);
}
