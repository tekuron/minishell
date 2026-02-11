/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:49:15 by danzamor          #+#    #+#             */
/*   Updated: 2026/02/11 12:29:47 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	word_len(char *str, int mode)
{
	int	i;

	i = 0;
	if (!mode)
	{
		while (str[i] && str[i] != ' ' && !(str[i] >= 9 && str[i] <= 13))
			i++;
	}
	else if (mode == 1)
	{
		while (str[i] && str[i] != '\'')
			i++;
	}
	else if (mode == 2)
	{
		while (str[i] && str[i] != '\"')
			i++;
	}
	return (i);
}

static int	word_count(char *str)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			ret++;
			i++;
			i += 1 + word_len(str + i, 1 * (str[i - 1] == '\'')
					+ 2 * (str[i - 1] == '\"'));
		}
		else if (str[i] != ' ' && !(str[i] >= 9 && str[i] <= 13))
		{
			ret++;
			while (str[i] && str[i] != ' ' && !(str[i] >= 9 && str[i] <= 13))
				i++;
		}
		else
			i++;
	}
	return (ret);
}

static void	save_words(char **ret, char *str)
{
	int	i;
	int	word;
	int	len;

	i = -1;
	word = 0;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			len = 2 + word_len(str + i + 1, 1 * (str[i] == '\'')
					+ 2 * (str[i] == '\"'));
			ret[word] = malloc((len + 1) * sizeof(char));
			i += ft_strlcpy(ret[word], str + i, len + 1) * 0 + len - 1;
			word++;
		}
		else if (str[i] != ' ' && !(str[i] >= 9 && str[i] <= 13))
		{
			len = word_len(str + i, 0);
			ret[word] = malloc((len + 1) * sizeof(char));
			i += ft_strlcpy(ret[word], str + i, len + 1) * 0 + len - 1;
			word++;
		}
	}
}

char	**split_cmd(char *str)
{
	int		wc;
	char	**ret;

	wc = word_count(str);
	ret = malloc((wc + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	save_words(ret, str);
	ret[wc] = 0;
	return (ret);
}
