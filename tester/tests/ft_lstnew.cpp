#include "test_helpers.hpp"

void	test_ft_lstnew(tester::Report &report)
{
	t_list	*node;

	node = ft_lstnew(tester::new_int(42));
	tester::expect_malloc(report, "ft_lstnew", node, true);
	if (node)
	{
		tester::expect_eq(report, "ft_lstnew content", 42,
			*(int *)node->content);
		tester::expect_eq(report, "ft_lstnew next null", (void *)NULL,
			node->next);
		ft_lstdelone(node, tester::free_int);
	}
	node = ft_lstnew(NULL);
	tester::expect_malloc(report, "ft_lstnew null content", node, true);
	if (node)
	{
		tester::expect_eq(report, "ft_lstnew null content value", (void *)NULL,
			node->content);
		tester::expect_eq(report, "ft_lstnew null content next", (void *)NULL,
			node->next);
		ft_lstdelone(node, tester::free_int);
	}
	{
		int	*content = tester::new_int(99);

		malloc_fail_on(1);
		node = ft_lstnew(content);
		malloc_fail_off();
		tester::expect_malloc(report, "ft_lstnew malloc failure", node, false);
		if (node)
			ft_lstdelone(node, tester::free_int);
		else
			tester::free_int(content);
	}
}
