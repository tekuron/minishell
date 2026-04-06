/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:19:41 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/06 21:50:21 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_err(char *str, char *str2, char *str3)
{
	int	length;

	if (!str)
		return ;
	length = ft_strlen(str);
	write(STDERR_FILENO, str, length);
	if (!str2)
		return ;
	length = ft_strlen(str2);
	write(STDERR_FILENO, str2, length);
	if (!str3)
		return ;
	length = ft_strlen(str3);
	write(STDERR_FILENO, str3, length);
}
