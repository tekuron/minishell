/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:54 by danz              #+#    #+#             */
/*   Updated: 2026/02/25 22:01:23 by dplazas-         ###   ########.fr       */
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

void	parse_and_exec(char *line, t_shell *shell, struct sigaction sa[4])
{
	t_command	*cmd;
	
	if (check_cmd(line))
	{
		shell->last_exit = 2;
		free_cmd(line, NULL, CONT, NULL);
		return ;
	}
	cmd = get_cmd(line, shell->envp);
	if (!cmd)
		free_cmd(line, NULL, STOP, "malloc");
	debug(cmd);
	shell->last_exit = exec_command(cmd, shell->envp, sa);
	free_cmd(line, cmd, CONT, NULL);
}

char	*display_prompt(t_shell *shell)
{
	char	*line;

	if (shell->interactive)
		line = readline(prompt(shell->last_exit));
	else
		line = readline("");
	if (!line)
	{
		write(1, "exit\n", 6);
		ft_lstclear(&shell->envp, free);
		exit(shell->last_exit);
	}
	if (shell->interactive && !append_to_history(line))
		return (NULL);
	return (line);
}

int	loop(t_shell shell, struct sigaction sa[4])
{
	char		*line;

	while (1)
	{
		line = display_prompt(&shell);
		if (!line)
			continue ;
		parse_and_exec(line, &shell, sa);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_list				*envl;
	struct sigaction	sa[4];
	t_shell				shell;
	char				*status;
	
	(void)argc;
	(void)argv;
	initialize_signals(sa, 4);
	sa[0].sa_handler = s_int_handler_input;
	sa[2].sa_handler = s_backslash_handler;
	sigaction(SIGINT, &sa[0], &sa[1]);
	sigaction(SIGQUIT, &sa[2], &sa[3]);
	status = malloc(sizeof(char) * 14);
	if (!status)
		return (EXIT_FAILURE);
	envl = lst_from_char(envp);
	ft_memmove(status, "?=0\0", 4);
	ft_lstadd_front(&envl, ft_lstnew(ft_strdup("?=0")));
	shell.envp = envl;
	shell.interactive = isatty(STDIN_FILENO);
	shell.last_exit = 0;
	loop(shell, sa);
	ft_lstclear(&envl, free);
	return (0);
}
