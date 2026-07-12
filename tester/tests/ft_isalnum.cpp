#include "test_helpers.hpp"

void	test_ft_isalnum(tester::Report &report)
{
	for (int c = -128; c <= 255; ++c)
		tester::expect_eq(report, "ft_isalnum",
			test_helpers::is_alnum_ref(c), ft_isalnum(c));
}
