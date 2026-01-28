/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:41:09 by danzamor          #+#    #+#             */
/*   Updated: 2025/10/05 13:30:04 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*new;

	i = 0;
	if (start < ft_strlen(s))
	{
		while (s[start + i] && i < len)
			i++;
	}
	new = malloc((i + 1) * sizeof(char));
	if (!new)
		return (NULL);
	if (start < ft_strlen(s))
	{
		i = 0;
		while (s[start + i] != '\0' && i < len)
		{
			new[i] = s[start + i];
			i++;
		}
	}
	new[i] = '\0';
	return (new);
}
