#include "test_helpers.hpp"

void	test_ft_substr(tester::Report &report)
{
	char	*result;

	result = ft_substr("abcdef", 1, 3);
	tester::expect_malloc(report, "ft_substr basic", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr basic text", std::string("bcd"),
			std::string(result));
	std::free(result);
	result = ft_substr("abcdef", 2, 99);
	tester::expect_malloc(report, "ft_substr long len", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr long len text",
			std::string("cdef"), std::string(result));
	std::free(result);
	result = ft_substr("abcdef", 99, 3);
	tester::expect_malloc(report, "ft_substr out of range", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr out of range text",
			std::string(""), std::string(result));
	std::free(result);
	result = ft_substr("abcdef", 0, 0);
	tester::expect_malloc(report, "ft_substr zero len", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr zero len text", std::string(""),
			std::string(result));
	std::free(result);
	result = ft_substr("", 0, 10);
	tester::expect_malloc(report, "ft_substr empty source", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr empty source text",
			std::string(""), std::string(result));
	std::free(result);
	malloc_fail_on(1);
	result = ft_substr("abcdef", 1, 3);
	malloc_fail_off();
	tester::expect_malloc(report, "ft_substr malloc failure", result, false);
	std::free(result);
}
