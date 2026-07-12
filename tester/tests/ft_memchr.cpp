#include "test_helpers.hpp"

void	test_ft_memchr(tester::Report &report)
{
	unsigned char	data[] = {0, 1, 2, 3, 2, 1, 0};
	std::mt19937	&rng = tester::random_engine();

	tester::expect_eq(report, "ft_memchr first match", (void *)(data + 2),
		ft_memchr(data, 2, sizeof(data)));
	tester::expect_eq(report, "ft_memchr nul byte", (void *)data,
		ft_memchr(data, 0, sizeof(data)));
	tester::expect_eq(report, "ft_memchr missing", (void *)NULL,
		ft_memchr(data, 9, sizeof(data)));
	tester::expect_eq(report, "ft_memchr zero size", (void *)NULL,
		ft_memchr(data, 0, 0));
	tester::expect_eq(report, "ft_memchr stops before later match",
		(void *)NULL, ft_memchr(data, 3, 3));
	for (int round = 0; round < 8; ++round)
	{
		std::uniform_int_distribution<int>	size_dist(1, 64);
		std::uniform_int_distribution<int>	byte_dist(0, 255);
		size_t								size = size_dist(rng);
		int									value = byte_dist(rng);
		std::vector<unsigned char>			src(size);

		test_helpers::fill_random_bytes(src, rng);
		tester::expect_eq(report, "ft_memchr random",
			std::memchr(src.data(), value, size),
			ft_memchr(src.data(), value, size));
	}
}
