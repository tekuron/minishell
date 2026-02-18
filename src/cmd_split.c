/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:49:15 by danzamor          #+#    #+#             */
/*   Updated: 2026/02/18 12:21:58 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	word_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ' && !(str[i] >= 9 && str[i] <= 13))
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
		}
		if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
		}
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
		if (str[i] != ' ' && !(str[i] >= 9 && str[i] <= 13))
		{
			ret++;
			i += word_len(str + i);
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

	i = 0;
	word = 0;
	while (str[i])
	{
		if (str[i] != ' ' && !(str[i] >= 9 && str[i] <= 13))
		{
			len = word_len(str + i);
			ret[word] = malloc((len + 1) * sizeof(char));
			ft_strlcpy(ret[word], str + i, len + 1);
			i += len;
			word++;
		}
		else
			i++;
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
