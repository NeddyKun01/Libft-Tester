#include "test_helpers.hpp"

void	test_ft_strlen(tester::Report &report)
{
	const char	hidden[] = {'a', 'b', '\0', 'c', '\0'};
	std::mt19937	&rng = tester::random_engine();

	tester::expect_eq(report, "ft_strlen empty", static_cast<size_t>(0),
		ft_strlen(""));
	tester::expect_eq(report, "ft_strlen normal", std::strlen("hello world"),
		ft_strlen("hello world"));
	tester::expect_eq(report, "ft_strlen whitespace", std::strlen(" \t\n"),
		ft_strlen(" \t\n"));
	tester::expect_eq(report, "ft_strlen punctuation",
		std::strlen("42 Lisboa!"), ft_strlen("42 Lisboa!"));
	tester::expect_eq(report, "ft_strlen stops at nul", static_cast<size_t>(2),
		ft_strlen(hidden));
	for (int round = 0; round < 8; ++round)
	{
		std::string	sample = tester::random_printable(rng, 32);

		tester::expect_eq(report, "ft_strlen random", sample.size(),
			ft_strlen(sample.c_str()));
	}
}
