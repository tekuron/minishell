/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 11:39:33 by danz              #+#    #+#             */
/*   Updated: 2026/02/16 12:30:50 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*prep_cmd(char **wds, t_list *envp)
{
	t_list	*ret;

	ret = lst_from_char(wds);
	if (!ret)
		return (NULL);
	insert_env(ret, envp);
}
