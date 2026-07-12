#include "test_helpers.hpp"

void	test_ft_strmapi(tester::Report &report)
{
	char	*mapped;

	mapped = ft_strmapi("abcd", test_helpers::shift_by_index);
	tester::expect_malloc(report, "ft_strmapi", mapped, true);
	if (mapped)
		tester::expect_eq(report, "ft_strmapi text", std::string("aceg"),
			std::string(mapped));
	std::free(mapped);
	mapped = ft_strmapi("", test_helpers::shift_by_index);
	tester::expect_malloc(report, "ft_strmapi empty", mapped, true);
	if (mapped)
		tester::expect_eq(report, "ft_strmapi empty text", std::string(""),
			std::string(mapped));
	std::free(mapped);
	malloc_fail_on(1);
	mapped = ft_strmapi("abcd", test_helpers::shift_by_index);
	malloc_fail_off();
	tester::expect_malloc(report, "ft_strmapi malloc failure", mapped, false);
	std::free(mapped);
}
