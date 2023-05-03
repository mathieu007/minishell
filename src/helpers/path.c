/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 12:54:35 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline char *_join(const char *path1, const char *path2)
{
	int32_t	len;
	int32_t	len2;
	char 	*new;
	
	if (!path1 || !path2)
		return (false);		
	len = ft_strlen(path1);
	len2 = ft_strlen(path2);
	new = malloc(len * len2);
	if (!new)
		return (NULL);
	while (*path1)
		*new++ = *path1++;
	while (*path2)
		*new++ = *path1++;
	return (new);
}

/// @brief free path2.
/// @param path 
/// @param path2 
/// @return 
char	*join_free2(const char *path1, char *path2)
{
	char 	*new;
	
	if (!path1 || !path2)
		return (false);
	new = _join(path1, path2);
	return (free(path2), new);	
}

/// @brief path2 is a constant and not malloc'ed.
/// @param path 
/// @param path2 
/// @return 
char	*join_free1(char *path1, const char *path2)
{
	char 	*new;
	
	if (!path1 || !path2)
		return (false);
	new = _join(path1, path2);
	return (free(path1), new);	
}

/// @brief path1 and path2 will not be freed.
/// @param path 
/// @param path2 
/// @return 
char	*join(const char *path1, const char *path2)
{
	char 	*new;
	
	if (!path1 || !path2)
		return (false);
	new = _join(path1, path2);
	return (new);	
}

/// @brief all path must have been malloc'ed.
/// @param path 
/// @param path2 
/// @return 
char	*join_free(char *path, char *path2)
{
	char 	*new;
	
	if (!path || !path2)
		return (false);
	new = _join(path, path2);
	return (free(path), free(path2), new);
}
