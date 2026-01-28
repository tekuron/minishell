/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:15:05 by danz              #+#    #+#             */
/*   Updated: 2025/10/10 00:35:56 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

void	format_init(t_format *format)
{
	format->alt = 0;
	format->minus = 0;
	format->plus = 0;
	format->space = 0;
	format->zero = 0;
	format->width = 0;
	format->precision = -1;
	format->type = 0;
	format->null = 0;
}

int	flag_check(char *str, t_format *format)
{
	if (*str == '-')
		format->minus = 1;
	else if (*str == '+')
		format->plus = 1;
	else if (*str == '#')
		format->alt = 1;
	else if (*str == '0')
		format->zero = 1;
	else if (*str == ' ')
		format->space = 1;
	else if (*str == '.')
	{
		str++;
		format->precision = ft_atoi(str);
	}
	else if (*str == 'c' || *str == 's' || *str == 'p'
		|| *str == 'd' || *str == 'i' || *str == 'u'
		|| *str == 'x' || *str == 'X' || *str == '%')
		format->type = *str;
	else if (*str >= '1' && *str <= '9')
		format->width = ft_atoi(str);
	else
		return (0);
	return (1);
}

int	print_arg(char *str, va_list vars)
{
	t_format	*format;
	int			printed;

	format = malloc(sizeof(t_format));
	if (!format)
		return (-1);
	format_init(format);
	while (*str && flag_check(str, format))
	{
		if (format->type)
			break ;
		if (format->precision != -1 && *str == '.')
		{
			str++;
			str += nums(str) - 1;
		}
		if (format->width && nums(str))
			str += nums(str) - 1;
		str++;
	}
	printed = print_format(format, vars);
	free(format);
	return (printed);
}

int	flags(char *str)
{
	t_format	*format;
	int			i;

	format = malloc(sizeof(t_format));
	format_init(format);
	i = 0;
	while (flag_check(&str[i], format))
	{
		if (format->type)
		{
			i++;
			break ;
		}
		if (format->precision != -1 && str[i] == '.')
		{
			i++;
			i += nums(&str[i]) - 1;
		}
		if (format->width && nums(&str[i]))
			i += nums(&str[i]) - 1;
		i++;
	}
	free(format);
	return (i);
}

int	ft_printf(const char *str, ...)
{
	va_list	vars;
	int		i;
	int		ret;

	if (!str--)
		return (-1);
	va_start(vars, str);
	ret = 0;
	i = 0;
	while (*(++str))
	{
		if (*str == '%')
		{
			str++;
			i = print_arg((char *)str, vars);
			if (i == -1)
				return (-1);
			ret += i;
			str += flags((char *)str) - 1;
		}
		else
			ret += write(1, str, 1);
	}
	va_end(vars);
	return (ret);
}
