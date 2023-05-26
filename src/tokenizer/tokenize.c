/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/26 16:42:27 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_group	*tokenize_groups(char *str)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	i = 0;
	str = ft_strtrim(str, " ");
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_type_len(type);
		if (t_len == 0)
			t_len = 1;
		if (type_is_end_of_seq(type))
		{
			if (type == TK_CMD_SEQ_END)
			{
				i++;
				add_token_group(str, type, i);
				break ;
			}
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
	if (str[i] == '\0')
	{
		add_token_group(&str[0], TK_CMD_SEQ_END, i);
	}	
	return (get_process()->token_groups);
}

int32_t	add_token_space(char *str, int32_t pos, t_token_group *group)
{
	t_token_type	type;

	type = TK_SPACE;
	add_token(pos, TK_SPACE, group);
	pos++;
	type = get_token_type(&str[pos]);
	while (type == TK_SPACE)
	{		
		pos++;
		type = get_token_type(&str[pos]);
	}
	return (pos);
}

int32_t	add_token_dash(char *str, int32_t pos, t_token_group *group)
{

	add_token(pos, TK_DASH, group);
	pos++;
	while (str[pos] && ft_isalnum(str[pos]) == 1)
		pos++;
	return (pos);
}

int32_t	add_token_dashdash(char *str, int32_t pos, t_token_group *group)
{
	add_token(pos, TK_DASHDASH, group);
	pos += 2;
	while (str[pos] && ft_isalnum(str[pos]) == 1)
		pos++;
	return (pos);
}

int32_t	add_token_env(char *str, int32_t pos, t_token_group *group, bool inside_dbl_quotes)
{
	t_token	*token;
	bool	withcurly_brace;

	withcurly_brace = false;
	token = add_token(pos, TK_ENVIRONEMENT_VAR, group);
	if (str[pos + 1] == '{')
		withcurly_brace = true;
	token->inside_dbl_quotes = inside_dbl_quotes;	
	if (withcurly_brace)
		pos += 2;
	return (pos);
}

void	split_group_tokens(t_token_group *group)
{
	t_token	*token;
	char	*str;
	int32_t	start;
	int32_t	len;

	str = group->str;
	token = group->first_token;
	while (token && token->next)
	{	
		len = token->tolal_len - token->token_len - token->next->token_len;
		start = token->pos + token->token_len;
		if (token->type == TK_SPACE)
		{
			while (str[start] == ' ')
			{
				start++;
				len--;
			}
		}
		else if (token->type == TK_ENVIRONEMENT_VAR)
		{
			get_env_value(get_env_variable(token->str));
		}
		token->str = ft_substr(str, start, len);
		token->token_str = ft_substr(str, start - token->token_len, token->token_len);
		token = token->next;
	}
}

t_token	*tokenize(t_token_group *group)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	t_process		*data;
	char			*str;

	data = get_process();
	i = 0;
	str = group->str;
	add_token(0, TK_CMD_SEQ_START, group)->tolal_len = 0;
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
		else if (type == TK_DASHDASH)
			i = add_token_dashdash(str, i, group);
		else if (type == TK_DASH)
			i = add_token_dash(str, i, group);
		else if (str_is_env_variable(&str[i]))
			i = add_token_env(str, i, group, false);
		else if (type == TK_CMD_SEQ_END)
			i++;
		else if (type != TK_UNKNOWN)
		{
			add_token(i, type, group);
			i += t_len;
		}
		else
			i += t_len;
	}
	add_token(i, TK_CMD_SEQ_END, group)->tolal_len = 0;
	split_group_tokens(group);
	return (data->tokens);
}
