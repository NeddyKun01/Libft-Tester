#include "test_helpers.hpp"

static void	check_itoa(tester::Report &report, const char *label, int value)
{
	char	*result = ft_itoa(value);

	tester::expect_malloc(report, std::string("ft_itoa ") + label,
		result, true);
	if (result)
		tester::expect_eq(report, std::string("ft_itoa ") + label + " text",
			std::to_string(value), std::string(result));
	std::free(result);
}

void	test_ft_itoa(tester::Report &report)
{
	char	*result;

	check_itoa(report, "INT_MIN", INT_MIN);
	check_itoa(report, "zero", 0);
	check_itoa(report, "positive", 123456789);
	check_itoa(report, "INT_MAX", INT_MAX);
	check_itoa(report, "negative", -987654321);
	for (int value : {-1, 1, 9, 10, -10, 1000, -1000})
		check_itoa(report, "selected value", value);
	malloc_fail_on(1);
	result = ft_itoa(42);
	malloc_fail_off();
	tester::expect_malloc(report, "ft_itoa malloc failure", result, false);
	std::free(result);
}
