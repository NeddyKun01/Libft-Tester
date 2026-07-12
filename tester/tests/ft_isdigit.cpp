#include "test_helpers.hpp"

void	test_ft_isdigit(tester::Report &report)
{
	for (int c = -128; c <= 255; ++c)
		tester::expect_eq(report, "ft_isdigit",
			test_helpers::is_digit_ref(c), ft_isdigit(c));
}
