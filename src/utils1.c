/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 19:43:22 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/22 20:12:57 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	t_command_size(t_command *cmd)
{
	int	length;

	length = 0;
	while (cmd)
	{
		cmd = cmd->next;
		length++;
	}
	return (length);
}
