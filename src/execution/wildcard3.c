/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 21:48:35 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_cwd_files_array(char *str_pattern, char *separator)
{
	char	*cwd;
	char	**patterns;
	char	**matching_files;
	char	*start_with;
	char	*end_with;

	if (!str_pattern || !separator)
		return (NULL);
	start_with = get_start_with(str_pattern);
	end_with = get_end_with(str_pattern);
	patterns = get_patterns(str_pattern);
	cwd = get_cwd();
	if (!cwd)
		return (free_ptr(start_with), NULL);
	matching_files = find_matching_files(cwd, patterns, start_with, end_with);
	if (!matching_files || !*matching_files)
	{
		if (*matching_files)
			free(matching_files);
		return (NULL);
	}
	return (free_ptr(cwd), free_ptr(start_with), free_2d_char_array(patterns),
		matching_files);
}

char	*get_cwd_files_as_string(char *str_pattern, char *separator)
{
	char	*cwd;
	char	**patterns;
	char	**matching_files;
	char	*start_with;
	char	*end_with;

	if (!str_pattern || !separator)
		return (NULL);
	start_with = get_start_with(str_pattern);
	end_with = get_end_with(str_pattern);
	patterns = get_patterns(str_pattern);
	cwd = get_cwd();
	if (!cwd)
		return (free(cwd), free_ptr(start_with), NULL);
	matching_files = find_matching_files(cwd, patterns, start_with, end_with);
	if (!matching_files)
		return (free(cwd), free_ptr(start_with), NULL);
	return (free(cwd), free_ptr(start_with), free_2d_char_array(patterns),
		add_files_to_str(matching_files, separator));
}
