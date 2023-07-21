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

int32_t	match_patterns(char *file, char **patterns)
{
	while (*patterns)
	{			
		while (*file)
		{
			if (match_pattern(file, *patterns) == 1 && !patterns[1])
				return (true);
			file++;
		}
		patterns++;
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

size_t	count_matches(char **patterns)
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
			if (entry->d_name[0] != '.' && match_patterns(entry->d_name, patterns))
				count++;
		}
		else
			if (match_patterns(entry->d_name, patterns))
				count++;
		entry = readdir(dp);
	}
	closedir(dp);
	return (count);
}

char 	*get_matching_entry(struct dirent *entry, char **patterns)
{
	if (patterns[0][0] != '.' && entry->d_name[0] != '.'
			&& match_patterns(entry->d_name, patterns))
				return (ft_strdup(entry->d_name));
	else if (match_patterns(entry->d_name, patterns))
		return (ft_strdup(entry->d_name));
	return (NULL);
}

char 	**find_matching_files(char *cwd, char **patterns)
{
	DIR				*dp;
	struct dirent	*entry;
	char			**new;
	int32_t			i;

	new = malloc((count_matches(patterns) + 1) * sizeof(char *));
	if (!new)
		return (NULL);
	dp = opendir(cwd);
	entry = readdir(dp);
	i = 0;
	while (entry)
	{
		new[i] = get_matching_entry(entry, patterns);
		if (new[i])
			i++;
		entry = readdir(dp);
	}
	closedir(dp);
	free_2d_char_array(patterns);
	return (new);
}

size_t	count_entries_size(char **entries)
{
	size_t size;
	int32_t i;

	size = 0;
	i = 0;
	while (entries[i])
	{
		size += ft_strlen(entries[i]);
		i++;
	}		
	return (size);
}

char *add_files_to_str(char **matching_files)
{
	char	*str;
	int32_t i;

	str = malloc(count_entries_size(matching_files));
	if (!str)
		free_all_and_exit2(errno, "malloc error");
	i = 0;
	while (*matching_files)
	{
		ft_strcpy(&str[i], *matching_files);
		matching_files++;
		i +=  ft_strlen(*matching_files);
	}
	return (str);
}

char *get_cwd_files_as_string(char *str_pattern, char *separator)
{
	char 	*cwd;
	char 	**patterns;
	char 	**matching_files;

	if (!str_pattern || !separator)
		return (NULL);
	patterns = get_patterns(str_pattern);
	cwd = get_cwd();
	if (!cwd)
		return (NULL);
	matching_files = find_matching_files(cwd, patterns);
	if (!matching_files)
		return (NULL);
	return (add_files_to_str(matching_files));
}
