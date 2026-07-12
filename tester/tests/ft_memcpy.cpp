#include "test_helpers.hpp"

void	test_ft_memcpy(tester::Report &report)
{
	unsigned char	source[] = {0, 1, 2, 3, 255, 4};
	unsigned char	destination[] = {9, 9, 9, 9, 9, 9};
	unsigned char	reference[] = {0, 1, 2, 3, 255, 4};
	std::mt19937	&rng = tester::random_engine();

	tester::expect_eq(report, "ft_memcpy return", (void *)destination,
		ft_memcpy(destination, source, sizeof(source)));
	tester::expect_mem(report, "ft_memcpy fixed", reference, destination,
		sizeof(reference));
	tester::expect_eq(report, "ft_memcpy zero size", (void *)destination,
		ft_memcpy(destination, source, 0));
	tester::expect_mem(report, "ft_memcpy zero size unchanged", reference,
		destination, sizeof(reference));
	std::memset(destination, 9, sizeof(destination));
	std::memset(reference, 9, sizeof(reference));
	std::memcpy(reference + 2, source + 1, 3);
	tester::expect_eq(report, "ft_memcpy offset return",
		(void *)(destination + 2), ft_memcpy(destination + 2, source + 1, 3));
	tester::expect_mem(report, "ft_memcpy offset copy", reference, destination,
		sizeof(reference));
	for (int round = 0; round < 8; ++round)
	{
		std::uniform_int_distribution<int>	size_dist(1, 64);
		size_t								size = size_dist(rng);
		size_t								len;
		std::vector<unsigned char>			src(size);
		std::vector<unsigned char>			dst(size + 8, 0xAA);
		std::vector<unsigned char>			expected(size + 8, 0xAA);

		test_helpers::fill_random_bytes(src, rng);
		len = std::uniform_int_distribution<int>(0, size)(rng);
		tester::expect_eq(report, "ft_memcpy random return", (void *)dst.data(),
			ft_memcpy(dst.data(), src.data(), len));
		std::memcpy(expected.data(), src.data(), len);
		tester::expect_mem(report, "ft_memcpy random bytes", expected.data(),
			dst.data(), dst.size());
	}
}
