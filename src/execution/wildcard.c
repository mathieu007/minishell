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

bool	match_patterns(char *file, char **patterns, char *start_with, char *end_with)
{
	int32_t i;
	int32_t j;

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

size_t	count_matches(char **patterns, char *start_with, char *end_with)
{
	DIR				*dp;
	struct dirent	*entry;
	size_t			count;
	char			*cwd;

	cwd = get_cwd();
	dp = opendir(cwd);
	entry = readdir(dp);
	count = 0;
	while (entry)
	{
		if (*patterns[0] != '.')
		{
			if (entry->d_name[0] != '.'
				&& match_patterns(entry->d_name, patterns, start_with, end_with))
				count++;
		}
		else
			if (match_patterns(entry->d_name, patterns, start_with, end_with))
				count++;
		entry = readdir(dp);
	}
	free(cwd);
	closedir(dp);
	return (count);
}

char 	*get_matching_entry(struct dirent *entry, char **patterns, char *start_with, char *end_with)
{
	if (patterns[0][0] != '.' && entry->d_name[0] != '.'
			&& match_patterns(entry->d_name, patterns, start_with, end_with))
				return (ft_strdup(entry->d_name));
	else if (match_patterns(entry->d_name, patterns, start_with, end_with))
		return (ft_strdup(entry->d_name));
	return (NULL);
}

char 	**find_matching_files(char *cwd, char **patterns, char *start_with, char *end_with)
{
	DIR				*dp;
	struct dirent	*entry;
	char			**new;
	int32_t			i;
	int32_t			count;

	count = count_matches(patterns, start_with, end_with);
	new = malloc((count + 1) * sizeof(char *));
	if (!new)
		return (NULL);
	new[count] = NULL;
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

char *add_files_to_str(char **matching_files, char *separators)
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

char *get_cwd_files_as_string(char *str_pattern, char *separator)
{
	char 	*cwd;
	char 	**patterns;
	char 	**matching_files;
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

char **get_cwd_files_array(char *str_pattern, char *separator)
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
	if (!matching_files)
		return (NULL);
	return (free_ptr(cwd), free_ptr(start_with),
		free_2d_char_array(patterns), matching_files);
}
