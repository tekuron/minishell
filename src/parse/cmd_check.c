/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:37:06 by danz              #+#    #+#             */
/*   Updated: 2026/04/01 18:23:45 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_vars(char *str, int *pipe, int *redir, int word)
{
	if (*str == '|')
	{
		if (*pipe || *redir || !word)
			return (1);
		*pipe = 1;
	}
	else if (*str == '>')
	{
		if (*pipe || *redir)
			return (1);
		*redir = 1;
	}
	else if (*str != ' ' && !(*str >= 9 && *str <= 13))
	{
		*pipe = 0;
		*redir = 0;
	}
	if (*(str + 1) == 0 && (*pipe || *redir))
		return (1);
	return (0);
}

int	check_cmd(char *str)
{
	int	pipe;
	int	redir;
	int	word;

	pipe = 0;
	redir = 0;
	word = 0;
	while (*str)
	{
		if (set_vars(str, &pipe, &redir, word))
			return (1);
		if (*str != ' ' && !(*str >= 9 && *str <= 13))
		{
			if (!(*str == '|' || *str == '<' || *str == '>'))
				word = 1;
			str += word_len(str);
		}
		else
			str++;
	}
	return (0);
}
