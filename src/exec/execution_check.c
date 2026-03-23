/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:35:13 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/23 19:29:37 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

int	is_builtin(t_command *cmd)
{
	if (!cmd->command || !cmd->command[0])
		return (-1); //Add check in try_builtin
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
	else if (ft_strncmp(cmd->command[0], "pwd", 4) == 0)
		return (PWD_BI);
	return (0);
}

int	execute_builtin(t_command *cmd, t_list *envp, int builtin)
{
	(void) cmd;
	(void) envp;
	(void) builtin;
	// if (builtin == ECHO_BI)
	// 	return (echo_builtin(cmd, envp));
	if (builtin == CD_BI)
		return(cd_builtin(cmd, envp));
	// else if (builtin == EXPORT_BI)
	// 	return (export_builtin(cmd, &(envp->next)));
	// else if (builtin == UNSET_BI)
	// 	return (unset_builtin(cmd, envp));
	// else if (builtin == EXIT_BI)
	// 	return (exit_builtin(cmd, envp));
	else if (builtin == ENV_BI)
		return (env_builtin(cmd, envp));
	else if (builtin == PWD_BI)
		return (pwd_builtin(cmd, envp));
	return (1);
}

int	try_builtin(t_command *cmd, t_list *envp, int *status)
{
	int	builtin;
	int	exit_status;

	builtin = 0;
	if (!cmd->next)
	{
		builtin = is_builtin(cmd);
		if (builtin && builtin != -1)
		{
			if (!redirecting(cmd))
			{
				ft_lstclear(&envp, free);
				free_cmd(NULL, cmd, STOP, "minishell");
			}
			exit_status = execute_builtin(cmd, envp, builtin);
			change_exit(envp, exit_status);
			*status = exit_status;
			return (builtin);
		}
	}
	return (-1);
}

char	**get_paths(t_list *envp)
{
	char	*path_env;
	char	**paths;
	path_env = ft_getenv("$PATH", envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

char	*try_access(t_command *cmd, t_list *envp)
{
	char	*path;
	char	*partial_path;
	char	**paths;
	int		i;

	i = 0;
	if (!cmd->command || !*cmd->command || is_dir(cmd->command[0]) == 1)
		return (NULL);
	if (access(cmd->command[0], X_OK) == 0)
		return (cmd->command[0]);
	paths = get_paths(envp);
	while (paths && paths[i])
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
	free_strs(paths);
	return (NULL);
}
