/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:03:13 by danz              #+#    #+#             */
/*   Updated: 2025/10/05 13:51:49 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_charset(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i] != 0)
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	char	*cpy;

	i = 0;
	while (s1[i] && is_charset((char)s1[i], (char *)set))
		i++;
	j = ft_strlen(s1);
	while (j > i && is_charset((char)s1[j - 1], (char *)set))
		j--;
	cpy = malloc((j - i + 1) * sizeof(char));
	if (!cpy)
		return (NULL);
	ft_strlcpy(cpy, (char *)(s1 + i), j - i + 1);
	cpy[j - i] = 0;
	return (cpy);
}
