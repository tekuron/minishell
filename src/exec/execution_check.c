/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:35:13 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/29 18:50:45 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

int	is_builtin(t_command *cmd)
{
	if (!cmd->command || !cmd->command[0])
		return (-1);
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

int	execute_builtin(t_command *cmd, t_shell *shell, int builtin)
{
	if (builtin == ECHO_BI)
		return (echo_builtin(cmd));
	if (builtin == CD_BI)
		return(cd_builtin(cmd, shell));
	else if (builtin == EXPORT_BI)
		return (export_builtin(cmd, shell->envp));
	else if (builtin == UNSET_BI)
		return (unset_builtin(cmd, shell->envp));
	else if (builtin == EXIT_BI)
		return (exit_builtin(cmd, shell->envp));
	else if (builtin == ENV_BI)
		return (env_builtin(cmd, *shell->envp));
	else if (builtin == PWD_BI)
		return (pwd_builtin(cmd, *shell->envp));
	return (1);
}

void	fd_cloning(int	mode, int fds[2], t_command *cmd, t_list **envp)
{	
	if (mode == SAVE)
	{
		fds[0] = dup(STDIN_FILENO);
		if (fds[0] < 0)
			free_and_exit(envp, cmd, 1, 1);
		fds[1] = dup(STDOUT_FILENO);
		if (fds[1] < 0)
		{
			close(fds[0]);
			free_and_exit(envp, cmd, 1, 1);
		}
	}
	if (mode == RESTORE)
	{
		if (dup2(fds[0], STDIN_FILENO) < 0|| dup2(fds[1], STDOUT_FILENO) < 0)
		{
			close(fds[0]);
			close(fds[1]);
			free_and_exit(envp, cmd, 1, 1);
		}
		close(fds[0]);
		close(fds[1]);
	}
}

int	try_builtin_child(t_command *cmd, t_shell *shell, int *status)
{
	int		builtin;
	int		exit_status;
	
	builtin = 0;
	builtin = is_builtin(cmd);
	if (builtin && builtin != -1)
	{
		if (!redirecting(cmd))
		{
			perror("minishell");
			return (1);
		}
		exit_status = execute_builtin(cmd, shell, builtin);
		*status = exit_status;
		return (builtin);
	}
	return (-1);
}

int	try_builtin_parent(t_command *cmd, t_shell *shell, int *status)
{
	int		builtin;
	int		exit_status;
	int		fds[2];
	
	builtin = 0;
	if (!cmd->next)
	{
		builtin = is_builtin(cmd);
		if (builtin && builtin != -1)
		{
			fd_cloning(SAVE, fds, cmd, shell->envp);
			if (!redirecting(cmd))
			{
				fd_cloning(RESTORE, fds, cmd, shell->envp);
				return (perror("minishell"), 1);
			}
			exit_status = execute_builtin(cmd, shell, builtin);
			fd_cloning(RESTORE, fds, cmd, shell->envp);
			*status = exit_status;
			return (builtin);
		}
	}
	return (-1);
}

char	**get_paths(t_shell *shell)
{
	char	*path_env;
	char	**paths;
	
	path_env = ft_getenv("$PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

char	*try_access(t_command *cmd, t_shell *shell)
{
	char	*path;
	char	*partial_path;
	char	**paths;
	int		i;

	i = 0;
	if (!cmd->command || !*cmd->command || is_dir(cmd->command[0]) == 1)
		return (NULL);
	if (access(cmd->command[0], X_OK) == 0)
		return (ft_strdup(cmd->command[0]));
	paths = get_paths(shell);
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
