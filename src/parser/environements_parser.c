/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement_parser.c                                              :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
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
	i = 0;
	while (env_start[i] && ft_isalnum(env_start[i++]) == 1)
		len++;
	return (len);
}

char	*parse_env_var_name(t_token *token)
{
	size_t	len;
	char	*var_name;
	char	*env_start;

	env_start = token->token_str;
	if (*env_start == '$')
		env_start++;
	if (*env_start == '{')
		env_start++;
	len = parse_env_var_name_len(env_start);
	var_name = ft_strncpy(env_start, len);
	return (var_name);
}

/// @brief this function assume that the input is a var_name
/// the return value need to be freed.
char	*parse_env_var_value(t_token *token)
{
	char	*var_value;

	var_value = get_env_value(token->str);
	if (!var_value)
	{
		var_value = malloc(1);
		var_value[0] = '\0';
	}
	return (var_value);
}

char	**get_env_path(void)
{
	char	**split_env;
	char	*env_value;

	split_env = NULL;
	env_value = get_env_value("PATH");
	if (!env_value)
		return (NULL);
	split_env = ft_split(env_value, ':');
	free(env_value);
	return (split_env);
}
