#include "test_helpers.hpp"

static void	check_strlcpy(tester::Report &report, const char *label,
	const char *src, size_t size)
{
	char	buffer[16];
	char	reference[16];

	std::memset(buffer, 0xCC, sizeof(buffer));
	std::memset(reference, 0xCC, sizeof(reference));
	tester::expect_eq(report, std::string("ft_strlcpy ") + label + " length",
		tester::strlcpy_reference(reference, src, size),
		ft_strlcpy(buffer, src, size));
	tester::expect_mem(report, std::string("ft_strlcpy ") + label + " buffer",
		reference, buffer, sizeof(buffer));
}

void	test_ft_strlcpy(tester::Report &report)
{
	check_strlcpy(report, "size zero", "hello", 0);
	check_strlcpy(report, "size one", "hello", 1);
	check_strlcpy(report, "full", "hello", 16);
	check_strlcpy(report, "trunc", "hello", 4);
	check_strlcpy(report, "empty source", "", 16);
}
