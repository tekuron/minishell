/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:37:10 by danz              #+#    #+#             */
/*   Updated: 2025/11/14 01:44:34 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size != 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dest[i] = src [i];
			i++;
		}
		dest[i] = '\0';
	}
	while (src[i] != '\0')
		i++;
	return (i);
}

size_t	gnl_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (*str)
	{
		str++;
		i++;
	}
	return (i);
}

char	*gnl_strchr(const char *s, int c)
{
	while (*s)
	{
		if (c == *s)
			return ((char *)s);
		s++;
	}
	if (c == *s)
		return ((char *)s);
	return (0);
}

char	*gnl_strdup(const char *src)
{
	int		i;
	char	*new;

	i = 0;
	if (!src)
		return (NULL);
	new = malloc((gnl_strlen(src) + 1) * sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	while (src[i])
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char	*gnl_strjoin2(char *str1, char *str2)
{
	char	*ret;
	size_t	totallen;
	int		i;

	i = 0;
	if (!str1)
		return (gnl_strdup(str2));
	totallen = gnl_strlen(str1) + gnl_strlen(str2);
	ret = malloc(totallen + 1);
	if (!ret)
	{
		free(str1);
		return (NULL);
	}
	i = gnl_strlcpy(ret, str1, totallen - i + 1);
	gnl_strlcpy(ret + i, str2, totallen - i + 1);
	free(str1);
	return (ret);
}
