/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:57:36 by danz              #+#    #+#             */
/*   Updated: 2026/04/08 16:47:58 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredocs(t_command *cmd, int number)
{
	t_io	*redir;
	int		i;

	i = 0;
	while (cmd)
	{
		if (cmd->redirs && number != i)
		{
			redir = cmd->redirs;
			while (redir)
			{
				if (redir->rd == REDIR_HEREDOC)
					close(redir->heredoc_fd);
				redir = redir->next;
			}
		}
		i++;
		cmd = cmd->next;
	}
}

int	wait_for_children(t_process *data)
{
	int	i;
	int	status;
	int	last_exit;

	i = -1;
	last_exit = -1;
	while (++i < data->process)
	{
		while (waitpid(data->ids[i], &status, 0) == -1 && errno == EINTR)
			(void)i;
	}
	free(data->ids);
	if (WIFEXITED(status))
		last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		last_exit = 128 + WTERMSIG(status);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	set_signals(SHELL);
	return (last_exit);
}

int	initialize_data(t_process *data, t_command *cmd)
{
	data->cmd = cmd;
	data->process = t_command_size(cmd);
	data->pipes = create_pipes(data->process - 1);
	if (!data->pipes)
	{
		close_heredocs(cmd, -1);
		return (-1);
	}
	data->ids = malloc(sizeof(pid_t) * data->process);
	if (!data->ids)
	{
		close_heredocs(cmd, -1);
		free_pipes(data->pipes, data->process - 1);
		return (-2);
	}
	return (1);
}

int	exec_command(t_command *cmd, t_shell *shell)
{
	t_process	data;
	t_pair		pair;

	pair = (t_pair){0};
	pair.cont = heredoc_handling(cmd, shell);
	if (!pair.cont)
		return (130);
	pair.cont = try_builtin_parent(cmd, shell, &pair.status);
	if (pair.cont > 0)
		return (pair.status);
	data = (t_process){0};
	pair.status = initialize_data(&data, cmd);
	if (pair.status < 0)
		return (pair.status);
	pair.status = forking(shell, &data, data.process);
	close_heredocs(cmd, -1);
	if (!pair.status)
	{
		set_signals(SHELL);
		return (-3);
	}
	return (wait_for_children(&data));
}
