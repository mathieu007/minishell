/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/02/28 07:52:12 by mroy              #+#    #+#             */
/*   Updated: 2023/02/28 07:52:12 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# define MAX_POOLS 60
# define SMALL_POOL_SIZE 8388480
# define MED_POOL_SIZE 33553920
# define LARGE_POOL_SIZE 134215680
# define TEMP_BUFFER 1000000
# define BUFF_SIZE 4096
# define INITIAL_HASH_CAPACITY 16
# define FNV_OFFSET 14695981039346656037UL
# define FNV_PRIME 1099511628211UL
# include "lst.h"
# include "ft_printf.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// typedef struct s_hash_table_entry
// {
// 	const char			*key;
// 	void				*value;
// }						t_hash_table_entry;

// typedef struct s_hash_iterator
// {
// 	const char			*key;
// 	void				*value;

// 	t_hash_table		*_table;
// 	size_t				_index;
// }						t_hash_iterator;

// typedef struct s_hash_table
// {
// 	hash_table_entry	*entries;
// 	size_t				capacity;
// 	size_t				length;
// }						t_hash_table;

typedef struct s_data_pool
{
	uint8_t			*data;
	int32_t			capacity;
	int32_t			byte_used;
}					t_data_pool;

typedef struct s_pool_info
{
	uint8_t			**slots;
	int32_t			count;
	int32_t			chunk_size;
	int32_t			capacity;
}					t_pool_info;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_file
{
	int				fd;
	char			buf[BUFF_SIZE + 1];
	int				i;
}					t_file;

// t_hash_table		*hash_new(void);
// void				hash_destroy(ht *table);
// void				*hash_get(ht *table, const char *key);
// const char			*hash_set(ht *table, const char *key, void *value);
// size_t				hash_length(ht *table);
// t_hash_iterator		*hash_iterator(ht *table);
// bool				hash_next(hti *it);

bool				ft_strstartwith(char *src, char *start_with);
bool				ft_strendwith(char *src, char *end_with);
size_t				ft_count2darray(const char **src);
bool				ft_strcontains(char *src, const char *to_search);
bool				ft_strisempty(char *str);
bool				ft_striswhitespace(char *str);
int32_t				ft_min(int32_t nbr, int32_t nbr2);
int32_t				ft_max(int32_t nbr, int32_t nbr2);
char				*ft_strcpy(char *dest, const char *src);
size_t				ft_strlen(const char *str);
size_t				ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
int					ft_isprint(int c);
int					ft_isascii(int c);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
bool				ft_isnum(char *str);
bool				ft_isdouble(char *str);
int					ft_isdigit(int c);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
long				ft_atoi_long(char *str);
int					ft_atoi(const char *str);
double				ft_atof(const char *str);
int					ft_tolower(int c);
int					ft_toupper(int c);
int					ft_lstsize(t_list *lst);
void				*ft_calloc(size_t count, size_t size);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memset(void *b, int c, size_t len);
void				*ft_memset(void *b, int c, size_t len);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putnbr_fd(int n, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putstr_fd(char *c, int fd);
void				ft_putchar_fd(char c, int fd);
void				ft_lstadd_front(t_list **lst, t_list *new_a);
void				ft_lstadd_back(t_list **lst, t_list *new_a);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
char				*ft_strnstr(const char *haystack, const char *needle,
						size_t len);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strdup(const char *s1);
char				**ft_split(char const *s, char c);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_itoa(int n);
char				*ft_itoa_temp(int n);
t_list				*ft_lstnew(void *content);
t_list				*ft_lstlast(t_list *lst);
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
void				ft_print_bits(size_t const size, void const *const ptr);
void				ft_putstr(char *c);
t_file				*ft_fopen(const char *path);
int					ft_fgetc(t_file *stream);
int					ft_fclose(t_file **stream);
uint8_t				*mem_pool(int32_t size_in_bytes);
uint8_t				*temp_pool(int32_t size_in_bytes);
char				**ft_split_many_temp(char *s, char *separators);
char				*ft_substr_temp(char const *s, unsigned int start,
						size_t len);
char				**ft_split_temp(char const *s, char c);
void				*malloc_1(void);
void				free_pool(void *data);
t_data_pool			*get_small_pool_data(void);
t_data_pool			*get_med_pool_data(void);
t_data_pool			*get_large_pool_data(void);
void				*alloc(size_t size_in_bytes);
void				init_pools(void);
int32_t				ft_log2_64(uint64_t value);
int64_t				ft_exponent(int32_t a, int32_t b);
void				*resize_pool(t_pool_info *pool);
char				*ft_strfill(char c, int32_t count);
char				*ft_strjoin_temp(char const *s1, char const *s2);
void				free_p_all(void);
char				**ft_split_many(char *s, char *separators);
int32_t				ft_count_char(char *str, char c);
char				*ft_strdup_temp(const char *s1);
char				*ft_replace_char_temp(char *str, char to_replace,
						char *replace_by);
bool				ft_strn_right_cmp(const char *s1, const char *s2, size_t n);
char				*ft_strncpy(char *src, size_t len);
char				*ft_strjoinfree(char *s1, char *s2);
char				*ft_strjoinfree2(char *s1, char *s2);
char				*ft_strchrlast(const char *s, char c);
char				*ft_strtrimfree(char *s1, char *set);
bool				ft_strequal(char *str, char *str2);
#endif
