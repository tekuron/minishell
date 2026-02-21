/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:35:13 by dplazas-          #+#    #+#             */
/*   Updated: 2026/02/21 18:52:49 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_command *cmd)
{
	if (ft_strncmp(cmd->command[0], "echo", 5) == 0)
			return (ECHO_BI);
	else if (ft_strncmp(cmd->command[0], "cd", 3) == 0)
			return (CD_BI);
	else if (ft_strncmp(cmd->command[0], "export", 7) == 0)
			return (EXPORT_BI);
	else if (ft_strncmp(cmd->command[0], "unset", 6) == 0)
			return (UNSET_BI);
	else if (ft_strncmp(cmd->command[0], "exit", 5) == 0)
			return (EXIT_BI);
	else if (ft_strncmp(cmd->command[0], "env", 4) == 0)
			return (ENV_BI);
	return (0);
}

int	execute_builtin(t_command *cmd, t_list *envp, int builtin)
{
	(void) cmd;
	(void) envp;
	(void) builtin;
	// if (builtin == ECHO)
	// 	echo_builtin(arr_len(cmd, envp));
	// else if (ft_strncmp(cmd->command[0], "cd", 3) == 0)
	// 	cd_builtin(cmd, envp);
	// else if (ft_strncmp(cmd->command[0], "export", 7) == 0)
	// 	export_builtin(cmd, envp);
	// else if (ft_strncmp(cmd->command[0], "unset", 6) == 0)
	// 	unset_builtin(cmd, envp);
	// else if (ft_strncmp(cmd->command[0], "exit", 5) == 0)
	// 	exit_builtin(cmd, envp);
	// else if (ft_strncmp(cmd->command[0], "env", 4) == 0)
	// 	env_builtin(cmd, envp);
	return (1);
}

int	try_builtin(t_command *cmd, t_list *envp)
{
	int	builtin;

	if (!envp)
		return (-1); //handle envp error
	if (!cmd->next)
	{
		builtin = is_builtin(cmd);
		if (builtin)
		{
			redirecting(cmd); //handle failure of dup2 and open
			execute_builtin(cmd, envp, builtin);
		}	
	}
	return (0);
}

char	*try_access(t_command *cmd)
{
	char 	*path;
	char	*partial_path;
	char	**paths;
	int		i;

	i = 0;
	if (access(cmd->command[0], X_OK))
		return (cmd->command[0]);
	paths = ft_split(getenv("PATH"), ':');
	while (paths && path[i])
	{
		partial_path = ft_strjoin(paths[i], "/");
		if (!partial_path)
			return (free_strs(paths), NULL);
		path = ft_strjoin(partial_path, cmd->command[0]);
		free(partial_path);
		if (!path)
			return (free_strs(paths), NULL);
		if (access(path, X_OK) == 0)
			return (free_strs(paths), path);
		free(path);
		i++;
	}
	return (NULL);
}