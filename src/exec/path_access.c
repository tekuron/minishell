/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_access.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:35:13 by dplazas-          #+#    #+#             */
/*   Updated: 2026/04/03 13:43:20 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	i = -1;
	if (!cmd->command || !*cmd->command || is_dir(cmd->command[0]) == 1)
		return (NULL);
	if (ft_strchr(cmd->command[0], '/') && access(cmd->command[0], X_OK) == 0)
		return (ft_strdup(cmd->command[0]));
	paths = get_paths(shell);
	while (paths && paths[++i])
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
	}
	return ((void) free_strs(paths), NULL);
}
