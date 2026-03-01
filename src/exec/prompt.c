/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 18:09:05 by danz              #+#    #+#             */
/*   Updated: 2026/02/28 12:51:20 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BRED "\e[1;31m"
#define BGREEN "\e[1;32m"
#define BCYAN "\e[6;36m"
#define CRESET "\e[0m"
#define PROMPT_SIZE 37

char	*display_prompt(t_shell *shell)
{
	char	*line;

	if (shell->interactive)
		line = readline(prompt(shell->last_exit));
	else
		line = readline("");
	if (!line)
	{
		if (shell->interactive)
			write(1, "exit\n", 6);
		ft_lstclear(&shell->envp, free);
		rl_clear_history();
		exit(shell->last_exit);
	}
	if (shell->interactive && !append_to_history(line))
		return (NULL);
	return (line);
}

char	*prompt(int last_exit)
{
	static char	*prompt;

	if (!prompt)
		prompt = malloc(PROMPT_SIZE);
	if (last_exit != 0)
		ft_strlcpy(prompt, BRED, PROMPT_SIZE);
	else
		ft_strlcpy(prompt, BGREEN, PROMPT_SIZE);
	ft_strlcat(prompt, "ଠ "BCYAN" minishell "CRESET, PROMPT_SIZE);
	return (prompt);
}
