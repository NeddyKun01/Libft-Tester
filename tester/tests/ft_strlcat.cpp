#include "test_helpers.hpp"

static void	check_strlcat(tester::Report &report, const char *label,
	const char *dst_start, const char *src, size_t size)
{
	char	buffer[16];
	char	reference[16];

	std::memset(buffer, 0xCC, sizeof(buffer));
	std::memset(reference, 0xCC, sizeof(reference));
	std::strcpy(buffer, dst_start);
	std::strcpy(reference, dst_start);
	tester::expect_eq(report, std::string("ft_strlcat ") + label + " length",
		tester::strlcat_reference(reference, src, size),
		ft_strlcat(buffer, src, size));
	tester::expect_mem(report, std::string("ft_strlcat ") + label + " buffer",
		reference, buffer, sizeof(buffer));
}

void	test_ft_strlcat(tester::Report &report)
{
	check_strlcat(report, "size smaller", "abc", "XYZ", 2);
	check_strlcat(report, "trunc", "abc", "XYZ", 5);
	check_strlcat(report, "full", "abc", "XYZ", 16);
	check_strlcat(report, "size zero", "abc", "XYZ", 0);
	check_strlcat(report, "empty dst", "", "XYZ", 16);
	check_strlcat(report, "empty src", "abc", "", 16);
}
