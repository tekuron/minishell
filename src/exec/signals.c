/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 22:26:11 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/08 20:05:28 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

void	set_signal_status(int sig)
{
	g_sig = sig;
}

int	get_signal_status(void)
{
	return (g_sig);
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

void	set_signals(int mode)
{
	static t_sa	sigint_default;
	static t_sa	sigquit_default;

	if (mode == START && check_tty())
	{
		assign_signal(SIGINT, s_int_handler_input, &sigint_default);
		assign_signal(SIGQUIT, s_backslash_handler, &sigquit_default);
		assign_signal(SIGPIPE, s_backslash_handler, NULL);
		rl_event_hook = exit_prompt;
		rl_catch_signals = 0;
	}
	else if (mode == SHELL && check_tty())
	{
		assign_signal(SIGINT, s_int_handler_input, NULL);
		assign_signal(SIGQUIT, s_backslash_handler, NULL);
	}
	else if (mode == HEREDOC && check_tty())
		assign_signal(SIGINT, s_int_handler_heredoc, NULL);
	else if (mode == EXECUTION && check_tty())
	{
		sigaction(SIGINT, &sigint_default, NULL);
		sigaction(SIGQUIT, &sigquit_default, NULL);
	}
	else if (mode == IGNORE && check_tty())
		assign_signal(SIGINT, s_backslash_handler, NULL);
}
