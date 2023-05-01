/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:51 by math              #+#    #+#             */
/*   Updated: 2023/05/01 06:52:58 by math             ###   ########.fr       */
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
	TK_CMD = 27,
	TK_GREAT = (int32_t)'>',
	TK_LESS = (int32_t)'<',
	TK_PIPE = (int32_t)'|',
	TK_AMPERSAND = (int32_t)'&',
	TK_GREATGREAT = TK_GREAT * TK_GREAT + TK_GREAT,
	TK_LESSLESS = TK_LESS * TK_LESS + TK_LESS,
	TK_OR = TK_PIPE * TK_PIPE + TK_PIPE,
	TK_AND = TK_AMPERSAND * TK_AMPERSAND + TK_AMPERSAND,
	TK_DOUBLEQUOTE = (int32_t)'"',
	TK_SINGLEQUOTE = (int32_t)'\'',
	TK_DOLLAR_SIGN = (int32_t)'$',
	TK_BACKTICK = (int32_t)'`',
	TK_SEMICOLON = (int32_t)';',
	TK_CURLYBRACES_OPEN = (int32_t)'{',
	TK_CURLYBRACES_CLOSE = (int32_t)'}',
	TK_PARENTHESES_OPEN = (int32_t)'(',
	TK_PARENTHESES_CLOSE = (int32_t)')',
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
	ESC_HEX_DIGIT = '\x',
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

/// @brief
typedef enum e_builtin_cmd
{
	BUILTIN_CD,
	BUILTIN_ECHO,
	BUILTIN_ALIAS,
	BUILTIN_EXPORT,
	BUILTIN_PWD,
	BUILTIN_ENV,
	BUILTIN_HISTORY,
	BUILTIN_EXIT,
	BUILTIN_SOURCE,
	BUILTIN_SET,
	BUILTIN_UNSET,
	BUILTIN_TYPE
}				t_builtin_cmd;

/// @brief the type of command sequences
/// CMD_PIPE = command1 | command2
/// CMD_SEQUENTIAL = command1; command2;
/// CMD_LOG_AND = command1 && command2
/// CMD_LOG_OR = command1 || command2
/// CMD_BACKGROUND_EXEC = command &
/// CMD_SUBSTITUTION = $(command)
/// CMD_INPUT_REDIRECT = command < input_file
/// CMD_OUTPUT_REDIRECT = command > output_file
/// CMD_APPEND_OUTPUT_REDIRECT = command >> output_file
/// CMD_GROUPING = (command1; command2) or { command1; command2; }
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

/// @brief
typedef struct s_cmd
{
	struct s_cmd	*next;
	struct s_cmd	*prev;
	char			*name;
	char			**args;
	char			**options;
	bool			is_builtin;
	t_cmd_seq		cmd_seq_type;
	pid_t			pid;
}				t_cmd;

/// @brief
typedef struct s_token
{
	struct s_token	*next;
	struct s_token	*prev;
	char			*value;
	int32_t			len;
	int32_t			index;
	int32_t			start_pos;
	int32_t			end_pos;
	t_token_type	type;
}				t_token;


typedef struct s_data
{
	int32_t			argc;
	char			**argv;
	char			**env;
	char			**paths;
	int32_t			token_count;
}				t_data;

/// @brief all the data functions
t_data			*get_data(void);
t_token			*get_tokens(void);
t_token			*add_token(char *token_value, int32_t char_pos,
					t_token_type type);
t_token_type	get_token_type(char *str);
int32_t			*get_token_counter(void);
t_token			*remove_token(t_token *tokens);
int32_t			get_token_type_count(t_token_type type);

bool			is_escaped_single_quote(t_token *tokens);

int32_t			increment_counter(t_token_type type);
int32_t			decrement_counter(t_token_type type);	

void			*free_all(void);

#endif