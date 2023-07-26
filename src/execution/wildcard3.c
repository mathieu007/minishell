/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/26 10:43:51 by mroy             ###   ########.fr       */
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
		return (free_ptr(start_with), free_2d_char_array(patterns),
			free_all_and_exit2(1, "cwd error"), NULL);
	matching_files = find_matching_files(cwd, patterns, start_with, end_with);
	if (!matching_files || !*matching_files)
	{
		if (matching_files || *matching_files)
			free_2d_char_array(matching_files);
		return (free_2d_char_array(patterns), free(cwd),
			free_ptr(start_with), NULL);
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

char	*add_files_to_str(char **matching_files, char *separators)
{
	char	*str;
	int32_t	i;
	size_t	size;
	int32_t	sep_size;
	char	**start;

	start = matching_files;
	sep_size = ft_strlen(separators);
	size = count_entries_size(matching_files);
	size += size * sep_size;
	str = malloc(size + 1);
	if (!str)
		free_all_and_exit2(errno, "malloc error");
	str[size] = '\0';
	i = 0;
	while (*matching_files)
	{
		ft_strcpy(&str[i], *matching_files);
		i += ft_strlen(*matching_files);
		ft_strcpy(&str[i], separators);
		i += sep_size;
		matching_files++;
	}
	return (free_2d_char_array(start), str);
}
