/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 21:41:11 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	is_escaped_env_variable(char *str, int32_t i)
{
	if (!str || !str[i])
		return (false);
	return (str[i] == '\\' && str[i + 1] != '\0' && str[i + 1] == '$');
}

inline bool	is_env_variable(char *str, int32_t i)
{
	if (!str || !str[i])
		return (false);
	return (str[i] == '$');
}

inline int32_t	get_env_variable_len(char *str, int32_t i)
{
	size_t	len;
	int32_t	start;

	len = 0;
	if (!str || !str[i])
		return (0);
	start = i + 1;
	if (str[i] == '$' && ft_isalpha(str[start]) == 1)
	{
		i++;
		while (ft_isalnum(str[i]) == 1)
			len++;
	}
	return (len);
}

inline char	*get_env_variable(char *str, int32_t i)
{
	size_t	len;
	int32_t	start;
	char	*var_name;

	if (!str || !str[i])
		return (0);
	len = get_env_variable_len(str, i);
	start = i + 1;
	if (str[i] == '$' && ft_isalpha(str[start]) == 1)
	{
		var_name = ft_strncpy(&str[start], len);
		return (var_name);
	}
	return (NULL);
}
