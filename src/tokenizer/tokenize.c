/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 07:12:32 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_group	*tokenize_groups(char *str)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*start;

	i = 0;
	start = str;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_type_len(type);
		if (type_is_end_of_seq(type))
		{
			add_token_group(start, &str[i + t_len] - start);
			start = &str[i + t_len];
		}
		i += t_len;
	}
	if (start != &str[i])
		add_token_group(start, &str[i] - start);
	return (get_first_token_group());
}

int32_t	add_token_space(char *str, int32_t pos, t_token_group *group)
{
	t_token			*token;
	t_token_type	type;
	int32_t			start;
	int32_t			repeat;

	type = TK_SPACE;
	start = pos;
	repeat = 0;
	while (type == TK_SPACE)
	{	
		type = get_token_type(&str[pos]);
		pos++;
		repeat++;
	}
	pos--;
	token = add_token(ft_strncpy(&str[start], pos - start), TK_SPACE, group);
	token->repeat = repeat;
	return (pos);
}

int32_t	add_token_dash(char *str, int32_t pos, t_token_group *group)
{
	t_token			*token;
	t_token_type	type;
	int32_t			start_pos;

	start_pos = pos;
	token = add_token(pos, TK_DASH, group);
	while (str[pos] && str[pos] != ' ')
		pos++;
	token->start = ft_strncpy(&str[start_pos], pos - start_pos);
	return (pos);
}

// void	set_tokens_str(t_token_group *group, char *str)
// {
// 	t_token	*token;
// 	t_token	*next_token;

// 	while (group)
// 	{
// 		token = group->first;
// 		next_token = token;
// 		while (token)
// 		{
// 			while (next_token && next_token->pos == token->pos)
// 				next_token = token->next;
// 			token->start = ft_substr(str, token->pos, next_token->pos - token->pos);
// 			token = token->next;
// 		}
// 		group = group->next;
// 	}
// }

t_token_group	*tokenize(char *str)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	t_token_group	*group;

	str = ft_strtrim(str, " ");
	if (!str || *str)
		return (NULL);
	i = 0;
	group = tokenize_groups(str);
	while (group)
	{
		while (str[i] && str[i] == ' ')
			i++;
		add_token(i, TK_CMD_SEQ_START, group);
		while (str[i])
		{
			type = get_token_type(&str[i]);
			t_len = get_token_type_len(type);
			if (type == TK_SINGLEQUOTE)
				i = tokenize_single_quote(str, i, group);
			else if (type == TK_DOUBLEQUOTE)
				i = tokenize_double_quote(str, i, group);
			else if (type == TK_SPACE)
				i = add_token_space(str, i, group);
			else if (type == TK_DASH)
				i = add_token_dash(str, i, group);
			else if (str_is_env_variable(&str[i]))
				add_token(i++, TK_ENVIRONEMENT_VAR, group);
			else if (type != TK_UNKNOWN)
				add_token(i += t_len, type, group);
			else
				i += t_len;
		}
		add_token(str, i, TK_CMD_SEQ_END, group);
		group = group->next;
	}
	// set_tokens_str(get_first_token_group(), str);
	return (get_first_token_group());
}
