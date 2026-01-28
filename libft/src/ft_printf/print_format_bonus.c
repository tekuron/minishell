/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_format_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:22:37 by danz              #+#    #+#             */
/*   Updated: 2025/10/12 15:07:11 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

int	print_format(t_format *format, va_list vars)
{
	if (format->type == 'c')
		return (print_char(va_arg(vars, int), format));
	else if (format->type == 's')
		return (print_str(va_arg(vars, char *), format));
	else if (format->type == 'd' || format->type == 'i')
		return (print_nb(va_arg(vars, int), format));
	else if (format->type == 'u')
		return (print_uns(va_arg(vars, unsigned int), format));
	else if (format->type == 'p')
		return (print_ptr(va_arg(vars, unsigned long long), format));
	else if (format->type == 'x' || format->type == 'X')
		return (print_hex(va_arg(vars, unsigned int), format));
	else if (format->type == '%')
		return (write(1, "%", 1));
	else
		return (-1);
	return (0);
}
