/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/02 16:19:24 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void *free_slpit(char **split)
{
	if (split != NULL)
	{
		split++;
		while (*split)
			free(*split);
		free(split);	
	}
	return (NULL);
}

bool	is_end_of_cmd(t_token_type type, t_token *token)
{
	return (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
			|| type == TK_AMPERSAND || type == TK_GREATGREAT 
			|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE
			|| type == TK
			|| token->next == NULL);
}

t_cmd	*parse_cmd_name(t_token *token)
{
	t_cmd 			*cmds;
	char			*cmd_name;
	t_data			*data;
	t_token_type	t;
	char			*split;
		
	cmds = get_cmds();
	cmd_name = NULL;
	data = get_data();
	split = ft_split(&(data->cmds[token->pos]), ' ');
	while (token)
	{
		t = token->type;
		if ()
			cmd_name = split[0];
		token = token->next;
	}
}
