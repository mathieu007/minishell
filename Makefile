#Compiler and Linker
CC          	= gcc

#The Target Binary Program
NAME			= minishell

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR			= src
INCDIR			= include
BUILDDIR		= obj
TARGETDIR		= bin
SRCEXT			= c
OBJEXT			= o

CFLAGS				= -Wall -Wextra -g 
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

OBJECTS		= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

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