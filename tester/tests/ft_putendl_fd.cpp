#include "test_helpers.hpp"

static void	check_normal(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd)
	{
		ft_putendl_fd((char *)"hello", fd);
	});
	tester::expect_eq(report, "ft_putendl_fd", std::string("hello\n"),
		captured);
}

static void	check_empty(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd)
	{
		ft_putendl_fd((char *)"", fd);
	});
	tester::expect_eq(report, "ft_putendl_fd empty", std::string("\n"),
		captured);
}

static void	check_inner_newline(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd)
	{
		ft_putendl_fd((char *)"line\ninside", fd);
	});
	tester::expect_eq(report, "ft_putendl_fd keeps inner newline",
		std::string("line\ninside\n"), captured);
}

static void	check_null(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd) { ft_putendl_fd(NULL, fd); });
	tester::expect_eq(report, "ft_putendl_fd null", std::string(""), captured);
}

static void	check_digits(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd)
	{
		ft_putendl_fd((char *)"42", fd);
	});
	tester::expect_eq(report, "ft_putendl_fd digits", std::string("42\n"),
		captured);
}

void	test_ft_putendl_fd(tester::Report &report)
{
	tester::run_isolated(report, "ft_putendl_fd normal", check_normal);
	tester::run_isolated(report, "ft_putendl_fd empty", check_empty);
	tester::run_isolated(report, "ft_putendl_fd inner newline",
		check_inner_newline);
	tester::run_isolated(report, "ft_putendl_fd null", check_null);
	tester::run_isolated(report, "ft_putendl_fd digits", check_digits);
}
