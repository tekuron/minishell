/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:24:17 by danzamor          #+#    #+#             */
/*   Updated: 2026/02/11 12:23:13 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned long	min(unsigned long n1, unsigned long n2)
{
	if (n1 >= n2)
		return (n2);
	return (n1);
}

void	*ft_realloc(void *ptr, size_t size, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	ft_memmove(new_ptr, ptr, min(size, new_size));
	free(ptr);
	return (new_ptr);
}
