/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:54 by danz              #+#    #+#             */
/*   Updated: 2026/02/26 22:10:14 by dplazas-         ###   ########.fr       */
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

void	parse_and_exec(char *line, t_shell *shell)
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
	shell->last_exit = exec_command(cmd, shell->envp);
	free_cmd(line, cmd, CONT, NULL);
}

int	loop(t_shell shell)
{
	char		*line;

	while (1)
	{
		line = display_prompt(&shell);
		if (!line)
			continue ;
		parse_and_exec(line, &shell);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_list				*envl;
	t_shell				shell;
	char				*status;
	
	(void)argc;
	(void)argv;
	set_signals(START);
	status = malloc(sizeof(char) * 14);
	if (!status)
		return (EXIT_FAILURE);
	envl = lst_from_char(envp);
	ft_memmove(status, "?=0\0", 4);
	ft_lstadd_front(&envl, ft_lstnew(ft_strdup("?=0")));
	shell.envp = envl;
	shell.interactive = isatty(STDIN_FILENO);
	shell.last_exit = 0;
	loop(shell);
	ft_lstclear(&envl, free);
	return (0);
}
