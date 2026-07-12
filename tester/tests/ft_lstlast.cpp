#include "test_helpers.hpp"

void	test_ft_lstlast(tester::Report &report)
{
	t_list	*list = test_helpers::build_list({1, 2, 3});
	t_list	*single = ft_lstnew(tester::new_int(42));

	tester::expect_eq(report, "ft_lstlast null", (t_list *)NULL,
		ft_lstlast(NULL));
	tester::expect_eq(report, "ft_lstlast single node", single,
		ft_lstlast(single));
	tester::expect_eq(report, "ft_lstlast", 3,
		*(int *)ft_lstlast(list)->content);
	tester::expect_eq(report, "ft_lstlast next null", (void *)NULL,
		ft_lstlast(list)->next);
	ft_lstdelone(single, tester::free_int);
	ft_lstclear(&list, tester::free_int);
}
