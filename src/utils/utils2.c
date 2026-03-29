/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 14:54:53 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/29 12:43:56 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	atoll_safe(char *str, long long *res)
{
	int			sign;
	
	sign = 1;
	while(str && ft_isspace(*str))
		str++;
	if (*str && (*str == '+' || *str == '-'))
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	while(*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (0);
		if (*res > (LLONG_MAX - (*str - '0')) / 10)
			return (0);
		*res = *res * 10 + (*str - '0');
		str++;
	}
	*res = *res * sign;
	return (1);
}