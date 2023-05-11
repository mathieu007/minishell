/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 11:37:00 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief this function assusme that preceding char is not an escaped char 
/// an environement variable must start with a $[alpha]
/// @param str 
/// @param i 
/// @return 
int32_t	parse_env_var_name_len(char *env_start, int32_t i)
{
	size_t	len;
	int32_t	start;

	len = 0;
	if (!env_start || !env_start[i])
		return (0);
	start = i + 1;
	if (env_start[i] == '$' && ft_isalpha(env_start[start]) == 1)
	{
		i++;
		while (ft_isalnum(env_start[i]) == 1)
			len++;
	}
	return (len);
}

char	*parse_env_var_name(char *env_start, int32_t i)
{
	size_t	len;
	int32_t	start;
	char	*var_name;

	if (!env_start || !env_start[i])
		return (NULL);
	len = parse_env_var_name_len(env_start, i);
	if (len == 0)
		return (NULL);
	start = i + 1;
	if (env_start[i] == '$' && ft_isalpha(env_start[start]) == 1)
	{
		var_name = ft_strncpy(&env_start[start], len);
		return (var_name);
	}
	return (NULL);
}

/// @brief this function assume that the input is a var_name
char	*cpy_env_var_value(char *input, char *output, int32_t *i)
{
	char	*var_name;
	char	*var_value;
	int32_t	var_name_len;
	int32_t	var_value_len;
	
	if (!input[*i])
		return (output);
	var_name_len = parse_env_var_name_len(input, i);		
	var_name = parse_env_var_name(input, i);	
	var_value = get_env_value(var_name);
	var_value_len = ft_strlen(var_value);
	while (var_value_len != 0)
	{
		*output++ = input[(*i)++];
		var_value_len--;
	}
	*i = i + var_name_len + 1;
	return (output);
}

/// @brief The input parameter is the original input string,
/// the output parameter is the new dst string.
/// This function replace all environements variables name with their
/// values, excluding var inside single quote string. 
/// @param input 
/// @param output 
void	replace_env_name(char *input, char *output)
{
	int32_t	i;

	i = 0;
	while (input[i])
	{
		if (is_opening_single_quote(input, i))
			output = cpy_single_quote_str(input, output, &i);
		else if (is_esc_env_var(input, i))
			output = cpy_esc_env_var(input, output, &i);
		else if (is_env_variable(input, i))
			output = cpy_env_var_value(input, output, &i);
		else
			*output++ = input[i++];
	}
}

int32_t	get_new_env_len(char *str)
{
	int32_t	i;
	int32_t	env_len;
	char	*var_name;

	i = 0;
	env_len = 0;
	if (!*str || !str[i])
		return (0);
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		else if (is_env_variable(str, i))
		{
			var_name = parse_env_var_name(str, i);
			env_len += ft_strlen(get_env_value(var_name));
			i++;
		}
		i++;
	}
	return (env_len + i);
}

char	*parse_env(char *str)
{
	int32_t	i;
	int32_t	new_env_len;
	char 	*dest;

	new_env_len = get_new_env_len(str);
	if (new_env_len == 0)
		return (NULL);
	dest = malloc(new_env_len + 1);
	replace_env_name(str, dest);
	return (dest);
}

char	**parse_env_path(char **env)
{
	char			**env;
	const int32_t	var_name_len = 4;
	char			**split_env;
	
	split_env = NULL;
	if (!env || !*env)
		return (NULL);
	while (*env)
	{
		if (ft_strnstr(*env, "PATH", var_name_len) != 0)
		{
			split_env = ft_split(*env + var_name_len + 1, ':');
			break ;
		}
		env++;
	}
	return (split_env);
}
