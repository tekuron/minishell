/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/02/17 23:11:43 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_cmd(char *line, t_command *cmd, int cont, int ex_status)
{
	t_command	*aux;
	t_io		*supp;
	int			i;

	i = -1;
	if (line != NULL)
		free(line);
	while (cmd != NULL)
		{
			while (cmd->command[++i])
				free(cmd->command[i]);
			while(cmd->redirs != NULL)
			{
				free(cmd->redirs->path);
				supp = cmd->redirs;
				cmd->redirs = cmd->redirs->next;
				free(supp);
			}
			aux = cmd;
			cmd = cmd->next;
			free(aux);
		}
	if (!cont)
		exit (ex_status);
}

// int	exec_command(t_command *cmd, char *envp)
// {
// 	pid_t	*ids;
	
// 	//1. Redirections (Fails -> All Programs preceeding 
// 	//   get executed, send sigint inmediately)
// 	//2. Pipes (Same)
// 	//3. Access (same)
// 	//4. Execute and store status.
// 	handle_redirection(cmd, envp);
// 	handle_pipes(cmd, envp);
// 	try_access(cmd, envp);
// 	ids = execute_all(cmd, envp);
// 	wait_and_status(cmd, ids);
// }

int	append_to_history(char *line)
{
	if (!*line)
	{
		free(line);
		return (0);
	}
	add_history(line);
	return (1);
}
