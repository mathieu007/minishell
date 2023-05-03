/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:51 by math              #+#    #+#             */
/*   Updated: 2023/05/03 15:47:51 by mroy             ###   ########.fr       */
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
	TK_CMD = -1,
	TK_GREAT = (int32_t)'>',
	TK_LESS = (int32_t)'<',
	TK_PIPE = (int32_t)'|',
	TK_AMPERSAND = (int32_t)'&',
	TK_GREATGREAT = TK_GREAT * TK_GREAT + TK_GREAT,
	TK_LESSLESS = TK_LESS * TK_LESS + TK_LESS,
	TK_OR = TK_PIPE * TK_PIPE + TK_PIPE,
	TK_AND = '&' * '&' + '&',
	TK_DOUBLEQUOTE = (int32_t)'"',
	TK_SINGLEQUOTE = (int32_t)'\'',
	TK_DOLLAR_SIGN = (int32_t)'$',
	TK_LAST_PIPE_EXIT = TK_DOLLAR_SIGN * '?' + '?',
	TK_SEMICOLON = (int32_t)';',
	// TK_CURLYBRACES_OPEN = (int32_t)'{',
	// TK_CURLYBRACES_CLOSE = (int32_t)'}',
	// TK_PARENTHESES_OPEN = (int32_t)'(',
	// TK_PARENTHESES_CLOSE = (int32_t)')',
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

/// @brief
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
/// CMD_PIPE = command1 | command2\n
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
	CMD_NONE,
	CMD_PIPE,
	CMD_LOG_AND,
	CMD_LOG_OR,
	CMD_BACKGROUND_EXEC,
	CMD_FILEOUT,
	CMD_FILEIN,
	CMD_SEQUENTIAL
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
	char			*name;
	char			*full_name;
	char			**args;
	char			**options;
	bool			is_builtin;
	t_cmd_seq		cmd_seq_type;
	t_pipe			*pipe;
	t_redirect		*redirect;
	pid_t			pid;
}				t_cmd;

typedef struct s_token
{
	struct s_token	*next;
	struct s_token	*prev;
	char			*value;
	int32_t			len;
	int32_t			index;
	int32_t			pos;
	t_token_type	type;
}				t_token;

typedef struct s_data
{
	int32_t			argc;
	char			*cmds;
	char			**argv;
	char			**envp;
	char			**paths;
	int32_t			token_count;
	t_token			*last_token;
}				t_data;

/// @brief all the data functions
t_data			*get_data(void);
t_token			*get_first_token(void);
t_token			*add_token(char *token_value, int32_t char_pos,
					t_token_type type);
t_token_type	get_token_type(char *str);
int32_t			*get_token_counter(void);
t_token			*remove_token(t_token *tokens);
int32_t			get_token_type_count(t_token_type type);
t_token			*new_token_after(t_token *curr);
char			**get_builtins_cmd(void);

/// @brief simple helpers methods.

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
char			*get_full_path(char *rel_path);

int32_t			tokenize_curlybrace(char *str, int32_t i);
int32_t			tokenize_parenthese(char *str, int32_t i);
int32_t			tokenize_double_quote(char *str, int32_t i);
int32_t			tokenize_single_quote(char *str, int32_t i);

int32_t			increment_counter(t_token_type type);
int32_t			decrement_counter(t_token_type type);

char			**parse_env(char **envp, char *env_name);
t_cmd			*parse_cmds(t_token *token);

void			close_pipe_fds(t_cmd *cmd);

void			*free_all(void);
void			*free_all_and_exit(void);
t_pipe			*new_pipe(t_cmd *cmd);
void			*free_pipe(t_cmd *cmd);
t_redirect		*new_redirect(t_cmd *cmd);
void			*free_redirect(t_cmd *cmd);
t_cmd			*new_cmd_after(t_cmd *curr);
t_cmd			*get_cmds(void);
void			*free_cmd(t_cmd *cmd);

#endif