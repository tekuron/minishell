/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 19:43:22 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/25 12:19:54 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	t_command_size(t_command *cmd)
{
	int	length;

	length = 0;
	while (cmd)
	{
		cmd = cmd->next;
		length++;
	}
	return (length);
}

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