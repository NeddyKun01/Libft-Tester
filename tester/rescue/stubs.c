/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stubs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: libft-tester <opensource@example.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:00:00 by libft-tester          #+#    #+#             */
/*   Updated: 2026/07/07 20:00:00 by libft-tester         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define WEAK __attribute__((weak))

WEAK int	ft_isalpha(int c) { return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')); }
WEAK int	ft_isdigit(int c) { return (c >= '0' && c <= '9'); }
WEAK int	ft_isalnum(int c) { return (ft_isalpha(c) || ft_isdigit(c)); }
WEAK int	ft_isascii(int c) { return (c >= 0 && c <= 127); }
WEAK int	ft_isprint(int c) { return (c >= 32 && c <= 126); }
WEAK int	ft_toupper(int c) { return ((c >= 'a' && c <= 'z') ? c - 32 : c); }
WEAK int	ft_tolower(int c) { return ((c >= 'A' && c <= 'Z') ? c + 32 : c); }

WEAK size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

WEAK void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*bytes;
	size_t			i;

	bytes = (unsigned char *)s;
	i = 0;
	while (bytes && i < n)
		bytes[i++] = (unsigned char)c;
	return (s);
}

WEAK void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

WEAK void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	i = 0;
	while (d && s && i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

WEAK void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	if (!d || !s || d == s)
		return (dst);
	if (d < s)
		return (ft_memcpy(dst, src, len));
	while (len > 0)
	{
		len--;
		d[len] = s[len];
	}
	return (dst);
}

WEAK void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*bytes;
	size_t				i;

	bytes = (const unsigned char *)s;
	i = 0;
	while (bytes && i < n)
	{
		if (bytes[i] == (unsigned char)c)
			return ((void *)(bytes + i));
		i++;
	}
	return (NULL);
}

WEAK int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*a;
	const unsigned char	*b;
	size_t				i;

	a = (const unsigned char *)s1;
	b = (const unsigned char *)s2;
	i = 0;
	while (a && b && i < n)
	{
		if (a[i] != b[i])
			return (a[i] - b[i]);
		i++;
	}
	return (0);
}

WEAK char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s && s[i])
	{
		if ((unsigned char)s[i] == (unsigned char)c)
			return ((char *)(s + i));
		i++;
	}
	if (s && (unsigned char)c == '\0')
		return ((char *)(s + i));
	return (NULL);
}

WEAK char	*ft_strrchr(const char *s, int c)
{
	char	*last;

	last = NULL;
	while (s && *s)
	{
		if ((unsigned char)*s == (unsigned char)c)
			last = (char *)s;
		s++;
	}
	if (s && (unsigned char)c == '\0')
		return ((char *)s);
	return (last);
}

WEAK int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1 && s2 && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i] || !s1[i] || !s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

WEAK size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(src);
	i = 0;
	if (dstsize > 0)
	{
		while (src && src[i] && i + 1 < dstsize)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (len);
}

WEAK size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	i;

	dst_len = 0;
	while (dst && dst_len < dstsize && dst[dst_len])
		dst_len++;
	if (dst_len == dstsize)
		return (dstsize + ft_strlen(src));
	i = 0;
	while (src && src[i] && dst_len + i + 1 < dstsize)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (dst && dst_len + i < dstsize)
		dst[dst_len + i] = '\0';
	return (dst_len + ft_strlen(src));
}

WEAK char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!little || !*little)
		return ((char *)big);
	i = 0;
	while (big && big[i] && i < len)
	{
		j = 0;
		while (big[i + j] && little[j] && i + j < len
			&& big[i + j] == little[j])
			j++;
		if (!little[j])
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}

WEAK int	ft_atoi(const char *nptr)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (nptr && (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13)))
		nptr++;
	if (nptr && (*nptr == '-' || *nptr == '+'))
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (nptr && *nptr >= '0' && *nptr <= '9')
		result = result * 10 + (*nptr++ - '0');
	return ((int)(result * sign));
}

WEAK void	*ft_calloc(size_t count, size_t size) { (void)count; (void)size; return (NULL); }
WEAK char	*ft_strdup(const char *s) { (void)s; return (NULL); }
WEAK char	*ft_substr(char const *s, unsigned int start, size_t len) { (void)s; (void)start; (void)len; return (NULL); }
WEAK char	*ft_strjoin(char const *s1, char const *s2) { (void)s1; (void)s2; return (NULL); }
WEAK char	*ft_strtrim(char const *s1, char const *set) { (void)s1; (void)set; return (NULL); }
WEAK char	**ft_split(char const *s, char c) { (void)s; (void)c; return (NULL); }
WEAK char	*ft_itoa(int n) { (void)n; return (NULL); }
WEAK char	*ft_strmapi(char const *s, char (*f)(unsigned int, char)) { (void)s; (void)f; return (NULL); }
WEAK void	ft_striteri(char *s, void (*f)(unsigned int, char *)) { (void)s; (void)f; }
WEAK void	ft_putchar_fd(char c, int fd) { (void)c; (void)fd; }
WEAK void	ft_putstr_fd(char *s, int fd) { (void)s; (void)fd; }
WEAK void	ft_putendl_fd(char *s, int fd) { (void)s; (void)fd; }
WEAK void	ft_putnbr_fd(int n, int fd) { (void)n; (void)fd; }
WEAK t_list	*ft_lstnew(void *content) { (void)content; return (NULL); }
WEAK void	ft_lstadd_front(t_list **lst, t_list *new) { (void)lst; (void)new; }
WEAK int	ft_lstsize(t_list *lst) { (void)lst; return (0); }
WEAK t_list	*ft_lstlast(t_list *lst) { return (lst); }
WEAK void	ft_lstadd_back(t_list **lst, t_list *new) { (void)lst; (void)new; }
WEAK void	ft_lstdelone(t_list *lst, void (*del)(void *)) { (void)lst; (void)del; }
WEAK void	ft_lstclear(t_list **lst, void (*del)(void *)) { (void)lst; (void)del; }
WEAK void	ft_lstiter(t_list *lst, void (*f)(void *)) { (void)lst; (void)f; }
WEAK t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *)) { (void)lst; (void)f; (void)del; return (NULL); }
