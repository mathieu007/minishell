/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/04 16:30:42 by mroy             ###   ########.fr       */
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

bool	is_end_cmd_tk(t_token *token)
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

int32_t	options_count(t_token *token, int32_t num_tk)
{
	int32_t	count;
	char	**options;

	count = 0;
	token = token->prev;
	while (token && token->pos > num_tk)
	{
		if (token->type == TK_DASH || token->type == TK_DASHDASH)
			count++;
		if (token->prev == NULL)
			break ;
		token = token->prev;
	}
	return (count);
}

char	**get_options(t_token *token, int32_t end)
{
	int32_t	count;
	char	**options;
	char	*value;

	count = options_count(token, end);
	if (count == 0)
		return (NULL);	
	options = malloc(sizeof(char *) * count);
	count--;
	while (token && end > 0)
	{
		value = token->value;
		if (token->type == TK_DASH || token->type == TK_DASHDASH)
		{
			while (*value != ' ' || *value != '\t')
				value++;
			options[count] = ft_strdupn(token->value, value - token->value);
			count--;
			end--;
		}			
		token = token->prev;
	}
	return (options);
}

char	**get_arguments(t_token *token, int32_t tk_pos)
{
	
}

t_cmd_seq	get_sequence_type(t_token *token)
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
	t_cmd		*cmd;
	char		*split;
	int32_t		i;
	int32_t		start_i;
	int32_t		char_pos;

	if (!token)
		return (NULL);
	cmd = get_cmds();
	start_i = 0;
	i = 0;
	char_pos = 0;
	while (token)
	{
		if (is_end_cmd_tk(token))
		{
			split = ft_split(&str[get_token_at(start_i)->pos], ' ');
			if (split && *split)
				cmd->name = *split;
			else
			{
				continue;
				i++;
			}
			cmd->full_path_name = get_full_path(cmd->name);
			cmd->is_builtin = is_builtins(cmd->name);
			cmd->options = get_options(get_token_at(start_i), i);
			cmd->cmd_seq_type = get_sequence_type(token);
			cmd->args = get_arguments(get_token_at(start_i), i);
			start_i = i;
		}
		i++;
		token = token->next;
	}
}
