/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:54 by danz              #+#    #+#             */
/*   Updated: 2026/02/18 12:16:47 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int	in_input = 1;

int	loop(t_list *envp)
{
	int			exit_code;
	t_command	*cmd;
	char		*line;

	exit_code = 0;
	(void)cmd;
	while (1)
	{
		line = readline(prompt(exit_code));
		if (!line)
			free_cmd(line, NULL, STOP, EXIT_FAILURE);
		if (!append_to_history(line))
				continue ;
		cmd = get_cmd(line, envp);
		if (!cmd)
			free_cmd(line, NULL, STOP, EXIT_FAILURE);
		while (cmd)
		{
			while (*(cmd->command))
				printf("%s\n", *(cmd->command++));
			while (cmd->redirs)
			{
				printf("%i %s\n", cmd->redirs->rd, cmd->redirs->path);
				cmd->redirs = cmd->redirs->next;
			}
			printf("%i\n", cmd->pipe);
			cmd = cmd->next;
		}
		// exec_cmd(cmd, envp);
		free_cmd(line, cmd, CONT, -1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*envl;
	// struct sigaction sa;

	(void)argc;
	(void)argv;
	// sa.sa_handler = s_int_handler;
	// sigaction(SIGINT, &sa, NULL);
	envl = lst_from_char(envp);
	loop(envl);
	ft_lstclear(&envl, free);
}
