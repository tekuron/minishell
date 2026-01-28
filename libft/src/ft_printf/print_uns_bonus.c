/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_uns_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 18:52:41 by danzamor          #+#    #+#             */
/*   Updated: 2025/10/12 14:02:06 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

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

static char	*format_nb(unsigned int nb, t_format format)
{
	size_t	size;
	char	*tmp;
	char	*ret;

	size = max(ft_numlen(nb), format.width);
	size = max(size, format.precision);
	tmp = ft_utoa(nb);
	ret = malloc(size + 1);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, tmp, ft_strlen(tmp) + 1);
	free(tmp);
	if (format.precision > ft_numlen(nb))
		ret = apply_precision(ret, format);
	else if (format.precision == 0 && nb == 0)
		ft_bzero(ret, size);
	if ((int)ft_strlen(ret) < format.width)
		ret = apply_width(ret, format);
	return (ret);
}

int	print_uns(unsigned int nb, t_format *format)
{
	char	*print;
	int		ret;

	print = format_nb(nb, *format);
	ret = write(1, print, ft_strlen(print));
	if (ret == -1)
		return (-1);
	free(print);
	return (ret);
}
