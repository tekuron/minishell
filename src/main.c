/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:54 by danz              #+#    #+#             */
/*   Updated: 2026/02/01 19:32:45 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	loop(char **envp)
{
	int			exit_code;
	t_command	*cmd;
	char		*line;

	exit_code = 0;
	(void)cmd;
	while (1)
	{
		line = readline(prompt(exit_code));
		// if (!line)
		// 	cleanup_and_exit();
		cmd = get_cmd(line, envp);
		// if (!cmd)
		// 	cleanup_and_exit();
		// exec_cmd()
		// free_cmd()
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	loop(envp);
}
