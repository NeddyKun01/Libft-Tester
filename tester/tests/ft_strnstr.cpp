#include "test_helpers.hpp"

static void	random_strnstr(tester::Report &report, const std::string &haystack,
	std::mt19937 &rng)
{
	std::uniform_int_distribution<int>	pos_dist;
	std::uniform_int_distribution<int>	len_dist;
	std::string							needle;
	const char							*reference;
	const char							*result;
	size_t								start;
	size_t								len;

	if (haystack.empty())
		return ;
	pos_dist = std::uniform_int_distribution<int>(
		0, static_cast<int>(haystack.size() - 1));
	start = static_cast<size_t>(pos_dist(rng));
	len_dist = std::uniform_int_distribution<int>(
		0, static_cast<int>(haystack.size() - start));
	len = static_cast<size_t>(len_dist(rng));
	needle = haystack.substr(start, len);
	len = static_cast<size_t>(std::uniform_int_distribution<int>(
		0, static_cast<int>(haystack.size() + 3))(rng));
	reference = tester::strnstr_reference(haystack.c_str(), needle.c_str(), len);
	result = ft_strnstr(haystack.c_str(), needle.c_str(), len);
	tester::expect_eq(report, "ft_strnstr random offset",
		test_helpers::pointer_offset(haystack.c_str(), reference),
		test_helpers::pointer_offset(haystack.c_str(), result));
}

void	test_ft_strnstr(tester::Report &report)
{
	const char		*big = "abcdef";
	const char		*small = "abc";
	std::mt19937	&rng = tester::random_engine();

	tester::expect_eq(report, "ft_strnstr empty needle",
		small, ft_strnstr(small, "", 3));
	tester::expect_eq(report, "ft_strnstr empty needle zero len",
		big, ft_strnstr(big, "", 0));
	tester::expect_eq(report, "ft_strnstr exact fit",
		big + 2, ft_strnstr(big, "cde", 5));
	tester::expect_eq(report, "ft_strnstr too short", (char *)NULL,
		ft_strnstr("abcdef", "cde", 4));
	tester::expect_eq(report, "ft_strnstr start",
		big, ft_strnstr(big, "abc", 3));
	tester::expect_eq(report, "ft_strnstr missing",
		(char *)NULL, ft_strnstr(big, "gh", 6));
	tester::expect_eq(report, "ft_strnstr partial needle at end",
		(char *)NULL, ft_strnstr(big, "efg", 6));
	for (int round = 0; round < 8; ++round)
		random_strnstr(report, tester::random_printable(rng, 32), rng);
}
