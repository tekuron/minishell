/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 18:09:05 by danz              #+#    #+#             */
/*   Updated: 2026/02/18 17:51:53 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BRED "\e[1;31m"
#define BGREEN "\e[1;32m"
#define BCYAN "\e[6;36m"
#define CRESET "\e[0m"
#define PROMPT_SIZE 37

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
