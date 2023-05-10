/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 21:44:29 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_group	*tokenize_groups(char *str)
{
	int32_t			i;
	t_token_type	type;
	char			*start;

	i = 0;
	start = str;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		if (type_is_end_of_seq(type))
		{
			if (get_token_type_len(type) == 2)
				i++;
			add_token_group(start, &str[i + 1]);	
			start = &str[i + 1];			
		}
		i++;
	}
	if (start != &str[i])
		add_token_group(start, &str[i]);
	return (get_first_token_group());
}

void	replace_env_name(char *str)
{
	int32_t	i;

	i = 0;
	while (str[i])
	{
		if (is_opening_single_quote(str, i))
		{
			while (str[i])
			{
				if (is_closing_single_quote(str, i))
					break ;
				i++;
			}
		}
		else if (is_escaped_env_variable(str, i))
			i++;
		else if (is_env_variable(str, i))
			
		i++;
	}
}

int32_t	get_parsed_len(char *str)
{
	int32_t	i;
	int32_t	var_len;
	int32_t	full_len;
	char	*var_name;

	i = 0;
	var_len = 0;
	full_len = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		else if (is_env_variable(str, i))
		{
			var_name = get_env_variable(str, i);
			full_len += ft_strlen(get_env_value(var_name));
			i++;
		}
		i++;
	}
	return (full_len + i);
}

t_token	*tokenize(char *str)
{
	int32_t			i;
	t_token_type	type;
	t_token_group	*group;
	t_token			*token;

	group = tokenize_groups(str);
	while (group)
	{
		i = 0;
		str = group->start;
		token = NULL;
		while (str[i])
		{			
			type = get_token_type(&str[i]);
			if (is_opening_single_quote(str, i))
				i += tokenize_single_quote(str, i, group);
			else if (is_opening_double_quote(str, i))
				i += tokenize_double_quote(str, i, group);
			else if (type != TK_UNKNOWN)
				token = add_token(str, i, type, group);
			if (get_token_type_len(type) == 2)
				i++;
			i++;
		}		
	}
	return (get_first_token());
}
