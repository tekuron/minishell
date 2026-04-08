/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 14:54:53 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/08 20:25:58 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	atoll_safe(char *str, long long *res)
{
	int			sign;

	sign = 1;
	while (str && ft_isspace(*str))
		str++;
	if (*str && (*str == '+' || *str == '-'))
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (0);
		if (*res > (LLONG_MAX - (*str - '0')) / 10)
			return (0);
		*res = *res * 10 + (*str - '0');
		str++;
	}
	*res = *res * sign;
	return (1);
}

int	append_to_history(char *line)
{
	if (!*line)
	{
		free(line);
		return (0);
	}
	add_history(line);
	return (1);
}

void	free_and_exit(t_list **envp, t_command *cmd, int err, int ex_code)
{
	ft_lstclear(envp, free);
	free_cmd(NULL, cmd, CONT, NULL);
	if (err)
		perror("minishell");
	rl_clear_history();
	prompt(ex_code, EXIT_SHELL);
	exit(ex_code);
}

int	check_tty(void)
{
	static int	val;

	if (!val)
	{
		val = isatty(STDIN_FILENO) + 1;
		return (val - 1);
	}
	else if (val == 2)
		return (1);
	return (0);
}
