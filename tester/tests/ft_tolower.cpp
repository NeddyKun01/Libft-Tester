#include "test_helpers.hpp"

void	test_ft_tolower(tester::Report &report)
{
	for (int c = -128; c <= 255; ++c)
		tester::expect_eq(report, "ft_tolower",
			test_helpers::tolower_ref(c), ft_tolower(c));
}
