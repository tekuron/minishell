/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danzamor <danzamor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:49 by danz              #+#    #+#             */
/*   Updated: 2026/02/19 15:50:53 by danzamor         ###   ########.fr       */
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

typedef enum e_exit
{
	STOP,
	CONT
}		t_exit;

typedef enum e_mode
{
	INPUT_MODE,
	EXECUTION_MODE,
	HEREDOC_MODE
}		t_mode;

typedef enum e_redir
{
	ERROR,
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
void		free_cmd(char *line, t_command *cmd, int cont, char *err);
void		free_pipes(int **pipes, int total);
char		*prompt(int last_exit);
t_command	*get_cmd(char *line, t_list *envp);
void		*ft_realloc(void *ptr, size_t size, size_t new_size);
void		free_strs(char **strs);
t_list		*lst_from_char(char **wds);
int			check_cmd(char *str);
int			word_len(char *str);
char		**split_cmd(char *str);
void		insert_env(t_list **lst, t_list *envp);
char		*ft_getenv(char *var, t_list *envp);
t_command	*save_cmds(t_list *cmd, t_command *ret);
int			is_redir(char *str);
int			append_to_history(char *line);
void		s_int_handler(int sig);
void		s_backslash_handler(int sig);
void	piping(int **pipes, int total, int id);
void	redirecting(t_command *cmd);

#endif