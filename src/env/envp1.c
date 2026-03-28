/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 11:39:33 by danz              #+#    #+#             */
/*   Updated: 2026/03/28 12:51:58 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char *var, t_shell *shell)
{
	size_t	len;
	t_list *envp;

	len = 0;
	envp = *shell->envp;
	while (*var && *var != '$')
		var++;
	if (!(*var))
		return (NULL);
	if (!ft_strncmp(var, "$?", 3))
		
	var++;
	while (ft_isalnum(var[len]) || var[len] == '_')
		len++;
	if (var[len] == '?')
		len++;
	while (envp)
	{
		if (!ft_strncmp(var, (char *)envp->content, len)
			&& ((char *)envp->content)[len] == '=')
			return ((char *)(envp->content) + len + 1);
		envp = envp->next;
	}
	return (NULL);
}

size_t	envlen(char *env)
{
	size_t len;

	len = 0;
	while (env[len] && env[len] != '=' && env[len] != '+')
		len++;
	return (len);
}
