#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "history.h"
# include "readline.h"
# include <libft.h>
# include <limits.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>

# define BUILTINS_EXPORT "export"
# define BUILTINS_UNSET "unset"
# define BUILTINS_ENV "env"
# define BUILTINS_EXIT "exit"
# define BUILTINS_PWD "pwd"
# define BUILTINS_CD "cd"
# define BUILTINS_ECHO "echo"

# define SEPARATOR (char)29
# define _DEBUG

#ifndef PATH_MAX
# define PATH_MAX 1024
#endif

extern char	**environ;

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
///	TK_BACKSLASH \,
typedef enum e_token_type
{
	TK_UNKNOWN = -1,
	TK_CMD_SEQ_START = -2,
	TK_CMD_SEQ_END = 0,
	TK_CLOSINGDOUBLEQUOTE = -4,
	TK_CLOSINGSINGLEQUOTE = -5,
	TK_ENVIRONEMENT_VAR = -6,
	TK_GREAT = (int32_t)'>',
	TK_LESS = (int32_t)'<',
	TK_PIPE = (int32_t)'|',
	TK_SPACE = (int32_t)' ',
	TK_AMPERSAND = (int32_t)'&',
	TK_GREATGREAT = TK_GREAT * 255 + TK_GREAT,
	TK_LESSLESS = TK_LESS * 255 + TK_LESS,
	TK_OR = TK_PIPE * 255 + TK_PIPE,
	TK_AND = TK_AMPERSAND * 255 + TK_AMPERSAND,
	TK_DOUBLEQUOTE = (int32_t)'"',
	TK_DASH = (int32_t)'-',
	TK_DASHDASH = TK_DASH * 255 + TK_DASH,
	TK_SINGLEQUOTE = (int32_t)'\'',
	TK_DOLLAR_SIGN = (int32_t)'$',
	TK_LAST_PIPE_EXIT = TK_DOLLAR_SIGN * 255 + '?',
	TK_SEMICOLON = (int32_t)';',
	TK_BACKSLASH = (int32_t)'\\',
	TK_BACKSLASHSINGLEQUOTE = (TK_BACKSLASH * 255) + TK_SINGLEQUOTE,
	TK_BACKSLASHDOUBLEQUOTE = (TK_BACKSLASH * 255) + TK_DOUBLEQUOTE,
	TK_WILDCARD = (int32_t)'*'
}							t_token_type;

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
}							t_escaped_char;
typedef enum e_continuation_char
{
	CONTC_BACKSLASH = '\\',
	CONTC_PIPE = '|',
	CONTC_AMPERSAND = '&',
	CONTC_SEMICOLON = ';',
	CONTC_DOUBLE_AMPERSAND = CONTC_AMPERSAND * CONTC_AMPERSAND +
		CONTC_AMPERSAND,
	CONTC_DOUBLE_PIPE = CONTC_PIPE * CONTC_PIPE + CONTC_PIPE
}							t_continuation_char;

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
	CMD_LOG_AND = TK_AND, // &&
	CMD_LOG_OR = TK_OR, // ||
	CMD_BACKGROUND_EXEC = TK_AMPERSAND, // &
	CMD_FILEOUT_APPPEND = TK_GREATGREAT,
	CMD_FILEIN_APPPEND = TK_LESSLESS,
	CMD_FILEOUT = TK_GREAT,
	CMD_FILEIN = TK_LESS,
	CMD_SEQUENTIAL = TK_SEMICOLON // ;
}							t_cmd_seq;

typedef struct s_redirect
{
	int32_t					fd_in;
	int32_t					fd_out;
	char					*file_in;
	char					*file_out;
}							t_redirect;

typedef struct s_pipe
{
	int32_t					fd_in;
	int32_t					fd_out;
}							t_pipe;

typedef struct s_env_cpy
{
	char				*variable;
	char				*value;
	struct s_env_cpy	*next;
	struct s_env_cpy	*prev;
}						t_env_cpy;

/// token->value contains the value of the token but whithout the termination char.
/// if cmd = echo -n "fewfew" and token is -, value == -n "fewfew"
typedef struct s_token
{
	struct s_token	*next;
	struct s_token	*prev;
	char			*str;
	char			*token_str;
	int32_t			token_len;
	int32_t			token_repeat_len;
	int32_t			pos;
	int32_t			offset;
	int32_t			tolal_len;
	bool			inside_dbl_quotes;
	t_token_type	type;
}				t_token;

/// @brief A token group is just a group of tokens.
/// each group of token end by one of the endings token type.
typedef struct s_token_group
{
	struct s_token_group	*next;
	struct s_token_group	*prev;
	char					*str;
	int32_t					len;
	t_token					*first_token;
	t_token					*last_token;
	int32_t					token_count;
	t_cmd_seq				cmd_seq_type;
}							t_token_group;

/// "ec"$VAR -naaaaznnnnzzzz 123$VAR2"123   test" $VAR=ho $VAR2=" 6    6 "
//name echo , option: -n, -a, -z; args: [echo, -naaaaznnnnzzzz, 123, "6    6" "123    test"]
typedef struct s_cmd
{
	int32_t			(*func)(struct s_cmd *);
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


/// @brief is the current processes data in which the cmd will
/// be executed, we keep the ino_t and current working dir, also
/// to keep track of the cwd if the cwd is moved, renamed or deleted..
typedef struct s_process
{
	int32_t			argc;
	char			**argv;
	char			**env;
	ino_t			dir_id;
	char			*cwd;
	t_env_cpy		*env_cpy;
	int32_t			tokens_count;
	int32_t			token_groups_count;
	t_token			*tokens;
	t_token_group	*token_groups;
	t_cmd			*cmds;
	t_cmd			*last_cmd;
	t_token_group	*last_token_group;
}					t_process;

/// @brief The entities functions

t_env_cpy		*new_env(char *variable, char *value);
char			*replace_env_value(char *variable, char *value);
char			*get_cmd_env_value(char *variable, t_cmd *cmd);
t_redirect		*new_redirect(t_cmd *cmd);
t_process		*get_process(void);
t_token_group	*get_first_token_group(void);
t_token_group	*get_last_token_group(void);
t_token			*get_first_token(void);
t_token			*get_last_token(void);
t_cmd			*get_first_cmd(void);
t_cmd			*get_last_cmd(void);
char			**get_builtins_cmd(void);
t_token_type	get_token_type(char *str);
int32_t			*get_token_counter(void);
int32_t			get_token_type_count(t_token_type type);
t_token			*add_token(int32_t char_pos, t_token_type type, t_token_group *group);
t_token_group	*add_token_group(char *start, t_token_type type, int32_t len);
t_cmd			*add_cmd(void);
void			dup_env(void);
t_token			*new_token();
t_cmd			*new_cmd();
t_token_group	*new_token_group();

/// @brief Simples and short helpers methods.

int32_t			get_env_var_name_len(char *str);
char			*get_env_variable(char *str, int32_t i);
char			*get_end_of_cmd(char *str);
int32_t			get_token_type_len(t_token_type type);
bool			type_is_end_of_seq(t_token_type type);
bool			is_env_variable(t_token *token);
bool			str_is_env_variable(char *str);
bool			is_esc_env_var(char *str, int32_t i);
bool			is_escaped_char(char *str, int32_t i);
int32_t			skip_escaped_quotes(char *str, int32_t i);
int32_t			skip_esc_single_quote(char *str, int32_t i);
int32_t			skip_esc_double_quote(char *str, int32_t i);
int32_t			skip_escaped_char(char *str, int32_t i);
char			*skip_escaped_char2(char *str);
t_token			*advance_to(t_token *token, t_token_type type);
bool			is_builtins(char *str);
bool			is_sibling_quote(t_token *token);
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

/// get full path from relative path, absolute or env path.
char			*get_full_path(t_cmd *cmd);
char			*get_cwd(t_cmd *cmd);

/// tokenizer functions
void			reset_token_group(t_token_group *group);
int32_t			add_token_env(char *str, int32_t pos, t_token_group *group, bool inside_dbl_quotes);
t_token_group	*tokenize_groups(char *str);
t_token			*tokenize(t_token_group *group);
int32_t			tokenize_curlybrace(char *str, int32_t i);
int32_t			tokenize_parenthese(char *str, int32_t i);
int32_t			tokenize_double_quote(char *str, int32_t i, t_token_group *group);
int32_t			tokenize_single_quote(char *str, int32_t i, t_token_group *group);
char			*cpy_single_quote_str(char *str, char *output, int32_t *i);
char			*cpy_esc_env_var(char *input, char *output, int32_t *i);
char			*cpy_env_var_value(char *input, char *output, int32_t *i);

int32_t			increment_counter(t_token_type type);
int32_t			decrement_counter(t_token_type type);

/// parsing
t_cmd			*parse_cmd(t_token_group *token_group);
char			*group_to_str(t_token_group *group);
int32_t			count_env_words(char *str);
t_cmd			*get_seq_cmds(t_token_group *group);
char			**parse_args(t_token_group *group);
char			**get_options(t_token_group *group);
int32_t			get_args_len(t_token_group *group);
void			get_args(t_token_group *group, char **split);
 t_token		*get_quotes_str(t_token *token, char *str, char **ouput);
char			*get_single_quote_str(t_token *token, char *str);
char			*get_double_quote_str(t_token *token, char *str);
t_token_group	*parse_env(t_token_group *group);
void			replace_env_name(char *input, char *output);
char			**get_env_path();
t_token			*get_token_at(int32_t index);
t_token			*get_closing_double_quote_token(t_token *token);
t_token			*get_closing_single_quote_token(t_token *token);
t_cmd_seq		get_sequence_type(t_token_type type);

t_env_cpy					*init_env(t_process *data);
bool						is_end_of_seq(t_token *token);
void						close_pipe_fds(t_cmd *cmd);
t_pipe						*new_pipe(t_cmd *cmd);
void						*free_pipe(t_cmd *cmd);
void						*free_redirect(t_cmd *cmd);
void						*free_cmd(t_cmd *cmd);
void						init_data(int32_t argc, char **argv, char **env);
int32_t						init_cwd_fd(char *cwd);
void						free_t_env_cpy(t_env_cpy *env_cpy);

//link list section
char						*get_env_value(char *variable);
void						add_env_node(t_process *data, char *variable, char *value);

/// execution
int32_t					add_execve_func(t_cmd *cmd);
int32_t					exec_cmds(char *str);

//built in section
int						add_built_in_func(t_cmd *cmd);
int						cd_cmd(t_cmd *cmd);
int						echo_cmd(t_cmd *cmd);
int						env_cmd(t_cmd *cmd);
int						pwd_cmd(t_cmd *cmd);
int						export_cmd(t_cmd *cmd);
int						unset_cmd(t_cmd *cmd);
int						exit_cmd(t_cmd *cmd);

//free section
void					free_2d_char_array(char **tab);
void					free_t_cmd(t_cmd *cmd);
void					free_t_tokens(t_token *token);
void					free_t_token_groups(t_token_group *token_group);
void					free_t_redirect(t_redirect *redirect);
void					free_t_data(t_process *data);
void					free_2d_array(void **tab);
void					free_all();
void					*free_all_and_exit(int32_t status);
void					*free_ptr(void *ptr);

void					print_token_group(t_token_group *token);
void					print_token(t_token *token);
void					print_groups_and_tokens();
void					print_cmd(t_cmd *command);
void					print_env(t_env_cpy *env);

#endif
