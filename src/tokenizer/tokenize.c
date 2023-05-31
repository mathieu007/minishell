/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/31 10:04:30 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_sequence	*tokenize_sequences(char *str)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	i = 0;
	str = ft_strtrim(str, " ");
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (type_is_end_of_seq(type))
		{
			if (type == TK_CMD_SEQ_END)
			{
				i++;
				add_token_group(str, type, i);
				break ;
			}
			add_token_group(str, type, i);
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
		add_token_group(str, TK_CMD_SEQ_END, i);
	return (get_process()->token_groups);
}

int32_t	add_token_space(char *str, int32_t pos, t_token_sequence *group)
{
	t_token			*token;

	token = add_token(pos, TK_SPACE, group);
	token->token_len = get_token_space_len(&str[pos]);
	token->token_str = ft_strdup(" ");
	token->end = pos + token->token_len;
	return (token->end);
}

int32_t	add_token_dash(char *str, int32_t pos, t_token_sequence *group)
{
	t_token	*token;

	token = add_token(pos, TK_DASH, group);
	token->token_len = get_token_dash_len(&str[pos]);
	token->token_str = ft_substr(str, pos, token->token_len);
	token->end = pos + token->token_len;
	return (token->end);
}

int32_t	get_token_dashdash_len(char *str)
{
	int32_t	i;

	i = 2;
	while (str[i] && ft_isalnum(str[i]) == 1)
		i++;
	return (i);
}

int32_t	get_token_space_len(char *str)
{
	int32_t	pos;

	pos = 1;
	while (str[pos] && str[pos] == ' ')
		pos++;
	return (pos);
}

int32_t	get_token_dash_len(char *str)
{
	int32_t pos;

	pos = 1;
	while (str[pos] && str[pos] != ' ')
		pos++;
	return (pos);
}

int32_t	get_token_env_len(char *str)
{
	int32_t	pos;

	pos = 1;
	if (str[pos] == '{')
		pos++;
	while (str[pos] && ft_isalnum(str[pos]) == 1)
		pos++;
	if (str[pos] == '}')
		pos++;
	return (pos);
}

int32_t	add_token_dashdash(char *str, int32_t pos, t_token_sequence *group)
{
	t_token	*token;

	token = add_token(pos, TK_DASHDASH, group);
	token->token_len = get_token_dashdash_len(&str[pos]);
	token->end = pos + token->token_len;
	token->token_str = ft_substr(str, pos, token->token_len);
	return (token->end);
}

int32_t	add_token_env(char *str, int32_t pos,
	t_token_sequence *group, bool inside_dbl_quotes)
{
	t_token		*token;

	token = add_token(pos, TK_ENVIRONEMENT_VAR, group);
	token->token_len = get_token_env_len(&str[pos]);
	token->inside_dbl_quotes = inside_dbl_quotes;
	token->end = pos + token->token_len;
	token->token_str = ft_substr(str, pos, token->token_len);
	return (token->end);
}

void	split_tokens(t_token_sequence *group)
{
	t_token	*token;
	char	*str;
	int32_t	start;
	int32_t	len;

	str = group->str;
	token = group->first_token;
	while (token && token->next)
	{	
		len = token->next->start - token->end;
		start = token->end;
		token->str = ft_substr(str, start, len);
		token->str_len = len;
		token = token->next;
	}
}

static int32_t	add_other_token(char *token_str, t_token_type type,
	int32_t i, t_token_sequence *group)
{
	t_token			*token;

	token = add_token(i, type, group);
	token->token_len = ft_strlen(token_str);
	token->token_str = token_str;
	token->end = i + token->token_len;
	return (i);
}

t_token	*tokenize(t_token_sequence *group)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	t_process		*proc;
	char			*str;

	proc = get_process();
	i = 0;
	str = ft_strtrim(group->str, " ");
	add_other_token(ft_strdup(""), TK_CMD_SEQ_START, 0, group);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (type == TK_SINGLEQUOTE)
			i = tokenize_single_quote(str, i, group);
		else if (type == TK_DOUBLEQUOTE)
			i = tokenize_double_quote(str, i, group);
		// else if (type == TK)
		// 	i = tokeniu(str, i, group);			
		else if (type == TK_SPACE)
			i = add_token_space(str, i, group);
		else if (type == TK_DASHDASH)
			i = add_token_dashdash(str, i, group);
		else if (type == TK_DASH)
			i = add_token_dash(str, i, group);
		else if (str_is_env_variable(&str[i]))
			i = add_token_env(str, i, group, false);
		// else if (type == TK_CMD_SEQ_END)
		// 	i = add_other_token(ft_substr(&str[i], 0, t_len), type, i, group);
		else if (type != TK_UNKNOWN)
			i = add_other_token(ft_substr(&str[i], 0, t_len), type, i, group);
		else
			i += t_len;
	}
	add_other_token(ft_strdup(""), TK_CMD_SEQ_END, i, group);
	split_tokens(group);
	return (proc->tokens);
}
