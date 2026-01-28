/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:17:45 by danzamor          #+#    #+#             */
/*   Updated: 2025/09/29 15:10:00 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*pd;
	char	*ps;

	pd = (char *)dest;
	ps = (char *)src;
	if (!dest && !src)
		return (0);
	if (dest <= src)
		while (n--)
			*pd++ = *ps++;
	else if (dest > src)
	{
		pd += n - 1;
		ps += n - 1;
		while (n--)
			*pd-- = *ps--;
	}
	return (dest);
}
