/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:46:20 by danz              #+#    #+#             */
/*   Updated: 2025/10/12 15:36:55 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

int	ft_numlen(long nb)
{
	int	i;

	i = 0 + (nb == 0);
	while (nb)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

int	nums(char *str)
{
	int	ret;

	ret = 0;
	while (str[ret] >= '0' && str[ret] <= '9')
		ret++;
	return (ret);
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

char	*apply_width(char *str, t_format format)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	while (i < format.width)
		str[i++] = ' ';
	str[i] = 0;
	return (apply_minus(str, format));
}

char	*apply_minus(char *str, t_format format)
{
	int		i;
	int		j;
	char	c;
	int		sign;

	i = 0;
	sign = ((format.space || format.plus || str[0] == '-')
			&& format.zero && format.precision == -1);
	if (format.zero && format.precision == -1)
		c = '0';
	else
		c = ' ';
	while (str[i + sign] && str[i + sign] != ' ')
		i++;
	if (!format.minus)
	{
		ft_memmove(str + format.width - i, str + sign, i);
		ft_memmove(str + format.width - i - sign, str, sign);
		j = sign && c == '0';
		while (j < format.width - i - sign * !j)
			str[j++] = c;
	}
	return (str);
}
