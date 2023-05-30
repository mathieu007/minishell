#Compiler and Linker
CC          	= cc

#The Target Binary Program
NAME			= minishell
NAME_TESTS		= tests

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
READLINE			= $(INCDIR)/$(LIBREADLINE)
READLINEHISTORY		= $(INCDIR)/$(LIBREADLINEHISTORY)
INCLIBFTDEP 		= -I$(LIBFT_DIR)/$(INCDIR)

POST_CFLAGS := -lreadline -lncurses $(READLINE) $(READLINEHISTORY)

SOURCES     = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
TESTS       = $(shell find $(TESTSDIR) -type f -name *.$(SRCEXT))

OBJECTS		 = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
OBJECT_TESTS = $(patsubst $(TESTSDIR)/%,$(BUILDDIR)/%,$(TESTS:.$(SRCEXT)=.$(OBJEXT)))

all: directories $(TARGETDIR)/$(NAME) $(TARGETDIR)/$(NAME_TESTS)

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

test: re 
	cd bin && chmod 777 ./tests && chmod +x ./tests && ./tests

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

.PHONY: all clean fclean re