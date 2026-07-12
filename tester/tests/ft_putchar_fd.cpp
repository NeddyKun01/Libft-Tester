#include "test_helpers.hpp"

void	test_ft_putchar_fd(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd) { ft_putchar_fd('A', fd); });
	tester::expect_eq(report, "ft_putchar_fd", std::string("A"), captured);
	captured = tester::capture_fd_text([](int fd) { ft_putchar_fd('\n', fd); });
	tester::expect_eq(report, "ft_putchar_fd newline", std::string("\n"),
		captured);
	captured = tester::capture_fd_text([](int fd) { ft_putchar_fd('0', fd); });
	tester::expect_eq(report, "ft_putchar_fd digit", std::string("0"),
		captured);
	{
		std::vector<unsigned char>	raw;

		raw = tester::capture_fd_raw([](int fd) { ft_putchar_fd('\0', fd); });
		tester::expect_eq(report, "ft_putchar_fd null size",
			static_cast<size_t>(1), raw.size());
		tester::expect(report, "ft_putchar_fd null byte",
			!raw.empty() && raw[0] == 0);
	}
}
