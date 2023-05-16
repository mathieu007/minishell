/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/14 09:59:09 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief this function assusme that preceding char is not an escaped char 
/// an environement variable must start with a $[alpha]
/// @param str 
/// @param i 
/// @return 
int32_t	parse_env_var_name_len(char *env_start)
{
	size_t	len;
	int32_t	i;

	len = 0;
	i = 1;
	if (!env_start || env_start[0] != '$' || ft_isalpha(env_start[1]) == 0)
		return (0);
	while (env_start[i] && ft_isalnum(env_start[i++]) == 1)
		len++;
	return (len);
}

char	*parse_env_var_name(t_token *token)
{
	size_t	len;
	char	*var_name;
	char	*env_start;

	env_start = token->start;
	if (!env_start || env_start[0] != '$' || ft_isalpha(env_start[1]) == 0)
		return (NULL);
	len = parse_env_var_name_len(env_start);
	var_name = ft_strncpy(&env_start[1], len);
	return (var_name);
}

/// @brief this function assume that the input is a var_name
char	*parse_env_var_value(t_token *token)
{
	char	*var_name;
	char	*var_value;
	char	*env_start;

	env_start = token->start;
	if (!env_start || env_start[0] != '$' || ft_isalpha(env_start[1]) == 0)
		return (NULL);
	var_name = parse_env_var_name(token);
	var_value = get_env_value(var_name);
	if (!var_value)
	{
		var_value = malloc(1);
		var_value[0] = '\0';
	}
	free(var_name);
	return (var_value);
}

/// @brief We parse environement variable from these token group
/// @param group 
/// @return 
t_token_group	*parse_env(t_token_group *group)
{
	t_token	*token;
	char	*temp;
	char	*env_value;

	token = group->first;
	while (token)
	{
		if (token->type == TK_DOLLAR_SIGN && ft_isalpha(token->start[1]) == 1)
		{
			temp = token->start;
			env_value = parse_env_var_value(token);
			if (env_value)
			{
				token->start = env_value;
				free(temp);
			}
		}
		token = token->next;
	}
	return (group);
}

char	**parse_env_path(char **env)
{
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
