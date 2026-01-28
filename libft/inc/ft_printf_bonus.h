/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:19:27 by danz              #+#    #+#             */
/*   Updated: 2025/10/10 00:16:22 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H

# include <stdarg.h>
# include <unistd.h>

typedef struct s_format
{
	int		plus;
	int		minus;
	int		zero;
	int		width;
	int		precision;
	int		alt;
	char	type;
	int		space;
	int		null;
}	t_format;

int		print_str(char *str, t_format *format);
int		print_nb(int nbr, t_format *format);
int		print_uns(unsigned int nb, t_format *format);
int		print_hex(unsigned int nb, t_format *format);
int		print_ptr(unsigned long long start, t_format *format);
int		ft_printf(const char *str, ...);
int		nums(char *str);
int		max(int a, int b);
int		print_format(t_format *format, va_list vars);
int		print_char(char c, t_format *format);
int		ft_numlen(long nb);
char	*apply_width(char *str, t_format format);
char	*apply_minus(char *str, t_format format);
int		digits_hex(unsigned long long nb);

#endif