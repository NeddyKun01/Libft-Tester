#include "test_helpers.hpp"

void	test_ft_toupper(tester::Report &report)
{
	for (int c = -128; c <= 255; ++c)
		tester::expect_eq(report, "ft_toupper",
			test_helpers::toupper_ref(c), ft_toupper(c));
}
