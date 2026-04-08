/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:12:12 by danz              #+#    #+#             */
/*   Updated: 2026/04/08 20:33:54 by danzamor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

int		export_builtin(t_command *cmd, t_list **envp);
void	export_print(t_list *envp);
int		cd_builtin(t_command *cmd, t_shell *shell);
int		exit_builtin(t_command *cmd, t_shell *shell, int fds[2]);
int		echo_builtin(t_command *cmd);
int		unset_builtin(t_command *cmd, t_list **envp);
int		pwd_builtin(t_command *cmd, t_list *envp);
int		env_builtin(t_command *cmd, t_list *envp);

#endif