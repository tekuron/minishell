/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:54 by danz              #+#    #+#             */
/*   Updated: 2026/02/18 23:03:05 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	current_mode = INPUT_MODE;

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
			free_cmd(NULL, NULL, STOP, NULL);
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
		exit_code = 0;
		// exec_cmd(cmd, envp);
		free_cmd(line, cmd, CONT, NULL);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*envl;
	struct sigaction sa;

	(void)argc;
	(void)argv;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = s_int_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = s_backslash_handler;
	sigaction(SIGQUIT, &sa, NULL);
	envl = lst_from_char(envp);
	ft_lstadd_front(&envl, ft_lstnew(ft_strdup("?=0")));
	loop(envl);
	ft_lstclear(&envl, free);
}
