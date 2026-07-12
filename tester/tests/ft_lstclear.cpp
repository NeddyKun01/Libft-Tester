#include "test_helpers.hpp"

void	test_ft_lstclear(tester::Report &report)
{
	t_list	*list = test_helpers::build_list({4, 5});

	test_helpers::delete_count() = 0;
	ft_lstclear(&list, test_helpers::count_delete);
	tester::expect_eq(report, "ft_lstclear calls del twice", 2,
		test_helpers::delete_count());
	tester::expect_eq(report, "ft_lstclear two nodes resets head",
		(void *)NULL, list);
	ft_lstclear(&list, test_helpers::count_delete);
	tester::expect_eq(report, "ft_lstclear empty list stays null",
		(void *)NULL, list);
	list = test_helpers::build_list({8});
	ft_lstclear(&list, NULL);
	tester::expect_eq(report, "ft_lstclear null del keeps list", 8,
		*(int *)list->content);
	ft_lstclear(&list, tester::free_int);
}
