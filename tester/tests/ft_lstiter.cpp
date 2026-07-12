#include "test_helpers.hpp"

void	test_ft_lstiter(tester::Report &report)
{
	t_list				*list = test_helpers::build_list({1, 2, 3});
	std::vector<int>	values;

	ft_lstiter(list, test_helpers::increment_int);
	values = test_helpers::list_values(list);
	tester::expect_eq(report, "ft_lstiter size", static_cast<size_t>(3),
		values.size());
	tester::expect_eq(report, "ft_lstiter first", 2, values[0]);
	tester::expect_eq(report, "ft_lstiter second", 3, values[1]);
	tester::expect_eq(report, "ft_lstiter third", 4, values[2]);
	ft_lstiter(list, NULL);
	tester::expect_eq(report, "ft_lstiter null function keeps first", 2,
		*(int *)list->content);
	ft_lstclear(&list, tester::free_int);
	list = test_helpers::build_list({10});
	ft_lstiter(list, test_helpers::increment_int);
	tester::expect_eq(report, "ft_lstiter single node", 11,
		*(int *)list->content);
	ft_lstclear(&list, tester::free_int);
	list = NULL;
	ft_lstiter(list, test_helpers::increment_int);
	tester::expect(report, "ft_lstiter null list guard", true);
}
