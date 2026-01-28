/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:46:15 by danzamor          #+#    #+#             */
/*   Updated: 2025/10/05 12:36:13 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_strarr(char ***strarr)
{
	int	i;

	i = 0;
	while ((*strarr)[i])
	{
		free((*strarr)[i]);
		i++;
	}
	free(*strarr);
	*strarr = NULL;
}

static int	wc(char const *s, char c)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (s[i] != 0)
	{
		if (s[i] != c)
		{
			ret++;
			while (s[i] != c && s[i] != 0)
				i++;
		}
		else
			i++;
	}
	return (ret);
}

static void	save(char const *s, char c, char ***ret)
{
	int		i;
	int		j;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			j = 0;
			while (s[j] != c && s[j] != 0)
				j++;
			(*ret)[i] = malloc((j + 1) * sizeof(char));
			if (!(*ret)[i])
			{
				free_strarr(ret);
				return ;
			}
			ft_strlcpy((*ret)[i++], (char *)s, j + 1);
			while (*s != c && *s != 0)
				s++;
		}
		else
			s++;
	}
	(*ret)[i] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**ret;
	int		k;

	if (!s)
		return (0);
	ret = malloc((wc(s, c) + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	k = 0;
	while (k < (wc(s, c) + 1))
	{
		ret[k] = NULL;
		k++;
	}
	save(s, c, &ret);
	if (!ret)
		return (NULL);
	return (ret);
}
