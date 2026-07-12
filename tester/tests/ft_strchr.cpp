#include "test_helpers.hpp"

void	test_ft_strchr(tester::Report &report)
{
	const char		*sample = "hello world";
	const char		*empty = "";
	const char		hidden[] = {'a', 'b', '\0', 'c', '\0'};
	std::mt19937	&rng = tester::random_engine();

	tester::expect_eq(report, "ft_strchr found",
		std::strchr(sample, 'o'), ft_strchr(sample, 'o'));
	tester::expect_eq(report, "ft_strchr first char",
		std::strchr(sample, 'h'), ft_strchr(sample, 'h'));
	tester::expect_eq(report, "ft_strchr missing",
		std::strchr(sample, 'z'), ft_strchr(sample, 'z'));
	tester::expect_eq(report, "ft_strchr nul",
		std::strchr(sample, '\0'), ft_strchr(sample, '\0'));
	tester::expect_eq(report, "ft_strchr empty nul",
		std::strchr(empty, '\0'), ft_strchr(empty, '\0'));
	tester::expect_eq(report, "ft_strchr wrapped char",
		std::strchr(sample, 'h' + 256), ft_strchr(sample, 'h' + 256));
	tester::expect_eq(report, "ft_strchr stops at nul",
		(char *)NULL, ft_strchr(hidden, 'c'));
	for (int round = 0; round < 8; ++round)
	{
		std::uniform_int_distribution<int>	char_dist(32, 126);
		std::string							random = tester::random_printable(rng, 32);
		int									needle = char_dist(rng);

		tester::expect_eq(report, "ft_strchr random",
			std::strchr(random.c_str(), needle),
			ft_strchr(random.c_str(), needle));
	}
}
