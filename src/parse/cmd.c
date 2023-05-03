/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 14:14:51 by mroy             ###   ########.fr       */
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

bool	is_end_of_cmd(t_token *token)
{
	t_token_type type;
	
	type = token->type;
	return (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
			|| type == TK_AMPERSAND || type == TK_GREATGREAT 
			|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE
			|| token->next == NULL);
}

t_cmd	*parse_cmds(t_token *token)
{
	t_cmd 			*cmds;
	char			*cmd_name;
	t_data			*data;
	char			*split;
	int32_t			start_pos;
	
	if (!token)
		return (NULL);	
	cmds = get_cmds();
	cmd_name = NULL;
	data = get_data();
	start_pos = 0;
	while (token)
	{
		if (is_end_of_cmd(token))
		{
			split = ft_split(&(data->cmds[token->pos]), ' ');
			if (split && *split)
				cmd_name = *split;
			
		}		
		token = token->next;
	}
}
