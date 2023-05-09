/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:51 by math              #+#    #+#             */
/*   Updated: 2023/05/09 16:56:57 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <libft.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>

# define BUILTINS_EXPORT "export"
# define BUILTINS_UNSET "unset"
# define BUILTINS_ENV "env"
# define BUILTINS_EXIT "exit"
# define BUILTINS_PWD "pwd"
# define BUILTINS_CD "cd"
# define BUILTINS_ECHO "echo"

# define SEPARATOR (char)29

# ifndef PATH_MAX
# define PATH_MAX 1024
#endif

/// @brief 
/// TK_CMD ls, cd, grep...,
/// TK_GREAT >,
///	TK_LESS <,
///	TK_GREATGREAT >>,
///	TK_LESSLESS <<,
///	TK_GREATAMPERSAND >&,
///	TK_PIPE |,
///	TK_AMPERSAND &,
///	TK_DOUBLEQUOTE ",
///	TK_SINGLEQUOTE ',
/// TK_DOLLAR_SIGN $,
///	TK_CMD_SUBS_START $(,
///	TK_CMD_SUBS_END ),
///	TK_CMD_SUBS_BACKTICK `,
///	TK_SEMICOLON ;,
///	TK_CURLYBRACES_OPEN {,
///	TK_CURLYBRACES_CLOSE },
///	TK_PARENTHESES_OPEN (,
///	TK_PARENTHESES_CLOSE ),
///	TK_COND_AND &&,
///	TK_COND_OR ||,
///	TK_VAR_ASSIGN =,
///	TK_BACKSLASH \,
typedef enum e_token_type
{
	TK_UNKNOWN = 0,
	TK_END = -1,
	TK_GREAT = (int32_t)'>',
	TK_LESS = (int32_t)'<',
	TK_PIPE = (int32_t)'|',
	TK_AMPERSAND = (int32_t)'&',
	TK_GREATGREAT = TK_GREAT * TK_GREAT + TK_GREAT,
	TK_LESSLESS = TK_LESS * TK_LESS + TK_LESS,
	TK_OR = TK_PIPE * TK_PIPE + TK_PIPE,
	TK_AND = TK_AMPERSAND * TK_AMPERSAND + TK_AMPERSAND,
	TK_DOUBLEQUOTE = (int32_t)'"',
	TK_DASH = (int32_t)'-',
	TK_DASHDASH = TK_DASH * TK_DASH + TK_DASH,
	TK_SINGLEQUOTE = (int32_t)'\'',
	TK_DOLLAR_SIGN = (int32_t)'$',
	TK_LAST_PIPE_EXIT = TK_DOLLAR_SIGN * '?' + '?',
	TK_SEMICOLON = (int32_t)';',
	TK_BACKSLASH = (int32_t)'\\',
	TK_VAR_ASSIGN = (int32_t)'=',
	TK_WILDCARD = (int32_t)'*'
}				t_token_type;

/// @brief 
///	ESC_DOUBLEQUOTE ",
///	ESC_BACKSLASH \,
///	ESC_DOLLARSIGN $,
///	ESC_NEWLINE n,
///	ESC_TAB t,
///	ESC_VTAB v,
///	ESC_CARRIAGE_RETURN r,
///	ESC_FORM_FEED f,
///	ESC_HEX_DIGIT x
///	ESC_BACKSPACE b
typedef enum e_escaped_char
{
	ESC_DOUBLEQUOTE = '\"',
	ESC_BACKSLASH = '\\',
	ESC_DOLLARSIGN = '$',
	ESC_NEWLINE = '\n',
	ESC_TAB = '\t',
	ESC_VTAB = '\v',
	ESC_CARRIAGE_RETURN = '\r',
	ESC_FORM_FEED = '\f',
	ESC_HEX_DIGIT = '\x0',
	ESC_BACKSPACE = '\b',
	ESC_MINUS = '-',
	ESC_PLUS = '+',
	ESC_EXCLAMATION_MARK = '!',
	ESC_QUESTION_MARK = '?',
	ESC_PERCENT = '%'
}				t_escaped_char;
typedef enum e_continuation_char
{
	CONTC_BACKSLASH = '\\',
	CONTC_PIPE = '|',
	CONTC_AMPERSAND = '&',
	CONTC_SEMICOLON = ';',
	CONTC_DOUBLE_AMPERSAND = CONTC_AMPERSAND * CONTC_AMPERSAND +
	CONTC_AMPERSAND,
	CONTC_DOUBLE_PIPE = CONTC_PIPE * CONTC_PIPE + CONTC_PIPE
}			t_continuation_char;

/// @brief the type of "command sequences"
/// CMD_PIPE = echo "|" | command2\n
/// CMD_SEQUENTIAL = command1; command2;
/// CMD_LOG_AND = command1 && command2
/// CMD_LOG_OR = command1 || command2
/// CMD_BACKGROUND_EXEC = command &
/// CMD_SUBSTITUTION = $(command)
/// CMD_INPUT_REDIRECT = command < input_file
/// CMD_OUTPUT_REDIRECT = command > output_file
/// CMD_APPEND_OUTPUT_REDIRECT = command >> output_file
/// CMD_GROUPING_PARENTHESE = ( command1; command2 )
/// CMD_GROUPING_CURLYBRACE = { command1; command2; }
typedef enum e_cmd_seq
{
	CMD_NONE = 0,
	CMD_PIPE = TK_PIPE,
	CMD_LOG_AND = TK_AND,
	CMD_LOG_OR = TK_OR,
	CMD_BACKGROUND_EXEC = TK_AMPERSAND,
	CMD_FILEOUT = TK_GREATGREAT,
	CMD_FILEIN = TK_LESSLESS,
	CMD_SEQUENTIAL = TK_SEMICOLON
}			t_cmd_seq;

typedef struct s_redirect
{
	int32_t		fd_in;
	int32_t		fd_out;
	char		*file_in;
	char		*file_out;
}				t_redirect;

typedef struct s_pipe
{
	int32_t		fd_in;
	int32_t		fd_out;
}				t_pipe;

typedef struct s_cmd
{
	struct s_cmd	*next;
	struct s_cmd	*prev;
	char			*name; /// the name of the command: cat, ls, echo ect...
	char			*full_path_name; /// only for execve, the full path name to the command ex: /bin/ls or /bin/cat
	char			**args; /// a terminating NULL list of string containing options and arguments
	char			**options; /// a terminating NULL list of string containing only options
	bool			is_builtin; /// is the command a builtins command?
	t_cmd_seq		cmd_seq_type;
	t_pipe			*pipe;
	t_redirect		*redirect;
	pid_t			pid;
}				t_cmd;

/// token->value contains the value of the token but whithout the termination char.
/// if cmd = echo -n "fewfew" and token is -, value == -n "fewfew"
typedef struct s_token
{
	struct s_token	*next;
	struct s_token	*prev;
	char			*start;
	int32_t			len;
	int32_t			pos;
	t_token_type	type;
}				t_token;

/// @brief A token group is just a group of tokens.
/// each group of token end by one of the endings token type.
typedef struct s_token_group
{
	struct s_token_group	*next;
	struct s_token_group	*prev;
	char					*start;
	char					*end;
	char					*parsed_str;
	int32_t					len;
	t_token					*first;
	t_token					*last;
	int32_t					token_count;
}				t_token_group;

typedef struct s_env
{
	char					*variable;
	char					*value;
	struct s_env_cpy		*next;

}							t_env_cpy;

typedef struct s_data
{
	int32_t			argc;
	char			*str_cmds;
	char			**argv;
	char			**env;
	t_env_cpy		*env_cpy;
	char			**paths;
	int32_t			cmds_count;
	int32_t			tokens_count;
	int32_t			token_groups_count;	
	t_token			*tokens;
	t_token_group	*token_groups;
	t_cmd			*cmds;
	t_cmd			*last_cmd;
	t_token			*last_token;
	t_token_group	*last_token_group;
}				t_data;

/// @brief Thehe entities functions
t_data			*get_data(void);
t_token			*get_first_token(void);
t_token			*add_token(char *str, int32_t char_pos, t_token_type type,
					t_token_group *group);
t_token_group	*add_token_group(char *start, char *end);
t_cmd			*add_cmd(void);
t_token_type	get_token_type(char *str);
int32_t			*get_token_counter(void);
t_token			*remove_token(t_token *tokens);
int32_t			get_token_type_count(t_token_type type);
t_token			*new_token();
t_cmd			*new_cmd();
t_cmd			*get_first_cmd(void);
t_token_group	*get_first_token_group(void);
t_token_group	*new_token_group();
char			**get_builtins_cmd(void);

/// @brief Simples and short helpers methods.
char			*get_end_of_cmd(char *str);
int32_t			get_token_type_len(t_token_type type);
bool			type_is_end_of_seq(t_token_type type);
bool			is_escaped_single_quote(char *str, int32_t i);
bool			is_escaped_double_quote(char *str, int32_t i);
bool			is_opening_single_quote(char *str, int32_t i);
bool			is_closing_single_quote(char *str, int32_t i);
bool			is_opening_double_quote(char *str, int32_t i);
bool			is_closing_double_quote(char *str, int32_t i);
bool			is_opening_parenthese(char *str, int32_t i);
bool			is_closing_parenthese(char *str, int32_t i);
bool			is_opening_curlybrace(char *str, int32_t i);
bool			is_closing_curlybrace(char *str, int32_t i);
char			*join_free2(const char *path, char *path2);
char			*join_free1(char *path, const char *path2);
char			*join(const char *path, const char *path2);
char			*join_free(char *path, char *path2);
bool			file_is_exec(char *absolute_path_to_file);
/// get full path from relative path.
char			*get_full_path(char *cmd_name);

t_token			*tokenize(char *str);
int32_t			tokenize_curlybrace(char *str, int32_t i);
int32_t			tokenize_parenthese(char *str, int32_t i);
int32_t			tokenize_double_quote(char *str, int32_t i, t_token_group *group);
int32_t			tokenize_single_quote(char *str, int32_t i, t_token_group *group);

int32_t			increment_counter(t_token_type type);
int32_t			decrement_counter(t_token_type type);

char			**parse_env_path(char **envp);
t_cmd			*parse_cmds(t_token *token);
t_token			*get_token_at(int32_t index);
bool			is_end_of_seq(t_token *token);

void			close_pipe_fds(t_cmd *cmd);

void			*free_all(void);
void			*free_all_and_exit(void);
t_pipe			*new_pipe(t_cmd *cmd);
void			*free_pipe(t_cmd *cmd);
t_redirect		*new_redirect(t_cmd *cmd);
void			*free_redirect(t_cmd *cmd);

void			*free_cmd(t_cmd *cmd);
char			*ft_strdupn(const char *s1, size_t n);
void 			init_data(int32_t argc, char **argv, char **envp);

#endif