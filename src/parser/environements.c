#include "minishell.h"

/// @brief this function assusme that preceding char is not an escaped char 
/// an environement variable must start with a $[alpha]
/// @param str 
/// @param i 
/// @return 
int32_t	parse_env_var_name_len(char *env_start)
{
	size_t	len;
	int32_t	i;

	len = 0;
	i = 0;
	// if (!env_start && ft_isalpha(env_start[1]) == 0)
	// 	return (0);
	while (env_start[i] && ft_isalnum(env_start[i++]) == 1)
		len++;
	return (len);
}

char	*parse_env_var_name(t_token *token)
{
	size_t	len;
	char	*var_name;
	char	*env_start;

	env_start = token->str;
	len = parse_env_var_name_len(env_start);
	var_name = ft_strncpy(env_start, len);
	return (var_name);
}

/// @brief this function assume that the input is a var_name
char	*parse_env_var_value(t_token *token)
{
	char	*var_name;
	char	*var_value;

	var_name = parse_env_var_name(token);
	var_value = get_env_value(var_name);	
	if (!var_value)
	{
		var_value = malloc(1);
		var_value[0] = '\0';
	}
	free(var_name);
	return (var_value);
}

int32_t	count_strip_consecutive_white_sapce(char *str)
{
	int32_t	strip_len;
	int32_t	i;

	strip_len = 0;
	i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == ' ' && str[i + 1] == ' ')
			strip_len--;
		strip_len++;
		i++;
	}
	return (strip_len);
}

char	*strip_consecutive_white_sapce(char *str)
{
	int32_t	strip_len;
	int32_t	i;
	int32_t	insert_i;
	char	*strip;

	strip_len = count_strip_consecutive_white_sapce(str);
	i = 0;
	insert_i = 0;
	strip = malloc(strip_len + 1);
	strip[strip_len] = '\0';
	while (str[i] && str[i + 1])
	{
		if ((str[i] == ' ' && str[i + 1] != ' ') || str[i] != ' ')
			strip[insert_i++] = str[i];
		else if (str[i] == ' ' && str[i + 1] == ' ')
		{
			i++;
			continue ;
		}
		i++;
	}
}

/// @brief We parse environement variable from these token group
/// @param group 
/// @return 
t_token_group	*parse_env(t_token_group *group)
{
	t_token	*token;
	char	*env_value;
	char	*str;
	int32_t	count;

	token = group->first_token;
	while (token)
	{
		str = token->str;
		if (token->type == TK_ENVIRONEMENT_VAR)
		{
			if (token->inside_dbl_quotes)
			{
				env_value = parse_env_var_value(token);
				token->str = env_value;
				token->tolal_len = ft_strlen(env_value);
				free(str);
			}
			else
			{
				env_value = parse_env_var_value(token);
				token->str = strip_consecutive_white_sapce(env_value);
				token->tolal_len = count_strip_consecutive_white_sapce(env_value);
				free(env_value);
				free(str);
			}
		}
		token = token->next;
	}
	return (group);
}

char	**parse_env_path(char **env)
{
	const int32_t	var_name_len = 4;
	char			**split_env;

	split_env = NULL;
	if (!env || !*env)
		return (NULL);
	while (*env)
	{
		if (ft_strnstr(*env, "PATH", var_name_len) != 0)
		{
			split_env = ft_split(*env + var_name_len + 1, ':');
			break ;
		}
		env++;
	}
	return (split_env);
}
