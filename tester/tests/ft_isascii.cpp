#include "test_helpers.hpp"

void	test_ft_isascii(tester::Report &report)
{
	for (int c = -128; c <= 255; ++c)
		tester::expect_eq(report, "ft_isascii",
			test_helpers::is_ascii_ref(c), ft_isascii(c));
}
