/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 21:56:54 by danz              #+#    #+#             */
/*   Updated: 2025/10/12 15:42:46 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

static char	*save_ptr(unsigned long long nb)
{
	char	*ret;
	char	*hex;
	int		size;

	hex = "0123456789abcdef";
	size = digits_hex(nb) + 2;
	ret = malloc((size + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	ret[0] = '0';
	ret[1] = 'x';
	ret[size--] = 0;
	while (size > 1)
	{
		ret[size] = hex[nb % 16];
		nb /= 16;
		size--;
	}
	return (ret);
}

static char	*format_ptr(unsigned long long nb, t_format format)
{
	size_t	size;
	char	*tmp;
	char	*ret;

	if (format.null)
	{
		size = max(5, format.width);
		tmp = ft_strdup("(nil)");
	}
	else
	{
		size = max(digits_hex(nb) + 2, format.width);
		tmp = save_ptr((unsigned long)nb);
	}
	ret = malloc(size + 1);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, tmp, ft_strlen(tmp) + 1);
	free(tmp);
	if ((int)ft_strlen(ret) < format.width)
		ret = apply_width(ret, format);
	return (ret);
}

int	print_ptr(unsigned long long nb, t_format *format)
{
	char	*print;
	int		ret;

	ret = 0;
	if (!nb)
		format->null = 1;
	print = format_ptr(nb, *format);
	ret += write(1, print, ft_strlen(print));
	if (ret == -1)
		return (-1);
	free(print);
	return (ret);
}
