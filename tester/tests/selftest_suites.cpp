#include "test_helpers.hpp"

void	test_ft_strlen(tester::Report &report);
void	test_ft_split(tester::Report &report);

void	test_ctype(tester::Report &report)
{
	(void)report;
}

void	test_memory(tester::Report &report)
{
	(void)report;
}

void	test_atoi(tester::Report &report)
{
	(void)report;
}

void	test_strings(tester::Report &report)
{
	test_ft_strlen(report);
	test_ft_split(report);
}

void	test_string_utils(tester::Report &report)
{
	test_ft_split(report);
}

void	test_output(tester::Report &report)
{
	(void)report;
}

void	test_lists(tester::Report &report)
{
	(void)report;
}
