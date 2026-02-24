/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:54 by danz              #+#    #+#             */
/*   Updated: 2026/02/24 10:33:47 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

void	debug(t_command *cmd)
{
	t_command *tmp_c = cmd;

	while (tmp_c)
	{
		char **args = tmp_c->command;
		while (args && *args)
			printf("Arg: [%s]\n", *args++);
		
		t_io *red = tmp_c->redirs;
		while (red)
		{
			printf("Redir: Type %i, Path [%s]\n", red->rd, red->path);
			red = red->next;
		}
		printf("Pipe: %i\n", tmp_c->pipe);
		tmp_c = tmp_c->next;
	}
}

int	loop(t_list *envp, struct sigaction sa[4])
{
	int			exit_code;
	t_command	*cmd;
	char		*line;

	exit_code = 0;
	(void) sa;
	while (1)
	{
		line = readline(prompt(exit_code));
		if (!append_to_history(line))
			continue ;
		if (check_cmd(line))
		{
			exit_code = 2;
			free_cmd(line, NULL, CONT, NULL);
			continue ;
		}
		cmd = get_cmd(line, envp);
		if (!cmd)
			free_cmd(line, NULL, STOP, "malloc");
		debug(cmd);
		exit_code = 0;
		exec_command(cmd, envp, sa);
		free_cmd(line, cmd, CONT, NULL);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_list				*envl;
	struct sigaction	sa[4];

	(void)argc;
	(void)argv;
	initialize_signals(sa, 4);
	sa[0].sa_handler = s_int_handler_input;
	sa[2].sa_handler = s_backslash_handler;
	sigaction(SIGINT, &sa[0], &sa[1]);
	sigaction(SIGQUIT, &sa[2], &sa[3]);
	envl = lst_from_char(envp);
	ft_lstadd_front(&envl, ft_lstnew(ft_strdup("?=0")));
	loop(envl, sa);
	ft_lstclear(&envl, free);
}
