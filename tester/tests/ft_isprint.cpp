#include "test_helpers.hpp"

void	test_ft_isprint(tester::Report &report)
{
	for (int c = -128; c <= 255; ++c)
		tester::expect_eq(report, "ft_isprint",
			test_helpers::is_print_ref(c), ft_isprint(c));
}
