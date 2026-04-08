/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 12:53:12 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/08 15:08:51 by dplazas-         ###   ########.fr       */
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

void	fd_cloning(int mode, int fds[2], t_command *cmd, t_list **envp)
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
		if (dup2(fds[0], STDIN_FILENO) < 0 || dup2(fds[1], STDOUT_FILENO) < 0)
		{
			close(fds[0]);
			close(fds[1]);
			free_and_exit(envp, cmd, 1, 1);
		}
		close(fds[0]);
		close(fds[1]);
	}
}

int	execute_builtin(t_command *cmd, t_shell *shell, int builtin, int fds[2])
{
	if (builtin == ECHO_BI)
		return (echo_builtin(cmd));
	if (builtin == CD_BI)
		return (cd_builtin(cmd, shell));
	else if (builtin == EXPORT_BI)
		return (export_builtin(cmd, shell->envp));
	else if (builtin == UNSET_BI)
		return (unset_builtin(cmd, shell->envp));
	else if (builtin == EXIT_BI)
		return (exit_builtin(cmd, shell->envp, fds));
	else if (builtin == ENV_BI)
		return (env_builtin(cmd, *shell->envp));
	else if (builtin == PWD_BI)
		return (pwd_builtin(cmd, *shell->envp));
	return (1);
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
		exit_status = execute_builtin(cmd, shell, builtin, NULL);
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
			exit_status = execute_builtin(cmd, shell, builtin, fds);
			fd_cloning(RESTORE, fds, cmd, shell->envp);
			*status = exit_status;
			return (builtin);
		}
	}
	return (-1);
}
