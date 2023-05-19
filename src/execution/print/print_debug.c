#include "minishell.h"
////////////delete me after /////////

void	print_groups_and_tokens()
{
	t_data			*data;
	t_token_group	*group;
	t_token			*token;

	data = get_data();
	group = data->token_groups;
	print_token_group(group);
	while (group)
	{		
		token = group->first_token;
		if (token)
		{
			print_token(token);
			token = token->next;
		}
		group = group->next;
	}	
}

static char	*get_print_token_type(t_token_type type)
{
	static char	*token_types[255 * 255 + (255 * 2)];
	char		**tk_type = &token_types[8];

	if (!token_types[0])
	{
		tk_type[TK_ENVIRONEMENT_VAR] = "TK_ENVIRONEMENT_VAR";
		tk_type[TK_UNKNOWN] = "TK_UNKNOWN";
		tk_type[TK_CMD_SEQ_START] = "TK_CMD_SEQ_START";
		tk_type[TK_CMD_SEQ_END] = "TK_CMD_SEQ_END";
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
		tk_type[TK_LAST_PIPE_EXIT] = "TK_LAST_PIPE_EXIT";
		tk_type[TK_PIPE] = "TK_PIPE";
		tk_type[TK_SINGLEQUOTE] = "TK_SINGLEQUOTE";
		tk_type[TK_VAR_ASSIGN] = "TK_VAR_ASSIGN";
		tk_type[TK_WILDCARD] = "TK_WILDCARD";
		tk_type[TK_SEMICOLON] = "TK_SEMICOLON";
		tk_type[TK_BACKSLASHDOUBLEQUOTE] = "TK_BACKSLASHDOUBLEQUOTE";
		tk_type[TK_BACKSLASHSINGLEQUOTE] = "TK_BACKSLASHSINGLEQUOTE";
	}
	return (tk_type[type]);
}

void	print_cmd(t_cmd *command)
{
	t_cmd	*cpy;
	int		i;
	int 	j;

	i = 0;
	j = 0;
	cpy = command;
	printf("[[CMD CHAINLIST]]\n\n");
	while (cpy)
	{
		printf("[[NODE # %i]]\n",j);
		printf("---------------------------------------------------------");
		printf("ARGS = \n");
		while (cpy->args[i])
		{
			printf("[%s] ", cpy->args[i]);
			i++;
		}
		printf("\n");
		printf(" cmd_seq_type = %i\n", (int)cpy->cmd_seq_type);
		printf("full_path_name = %s\n", cpy->full_path_name);
		printf("is_builtin = %i\n", cpy->is_builtin);
		printf("name = %s\n", cpy->name);
		i = 0;
		while (cpy->options && cpy->options[i])
		{
			printf("name = %s\n", cpy->options[i]);
			i++;
		}
		printf("\n");
		printf("---------------------------------------------------------");
		cpy = cpy->next;
		j++;
	}
}

void	print_token(t_token *token)
{
	t_token	*cpy;
	int j =0;

	cpy = token;
	printf("[[TOKEN CHAINLIST]]\n\n");
	while (cpy)
	{
		printf("[[NODE # %i]]\n",j);
		printf("---------------------------------------------------------\n");
		printf(" str = %s\n", cpy->str);
		printf(" token len = %i\n", cpy->token_len);
		printf(" token in dbl quotes = %i\n", (int)cpy->inside_dbl_quotes);
		printf(" pos = %i\n", cpy->pos);
		printf(" token type = %s\n", get_print_token_type(cpy->type));
		printf(" total len = %i\n", cpy->tolal_len);
		printf("---------------------------------------------------------\n");
		cpy = cpy->next;
		j++;
	}
}

void	print_env(t_env_cpy *env)
{
	t_env_cpy	*cpy;
	int j = 0;

	cpy = env;
	printf("[[ENVIRONEMENTS CHAINLIST]]\n\n");
	while (cpy)
	{
		printf("[[NODE # %i]]\n",j);
		printf("---------------------------------------------------------\n");
		printf(" var = %s value = %s\n", cpy->variable, cpy->value);
		printf("---------------------------------------------------------\n");
		cpy = cpy->next;
		j++;
	}
}

void	print_token_group(t_token_group *token)
{
	t_token_group	*cpy;
	int j = 0;

	cpy = token;
	printf("[[TOKEN GROUP CHAINLIST]]\n\n");
	while (cpy)
	{
		printf("[[NODE # %i]]\n",j);
		printf("---------------------------------------------------------\n");
		printf(" str = %s\n", cpy->str);
		printf(" len = %i\n", cpy->len);
		printf(" cmd sequence type = %i\n", cpy->cmd_seq_type);
		printf(" token_count = %i\n", cpy->token_count);
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
