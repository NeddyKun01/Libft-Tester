#include "test_helpers.hpp"

void	test_ft_memcmp(tester::Report &report)
{
	unsigned char	left[] = {0, 1, 2, 255};
	unsigned char	right[] = {0, 1, 2, 0};
	unsigned char	shorter[] = {0, 1, 3, 0};
	std::mt19937	&rng = tester::random_engine();

	tester::expect_eq(report, "ft_memcmp equal", 0,
		ft_memcmp(left, left, sizeof(left)));
	tester::expect_eq(report, "ft_memcmp zero size", 0,
		ft_memcmp(left, right, 0));
	tester::expect(report, "ft_memcmp unsigned diff",
		ft_memcmp(left, right, sizeof(left)) > 0);
	tester::expect(report, "ft_memcmp negative diff",
		ft_memcmp(left, shorter, 3) < 0);
	tester::expect_eq(report, "ft_memcmp equal prefix", 0,
		ft_memcmp(left, shorter, 2));
	for (int round = 0; round < 8; ++round)
	{
		std::uniform_int_distribution<int>	size_dist(1, 64);
		size_t								size = size_dist(rng);
		size_t								len;
		std::vector<unsigned char>			src(size);
		std::vector<unsigned char>			ref(size);

		test_helpers::fill_random_bytes(src, rng);
		ref = src;
		len = std::uniform_int_distribution<int>(0, size)(rng);
		if (len > 0)
			ref[len - 1] = static_cast<unsigned char>(ref[len - 1] + 1);
		tester::expect(report, "ft_memcmp random sign",
			test_helpers::same_sign(std::memcmp(src.data(), ref.data(), len),
				ft_memcmp(src.data(), ref.data(), len)));
	}
}
