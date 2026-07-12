#include "test_helpers.hpp"

void	test_ft_strrchr(tester::Report &report)
{
	const char		*sample = "hello world";
	const char		*empty = "";
	const char		hidden[] = {'a', 'b', '\0', 'c', '\0'};
	std::mt19937	&rng = tester::random_engine();

	tester::expect_eq(report, "ft_strrchr found",
		std::strrchr(sample, 'o'), ft_strrchr(sample, 'o'));
	tester::expect_eq(report, "ft_strrchr first char",
		std::strrchr(sample, 'h'), ft_strrchr(sample, 'h'));
	tester::expect_eq(report, "ft_strrchr missing",
		std::strrchr(sample, 'z'), ft_strrchr(sample, 'z'));
	tester::expect_eq(report, "ft_strrchr nul",
		std::strrchr(sample, '\0'), ft_strrchr(sample, '\0'));
	tester::expect_eq(report, "ft_strrchr empty nul",
		std::strrchr(empty, '\0'), ft_strrchr(empty, '\0'));
	tester::expect_eq(report, "ft_strrchr wrapped char",
		std::strrchr(sample, 'h' + 256), ft_strrchr(sample, 'h' + 256));
	tester::expect_eq(report, "ft_strrchr stops at nul",
		(char *)NULL, ft_strrchr(hidden, 'c'));
	for (int round = 0; round < 8; ++round)
	{
		std::uniform_int_distribution<int>	char_dist(32, 126);
		std::string							random = tester::random_printable(rng, 32);
		int									needle = char_dist(rng);

		tester::expect_eq(report, "ft_strrchr random",
			std::strrchr(random.c_str(), needle),
			ft_strrchr(random.c_str(), needle));
	}
}
