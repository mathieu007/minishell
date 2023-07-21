/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_print_token_type(t_token_type type)
{
	static char	*token_types[255 * 255 + (255 * 2)];
	char		**tk_type;

	tk_type = &token_types[8];
	if (!token_types[0])
	{
		tk_type[TK_ENVIRONEMENT_VAR] = "TK_ENVIRONEMENT_VAR";
		tk_type[TK_UNKNOWN] = "TK_UNKNOWN";
		tk_type[TK_START] = "TK_SEQ_START";
		tk_type[TK_END] = "TK_SEQ_END";
		tk_type[TK_CLOSINGDOUBLEQUOTE] = "TK_CLOSINGDOUBLEQUOTE";
		tk_type[TK_CLOSINGSINGLEQUOTE] = "TK_CLOSINGSINGLEQUOTE";
		tk_type[TK_SPACE] = "TK_SPACE";
		tk_type[TK_AMPERSAND] = "TK_AMPERSAND";
		tk_type[TK_AND] = "TK_AND";
		tk_type[TK_BACKSLASH] = "TK_BACKSLASH";
		tk_type[TK_OR] = "TK_OR";
		tk_type[TK_DASH] = "TK_DASH";
		tk_type[TK_DASHDASH] = "TK_DASHDASH";
		tk_type[TK_DOLLAR_SIGN] = "TK_DOLLAR_SIGN";
		tk_type[TK_DOUBLEQUOTE] = "TK_DOUBLEQUOTE";
		tk_type[TK_GREAT] = "TK_GREAT";
		tk_type[TK_GREATGREAT] = "TK_GREATGREAT";
		tk_type[TK_LESS] = "TK_LESS";
		tk_type[TK_LESSLESS] = "TK_LESSLESS";
		tk_type[TK_LAST_CMD_EXIT] = "TK_LAST_PIPE_EXIT";
		tk_type[TK_PIPE] = "TK_PIPE";
		tk_type[TK_SINGLEQUOTE] = "TK_SINGLEQUOTE";
		tk_type[TK_WILDCARD] = "TK_WILDCARD";
		tk_type[TK_SEMICOLON] = "TK_SEMICOLON";
		tk_type[TK_BACKSLASHDOUBLEQUOTE] = "TK_BACKSLASHDOUBLEQUOTE";
	}
	return (tk_type[type]);
}

void	print_cmd(t_cmd *command)
{
	t_cmd	*cpy;
	int		i;
	int		j;

	i = 0;
	j = 0;
	cpy = command;
	printf("[[CMD CHAINLIST]]\n\n");
	while (cpy)
	{
		printf("[[NODE # %i]]\n", j);
		printf("\n");
		printf("cmd name = %s\n", cpy->name);
		printf("cmd_seq_type = %i\n", (int)cpy->type);
		printf("full_path_name = %s\n", cpy->full_path_name);
		printf("is_builtin = %i\n", cpy->is_builtin);
		printf("---------------------------------------------------------");
		printf("ARGS = \n");
		while (cpy->args[i])
		{
			printf("[%s] ", cpy->args[i]);
			i++;
		}
		printf("\n");
		printf("---------------------------------------------------------");
		printf("OPTIONS = \n");
		i = 0;
		while (cpy->options && cpy->options[i])
		{
			printf("[%s] ", cpy->options[i]);
			i++;
		}
		printf("\n");
		printf("---------------------------------------------------------");
		printf("\n");
		printf("[[END NODE # %i]]\n", j);
		cpy = cpy->next;
		j++;
	}
	printf("Command output:");
}

void	print_token(t_token *token)
{
	t_token	*cpy;
	int		j;

	j = 0;
	cpy = token;
	printf("[[TOKEN CHAINLIST]]\n\n");
	while (cpy)
	{
		printf("[[NODE # %i]]\n", j);
		printf("---------------------------------------------------------\n");
		printf(" str = %s\n", cpy->str);
		printf(" token len = %i\n", cpy->token_len);
		printf(" token in dbl quotes = %i\n", (int)cpy->inside_dbl_quotes);
		printf(" pos = %i\n", cpy->start);
		printf(" token type = %s\n", get_print_token_type(cpy->type));
		printf("---------------------------------------------------------\n");
		cpy = cpy->next;
		j++;
	}
}

void	print_env(t_env_cpy *env)
{
	t_env_cpy	*cpy;
	int			j;

	j = 0;
	cpy = env;
	printf("[[ENVIRONEMENTS CHAINLIST]]\n\n");
	while (cpy)
	{
		printf("[[NODE # %i]]\n", j);
		printf("---------------------------------------------------------\n");
		printf("  %s = %s\n", cpy->variable, cpy->value);
		printf("---------------------------------------------------------\n");
		cpy = cpy->next;
		j++;
	}
}

void	print_2darray(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		printf("[%s]\n", array[i]);
		i++;
	}
}
