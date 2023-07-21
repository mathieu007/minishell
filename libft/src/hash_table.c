/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:47:31 by mroy              #+#    #+#             */
/*   Updated: 2023/07/21 08:28:48 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// // Simple hash table implemented in C.

// #include "libft.h"
// #include <assert.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>

// t_hash_table	*hash_new(void)
// {
// 	t_hash_table	*table;

// 	table = malloc(sizeof(t_hash_table));
// 	if (table == NULL)
// 	{
// 		return (NULL);
// 	}
// 	table->length = 0;
// 	table->capacity = INITIAL_HASH_CAPACITY;
// 	table->entries = ft_calloc(table->capacity, sizeof(t_hash_table_entry));
// 	if (table->entries == NULL)
// 		return (free(table), NULL);
// 	return (table);
// }

// void	hash_destroy(t_hash_table *table)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < table->capacity)
// 	{
// 		free((void *)table->entries[i].key);
// 		i++;
// 	}
// 	free(table->entries);
// 	free(table);
// }

// // Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// // https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
// static uint64_t	hash_key(const char *key)
// {
// 	uint64_t	hash;
// 	const char	*p;

// 	hash = FNV_OFFSET;
// 	p = key;
// 	while (*p)
// 	{
// 		hash ^= (uint64_t)(unsigned char)(*p);
// 		hash *= FNV_PRIME;
// 		p++;
// 	}
// 	return (hash);
// }

// void	*hash_get(t_hash_table *table, const char *key)
// {
// 	uint64_t	hash;
// 	size_t		index;

// 	hash = hash_key(key);
// 	index = (size_t)(hash & (uint64_t)(table->capacity - 1));
// 	while (table->entries[index].key != NULL)
// 	{
// 		if (strcmp(key, table->entries[index].key) == 0)
// 			return (table->entries[index].value);
// 		index++;
// 		if (index >= table->capacity)
// 			index = 0;
// 	}
// 	return (NULL);
// }

// static const char	*hash_set_entry(t_hash_table_entry *entries,
// 									size_t capacity,
// 									const char *key,
// 									void *value,
// 									size_t *plength)
// {
// 	uint64_t	hash;
// 	size_t		index;

// 	hash = hash_key(key);
// 	index = (size_t)(hash & (uint64_t)(capacity - 1));
// 	while (entries[index].key != NULL)
// 	{
// 		if (strcmp(key, entries[index].key) == 0)
// 		{
// 			entries[index].value = value;
// 			return (entries[index].key);
// 		}
// 		index++;
// 		if (index >= capacity)
// 			index = 0;
// 	}
// 	if (plength != NULL)
// 	{
// 		key = strdup(key);
// 		if (key == NULL)
// 		{
// 			return (NULL);
// 		}
// 		(*plength)++;
// 	}
// 	entries[index].key = (char *)key;
// 	entries[index].value = value;
// 	return (key);
// }

// // Expand hash table to twice its current size. Return true on success,
// // false if out of memory.
// static bool	hash_expand(t_hash_table *table)
// {
// 	size_t				new_capacity;
// 	t_hash_table		*new_entries;
// 	t_hash_table_entry	entry;
// 	size_t				i;

// 	i = 0;
// 	new_capacity = table->capacity * 2;
// 	if (new_capacity < table->capacity)
// 		return (false);
// 	new_entries = calloc(new_capacity, sizeof(t_hash_table_entry));
// 	if (new_entries == NULL)
// 		return (false);
// 	while (i < table->capacity)
// 	{
// 		entry = table->entries[i];
// 		if (entry.key != NULL)
// 			hash_set_entry(new_entries, new_capacity, entry.key, entry.value,
// 				NULL);
// 		i++;
// 	}
// 	free(table->entries);
// 	table->entries = new_entries;
// 	table->capacity = new_capacity;
// 	return (true);
// }

// const char	*hash_set(t_hash_table *table, const char *key, void *value)
// {
// 	if (value == NULL)
// 		return (NULL);
// 	if (table->length >= table->capacity / 2)
// 	{
// 		if (!hash_expand(table))
// 			return (NULL);
// 	}
// 	return (hash_set_entry(table->entries, table->capacity, key, value,
// 			&table->length));
// }

// size_t	hash_length(t_hash_table *table)
// {
// 	return (table->length);
// }

// hti	hash_iterator(ht *table)
// {
// 	hti	it;

// 	it._table = table;
// 	it._index = 0;
// 	return (it);
// }

// bool	hash_next(hti *it)
// {
// 	// Loop till we've hit end of entries array.
// 	ht *table = it->_table;
// 	while (it->_index < table->capacity)
// 	{
// 		size_t i = it->_index;
// 		it->_index++;
// 		if (table->entries[i].key != NULL)
// 		{
// 			// Found next non-empty item, update iterator key and value.
// 			hash_entry entry = table->entries[i];
// 			it->key = entry.key;
// 			it->value = entry.value;
// 			return (true);
// 		}
// 	}
// 	return (false);
// }