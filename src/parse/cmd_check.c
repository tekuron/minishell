/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:37:06 by danz              #+#    #+#             */
/*   Updated: 2026/04/06 21:50:54 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	token_error(char *str, int code)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	if (code == 1)
		write(2, str, word_len(str));
	else
		write(2, "newline", 7);
	write(2, "\'\n", 2);
}

static int	set_vars(char *str, int *pipe, int *redir, int word)
{
	if (*str == '|')
	{
		if (*pipe || *redir || !word)
			return (1);
		*pipe = 1;
	}
	else if (*str == '<' || *str == '>')
	{
		if (*redir)
			return (1);
		*redir = 1;
	}
	else if (*str != ' ' && !(*str >= 9 && *str <= 13))
	{
		*pipe = 0;
		*redir = 0;
	}
	if (*(str + word_len(str)) == 0 && (*pipe || *redir))
		return (2);
	return (0);
}

int	check_cmd(char *str)
{
	int	pipe;
	int	redir;
	int	word;
	int	err;

	pipe = 0;
	redir = 0;
	word = 0;
	while (*str)
	{
		err = set_vars(str, &pipe, &redir, word);
		if (err)
			return (token_error(str, err), 1);
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
