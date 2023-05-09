/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 15:47:57 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_env(t_token *token)
{
	t_env *env;

	env = get_data()->env;
	
}

char	**parse_env_path(char **env)
{
	char	**env;
	int32_t	len;
	char	**split_env;

	len = ft_strlen("PATH");
	split_env = NULL;
	if (!env || !*env)
		return (NULL);
	while (*env)
	{
		if (ft_strnstr(*env, "PATH", len) != 0)
		{
			split_env = ft_split(*env + len + 1, ':');
			break ;
		}
		env++;
	}
	return (split_env);
}
