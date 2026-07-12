#include "test_helpers.hpp"

static void	check_putnbr(tester::Report &report, const char *label, int value)
{
	std::string	captured;

	captured = tester::capture_fd_text([value](int fd)
	{
		ft_putnbr_fd(value, fd);
	});
	tester::expect_eq(report, std::string("ft_putnbr_fd ") + label,
		std::to_string(value), captured);
}

void	test_ft_putnbr_fd(tester::Report &report)
{
	check_putnbr(report, "INT_MIN", INT_MIN);
	check_putnbr(report, "zero", 0);
	check_putnbr(report, "negative", -42);
	check_putnbr(report, "INT_MAX", INT_MAX);
	check_putnbr(report, "positive", 42);
}
