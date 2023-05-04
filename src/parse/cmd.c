/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 20:15:06 by math             ###   ########.fr       */
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

bool	is_builtins(char *str)
{
	int32_t	i;
	char	**builtins;
	int32_t	len;
	int32_t	str_len;

	builtins = get_builtins_cmd();
	str_len = ft_strlen(str) - 1;
	while (*builtins)
	{
		len = ft_strlen(*builtins);
		ft_strncmp(str[str_len - len]);
		*builtins = ++builtins;
	}
}

bool	is_cmd_tk(t_token *token)
{
	t_token_type	type;

	type = token->type;
	return (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE
		|| token->next == NULL);
}

t_cmd	*parse_cmds(t_token *token, char *str)
{
	t_cmd			*cmd;
	char			*split;
	int32_t			start_pos;

	if (!token)
		return (NULL);
	cmd = get_cmds();
	start_pos = 0;
	while (token)
	{
		if (is_cmd_tk(token))
		{
			split = ft_split(&str[token->pos], ' ');
			if (split && *split)
				cmd->name = *split;
			cmd->full_name = get_full_path(cmd->name);
			cmd->cmd_seq_type = 
		}
		token = token->next;
	}
}
