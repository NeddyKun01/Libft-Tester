#include "test_helpers.hpp"

static void	check_split(tester::Report &report, const char *input,
	char delimiter, const std::vector<std::string> &expected)
{
	char	**split;
	size_t	i;

	split = ft_split(input, delimiter);
	tester::expect_malloc(report, "ft_split allocation", split, true);
	tester::expect_eq(report, "ft_split token count", expected.size(),
		tester::split_count(split));
	i = 0;
	while (i < expected.size() && split && split[i])
	{
		tester::expect_eq(report, "ft_split token", expected[i],
			std::string(split[i]));
		i++;
	}
	tester::expect(report, "ft_split terminator", split && split[i] == NULL);
	tester::free_split(split);
}

void	test_ft_split(tester::Report &report)
{
	check_split(report, "one  two   three", ' ', {"one", "two", "three"});
	check_split(report, ",,,", ',', {});
	check_split(report, "", ',', {});
	check_split(report, ",one,two,", ',', {"one", "two"});
	check_split(report, "abc", ',', {"abc"});
	check_split(report, "abc", '\0', {"abc"});
	check_split(report, "a\tb\t\tc", '\t', {"a", "b", "c"});
	for (size_t fail_at = 1; fail_at <= 4; ++fail_at)
	{
		char	**split;

		malloc_fail_on(fail_at);
		split = ft_split("one two three", ' ');
		malloc_fail_off();
		tester::expect_malloc(report, "ft_split malloc failure", split, false);
		tester::free_split(split);
	}
}
