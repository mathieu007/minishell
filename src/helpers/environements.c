/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/28 15:47:21 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	is_esc_env_var(char *str, int32_t i)
{
	if (!str || !str[i])
		return (false);
	return (str[i] == '\\' && str[i + 1] != '\0' && str[i + 1] == '$');
}

/// @brief this function assusme that preceding char is not an escaped char
/// use is_escaped_env_variable(str ,i) before
/// @param str 
/// @param i 
/// @return 
inline bool	is_env_variable(t_token *token)
{
	char	*str;

	str = token->str;
	if (!str || !*str)
		return (false);
	if (*str == '$')
	{
		str++;
		if (ft_isalpha(*str) == 1)
			return (true);
		else if (*str++ == '{' && ft_isalpha(*str) == 1)
			return (true);
	}
	return (false);
}

/// @brief this function assusme that preceding char is not an escaped char
/// use is_escaped_env_variable(str ,i) before
/// @param str 
/// @param i 
/// @return 
inline bool	str_is_env_variable(char *str)
{
	if (!str || !*str)
		return (false);
	if (*str == '$')
	{
		str++;
		if (ft_isalpha(*str) == 1)
			return (true);
		else if (*str++ == '{' && ft_isalpha(*str) == 1)
			return (true);
	}
	return (false);
}

/// @brief this function assusme that preceding char is not an escaped char
/// use is_escaped_env_variable(str ,i) before
/// @param str 
/// @param i 
/// @return 
inline int32_t	get_env_var_name_len(char *str)
{
	char	*start;

	start = str;
	if (!str || !*str)
		return (0);
	if (*str == '$')
	{	
		str++;
		if (ft_isalpha(*str) == 1)
		{
			str++;
			while (*str && ft_isalnum(*str))
				str++;
			return (str - start - 1);
		}
		else if (*str == '{')
		{
			str++;
			while (*str && ft_isalnum(*str))
				str++;
			return (str - start - 3);
		}
	}
	return (0);
}

// char	*get_env_variable(char *str)
// {
// 	char	*start;
// 	int32_t	len;

// 	if (!str || !*str)
// 		return (NULL);
// 	if (*str == '$')
// 		str++;
// 	if (*str == '{')
// 		str++;
// 	start = str;
// 	if (ft_isalpha(*str) == 1)
// 	{
// 		str++;
// 		while (*str && ft_isalnum(*str))
// 			str++;
// 		return (ft_substr(start, 0, str - start));
// 	}
// 	return (NULL);
// }
