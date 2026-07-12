#include "test_helpers.hpp"

static void	check_normal(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd)
	{
		ft_putstr_fd((char *)"hello", fd);
	});
	tester::expect_eq(report, "ft_putstr_fd", std::string("hello"), captured);
}

static void	check_empty(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd)
	{
		ft_putstr_fd((char *)"", fd);
	});
	tester::expect_eq(report, "ft_putstr_fd empty", std::string(""), captured);
}

static void	check_whitespace(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd)
	{
		ft_putstr_fd((char *)" \t\n", fd);
	});
	tester::expect_eq(report, "ft_putstr_fd whitespace",
		std::string(" \t\n"), captured);
}

static void	check_null(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd) { ft_putstr_fd(NULL, fd); });
	tester::expect_eq(report, "ft_putstr_fd null", std::string(""), captured);
}

static void	check_digits(tester::Report &report)
{
	std::string	captured;

	captured = tester::capture_fd_text([](int fd)
	{
		ft_putstr_fd((char *)"42", fd);
	});
	tester::expect_eq(report, "ft_putstr_fd digits", std::string("42"),
		captured);
}

void	test_ft_putstr_fd(tester::Report &report)
{
	tester::run_isolated(report, "ft_putstr_fd normal", check_normal);
	tester::run_isolated(report, "ft_putstr_fd empty", check_empty);
	tester::run_isolated(report, "ft_putstr_fd whitespace", check_whitespace);
	tester::run_isolated(report, "ft_putstr_fd null", check_null);
	tester::run_isolated(report, "ft_putstr_fd digits", check_digits);
}
