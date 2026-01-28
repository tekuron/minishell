/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:46:09 by danzamor          #+#    #+#             */
/*   Updated: 2025/10/12 14:01:13 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

char	*hex_table(t_format format, char *c)
{
	char	*ret;

	ret = malloc(17);
	if (!ret)
		return (NULL);
	if (format.type == 'x')
	{
		ft_strlcpy(ret, "0123456789abcdef", 17);
		if (format.alt)
			*c = 'x';
	}
	else
	{
		ft_strlcpy(ret, "0123456789ABCDEF", 17);
		if (format.alt)
			*c = 'X';
	}
	return (ret);
}

static char	*apply_precision(char *str, t_format format)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	ft_memmove(str + format.precision - len, str, len + 1);
	while (i < format.precision - len)
		str[i++] = '0';
	return (str);
}

static char	*save_hex(unsigned int nb, t_format format)
{
	char	*ret;
	char	*hex;
	int		size;
	int		alt;

	alt = 2 * (format.alt) * (!!nb);
	size = digits_hex(nb) + alt;
	ret = malloc((size + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	hex = hex_table(format, &ret[1]);
	if (!hex)
		return (NULL);
	if (alt)
		ret[0] = '0';
	ret[size--] = 0;
	while (size >= alt)
	{
		ret[size] = hex[nb % 16];
		nb /= 16;
		size--;
	}
	free(hex);
	return (ret);
}

static char	*format_hex(unsigned int nb, t_format format)
{
	size_t	size;
	char	*tmp;
	char	*ret;

	size = max(digits_hex(nb) + 2 * (format.alt), format.width);
	size = max(size, format.precision);
	tmp = save_hex(nb, format);
	ret = malloc(size + 1);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, tmp, ft_strlen(tmp) + 1);
	free(tmp);
	if (format.precision > digits_hex(nb))
		ret = apply_precision(ret, format);
	else if (format.precision == 0 && nb == 0)
		ft_bzero(ret, size);
	if ((int)ft_strlen(ret) < format.width)
		ret = apply_width(ret, format);
	return (ret);
}

int	print_hex(unsigned int nb, t_format *format)
{
	char	*print;
	int		ret;

	ret = 0;
	print = format_hex(nb, *format);
	ret = write(1, print, ft_strlen(print));
	if (ret == -1)
		return (-1);
	free(print);
	return (ret);
}
