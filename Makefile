CC					= gcc

NAME				= minishell
NAME_TESTS			= tests
NAME_REDIR_TESTS	= tests_redir
NAME_VAL_TESTS		= tests_valgrind
NUM_VALGRIND_CMDS  	= 2

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR			= src
TESTSDIR		= tests
TESTS_VAL_DIR	= tests_valgrind
TESTS_REDIR_DIR	= tests_redir
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

SOURCES     = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
TESTS       = $(shell find $(TESTSDIR) -type f -name *.$(SRCEXT))
TESTS_VAL   = $(shell find $(TESTS_VAL_DIR) -type f -name *.$(SRCEXT))
TESTS_REDIR = $(shell find $(TESTS_REDIR_DIR) -type f -name *.$(SRCEXT))

OBJECTS		 = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
OBJECT_TESTS = $(patsubst $(TESTSDIR)/%,$(BUILDDIR)/%,$(TESTS:.$(SRCEXT)=.$(OBJEXT)))
OBJECT_VAL_TESTS = $(patsubst $(TESTS_VAL_DIR)/%,$(BUILDDIR)/%,$(TESTS_VAL:.$(SRCEXT)=.$(OBJEXT)))
OBJECT_REDIR_TESTS = $(patsubst $(TESTS_REDIR_DIR)/%,$(BUILDDIR)/%,$(TESTS_REDIR:.$(SRCEXT)=.$(OBJEXT)))

all: directories $(TARGETDIR)/$(NAME) $(TARGETDIR)/$(NAME_TESTS) $(TARGETDIR)/$(NAME_REDIR_TESTS) $(TARGETDIR)/$(NAME_VAL_TESTS)

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

test: all 
	cp $(TESTSDIR)/tests.txt $(TARGETDIR)/ && cd bin && chmod 777 ./tests && chmod +x ./tests && ./tests
test2: all 
	cp $(TESTS_REDIR_DIR)/tests_redir.txt $(TARGETDIR)/ && cd bin && chmod 777 ./* && chmod +x ./* && ./tests_redir
mem: re 
	cd bin && chmod 777 ./tests_valgrind && chmod +x ./tests_valgrind && ./tests_valgrind $(NUM_VALGRIND_CMDS)	
exec: re 
	cd bin && ./minishell

$(TARGETDIR)/$(NAME): $(OBJECTS)
	@$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(INCDEP) $(INCLIBFTDEP) -o $(TARGETDIR)/$(NAME) $^ $(POST_CFLAGS) $(LIBFT)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCDEP) $(INCLIBFTDEP) -c -o $@ $<

$(TARGETDIR)/$(NAME_TESTS): $(OBJECT_TESTS)
	cp $(TESTSDIR)/tests.txt $(TARGETDIR)/
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(NAME_TESTS) $^
	
$(BUILDDIR)/%.$(OBJEXT): $(TESTSDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETDIR)/$(NAME_REDIR_TESTS): $(OBJECT_REDIR_TESTS)
	@$(MAKE) -C $(LIBFT_DIR)
	cp $(TESTS_REDIR_DIR)/tests_redir.txt $(TARGETDIR)/
	cp $(TESTS_REDIR_DIR)/in1 $(TARGETDIR)/
	cp $(TESTS_REDIR_DIR)/in2 $(TARGETDIR)/
	cp $(TESTS_REDIR_DIR)/in3 $(TARGETDIR)/
	cp $(TESTS_REDIR_DIR)/outfile* $(TARGETDIR)/
	$(CC) $(CFLAGS) $(INCDEP) $(INCLIBFTDEP) -o $(TARGETDIR)/$(NAME_REDIR_TESTS) $^ $(LIBFT)
	
$(BUILDDIR)/%.$(OBJEXT): $(TESTS_REDIR_DIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCDEP) $(INCLIBFTDEP) -c -o $@ $<

$(TARGETDIR)/$(NAME_VAL_TESTS): $(OBJECT_VAL_TESTS)
	cp $(TESTS_VAL_DIR)/tests_valgrind.txt $(TARGETDIR)/
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(NAME_VAL_TESTS) $^
	
$(BUILDDIR)/%.$(OBJEXT): $(TESTS_VAL_DIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all clean fclean re