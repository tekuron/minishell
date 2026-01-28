/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_char_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:23:49 by danz              #+#    #+#             */
/*   Updated: 2025/10/08 13:44:36 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

char	*apply_width_char(char *str, t_format format)
{
	int	i;

	i = !str[0];
	while (str[i])
		i++;
	while (i < format.width)
		str[i++] = ' ';
	str[i] = 0;
	return (str);
}

static char	*apply_minus_char(char *str, t_format format)
{
	int	i;
	int	j;

	i = (!str[0]);
	while (str[i] && str[i] != ' ')
		i++;
	if (!format.minus)
	{
		ft_memmove(str + format.width - i, str, i);
		j = 0;
		while (j < format.width - i)
			str[j++] = ' ';
	}
	return (str);
}

static char	*format_char(char c, t_format format)
{
	size_t	size;
	char	*ret;

	size = max(1, format.width);
	ret = malloc(size + 1);
	ret[0] = c;
	while (size)
		ret[size--] = 0;
	if ((int)ft_strlen(ret) + (!c) < format.width)
	{
		ret = apply_width_char(ret, format);
		ret = apply_minus_char(ret, format);
	}
	return (ret);
}

int	print_char(char c, t_format *format)
{
	char	*print;
	int		ret;

	print = format_char(c, *format);
	ret = write(1, print, max(ft_strlen(print), format->width));
	free(print);
	return (ret);
}
