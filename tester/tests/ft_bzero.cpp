#include "test_helpers.hpp"

void	test_ft_bzero(tester::Report &report)
{
	unsigned char	buffer[16];
	unsigned char	ref[16];

	std::memset(buffer, 0xAB, sizeof(buffer));
	ft_bzero(buffer, sizeof(buffer));
	std::memset(ref, 0, sizeof(ref));
	tester::expect_mem(report, "ft_bzero clears bytes", ref, buffer,
		sizeof(buffer));
	std::memset(buffer, 0xAA, sizeof(buffer));
	std::memset(ref, 0xAA, sizeof(ref));
	ft_bzero(buffer + 4, 5);
	std::memset(ref + 4, 0, 5);
	tester::expect_mem(report, "ft_bzero partial clear", ref, buffer,
		sizeof(buffer));
	tester::expect_eq(report, "ft_bzero keeps byte before",
		static_cast<unsigned char>(0xAA), buffer[3]);
	tester::expect_eq(report, "ft_bzero keeps byte after",
		static_cast<unsigned char>(0xAA), buffer[9]);
	buffer[0] = 42;
	ft_bzero(buffer, 0);
	tester::expect_eq(report, "ft_bzero zero size",
		static_cast<unsigned char>(42), buffer[0]);
	buffer[1] = 99;
	ft_bzero(buffer + 1, 1);
	tester::expect_eq(report, "ft_bzero one byte",
		static_cast<unsigned char>(0), buffer[1]);
}
