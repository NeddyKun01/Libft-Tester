#include "test_helpers.hpp"

void	test_ft_lstadd_back(tester::Report &report)
{
	t_list	*empty = NULL;
	t_list	*list = NULL;
	t_list	*first = ft_lstnew(tester::new_int(1));
	t_list	*second = ft_lstnew(tester::new_int(2));
	t_list	*orphan = ft_lstnew(tester::new_int(3));

	ft_lstadd_back(&empty, NULL);
	tester::expect_eq(report, "ft_lstadd_back null node", (void *)NULL, empty);
	ft_lstadd_back(&list, first);
	tester::expect_eq(report, "ft_lstadd_back empty list", first, list);
	tester::expect_eq(report, "ft_lstadd_back empty next", (void *)NULL,
		first->next);
	ft_lstadd_back(&list, second);
	tester::expect_eq(report, "ft_lstadd_back preserves head", first, list);
	tester::expect_eq(report, "ft_lstadd_back links tail", second,
		first->next);
	tester::expect_eq(report, "ft_lstadd_back tail next null", (void *)NULL,
		second->next);
	ft_lstadd_back(NULL, orphan);
	ft_lstdelone(orphan, tester::free_int);
	ft_lstclear(&list, tester::free_int);
}
