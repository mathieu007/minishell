/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 17:11:53 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_env(t_token *token)
{
	t_env_cpy	*env;
	char		*env_name;
	char		*env_value;

	env = get_data()->env;
	while (token)
	{		
		if (token->type == TK_DOLLAR_SIGN)
		{
			env_name = ++(token->start);
			env_value = get_env_value(env_name);
			if (env_value == NULL)
					
		}
	}	
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
