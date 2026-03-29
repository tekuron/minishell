/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:54 by danz              #+#    #+#             */
/*   Updated: 2026/03/28 21:12:59 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char *exit_env;
	
	if (check_cmd(line))
	{
		shell->last_exit = 2;
		ft_strlcpy(shell->exit_env, "2", 2);
		free_cmd(line, NULL, CONT, NULL);
		return ;
	}
	cmd = get_cmd(line, shell);
	if (!cmd)
		free_cmd(line, NULL, STOP, "malloc");
	// debug(cmd);
	shell->last_exit = exec_command(cmd, shell);
	exit_env = ft_itoa(shell->last_exit);
	ft_strlcpy(shell->exit_env, exit_env, 10);
	free(exit_env);
	free_cmd(line, cmd, CONT, NULL);
}
	
int	loop(t_shell shell)
{
	char		*line;

	while (1)
	{
		line = display_prompt(&shell);
		if (get_signal_status())
		{
			set_signal_status(0);
			free(line);
			rl_replace_line("", 0);
			rl_on_new_line();
			shell.last_exit = 130;
			// change_exit(*(shell.envp), shell.last_exit);
			continue ;
		}
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
	// char				*status;
	
	(void)argc;
	(void)argv;
	set_signals(START);
	// status = malloc(sizeof(char) * 15);
	// if (!status)
	// 	return (EXIT_FAILURE);
	envl = lst_from_char(envp);
	// ft_memmove(status, "?=0", 4);
	// ft_lstadd_front(&envl, ft_lstnew(status));
	shell.envp = &envl;
	shell.interactive = isatty(STDIN_FILENO);
	shell.last_exit = 0;
	shell.exit_env[0] = '0';
	shell.exit_env[1] = '\0';
	loop(shell);
	ft_lstclear(&envl, free);
	return (0);
}
