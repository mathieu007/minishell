/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 20:34:33 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief env_bane = ex: PATH
/// @param envp 
/// @param env_name 
/// @return 
char	**parse_env(char **envp, char *env_name)
{
	char	**env;
	int32_t	len;
	char	**split_env;

	len = ft_strlen(env_name);
	split_env = NULL;
	if (!envp || !*envp)
		return (NULL);
	while (*envp)
	{
		if (ft_strnstr(*envp, env_name, len) != 0)
		{
			split_env = ft_split(*envp + len + 1, ':');
			break ;
		}
		envp++;
	}
	return (split_env);
}
