/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nb_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:26:58 by danz              #+#    #+#             */
/*   Updated: 2025/10/12 14:01:36 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

#include <stdio.h>

static char	*apply_sign(char *str, t_format format, int nb)
{
	char	c;

	if (format.plus)
		c = '+';
	else
		c = ' ';
	if (nb >= 0)
	{
		ft_memmove(str + 1, str, ft_strlen(str) + 1);
		str[0] = c;
	}
	return (str);
}

static char	*apply_precision(char *str, t_format format)
{
	int	i;
	int	len;
	int	sign;

	sign = (str[0] == '-' || format.plus || format.space);
	i = sign;
	len = ft_strlen(str) - i;
	ft_memmove(str + format.precision - len + sign, str + sign, len + 1);
	while (i < format.precision - len + sign)
		str[i++] = '0';
	return (str);
}

static char	*format_nb(int nb, t_format format)
{
	size_t	size;
	char	*tmp;
	char	*ret;
	int		sign;

	sign = (nb < 0 || format.plus || format.space);
	size = max(ft_numlen(nb) + sign, format.width);
	size = max(size, format.precision + sign);
	tmp = ft_itoa(nb);
	ret = malloc(size + 1);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, tmp, ft_strlen(tmp) + 1);
	free(tmp);
	if (format.precision > ft_numlen(nb))
		ret = apply_precision(ret, format);
	else if (format.precision == 0 && nb == 0)
		ft_bzero(ret, size);
	if (format.plus || format.space)
		ret = apply_sign(ret, format, nb);
	if ((int)ft_strlen(ret) < format.width)
		ret = apply_width(ret, format);
	return (ret);
}

int	print_nb(int nb, t_format *format)
{
	char	*print;
	int		ret;

	print = format_nb(nb, *format);
	ret = 0;
	if (!print)
		return (-1);
	if (print[0])
	{
		ret = write(1, print, ft_strlen(print));
		if (ret == -1)
			return (-1);
	}
	free(print);
	return (ret);
}
