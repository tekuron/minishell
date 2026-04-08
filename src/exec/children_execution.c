/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 13:28:20 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/08 15:12:12 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution_message(t_list **envp, t_process *data, t_command *cmd)
{
	if (!ft_strncmp(cmd->command[0], ".", 2))
	{
		write_err("minishell: .: filename argument required\n", NULL, NULL);
		write_err(".: usage . filename [arguments]\n", NULL, NULL);
		free_and_exit(envp, data->cmd, 0, 2);
	}
	else if (!ft_strncmp(cmd->command[0], "..", 3))
		free_and_exit(envp, data->cmd, 0, 127);
	else if (ft_strchr(cmd->command[0], '/') && is_dir(cmd->command[0]) == 1)
		write_err("minishell: ", cmd->command[0], ": Is a directory\n");
	else if (ft_strchr(cmd->command[0], '/') && !access(cmd->command[0], F_OK)
		&& access(cmd->command[0], X_OK) < 0)
	{
		perror(cmd->command[0]);
		free_and_exit(envp, data->cmd, 0, 126);
	}
	else
		write_err("minishell: ", cmd->command[0], ": command not found\n");
	free_and_exit(envp, data->cmd, 0, 127);
}

void	run_command_bi(t_process *data, t_shell *shell, t_command *cmd)
{
	t_pair		pair;

	pair.cont = try_builtin_child(cmd, shell, &pair.status);
	if (pair.cont > 0)
		free_and_exit(shell->envp, data->cmd, 0, pair.status);
}

void	handle_child(t_process *data, t_shell *shell, int total)
{
	char		*route;
	char		**real_envp;
	int			err;
	t_command	*cmd;

	cmd = t_command_index(data->cmd, data->process);
	err = !piping(data->pipes, total, data->process) || !redirecting(cmd);
	free_pipes(data->pipes, total - 1);
	if (err || !data->cmd->command || !*data->cmd->command)
		free_and_exit(shell->envp, data->cmd, err, err);
	run_command_bi(data, shell, cmd);
	route = try_access(cmd, shell);
	if (!route)
		execution_message(shell->envp, data, cmd);
	real_envp = t_list_to_char(*shell->envp);
	if (execve(route, cmd->command, real_envp) == -1)
	{
		free_strs(real_envp);
		free(route);
		free_and_exit(shell->envp, data->cmd, 1, 127);
	}
}

int	forking(t_shell *shell, t_process *data, int total)
{
	int	i;

	i = 0;
	set_signals(IGNORE);
	while (i < total)
	{
		data->process = i;
		data->ids[i] = fork();
		if (data->ids[i] < 0)
			return (0);
		if (data->ids[i] == 0)
		{
			free(data->ids);
			close_heredocs(data->cmd, i);
			set_signals(EXECUTION);
			handle_child(data, shell, total);
		}
		i++;
	}
	data->process = total;
	free_pipes(data->pipes, total - 1);
	return (1);
}
