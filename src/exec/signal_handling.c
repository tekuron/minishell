/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 22:26:11 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/23 16:11:33 by dplazas-         ###   ########.fr       */
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

void	assign_signal(int signal, void (*f )(int), t_sa *sa_old)
{
	t_sa	sa;

	sa = (struct sigaction){0};
	sa.sa_handler = f;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(signal, &sa, sa_old);
}

int	exit_prompt(void)
{
	if (g_sig)
	{
		rl_replace_line("", 0);
		rl_done = 1;
	}
	return (0);
}

void	set_signals(int	mode)
{
	static t_sa sigint_default;
	static t_sa sigquit_default;

	if (mode == START)
	{
		assign_signal(SIGINT, s_int_handler_input, &sigint_default);
		assign_signal(SIGQUIT, s_backslash_handler, &sigquit_default);
		rl_event_hook = exit_prompt;
		rl_catch_signals = 0;
	}
	else if (mode == SHELL)
	{
		assign_signal(SIGINT, s_int_handler_input, NULL);
		assign_signal(SIGQUIT, s_backslash_handler, NULL);
	}
	else if (mode == HEREDOC)
		assign_signal(SIGINT, s_int_handler_heredoc, NULL);
	else if (mode == EXECUTION)
	{
		sigaction(SIGINT, &sigint_default, NULL);
		sigaction(SIGQUIT, &sigquit_default, NULL);
	}
	else if (mode == IGNORE)
		assign_signal(SIGINT, s_backslash_handler, NULL);
}

