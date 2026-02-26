/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 22:26:11 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/26 22:23:39 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern sig_atomic_t	g_sig;

void	s_int_handler_input(int sig)
{
	(void) sig;
	g_sig = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	s_int_handler_heredoc(int sig)
{
	(void) sig;
	g_sig = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	s_backslash_handler(int sig)
{
	(void) sig;
}

void	assign_signal(int signal, void (*f )(int), t_sa *sa_old)
{
	t_sa	sa;

	sa = (struct sigaction){0};
	sa.sa_handler = f;
	sigaction(signal, &sa, sa_old);
}

void	set_signals(int	mode)
{
	static t_sa sigint_default;
	static t_sa sigquit_default;

	if (mode == START)
	{
		assign_signal(SIGINT, s_int_handler_input, &sigint_default);
		assign_signal(SIGQUIT, s_backslash_handler, &sigquit_default);
	}
	else if (mode == SHELL)
	{
		assign_signal(SIGINT, s_int_handler_input, NULL);
		assign_signal(SIGQUIT, s_backslash_handler, NULL);
	}
	else if (mode == HEREDOC)
		assign_signal(SIGINT, s_int_handler_heredoc, &sigint_default);
	else if (mode == EXECUTION)
	{
		sigaction(SIGINT, &sigint_default, NULL);
		sigaction(SIGQUIT, &sigquit_default, NULL);
	}
}