/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/04 12:54:25 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	*free_slpit(char **split)
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
	i = 0;
	while (builtins[i])
	{
		len = ft_strlen(builtins[i]);
		if (ft_strn_right_cmp(str, builtins[i], len))
			return (true);
		i++;
	}
	return (false);
}

bool	is_end_cmd(t_token *token)
{
	t_token_type	type;

	type = token->type;
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE
		|| token->next == NULL)
		return (true);
	return (false);
}

int32_t	options_count(t_token *token, int32_t tk_pos)
{
	int32_t	count;
	char	**options;

	count = 0;
	token = token->prev;
	while (token && token->pos > tk_pos)
	{
		if (token->type == TK_DASH || token->type == TK_DASHDASH)
			count++;
		if (token->prev == NULL)
			break ;
		token = token->prev;
	}
	return (count);
}

char	**get_options(t_token *token, int32_t tk_pos)
{
	int32_t	count;
	char	**options;

	count = options_count(token, tk_pos);
	if (count == 0)
		return (NULL);
	options = malloc(sizeof(char *) * count);
	count--;
	while (token && token->pos < tk_pos)
	{
		if (token->type == TK_DASH || token->type == TK_DASHDASH)
		{
			options[count] = ft_strdup(token->value);
			count--;
		}			
		token = token->prev;		
	}
}

t_cmd_seq	get_arguments(t_token *token, int32_t tk_pos)
{
	t_token_type	type;

	type = token->type;
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE)
		return ((t_cmd_seq)type);
	return (CMD_SEQUENTIAL);
}

t_cmd_seq	get_sequence_type(t_token *token, int32_t tk_pos)
{
	t_token_type	type;

	type = token->type;
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE)
		return ((t_cmd_seq)type);
	return (CMD_SEQUENTIAL);
}

t_cmd	*parse_cmds(t_token *token, char *str)
{
	t_cmd	*cmd;
	char	*split;
	int32_t	tk_pos;

	if (!token)
		return (NULL);
	cmd = get_cmds();
	tk_pos = 0;
	while (token)
	{
		if (is_end_cmd(token))
		{
			split = ft_split(&str[token->pos], ' ');
			if (split && *split)
				cmd->name = *split;
			cmd->full_name = get_full_path(cmd->name);
			cmd->is_builtin = is_builtins(cmd->name);
			cmd->options = get_options(token, tk_pos);
			cmd->cmd_seq_type = get_sequence_type(token, tk_pos);
			cmd->args = get_arguments(token, tk_pos);
			tk_pos = token->pos + 1;
		}
		token = token->next;
	}
}
