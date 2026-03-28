/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:30:40 by dplazas-          #+#    #+#             */
/*   Updated: 2026/03/28 13:02:04 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_list *find_node(char *target, t_list *envp)
{
	int	length;

	length = ft_strlen(target);
	while (envp)
	{
		if (envp->content 
			&& ft_strncmp(target, (char *)envp->content, length) == 0)
			return (envp);
		envp = envp->next;
	}
	return (envp);
}

void	change_pwds(t_list *envp, char *oldpwd_ch)
{
	t_list *oldpwd;
	t_list *newpwd;
	char	*curr_dir;
	
	oldpwd = find_node("OLDPWD=", envp);
	newpwd = find_node("PWD=", envp);
	if (oldpwd)
	{
		free(oldpwd->content);
		if (!oldpwd_ch)
			oldpwd->content = NULL;
		else
			oldpwd->content = ft_strjoin("OLDPWD=", oldpwd_ch);
	}
	curr_dir = getcwd(NULL, 0);
	if (newpwd)
	{
		free(newpwd->content);
		if (!curr_dir)
			newpwd->content = NULL;
		else
			newpwd->content = (void *)ft_strjoin("PWD=", curr_dir);
	}
	return ((void) free(oldpwd_ch), (void) free(curr_dir));
}

int	cd_builtin(t_command *cmd, t_shell *shell)
{
	char	*addr;
	char	*current_dir;

	if (arr_len(cmd->command) > 2)
		return ((void)write(1, "minishell: cd: too many arguments\n", 35), 1);
	if (arr_len(cmd->command) == 1)
		addr = ft_getenv("$HOME", shell);
	else
	{
		addr = cmd->command[1];
		if (strncmp("-", addr, 2) == 0)
		{
			addr = ft_getenv("$OLDPWD", shell);
			printf("%s\n", addr);
		}
	}
	current_dir = getcwd(NULL, 0);
	if (addr && chdir(addr) == 0)
	{
		change_pwds(*shell->envp, current_dir);
		return (0);
	}
	perror("minishell: cd");
	return (1);
}
