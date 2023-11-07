/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 01:15:53 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 21:16:44 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/time.h>
# include <string.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <limits.h>

# define PROMPT "\001\033[1;33m\002minishell$ \001\033[0m\002"

# define RET_COMMAND_NOT_FOUND 127
# define RET_CANNOT_EXECUTE    126

# define COMMAND_NOT_FOUND "command not found"
# define IS_A_DIRECTORY "Is a directory"

# define AMBIGUOUS 42

# define CD "cd"
# define ECHOO "echo"
# define ENV "env"
# define EXIT "exit"
# define EXPORT "export"
# define PWD "pwd"
# define UNSET "unset"

# define F_SETPIPE_SZ   1031
# define BUF_SIZE 		524288

extern int					g_status;
typedef enum e_tokentype	t_tokentype;

enum e_tokentype
{
	STR = 1,
	STR_QUOTED,
	HEREDOC,
	I_FILE,
	O_FILE_TRUNC,
	O_FILE_APPEND,
	PIPE,
	UNKNOWN
};

typedef struct s_exec
{
	int		pipe[2];
	int		prev_pipe;
	int		ret_value;
	pid_t	pid;
	pid_t	wpid;
	int		status;
	int		curr_heredoc;
}	t_exec;

typedef struct s_env
{
	char			*key;
	char			*value;
}	t_env;

typedef struct s_str
{
	char				*data;
	enum e_tokentype	data_type;
	enum e_tokentype	type;
	struct s_str		*next;
}	t_str;

typedef struct s_cmd
{
	t_str	*args;
	t_str	*redirs;
	t_env	*env;
	char	**paths;
	char	**command;
	char	*cmd_path;
	char	*error_path;
	char	*error_message;
	int		ret_value;
	bool	exec;
}	t_cmd;

typedef struct s_token
{
	char				*content;
	t_tokentype			type;
	bool				expand;
	bool				join;
	struct s_token		*prev;
	struct s_token		*next;
}	t_token;

typedef struct s_tokens
{
	t_token	*first;
	t_token	*last;
}	t_tokens;

typedef struct s_msh
{
	t_tokens	tokens;
	t_list		*pipeline;
	t_exec		exec;
	t_env		*env;
	int			*heredocs;
	bool		syntax_error;
	int			heredoc_nb;
	int			lines_counter;
}	t_msh;

typedef struct s_heredoc_line
{
	char					*line;
	struct s_heredoc_line	*next;
}	t_heredoc_line;

typedef struct s_heredoc
{
	t_heredoc_line		*first;
	t_heredoc_line		*last;
	int					lines_counter;
	bool				expand;
}	t_heredoc;

int			lexer(char *buffer, t_msh *msh);
void		clear_tokens(t_tokens *tokens);
void		print_tokens(t_tokens *tokens);
bool		is_redir(int id_token);
bool		command_file_or_name(t_token *tmp, t_tokens *tokens);
int			identify_delimiter(char *buffer, int pos);
bool		ft_realloc(char **buffer, int size);
void		print_tokens(t_tokens *tokens);
bool		add_token_list(char *content, int id_token, t_tokens *tokens);
int			tokenizer(t_msh *msh, t_tokens *tokens, char *buffer);
int			size_token(char *buffer, char *delimiters, t_token *tmp);
int			calc_size(char *buffer, char *delimiters, int start);
void		skip_spaces(char *buffer, int *i);
bool		is_metachar(char c);
int			push_token_str(char *buffer, int *i, t_tokens *tokens);
bool		push_metachar_token(char *buffer, int *i, t_tokens *tokens);
t_tokentype	get_metachar_tokentype(char *buffer, int *i);
bool		push_back_token(t_tokens *tokens);
int			get_unquoted_data(char *buffer, char **data);
int			get_quoted_data(char *buffer, char **data);
t_tokens	*init_token_list(void);
bool		parse_env(t_env **env, char **envp);
char		*get_env_value(t_env *env, char *key);
bool		env_replace_value(t_env *env, char *key, char *new_value);
void		clear_env(t_env **env);
void		env_delete_element(t_env *env, char *key);
bool		is_str(int c);
bool		parser(t_tokens *tokens, t_list **pipeline, t_env *env);
bool		parse_command(t_list **pipeline, t_token **curr, t_cmd **command);
bool		push_args_command(t_token **curr, t_cmd **command);
bool		push_redirs_command(t_token **curr, t_cmd **command);
bool		push_back_command(t_list **pipeline, t_cmd **command);
void		print_commands(t_list *pipeline);
void		clear_commands(t_list **pipeline);
char		*expander(char *str, t_env *env);
t_str		*ft_strnew(char *data, int data_type, int type);
void		ft_stradd_back(t_str **lst, t_str *new);
void		ft_strclear(t_str **lst);
bool		is_join(t_tokens *tokens, char *buffer, int *i);
bool		get_args_content(t_str **curr_data, t_token **curr,
				t_cmd **command);
int			exec(t_msh *msh);
int			ft_strsize(t_str *lst);
bool		parse_path(t_cmd **cmd, t_env *env, char *path);
void		exit_error_command(t_msh *msh, bool fork, char *msg);
int			execute_errors(t_msh *msh, t_cmd *cmd, int ret_value, bool invalid);
bool		get_cmd_path(t_cmd **cmd);
void		ft_close(int *fd);
void		free_struct(t_cmd *cmd, bool fd1, bool fd2, t_exec *exec);
void		close_pipes(t_exec *exec, bool fd1, bool fd2);
char		**join_env_to_arr(t_env *env);
int			env_len(t_env *env);
void		ft_close_pipe(int fd[2]);
void		handle_sigint(int sig);
void		clear_msh(t_msh *msh, bool env_clear);
void		handle_sig_in_fork(int sig);
int			setup_signals(void);
void		case_null_redir(t_str *curr_data, t_token **curr,
				t_cmd **command, char **arr);
void		child(t_msh *msh, t_cmd **cmd, t_list *curr);
void		do_redir(t_msh *msh, t_str *tmp, int *fd, int in_out);
int			size_element(char *str, int *index);
int			count_words(char *str);
void		replace_spaces(char *str);
void		replace_spaces_back(char *str);
void		case_null_args(t_str *curr_data, t_token **curr, t_cmd **command);
bool		add_args(t_str *curr_data, t_cmd **command);
int			size_tokens(t_tokens *tokens);
int			init_heredoc(t_msh *msh, int *curr_heredoc, char *eof, bool expand);
int			create_heredocs(t_msh *msh);
int			read_heredoc(t_msh *msh, t_heredoc *heredoc,
				char *eof, int pipe[2]);
void		handle_sig_in_heredoc(int sig);
void		close_fd_valid(t_msh *msh, int *heredocs, int heredoc_nb);
bool		add_redirs(t_str *curr_data, t_cmd **command, char **arr);
int			number_of_heredocs(t_list *list);

int			get_env_line(char *name, t_env *env);
char		*get_env(char *name, t_env *env);

//builtins
void		ft_echo(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork);
void		cd(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork);
void		pwd(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork);
void		ft_exit(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork);
void		export(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork);
void		unset(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork);
void		export_bis(t_msh *msh, char **args);
//utils
// t_env		*ft_stacknew(char *line, int index);
// void		ft_stackadd_front(t_env **lst, t_env *new);
// t_env		*ft_stacklast(t_env *lst);
int			verif_overflow(char *argv);
int			size(t_env	*env);
void		env_delete_element(t_env *env, char *key);

//utils2
t_env		*ft_add_elem_env(t_env *env, char *key, char *value);
char		*concaten_value(char **args);
void		free_old_env(t_env *env);
t_env		*copy_part_env(t_env *env, int start, int finish);

//utils3
t_env		*ft_rm_elem_env(t_env *env, char *key);
t_env		*ft_rm_elem_env_ter(t_env *env, t_env *part1, int size1, int pos);
bool		is_builtin(t_msh *msh, t_list *curr, t_cmd **cmd, bool fork);

//env
bool		env_replace_value(t_env *env, char *key, char *new_value);
char		*get_env_value(t_env *env, char *key);
bool		print_env(t_env *env, char *builtin);
// static int	get_env_size(char **envp);
bool		parse_env(t_env **env, char **envp);
int			nb_cmd( t_list *pipeline);
void		ft_env(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork);
bool		init_cmds(t_cmd **cmd);
void		pipes(t_msh *msh, t_list *curr);
void		init_fds(t_msh *msh, t_cmd *cmd, int *fd_in, int *fd_out);

bool		init_builtins(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork);
void		dupclose(int (*save)[2]);
bool		pipes_no_fork(t_msh *msh, t_list *curr);
void		end_builtins(t_msh *msh, bool fork, int (*save)[2]);
bool		check_print(int ret, char *builtin);
bool		fill_tokens_info(t_tokens *tokens, char *data,
				int type, bool expand);
void		exit_heredoc(t_msh *msh, bool fork, t_heredoc *heredoc, char *msg);
int			init_heredoc(t_msh *msh, int *curr_heredoc, char *eof, bool expand);
void		fill_heredocs(t_msh *msh);
void		free_heredoc(t_heredoc *heredoc);
int			read_heredoc(t_msh *msh, t_heredoc *heredoc, char *eof,
				int pipe[2]);
char		*find_eof(t_list *first, int heredoc_nb, bool *expand);
bool		end_heredoc(t_msh *msh, char *line, char *eof);
bool		push_back_heredoc(t_msh *msh, t_heredoc *heredoc, char *line);
void		write_heredoc(t_msh *msh, t_heredoc *heredoc, int pipe[2]);
int			is_numeric(char *str, int *dst);
int			is_valid_identifier(char *str);
void		set_pwd(t_msh *msh);
void		set_shlvl(t_msh *msh);
void		set_env_helper(t_msh *msh, char *key, char *value);

#endif