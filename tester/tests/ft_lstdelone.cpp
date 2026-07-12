#include "test_helpers.hpp"

void	test_ft_lstdelone(tester::Report &report)
{
	t_list	*node = ft_lstnew(tester::new_int(42));

	test_helpers::delete_count() = 0;
	ft_lstdelone(node, test_helpers::count_delete);
	tester::expect_eq(report, "ft_lstdelone calls del once", 1,
		test_helpers::delete_count());
	ft_lstdelone(NULL, tester::free_int);
	tester::expect(report, "ft_lstdelone null node guard", true);
}
