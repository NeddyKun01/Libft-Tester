#include "test_helpers.hpp"

static void	check_trim(tester::Report &report, const char *label,
	const char *text, const char *set)
{
	std::string	trimmed = tester::trim_reference(text, set);
	char		*result = ft_strtrim(text, set);

	tester::expect_malloc(report, std::string("ft_strtrim ") + label,
		result, true);
	if (result)
		tester::expect_eq(report, std::string("ft_strtrim ") + label
			+ " text", trimmed, std::string(result));
	std::free(result);
}

void	test_ft_strtrim(tester::Report &report)
{
	char	*result;

	check_trim(report, "spaces", "   hello   ", " ");
	check_trim(report, "dashes", "--libft--", "-");
	check_trim(report, "empty set", "hello", "");
	check_trim(report, "full trim", "xxxx", "x");
	check_trim(report, "no trim", "hello", "xyz");
	check_trim(report, "multi set", "xyhelloyx", "xy");
	malloc_fail_on(1);
	result = ft_strtrim("  hello  ", " ");
	malloc_fail_off();
	tester::expect_malloc(report, "ft_strtrim malloc failure", result, false);
	std::free(result);
}
