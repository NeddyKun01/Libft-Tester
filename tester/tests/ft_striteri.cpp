#include "test_helpers.hpp"

static void	check_normal(tester::Report &report)
{
	char	buffer[] = "libft";

	ft_striteri(buffer, test_helpers::make_upper);
	tester::expect_eq(report, "ft_striteri", std::string("LIBFT"),
		std::string(buffer));
}

static void	check_empty(tester::Report &report)
{
	char	buffer[] = "";

	ft_striteri(buffer, test_helpers::make_upper);
	tester::expect_eq(report, "ft_striteri empty", std::string(""),
		std::string(buffer));
}

static void	check_index(tester::Report &report)
{
	char	indexed[] = "abcde";

	ft_striteri(indexed, test_helpers::write_index_digit);
	tester::expect_eq(report, "ft_striteri index", std::string("01234"),
		std::string(indexed));
}

static void	check_null_function(tester::Report &report)
{
	char	unchanged[] = "safe";

	ft_striteri(unchanged, NULL);
	tester::expect_eq(report, "ft_striteri null function",
		std::string("safe"), std::string(unchanged));
}

static void	check_null_string(tester::Report &report)
{
	ft_striteri(NULL, test_helpers::make_upper);
	tester::expect(report, "ft_striteri null string", true);
}

void	test_ft_striteri(tester::Report &report)
{
	tester::run_isolated(report, "ft_striteri normal", check_normal);
	tester::run_isolated(report, "ft_striteri empty", check_empty);
	tester::run_isolated(report, "ft_striteri index", check_index);
	tester::run_isolated(report, "ft_striteri null function",
		check_null_function);
	tester::run_isolated(report, "ft_striteri null string", check_null_string);
}
