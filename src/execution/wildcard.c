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
}

char 	**find_cwd_files(char *str_pattern)
{
	DIR				*dp;
	struct dirent	*entry;
	char			**new;
	char			*cwd;
	char			**patterns;
	int32_t			i;

	patterns = get_patterns(str_pattern);
	new = malloc((count_matches(patterns) + 1) * sizeof(char *));
	if (!new)
		return (NULL);
	cwd = get_cwd();
	dp = opendir(cwd);
	entry = readdir(dp);
	i = 0;
	while (entry)
	{
		if (str_pattern != '.' && entry->d_name[0] != '.'
			&& match_patterns(entry->d_name, patterns))
				new[i++] = ft_strdup(entry->d_name);
		else if (match_patterns(entry->d_name, patterns))
			new[i++] = ft_strdup(entry->d_name);
		entry = readdir(dp);
	}
	free(cwd);
	closedir(dp);
	free_2d_char_array(patterns);
	return (new);
}
