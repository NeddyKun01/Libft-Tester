#include "test_helpers.hpp"

void	test_ft_memset(tester::Report &report)
{
	unsigned char	buffer[16];
	unsigned char	ref[16];
	std::mt19937	&rng = tester::random_engine();

	std::memset(ref, 0xAB, sizeof(ref));
	tester::expect_eq(report, "ft_memset return", (void *)buffer,
		ft_memset(buffer, 0xAB, sizeof(buffer)));
	tester::expect_mem(report, "ft_memset fills bytes", ref, buffer,
		sizeof(buffer));
	tester::expect_eq(report, "ft_memset zero size", (void *)buffer,
		ft_memset(buffer, 7, 0));
	tester::expect_eq(report, "ft_memset zero size content",
		static_cast<unsigned char>(0xAB), buffer[0]);
	ft_memset(buffer, 300, 3);
	std::memset(ref, 44, sizeof(ref));
	tester::expect_mem(report, "ft_memset unsigned char cast", ref, buffer, 3);
	for (int round = 0; round < 8; ++round)
	{
		std::uniform_int_distribution<int>	size_dist(1, 64);
		std::uniform_int_distribution<int>	byte_dist(0, 255);
		size_t								size = size_dist(rng);
		size_t								len;
		int									value;
		std::vector<unsigned char>			dst(size + 8, 0xAA);
		std::vector<unsigned char>			expected(size + 8, 0xAA);

		len = std::uniform_int_distribution<int>(0, size)(rng);
		value = byte_dist(rng);
		tester::expect_eq(report, "ft_memset random return", (void *)dst.data(),
			ft_memset(dst.data(), value, len));
		std::memset(expected.data(), value, len);
		tester::expect_mem(report, "ft_memset random bytes", expected.data(),
			dst.data(), dst.size());
	}
}
