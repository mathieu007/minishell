SRCDIR			= src
INCDIR			= include
BUILDDIR		= obj
TARGETDIR		= bin
SRCEXT			= c
OBJEXT			= o

NAME				= printf_stderr.a
INCLUDE				= include
CFLAGS				= -Wall -Wextra -Werror -g
INCDEP				= -I$(INCDIR)
LIBFT_DIR			= ./../libft
LIBFT_FILE			= libft.a
LIBFT				= $(LIBFT_DIR)/$(LIBFT_FILE)
INCLIBFTDEP 		= -I$(LIBFT_DIR)/$(INCDIR)

CC			= gcc
CFLAGS		= -Wall -Werror -Wextra
AR			= ar rcs

SOURCES     = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

OBJECTS		 = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

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

$(TARGETDIR)/$(NAME): $(OBJECTS)
	@$(MAKE) -C $(LIBFT_DIR)
	$(AR) $(TARGETDIR)/$(NAME) $(OBJECTS)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCDEP) $(INCLIBFTDEP) -c -o $@ $<

.PHONY:		all clean fclean re
