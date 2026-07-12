#include "test_helpers.hpp"

void	test_ft_lstsize(tester::Report &report)
{
	t_list	*list = test_helpers::build_list({1, 2, 3});

	tester::expect_eq(report, "ft_lstsize null", 0, ft_lstsize(NULL));
	tester::expect_eq(report, "ft_lstsize one", 1,
		ft_lstsize(ft_lstlast(list)));
	tester::expect_eq(report, "ft_lstsize", 3, ft_lstsize(list));
	ft_lstclear(&list, tester::free_int);
	tester::expect_eq(report, "ft_lstsize after clear", 0, ft_lstsize(list));
}
