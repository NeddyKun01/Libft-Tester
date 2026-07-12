#include "test_helpers.hpp"

void	test_ft_lstmap(tester::Report &report)
{
	t_list				*list = test_helpers::build_list({1, 2, 3});
	t_list				*mapped;
	std::vector<int>	values;

	mapped = ft_lstmap(list, test_helpers::double_int, tester::free_int);
	tester::expect_malloc(report, "ft_lstmap", mapped, true);
	values = test_helpers::list_values(mapped);
	tester::expect_eq(report, "ft_lstmap size", static_cast<size_t>(3),
		values.size());
	tester::expect_eq(report, "ft_lstmap first", 2, values[0]);
	tester::expect_eq(report, "ft_lstmap second", 4, values[1]);
	tester::expect_eq(report, "ft_lstmap third", 6, values[2]);
	ft_lstclear(&mapped, tester::free_int);
	ft_lstclear(&list, tester::free_int);
	list = test_helpers::build_list({1, 2, 3});
	mapped = ft_lstmap(list, test_helpers::fail_on_three,
		test_helpers::counting_free);
	tester::expect_malloc(report, "ft_lstmap failure returns null", mapped,
		false);
	if (mapped)
		ft_lstclear(&mapped, tester::free_int);
	ft_lstclear(&list, tester::free_int);
	list = test_helpers::build_list({1, 2, 3});
	for (size_t fail_at = 1; fail_at <= 3; ++fail_at)
	{
		malloc_fail_on(fail_at);
		mapped = ft_lstmap(list, test_helpers::identity_content,
			test_helpers::no_free);
		malloc_fail_off();
		tester::expect_malloc(report, "ft_lstmap malloc failure", mapped,
			false);
		if (mapped)
			ft_lstclear(&mapped, test_helpers::no_free);
	}
	ft_lstclear(&list, tester::free_int);
	{
		t_list	*none = NULL;

		tester::expect_eq(report, "ft_lstmap null list", (t_list *)NULL,
			ft_lstmap(none, test_helpers::double_int, tester::free_int));
	}
}
