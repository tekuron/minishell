/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 18:12:12 by danzamor          #+#    #+#             */
/*   Updated: 2025/10/09 19:42:22 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	digits_hex(unsigned long long nb)
{
	int	ret;

	ret = 1;
	while (nb >= 16)
	{
		nb /= 16;
		ret++;
	}
	return (ret);
}
