
#include "libft.h"

char	*ft_strdupn(const char *s1, size_t n)
{
	char		*dest;
	size_t		src_l;

	src_l = ft_strlen(s1);
	if (n > src_l)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * (n + 1));
	if (!dest)
		return (NULL);
	dest = ft_strcpy(dest, s1);
	return (dest);
}

