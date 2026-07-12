#include "test_helpers.hpp"

void	test_ft_strdup(tester::Report &report)
{
	char	text[] = "libft";
	char	empty[] = "";
	char	malloc_text[] = "malloc";
	char	*copy;

	copy = ft_strdup(text);
	tester::expect_malloc(report, "ft_strdup", copy, true);
	if (copy)
	{
		tester::expect_eq(report, "ft_strdup text", std::string("libft"),
			std::string(copy));
		std::free(copy);
	}
	copy = ft_strdup(empty);
	tester::expect_malloc(report, "ft_strdup empty", copy, true);
	if (copy)
	{
		tester::expect_eq(report, "ft_strdup empty text", std::string(""),
			std::string(copy));
		std::free(copy);
	}
	malloc_fail_on(1);
	copy = ft_strdup(malloc_text);
	malloc_fail_off();
	tester::expect_malloc(report, "ft_strdup malloc failure", copy, false);
	std::free(copy);
}
