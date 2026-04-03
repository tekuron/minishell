/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 12:46:57 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/03 18:19:28 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sig;

void	s_int_handler_input(int sig)
{
	(void) sig;
	if (!g_sig)
		write(1, "^C", 2);
	g_sig = 1;
}

void	s_int_handler_heredoc(int sig)
{
	(void) sig;
	if (!g_sig)
		write(1, "^C", 2);
	g_sig = 1;
}

void	s_backslash_handler(int sig)
{
	(void) sig;
}
