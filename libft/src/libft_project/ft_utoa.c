/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:07:20 by danzamor          #+#    #+#             */
/*   Updated: 2025/10/12 18:07:46 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_rev_str(char *str, int size)
{
	int	i;
	int	hold;

	i = 0;
	while (i < size / 2)
	{
		hold = str[i];
		str[i] = str[size - i - 1];
		str[size - i - 1] = hold;
		hold = 0;
		i++;
	}
}

static void	ft_itoa_mod(char *ret, long nb, int size)
{
	int	i;
	int	sign;

	i = 0;
	sign = 0;
	if (nb < 0)
	{
		ret[size] = '-';
		nb *= -1;
		sign = 1;
	}
	while (i < size)
	{
		ret[i] = nb % 10 + 48;
		if (nb > 0)
			nb = nb / 10;
		i++;
	}
	ft_rev_str(ret, i + sign);
	ret[i + sign] = 0;
}

char	*ft_utoa(unsigned int n)
{
	int		i;
	char	*ret;
	long	nb;

	i = 1;
	nb = n;
	while (nb >= 10)
	{
		nb /= 10;
		i++;
	}
	nb = n;
	ret = malloc((i + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	ft_itoa_mod(ret, nb, i);
	return (ret);
}
