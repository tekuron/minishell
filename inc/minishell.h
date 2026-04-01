/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplazas- <dplazas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:52:49 by danz              #+#    #+#             */
/*   Updated: 2026/04/01 16:31:18 by dplazas-         ###   ########.fr       */
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
# include <errno.h>
# include <limits.h>
# include "libft.h"
# include "ft_printf_bonus.h"

# define EXIT_SHELL 0
# define DISPLAY_PROMPT 1

typedef struct sigaction	t_sa;

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

typedef enum e_redir
{
	ERROR,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir;

typedef enum e_mode
{
	START,
	SHELL,
	EXECUTION,
	HEREDOC,
	IGNORE
}		t_mode;

typedef enum e_redir_state
{
	SAVE,
	RESTORE
}		t_redir_state;
typedef enum e_builtins
{
	ECHO_BI = 1,
	PWD_BI = 2,
	EXPORT_BI = 3,
	CD_BI = 4,
	UNSET_BI = 5,
	ENV_BI = 6,
	EXIT_BI = 7
}	t_builtins;

typedef struct s_pair
{
	int	cont;
	int	status;
}		t_pair;

typedef struct s_io
{
	t_redir		rd;
	char		*path;
	int			heredoc_fd;
	t_bool		has_qts;
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
void		free_cmd(char **line, t_command *cmd, int cont, char *err);

typedef struct s_process
{
	t_command			*cmd;
	pid_t				*ids;
	int					**pipes;
	int					process;
}		t_process;

typedef struct s_shell
{
	t_list	**envp;
	int		last_exit;
	char	exit_env[10];
	int		interactive;
}		t_shell;

void		set_signal_status(int sig);
int			get_signal_status(void);
void		free_pipes(int **pipes, int total);
char		*prompt(int last_exit, int mode);
t_command	*get_cmd(char *line, t_shell *shell);
void		*ft_realloc(void *ptr, size_t size, size_t new_size);
void		free_strs(char **strs);
t_list		*lst_from_char(char **wds);
int			check_cmd(char *str);
int			word_len(char *str);
char		**split_cmd(char *str);
void		insert_env(t_list **lst, t_shell *shell);
char		*ft_getenv(char *var, t_shell *shell);
char		*envcat(char *str, char *env);
size_t		envlen(char *env);
t_command	*save_cmds(t_list *cmd, t_command *ret);
int			is_redir(char *str);
int			append_to_history(char *line);
void		s_int_handler_input(int sig);
void		s_backslash_handler(int sig);
int			piping(int **pipes, int total, int id);
int			redirecting(t_command *cmd);
void		s_int_handler_heredoc(int sig);
char		*try_access(t_command *cmd, t_shell *shell);
int			try_builtin_parent(t_command *cmd, t_shell *shell, int *status);
int			**create_pipes(int total);
int			heredoc_handling(t_command *cmd, t_shell *shell);
char		**t_list_to_char(t_list *envp);
int			exec_command(t_command *cmd, t_shell *shell);
int			t_command_size(t_command *cmd);
void		*ft_realloc(void *ptr, size_t size, size_t new_size);
char		*display_prompt(t_shell *shell);
void		set_signals(int mode);
void		ft_swap(char *str, int length);
int			is_dir(char *path);
int			arr_len(char **strs);
int			ft_isspace(char c);
int			atoll_safe(char *str, long long *res);
void		remove_from_env(t_list **envp, char *str);
void		free_and_exit(t_list **envp, t_command *cmd, int err,
				int exit_code);
int			try_builtin_child(t_command *cmd, t_shell *shell, int *status);

#endif