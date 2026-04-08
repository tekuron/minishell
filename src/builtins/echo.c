/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 19:09:09 by danz              #+#    #+#             */
/*   Updated: 2026/04/08 20:09:53 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	echo_option(char *str)
{
	int	ret;

	ret = 0;
	if (!str || *str++ != '-')
		return (0);
	while (*str)
	{
		ret += *str != 'n';
		str++;
	}
	return (!ret);
}

static void	echo_print(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	write(STDOUT_FILENO, str, len);
}

int	echo_builtin(t_command *cmd)
{
	int		opt;
	int		i;

	opt = echo_option(cmd->command[1]);
	i = 1 + opt;
	while (cmd->command[i])
	{
		echo_print(cmd->command[i]);
		if (cmd->command[i + 1])
			echo_print(" ");
		i++;
	}
	if (!opt)
		echo_print("\n");
	return (0);
}
