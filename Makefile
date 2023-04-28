#Compiler and Linker
CC          	= cc

#The Target Binary Program
NAME			= bin/minishell

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR			= src
INCDIR			= include
BUILDDIR		= obj
TARGETDIR		= bin
SRCEXT			= c
OBJEXT			= o

CFLAGS      = -Wall -Wextra -Werror -g
INCDEP      = -I$(INCDIR)

SOURCES     =	src/main.c

OBJECTS			= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

all: directories $(NAME)

#Remake
remake: cleaner all
	
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)	

fclean: clean
	@$(RM) -rf $(TARGETDIR)
	
re:	fclean all

$(NAME): $(OBJECTS) 
	@$(CC) $(CFLAGS) $(INCDEP) -o $(NAME) $^

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCDEP) -c -o $@ $<

.PHONY: all clean fclean re