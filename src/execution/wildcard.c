/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 13:16:06 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	match_pattern(char *file, char *pattern)
{
	while (*file && *pattern && *file == *pattern)
	{
		file++;
		pattern++;
	}
	if (!*pattern)
		return (true);
	return (false);
}

bool	match_patterns(char *file, char **patterns, char *start_with,
		char *end_with)
{
	int32_t	i;
	int32_t	j;

	i = 0;
	j = 0;
	if (start_with && !ft_strstartwith(file, start_with))
		return (false);
	if (end_with && !ft_strendwith(file, end_with))
		return (false);
	while (patterns[i])
	{
		while (file[j])
		{
			if (match_pattern(&file[j], patterns[i]) == 1)
			{
				if (!patterns[i + 1])
					return (true);
				j += ft_strlen(patterns[i]);
				break ;
			}
			j++;
		}
		i++;
	}
	return (false);
}

char	**get_patterns(char *str)
{
	char	**splits;

	splits = ft_split(str, '*');
	if (!splits)
		return (NULL);
	return (splits);
}

char	*get_end_with(char *str_pattern)
{
	char	*end_with;

	end_with = NULL;
	if (!str_pattern)
		return (NULL);
	if (str_pattern[ft_strlen(str_pattern) - 1] != '*')
		end_with = ft_strchrlast(str_pattern, '*');
	if (end_with && end_with[0] == '*')
	{
		if (end_with[1] == '\0')
			end_with = NULL;
		else
			end_with = &end_with[1];
	}
	return (end_with);
}

char	*get_start_with(char *str_pattern)
{
	char	*start_with;
	int32_t	i;

	if (!str_pattern)
		return (NULL);
	start_with = NULL;
	if (str_pattern[0] != '*')
	{
		i = 0;
		while (str_pattern[i] && str_pattern[i] != '*')
			i++;
		start_with = ft_strncpy(str_pattern, i);
	}
	return (start_with);
}
