/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:25:53 by bmartin           #+#    #+#             */
/*   Updated: 2023/05/14 11:08:41 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
////////////delete me after /////////

void	print_cmd(t_cmd *command)
{
	t_cmd	*cpy;
	int		i;
	int 	j;

	i = 0;
	j=0;
	cpy = command;
	printf("[[CMD CHAINLIST]]\n\n");
	while (cpy)
	{
		printf("[[NODE # %i]]\n",j);
		printf("---------------------------------------------------------");
		printf("ARGS = \n");
		while (cpy->args[i])
		{
			printf("[%s]  ", cpy->args[i]);
			i++;
		}
		printf("\n");

		printf(" cmd_seq_type = %i\n", (int)cpy->cmd_seq_type);

		printf("full_path_name = %s\n", cpy->full_path_name);

		printf("is_builtin = %i\n", cpy->is_builtin);

		printf("name = %s\n", cpy->name);
		
		i = 0;
		while (cpy->options[i])
		{
			printf("name = %s\n", cpy->options[i]);
			i++;
		}
		printf("\n");

		printf("pid = %i\n", cpy->pid);
		printf("pid = %p\n", cpy->next);
		printf("pid = %p\n", cpy->prev);
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
		printf("---------------------------------------------------------");
		printf(" start = %s\n", cpy->start);
		printf(" len = %i\n", cpy->token_len);
		printf(" pos = %i\n", cpy->pos);
		printf(" type = %i\n", (int)cpy->type);
		printf("---------------------------------------------------------");
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
		printf("---------------------------------------------------------");
		printf(" start = %s\n", cpy->start);
		printf(" len = %i\n", cpy->len);
		printf(" token_count = %i\n", cpy->token_count);
		printf("---------------------------------------------------------");
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
		printf("%s\n", array[i]);
		i++;
	}
}
