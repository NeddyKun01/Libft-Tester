#include "test_helpers.hpp"

void	test_ft_strncmp(tester::Report &report)
{
	std::mt19937	&rng = tester::random_engine();

	tester::expect_eq(report, "ft_strncmp zero size", 0,
		ft_strncmp("abc", "xyz", 0));
	tester::expect_eq(report, "ft_strncmp equal", 0,
		ft_strncmp("libft", "libft", 5));
	tester::expect_eq(report, "ft_strncmp prefix",
		std::strncmp("abc", "abd", 2), ft_strncmp("abc", "abd", 2));
	tester::expect(report, "ft_strncmp negative",
		ft_strncmp("abc", "abd", 3) < 0);
	tester::expect(report, "ft_strncmp positive",
		ft_strncmp("abd", "abc", 3) > 0);
	tester::expect(report, "ft_strncmp shorter",
		ft_strncmp("abc", "abcd", 4) < 0);
	tester::expect_eq(report, "ft_strncmp unsigned",
		std::strncmp("\xff", "\x01", 1), ft_strncmp("\xff", "\x01", 1));
	for (int round = 0; round < 8; ++round)
	{
		std::uniform_int_distribution<int>	len_dist(0, 32);
		std::string							left = tester::random_printable(rng, 32);
		std::string							right = tester::random_printable(rng, 32);
		size_t								len = len_dist(rng);

		tester::expect(report, "ft_strncmp random sign",
			test_helpers::same_sign(std::strncmp(left.c_str(), right.c_str(),
					len), ft_strncmp(left.c_str(), right.c_str(), len)));
	}
}
