#include "test_helpers.hpp"

void	test_ft_isalpha(tester::Report &report);
void	test_ft_isdigit(tester::Report &report);
void	test_ft_isalnum(tester::Report &report);
void	test_ft_isascii(tester::Report &report);
void	test_ft_isprint(tester::Report &report);
void	test_ft_toupper(tester::Report &report);
void	test_ft_tolower(tester::Report &report);
void	test_ft_memset(tester::Report &report);
void	test_ft_bzero(tester::Report &report);
void	test_ft_memcpy(tester::Report &report);
void	test_ft_memmove(tester::Report &report);
void	test_ft_memchr(tester::Report &report);
void	test_ft_memcmp(tester::Report &report);
void	test_ft_calloc(tester::Report &report);
void	test_ft_atoi(tester::Report &report);
void	test_ft_strlen(tester::Report &report);
void	test_ft_strchr(tester::Report &report);
void	test_ft_strrchr(tester::Report &report);
void	test_ft_strncmp(tester::Report &report);
void	test_ft_strnstr(tester::Report &report);
void	test_ft_strlcpy(tester::Report &report);
void	test_ft_strlcat(tester::Report &report);
void	test_ft_strdup(tester::Report &report);
void	test_ft_substr(tester::Report &report);
void	test_ft_strjoin(tester::Report &report);
void	test_ft_strtrim(tester::Report &report);
void	test_ft_split(tester::Report &report);
void	test_ft_itoa(tester::Report &report);
void	test_ft_strmapi(tester::Report &report);
void	test_ft_striteri(tester::Report &report);
void	test_ft_putchar_fd(tester::Report &report);
void	test_ft_putstr_fd(tester::Report &report);
void	test_ft_putendl_fd(tester::Report &report);
void	test_ft_putnbr_fd(tester::Report &report);
void	test_ft_lstnew(tester::Report &report);
void	test_ft_lstadd_front(tester::Report &report);
void	test_ft_lstsize(tester::Report &report);
void	test_ft_lstlast(tester::Report &report);
void	test_ft_lstadd_back(tester::Report &report);
void	test_ft_lstdelone(tester::Report &report);
void	test_ft_lstclear(tester::Report &report);
void	test_ft_lstiter(tester::Report &report);
void	test_ft_lstmap(tester::Report &report);

void	test_ctype(tester::Report &report)
{
	test_ft_isalpha(report);
	test_ft_isdigit(report);
	test_ft_isalnum(report);
	test_ft_isascii(report);
	test_ft_isprint(report);
	test_ft_toupper(report);
	test_ft_tolower(report);
}

void	test_memory(tester::Report &report)
{
	test_ft_memset(report);
	test_ft_bzero(report);
	test_ft_memcpy(report);
	test_ft_memmove(report);
	test_ft_memchr(report);
	test_ft_memcmp(report);
	test_ft_calloc(report);
}

void	test_atoi(tester::Report &report)
{
	test_ft_atoi(report);
}

void	test_strings(tester::Report &report)
{
	test_ft_strlen(report);
	test_ft_strchr(report);
	test_ft_strrchr(report);
	test_ft_strncmp(report);
	test_ft_strnstr(report);
	test_ft_strlcpy(report);
	test_ft_strlcat(report);
	test_ft_strdup(report);
}

void	test_string_utils(tester::Report &report)
{
	test_ft_substr(report);
	test_ft_strjoin(report);
	test_ft_strtrim(report);
	test_ft_split(report);
	test_ft_itoa(report);
	test_ft_strmapi(report);
	test_ft_striteri(report);
}

void	test_output(tester::Report &report)
{
	test_ft_putchar_fd(report);
	test_ft_putstr_fd(report);
	test_ft_putendl_fd(report);
	test_ft_putnbr_fd(report);
}

void	test_lists(tester::Report &report)
{
	test_ft_lstnew(report);
	test_ft_lstadd_front(report);
	test_ft_lstsize(report);
	test_ft_lstlast(report);
	test_ft_lstadd_back(report);
	test_ft_lstdelone(report);
	test_ft_lstclear(report);
	test_ft_lstiter(report);
	test_ft_lstmap(report);
}
