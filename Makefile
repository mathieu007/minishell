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

CFLAGS			= -Wall -Wextra -Werror -g
POST_CFLAGS		= -lreadline
INCDEP			= -I$(INCDIR)
LIBFT_DIR		= libft
LIBFT_FILE		= libft.a
LIBFT			= $(LIBFT_DIR)/$(LIBFT_FILE)
INCLIBFTDEP 	= -I$(LIBFT_DIR)/$(INCDIR)

SOURCES			=	src/main.c \
					src/parser/parser.c \
					src/parser/tokens.c \
					src/data/data.c \
					src/data/free_all.c \
					src/data/token_type.c \
					src/data/tokens.c \

OBJECTS			= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

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
	$(CC) $(CFLAGS) $(INCDEP) $(INCLIBFTDEP) -c -o $@ $(POST_CFLAGS) $<

.PHONY: all clean fclean re