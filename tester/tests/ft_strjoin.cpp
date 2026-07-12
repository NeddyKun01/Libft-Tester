#include "test_helpers.hpp"

void	test_ft_strjoin(tester::Report &report)
{
	char	*join;

	join = ft_strjoin("hello", " world");
	tester::expect_malloc(report, "ft_strjoin", join, true);
	if (join)
		tester::expect_eq(report, "ft_strjoin text",
			std::string("hello world"), std::string(join));
	std::free(join);
	join = ft_strjoin("", "tail");
	tester::expect_malloc(report, "ft_strjoin empty left", join, true);
	if (join)
		tester::expect_eq(report, "ft_strjoin empty left text",
			std::string("tail"), std::string(join));
	std::free(join);
	join = ft_strjoin("head", "");
	tester::expect_malloc(report, "ft_strjoin empty right", join, true);
	if (join)
		tester::expect_eq(report, "ft_strjoin empty right text",
			std::string("head"), std::string(join));
	std::free(join);
	join = ft_strjoin("", "");
	tester::expect_malloc(report, "ft_strjoin both empty", join, true);
	if (join)
		tester::expect_eq(report, "ft_strjoin both empty text",
			std::string(""), std::string(join));
	std::free(join);
	malloc_fail_on(1);
	join = ft_strjoin("left", "right");
	malloc_fail_off();
	tester::expect_malloc(report, "ft_strjoin malloc failure", join, false);
	std::free(join);
}
