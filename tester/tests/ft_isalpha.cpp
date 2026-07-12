#include "test_helpers.hpp"

void	test_ft_isalpha(tester::Report &report)
{
	for (int c = -128; c <= 255; ++c)
		tester::expect_eq(report, "ft_isalpha",
			test_helpers::is_alpha_ref(c), ft_isalpha(c));
}
