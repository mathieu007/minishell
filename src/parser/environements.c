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
	while (env_start[i] && ft_isalnum(env_start[i++]) == 1)
		len++;
	return (len);
}

char	*parse_env_var_name(t_token *token)
{
	size_t	len;
	char	*var_name;
	char	*env_start;

	env_start = token->token_str;
	if (*env_start == '$')
		env_start++;
	if (*env_start == '{')
		env_start++;
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

// char	*strip_consecutive_white_space(char *str)
// {
// 	int32_t	strip_len;
// 	int32_t	i;
// 	char	*data;

// 	strip_len = 0;
// 	i = 0;
// 	while (str[i] && str[i + 1])
// 	{
// 		if (str[i] == ' ' && str[i + 1] == ' ')
// 			strip_len--;
// 		strip_len++;
// 		i++;
// 	}
// 	data = malloc(strip_len + 1);
// 	if (!data)
// 		return (NULL);
// 		while (str[i] && str[i + 1])
// 	{
// 		if (str[i] == ' ' && str[i + 1] == ' ')
// 			strip_len--;
// 		else if (str[i] == ' ')
// 			data[i]
// 		strip_len++;
// 		i++;
// 	}
// 	data[strip_len] = '\0';
// 	return (data);
// }


char	*join_env_to_str(t_token_sequence *group)
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
		if (token->type == TK_ENVIRONEMENT_VAR)
		{
			cpy = dest;
			dest = ft_strjoin(dest, get_env_value(token->str));
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

// /// @brief We parse environement variable from these token group
// /// @param group 
// /// @return 
// char	*group_to_str(t_token_group *group)
// {
// 	t_token	*token;
// 	char	*dest;
// 	char	*cpy;

// 	token = group->first_token;
// 	if (!token)
// 		return (NULL);
// 	dest = NULL;
// 	while (token)
// 	{	
// 		if (token->type == TK_SPACE)
// 		{
// 			cpy = dest;
// 			dest = ft_strjoin(dest, " ");
// 			if (cpy)
// 				free(cpy);
// 			cpy = dest;
// 			dest = ft_strjoin(dest, token->str);
// 			if (cpy)
// 				free(cpy);
// 		}
// 		else if (token->type == TK_DOUBLEQUOTE || token->type == TK_CLOSINGDOUBLEQUOTE)
// 		{
// 			cpy = dest;
// 			dest = ft_strjoin(dest, "\"");
// 			if (cpy)
// 				free(cpy);
// 			cpy = dest;
// 			dest = ft_strjoin(dest, token->str);
// 			if (cpy)
// 				free(cpy);
// 		}
// 		else if (token->type == TK_SINGLEQUOTE || token->type == TK_CLOSINGSINGLEQUOTE)
// 		{
// 			cpy = dest;
// 			dest = ft_strjoin(dest, "'");
// 			if (cpy)
// 				free(cpy);
// 			cpy = dest;
// 			dest = ft_strjoin(dest, token->str);
// 			if (cpy)
// 				free(cpy);
// 		}
// 		else if (token->type == TK_ENVIRONEMENT_VAR)
// 		{
// 			cpy = dest;
// 			dest = ft_strjoin(dest, token->str);
// 			if (cpy)
// 				free(cpy);
// 		}
// 		else
// 		{
// 			cpy = dest;
// 			dest = ft_strjoin(dest, token->token_str);
// 			if (cpy)
// 				free(cpy);
// 			cpy = dest;
// 			dest = ft_strjoin(dest, token->str);
// 			if (cpy)
// 				free(cpy);
// 		}
// 		token = token->next;
// 	}
// 	return (dest);
// }

/// @brief We parse environement variable from these token group
/// @param group 
/// @return 
char	*parse_env(t_token_sequence *group)
{
	t_token	*token;
	char	*str;
	char	*dest;
	char	*cpy;

	dest = NULL;
	token = group->first_token;
	while (token)
	{
		str = token->str;
		if (token->type == TK_ENVIRONEMENT_VAR)
			dest = ft_strjoin(dest, parse_env_var_value(token));
		else
		{
			cpy = dest;
			dest = ft_strjoin(dest, token->token_str);
			if (cpy)
				free(cpy);
		}
		cpy = dest;
		dest = ft_strjoin(dest, str);
		if (cpy)
			free(cpy);
		token = token->next;
	}
	return (dest);
}

char	**get_env_path(void)
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
