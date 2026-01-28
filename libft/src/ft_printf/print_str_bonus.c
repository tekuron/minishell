/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_str_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:32:12 by danz              #+#    #+#             */
/*   Updated: 2025/10/12 14:03:16 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

static char	*apply_minus_str(char *str, t_format format, int len)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] && i < len)
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

char	*apply_width_str(char *str, t_format format)
{
	int	i;
	int	len;

	i = 0;
	while (str[i])
		i++;
	len = i;
	while (i < format.width)
		str[i++] = ' ';
	str[i] = 0;
	return (apply_minus_str(str, format, len));
}

static char	*apply_precision_str(char *str, t_format format)
{
	int	i;

	i = 0;
	while (str[i] && i < format.precision)
		i++;
	while (str[i])
	{
		str[i] = 0;
		i++;
	}
	return (str);
}

static char	*format_str(char *str, t_format format)
{
	size_t	size;
	char	*ret;

	size = max(ft_strlen(str), format.width);
	ret = malloc(size + 1);
	ft_strlcpy(ret, str, size + 1);
	if (format.null && format.precision < 6 && format.precision != -1)
	{
		free(ret);
		ret = ft_strdup("");
	}
	if (format.precision >= 0)
		ret = apply_precision_str(ret, format);
	if ((int)ft_strlen(ret) < format.width)
		ret = apply_width_str(ret, format);
	return (ret);
}

int	print_str(char *str, t_format *format)
{
	char	*print;
	int		ret;

	if (!str)
	{
		format->null = 1;
		str = ft_strdup("(null)");
	}
	print = format_str(str, *format);
	ret = write(1, print, ft_strlen(print));
	if (ret == -1)
		return (-1);
	free(print);
	return (ret);
}
