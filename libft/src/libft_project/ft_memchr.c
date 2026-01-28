/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:45:00 by danzamor          #+#    #+#             */
/*   Updated: 2025/10/05 12:47:20 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;
	unsigned char	a;

	p = (unsigned char *)s;
	a = (unsigned char)c;
	while (n--)
	{
		if (a == *p)
			return (p);
		p++;
	}
	return (0);
}
