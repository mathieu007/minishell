
#include "libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	if (!dest || !src)
		return (NULL);
	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

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

