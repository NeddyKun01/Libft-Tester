#include "test_helpers.hpp"

void	test_ft_lstadd_front(tester::Report &report)
{
	t_list	*list = test_helpers::build_list({1, 2, 3});
	t_list	*orphan;

	ft_lstadd_front(&list, ft_lstnew(tester::new_int(0)));
	tester::expect_eq(report, "ft_lstadd_front head", 0,
		*(int *)list->content);
	tester::expect_eq(report, "ft_lstadd_front links old head", 1,
		*(int *)list->next->content);
	tester::expect_eq(report, "ft_lstsize after add_front", 4,
		ft_lstsize(list));
	ft_lstclear(&list, tester::free_int);
	ft_lstadd_front(&list, ft_lstnew(tester::new_int(7)));
	tester::expect_eq(report, "ft_lstadd_front empty list", 7,
		*(int *)list->content);
	orphan = ft_lstnew(tester::new_int(9));
	tester::expect_malloc(report, "ft_lstadd_front null list pointer setup",
		orphan, true);
	ft_lstadd_front(NULL, orphan);
	ft_lstdelone(orphan, tester::free_int);
	ft_lstadd_front(&list, NULL);
	tester::expect_eq(report, "ft_lstadd_front null node keeps head", 7,
		*(int *)list->content);
	ft_lstclear(&list, tester::free_int);
}
