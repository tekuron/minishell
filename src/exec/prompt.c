/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 18:09:05 by danz              #+#    #+#             */
/*   Updated: 2026/04/08 14:50:18 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BRED "\e[1;31m\002"
#define BGREEN "\e[1;32m\002"
#define BCYAN "\e[1;36m\002"
#define CRESET "\e[0m\002"
#define PROMPT_SIZE 100

char	*display_prompt(t_shell *shell)
{
	char	*line;

	if (shell->interactive)
		line = readline(prompt(shell->last_exit, DISPLAY_PROMPT));
	else
		line = readline("");
	if (!line)
	{
		if (shell->interactive)
			write(1, "exit\n", 6);
		ft_lstclear(shell->envp, free);
		rl_clear_history();
		prompt(shell->last_exit, EXIT_SHELL);
		exit(shell->last_exit);
	}
	if (shell->interactive && !get_signal_status() && !append_to_history(line))
		return (NULL);
	return (line);
}

char	*prompt(int last_exit, int mode)
{
	static char	*prompt;

	if (mode == EXIT_SHELL)
	{
		free(prompt);
		return (NULL);
	}
	if (!prompt)
	{
		prompt = malloc(PROMPT_SIZE);
		if (!prompt)
			return (NULL);
	}
	if (last_exit != 0)
		ft_strlcpy(prompt, BRED, PROMPT_SIZE);
	else
		ft_strlcpy(prompt, BGREEN, PROMPT_SIZE);
	ft_strlcat(prompt, "ଠ "BCYAN" minishell "CRESET, PROMPT_SIZE);
	return (prompt);
}
