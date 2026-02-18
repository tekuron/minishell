/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:49 by danz              #+#    #+#             */
/*   Updated: 2026/02/17 23:12:13 by dplazas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <string.h>
# include <termios.h>
# include <term.h>
# include "libft.h"

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef	enum e_exit
{
	STOP,
	CONT
}		t_exit;

typedef enum e_redir
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir;

typedef struct s_io
{
	t_redir		rd;
	char		*path;
	struct s_io	*next;
}	t_io;

t_io		*t_io_new(char *path, t_redir rd);
void		t_io_append(t_io *top, t_io *new);

typedef struct s_command
{
	char				**command;
	t_io				*redirs;
	t_bool				pipe;
	struct s_command	*next;
}	t_command;

t_command	*t_command_new(char **cmd);
void		t_command_append(t_command *top, t_command *new);
void		t_command_free(t_command *cmd);
void		free_cmd(char *line, t_command *cmd, int cont, int ex_status);
char		*prompt(int last_exit);
t_command	*get_cmd(char *line, t_list *envp);
void		*ft_realloc(void *ptr, size_t size, size_t new_size);
t_list		*lst_from_char(char **wds);
char		**split_cmd(char *str);
void		insert_env(t_list **lst, t_list *envp);
char		*ft_getenv(char *var, t_list *envp);
t_command	*save_cmds(t_list *cmd);
int			is_redir(char *str);
int			append_to_history(char *line);


#endif