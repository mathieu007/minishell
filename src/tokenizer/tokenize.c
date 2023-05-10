/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 07:27:30 by math             ###   ########.fr       */
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

/// @brief single quote str is a string literal
int32_t	cpy_single_quote_str(char *str, char *output, int32_t i)
{
	if (!str[i])
		return (i);
	if (is_opening_single_quote(str, i))
	{
		while (str[i])
		{
			output[i] = str[i];
			if (is_closing_single_quote(str, i))
			{
				i++;
				break ;
			}
			i++;
		}
	}
	return (i);
}

void	replace_env_name(char *input, char *output)
{
	int32_t	i;
	int32_t	out_i;
	char	*var_name;

	i = 0;
	out_i = 0;
	while (input[i])
	{
		i = cpy_single_quote_str(input, output, i);
		if (is_escaped_env_variable(input, i))
		{
			output[i] = input[i++];
			output[i] = input[i];
		}
		else if (is_env_variable(input, i))
		{
			var_name = get_env_variable(input, i);
		}
		i++;
	}
}

int32_t	get_parsed_env_len(char *str)
{
	int32_t	i;
	int32_t	env_len;
	char	*var_name;

	i = 0;
	env_len = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		else if (is_env_variable(str, i))
		{
			var_name = get_env_variable(str, i);
			env_len += ft_strlen(get_env_value(var_name));
			i++;
		}
		i++;
	}
	return (env_len + i);
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
