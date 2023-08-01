CC					= gcc

NAME				= minishell
NAME_TESTS			= tests
NUM_VALGRIND_CMDS  	= 2

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR			= src
TESTSDIR		= tests
INCDIR			= include
BUILDDIR		= obj
TARGETDIR		= bin
SRCEXT			= c
OBJEXT			= o

CFLAGS				= -Wall -Wextra -Werror -g
INCDEP				= -I$(INCDIR)
LIBFT_DIR			= libft
LIBFT_FILE			= libft.a
LIBREADLINE			= libreadline.a
LIBREADLINEHISTORY	= libhistory.a
LIBFT				= $(LIBFT_DIR)/$(LIBFT_FILE)
PRINTF				= $(LIBFT_DIR)/$(PRINTF_FILE)

READLINE			= $(INCDIR)/$(LIBREADLINE)
READLINEHISTORY		= $(INCDIR)/$(LIBREADLINEHISTORY)
INCLIBFTDEP 		= -I$(LIBFT_DIR)/$(INCDIR)

POST_CFLAGS := -lreadline -lncurses $(READLINE) $(READLINEHISTORY)

SOURCES      = src/cmd2.c	\
src/cmd3.c	\
src/entities/builtins.c	\
src/entities/cmd.c	\
src/entities/cmd_exec.c	\
src/entities/cmd_logical.c	\
src/entities/cmd_parenthese.c	\
src/entities/cmd_pipe.c	\
src/entities/cmd_redirection.c	\
src/entities/cmd_sequential.c	\
src/entities/environement_entity.c	\
src/entities/environement_entity2.c	\
src/entities/environement_entity3.c	\
src/entities/free/close_all_pipes.c	\
src/entities/free/free_all.c	\
src/entities/free/free_all2.c	\
src/entities/free/free_cmds.c	\
src/entities/free/free_struct.c	\
src/entities/free/free_utils.c	\
src/entities/process.c	\
src/entities/redirect.c	\
src/entities/token_type.c	\
src/entities/tokens2.c	\
src/execution/builtins/built_in_funcs.c	\
src/execution/builtins/cd.c	\
src/execution/builtins/echo.c	\
src/execution/builtins/env.c	\
src/execution/builtins/exit.c	\
src/execution/builtins/export.c	\
src/execution/builtins/export_utils.c	\
src/execution/builtins/pwd.c	\
src/execution/builtins/unset.c	\
src/execution/builtins/unset_utils.c	\
src/execution/continuation.c	\
src/execution/continuation_delimiter.c	\
src/execution/continuation_delimiter2.c	\
src/execution/create_file_redirection.c	\
src/execution/create_file_redirection2.c	\
src/execution/cwd.c	\
src/execution/cwd2.c	\
src/execution/cwd3.c	\
src/execution/error.c	\
src/execution/error2.c	\
src/execution/execution.c	\
src/execution/execution2.c	\
src/execution/execution3.c	\
src/execution/file_in_redirection.c	\
src/execution/file_in_redirection2.c	\
src/execution/file_out_redirection.c	\
src/execution/file_out_redirection2.c	\
src/execution/file_out_redirection3.c	\
src/execution/file_redirections.c	\
src/execution/fork_logical.c	\
src/execution/fork_pipes.c	\
src/execution/fork_sequential.c	\
src/execution/fork_subshell.c	\
src/execution/here_doc.c	\
src/execution/pipes.c	\
src/execution/process/fork_exec.c	\
src/execution/signal/child_signal.c	\
src/execution/signal/signal.c	\
src/execution/signal/signal2.c	\
src/execution/wait.c	\
src/execution/wildcard.c	\
src/execution/wildcard2.c	\
src/execution/wildcard3.c	\
src/helpers/cmd.c	\
src/helpers/redirection_helpers.c	\
src/helpers/token_type.c	\
src/main.c	\
src/parser/arguments.c	\
src/parser/arguments2.c	\
src/parser/cmd.c	\
src/parser/cmd2.c	\
src/parser/cmd3.c	\
src/parser/environements_parser.c	\
src/parser/options.c	\
src/parser/paths.c	\
src/parser/paths2.c	\
src/parser/paths3.c	\
src/tokenizer/generic_tokenizer.c	\
src/tokenizer/generic_tokenizer2.c	\
src/tokenizer/generic_tokenizer3.c	\
src/tokenizer/split_tokens.c	\
src/tokenizer/syntax_error.c	\
src/tokenizer/token_cmd.c	\
src/tokenizer/token_dash.c	\
src/tokenizer/token_environement.c	\
src/tokenizer/token_expansion.c	\
src/tokenizer/token_expansion2.c	\
src/tokenizer/token_parenthese.c	\
src/tokenizer/token_redirection.c	\
src/tokenizer/token_sequence.c	\
src/tokenizer/token_sequence2.c	\
src/tokenizer/token_space_quotes.c	\
src/tokenizer/token_space_quotes2.c	\
src/tokenizer/tokenize.c	\
src/tokenizer/tokenize2.c	\
src/tokenizer/tokenize3.c	\


OBJECTS		 = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

all: directories $(TARGETDIR)/$(NAME)

directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

clean:
	@$(RM) -rf $(BUILDDIR)
	@$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	@$(RM) -rf $(TARGETDIR)
	@$(MAKE) fclean -C $(LIBFT_DIR)
	
re:	fclean all

$(TARGETDIR)/$(NAME): $(OBJECTS)
	@$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(INCDEP) $(INCLIBFTDEP) -o $(TARGETDIR)/$(NAME) $^ $(POST_CFLAGS) $(LIBFT)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCDEP) $(INCLIBFTDEP) -c -o $@ $<

.PHONY: all clean fclean re

#VALGRIND TEST COMMAND : valgrind --leak-check=full  --show-reachable=yes --track-fds=yes --track-origins=yes --suppressions=./minishell.sup ./bin/minishell