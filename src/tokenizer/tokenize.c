/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/17 16:26:20 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_group	*tokenize_groups(char *str)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	i = 0;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_type_len(type);
		if (t_len == 0)
			t_len = 1;
		if (type_is_end_of_seq(type))
		{
			add_token_group(&str[0], type, i);
			i += t_len;
			while (str[i] && str[i] == ' ')
				i++;
			str = &str[i];
			i = 0;
			continue ;
		}
		i += t_len;
	}
	if (i != 0)
	{
		add_token_group(&str[0], TK_CMD_SEQ_END, i);
		while (str[i] && str[i] == ' ')
			i++;
	}	 	
	return (get_first_token_group());
}

int32_t	add_token_space(char *str, int32_t pos, t_token_group *group)
{
	t_token			*token;
	t_token_type	type;
	int32_t			repeat;
	int32_t			start;

	type = TK_SPACE;
	repeat = 1;
	start = pos;
	pos++;
	type = get_token_type(&str[pos]);
	while (type == TK_SPACE)
	{		
		pos++;		
		repeat++;
		type = get_token_type(&str[pos]);
	}
	token = add_token(start, TK_SPACE, group);
	token->tolal_len = repeat;
	return (pos);
}

int32_t	add_token_dash(char *str, int32_t pos, t_token_group *group)
{
	t_token			*token;
	int32_t			start_pos;

	start_pos = pos;
	token = add_token(pos, TK_DASH, group);
	while (str[pos] && str[pos] != ' ')
		pos++;
	token->str = ft_strncpy(&str[start_pos], pos - start_pos);
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

void	split_groups_tokens(t_token_group *group)
{
	t_token	*token;
	t_token *prev;
	int32_t	start;
	char	*str;
	
	while (group)
	{
		str = group->str;
		token = group->first_token;
		prev = token;
		while (token)
		{			
			start = prev->pos + prev->tolal_len;
			if (prev->type == TK_ENVIRONEMENT_VAR)
				start += 1;
			if (token->type == TK_ENVIRONEMENT_VAR && str[token->pos + 1])
				token->str = ft_substr(str, token->pos + 1, token->tolal_len);
			else if (str[start])
				token->str = ft_substr(str, start, token->pos - start);
			prev = token;
			token = token->next;
		}
		group = group->next;
	}	
}

t_token_group	*tokenize(char *str)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	t_token_group	*group;
	int32_t			len; 
	t_token			*token;
	
	len = ft_strlen(str);
	str = ft_strtrim(str, " ");	
	if (!str && *str)
		return (NULL);	
	group = tokenize_groups(str);
	while (group && group->str)
	{
		i = 0;
		str = group->str;
		while (str[i] && str[i] == ' ')
			i++;
		add_token(i, TK_CMD_SEQ_START, group)->tolal_len = 0;
		while (str[i])
		{
			type = get_token_type(&str[i]);
			t_len = get_token_type_len(type);
			if (t_len == 0)
				t_len = 1;
			if (type == TK_SINGLEQUOTE)
				i = tokenize_single_quote(str, i, group);
			else if (type == TK_DOUBLEQUOTE)
				i = tokenize_double_quote(str, i, group);
			else if (type == TK_SPACE)
				i = add_token_space(str, i, group);
			else if (type == TK_DASH)
				i = add_token_dash(str, i, group);
			else if (str_is_env_variable(&str[i]))
			{
				add_token(i, TK_PRE_ENVIRONEMENT_VAR, group);
				token = add_token(i, TK_ENVIRONEMENT_VAR, group);
				token->tolal_len = get_env_var_name_len(&str[i]);
				i++;
			}				
			else if (type != TK_UNKNOWN)
				add_token(i += t_len, type, group);
			else
				i += t_len;
		}
		add_token(i, TK_CMD_SEQ_END, group)->tolal_len = 0;
		group = group->next;
	}
	split_groups_tokens(get_first_token_group());	
	return (get_first_token_group());
}
