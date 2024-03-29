/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/26 10:19:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_all_files(struct dirent *entry, DIR *dp)
{
	size_t	count;

	count = 0;
	while (entry)
	{
		if (entry->d_name[0] != '.')
			count++;
		entry = readdir(dp);
	}
	return (count);
}

size_t	count_matches(char **patterns, char *start_with, char *end_with)
{
	DIR				*dp;
	struct dirent	*entry;
	size_t			count;
	char			*cwd;

	cwd = get_cwd();
	dp = opendir(cwd);
	entry = readdir(dp);
	if (!patterns || !*patterns)
		return (free(cwd), count_all_files(entry, dp));
	count = 1;
	while (entry)
	{
		if ((!ft_strequal(entry->d_name, ".")
				&& !ft_strequal(entry->d_name, ".."))
			&& match_patterns(entry->d_name, patterns, start_with, end_with))
			count++;
		entry = readdir(dp);
	}
	free(cwd);
	closedir(dp);
	return (count);
}

char	*get_matching_entry(struct dirent *entry, char **patterns,
		char *start_with, char *end_with)
{
	if (ft_strequal(entry->d_name, ".") || ft_strequal(entry->d_name, ".."))
		return (NULL);
	if ((!patterns || !*patterns))
		return (ft_strdup(entry->d_name));
	else if (match_patterns(entry->d_name, patterns, start_with, end_with))
		return (ft_strdup(entry->d_name));
	return (NULL);
}

char	**find_matching_files(char *cwd, char **patterns, char *start_with,
		char *end_with)
{
	DIR				*dp;
	struct dirent	*entry;
	char			**new;
	int32_t			i;
	int32_t			count;

	count = count_matches(patterns, start_with, end_with);
	new = ft_calloc((count + 10), sizeof(char *));
	if (!new)
		return (NULL);
	new[count] = NULL;
	new[0] = NULL;
	dp = opendir(cwd);
	entry = readdir(dp);
	i = 0;
	while (entry)
	{
		new[i] = get_matching_entry(entry, patterns, start_with, end_with);
		if (new[i])
			i++;
		entry = readdir(dp);
	}
	closedir(dp);
	return (new);
}

size_t	count_entries_size(char **entries)
{
	size_t	size;
	int32_t	i;

	size = 0;
	i = 0;
	while (entries[i])
	{
		size += ft_strlen(entries[i]);
		i++;
	}
	return (size);
}
