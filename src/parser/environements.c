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
/// the return value need to be freed.
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

char	*strip_consecutive_white_space_dst(char *str)
{
	int32_t	strip_len;
	int32_t	i;
	char	*data;

	strip_len = 0;
	i = 0;
	if (str[i] != ' ')
		return (NULL);
	while (str[i] && str[i + 1])
	{
		if (str[i] == ' ' && str[i + 1] == ' ')
			strip_len--;
		strip_len++;
		i++;
	}
	data = malloc(strip_len + 1);
	if (!data)
		return (NULL);
	data[strip_len] = '\0';
	return (data);
}

char	*strip_consecutive_white_space(char *str)
{
	int32_t	i;

	i = 0;
	while (str[i] == ' ' && str[i + 1] == ' ')
		i++;
	return (&str[i]);
}

/// @brief We parse environement variable from these token group
/// @param group 
/// @return 
char	*group_to_str(t_token_group *group)
{
	t_token	*token;
	char	*dest;
	char	*cpy;

	token = group->first_token;
	if (!token)
		return (NULL);
	dest = NULL;
	while (token)
	{	
		if (token->type == TK_SPACE)
		{
			cpy = dest;
			dest = ft_strjoin(dest, " ");
			if (cpy)
				free(cpy);
			cpy = dest;
			dest = ft_strjoin(dest, token->str);
			if (cpy)
				free(cpy);
		}
		else if (token->type == TK_DOUBLEQUOTE || token->type == TK_CLOSINGDOUBLEQUOTE)
		{
			cpy = dest;
			dest = ft_strjoin(dest, "\"");
			if (cpy)
				free(cpy);
			cpy = dest;
			dest = ft_strjoin(dest, token->str);
			if (cpy)
				free(cpy);
		}
		else if (token->type == TK_SINGLEQUOTE || token->type == TK_CLOSINGSINGLEQUOTE)
		{
			cpy = dest;
			dest = ft_strjoin(dest, "'");
			if (cpy)
				free(cpy);
			cpy = dest;
			dest = ft_strjoin(dest, token->str);
			if (cpy)
				free(cpy);
		}
		else if (token->type == TK_ENVIRONEMENT_VAR)
		{
			cpy = dest;
			dest = ft_strjoin(dest, token->str);
			if (cpy)
				free(cpy);
		}
		else
		{
			cpy = dest;
			dest = ft_strjoin(dest, token->token_str);
			if (cpy)
				free(cpy);
			cpy = dest;
			dest = ft_strjoin(dest, token->str);
			if (cpy)
				free(cpy);
		}
		token = token->next;
	}
	return (dest);
}

/// @brief We parse environement variable from these token group
/// @param group 
/// @return 
t_token_group	*parse_env(t_token_group *group)
{
	t_token	*token;
	char	*env_value;
	char	*str;

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
				printf("parse_env_var_value:%s\n", env_value);
				printf("token->str:%s\n", token->str);
				token->str = strip_consecutive_white_space(env_value);
				printf("token->str:%s\n", token->str);				
				token->tolal_len = ft_strlen(token->str);
				printf("token->pos:%i\n", token->pos);
				free(str);
				// if (token->prev && token->prev->type == TK_SPACE && token->str[0] == ' ')
				// 	token->str = ft_substr(token->str, 1, token->tolal_len);
				// if (token->next && token->next->type == TK_SPACE && token->str[token->tolal_len - 1] == ' ')
				// 	token->str = ft_substr(token->str, 0, token->tolal_len);
			}
		}
		token = token->next;
	}
	return (group);
}

char	**get_env_path()
{
	char			**split_env;
	char			*env_value;

	split_env = NULL;
	env_value = get_env_value("PATH");
	if (!env_value)
		return (NULL);
	split_env = ft_split(env_value, ':');
	return (split_env);
}
