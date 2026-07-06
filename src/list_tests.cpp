/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:37 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:38 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_fail.hpp"
#include "test_modules.hpp"

static int	g_delete_count = 0;

static void	increment_int(void *content)
{
	(*(int *)content)++;
}

static void	*double_int(void *content)
{
	return (tester::new_int((*(int *)content) * 2));
}

static void	*fail_on_three(void *content)
{
	if (*(int *)content == 3)
		return (NULL);
	return (tester::new_int(*(int *)content));
}

static void	*identity_content(void *content)
{
	return (content);
}

static void	no_free(void *content)
{
	(void)content;
}

static void	count_delete(void *content)
{
	g_delete_count++;
	tester::free_int(content);
}

static void	counting_free(void *content)
{
	static int	count = 0;

	count++;
	tester::free_int(content);
}

static std::vector<int>	list_values(t_list *list)
{
	std::vector<int>	values;

	while (list)
	{
		values.push_back(*(int *)list->content);
		list = list->next;
	}
	return (values);
}

static t_list	*build_list(const std::vector<int> &values)
{
	t_list	*list;
	size_t	i;

	list = NULL;
	i = 0;
	while (i < values.size())
	{
		ft_lstadd_back(&list, ft_lstnew(tester::new_int(values[i])));
		i++;
	}
	return (list);
}

static void	test_lstnew_add_back(tester::Report &report)
{
	t_list	*empty = NULL;
	t_list	*single = ft_lstnew(tester::new_int(42));

	tester::expect_malloc(report, "ft_lstnew", single, true);
	if (single)
	{
		tester::expect_eq(report, "ft_lstnew content", 42,
			*(int *)single->content);
		tester::expect_eq(report, "ft_lstnew next null", (void *)NULL,
			single->next);
		ft_lstdelone(single, tester::free_int);
	}
	single = ft_lstnew(NULL);
	tester::expect_malloc(report, "ft_lstnew null content", single, true);
	if (single)
	{
		tester::expect_eq(report, "ft_lstnew null content value", (void *)NULL,
			single->content);
		tester::expect_eq(report, "ft_lstnew null content next", (void *)NULL,
			single->next);
		ft_lstdelone(single, tester::free_int);
	}
	{
		int	*content = tester::new_int(99);

		malloc_fail_on(1);
		single = ft_lstnew(content);
		malloc_fail_off();
		tester::expect_malloc(report, "ft_lstnew malloc failure", single,
			false);
		if (single)
			ft_lstdelone(single, tester::free_int);
		else
			tester::free_int(content);
	}
	ft_lstadd_back(&empty, NULL);
	tester::expect_eq(report, "ft_lstadd_back null node", (void *)NULL, empty);
	{
		t_list	*list = NULL;
		t_list	*first = ft_lstnew(tester::new_int(1));
		t_list	*second = ft_lstnew(tester::new_int(2));
		t_list	*orphan = ft_lstnew(tester::new_int(3));

		ft_lstadd_back(&list, first);
		tester::expect_eq(report, "ft_lstadd_back empty list", first, list);
		tester::expect_eq(report, "ft_lstadd_back empty next", (void *)NULL,
			first->next);
		tester::expect_eq(report, "ft_lstlast single node", first,
			ft_lstlast(list));
		ft_lstadd_back(&list, second);
		tester::expect_eq(report, "ft_lstadd_back preserves head", first, list);
		tester::expect_eq(report, "ft_lstadd_back links tail", second,
			first->next);
		tester::expect_eq(report, "ft_lstadd_back tail next null",
			(void *)NULL, second->next);
		tester::expect_eq(report, "ft_lstlast after add_back", second,
			ft_lstlast(list));
		ft_lstadd_back(NULL, orphan);
		ft_lstdelone(orphan, tester::free_int);
		ft_lstclear(&list, tester::free_int);
	}
}

static void	test_lstsize_last_front_clear(tester::Report &report)
{
	t_list	*list = build_list({1, 2, 3});

	tester::expect_eq(report, "ft_lstsize null", 0, ft_lstsize(NULL));
	tester::expect_eq(report, "ft_lstlast null", (t_list *)NULL,
		ft_lstlast(NULL));
	tester::expect_eq(report, "ft_lstsize one", 1,
		ft_lstsize(ft_lstlast(list)));
	tester::expect_eq(report, "ft_lstsize", 3, ft_lstsize(list));
	tester::expect_eq(report, "ft_lstlast", 3,
		*(int *)ft_lstlast(list)->content);
	tester::expect_eq(report, "ft_lstlast next null", (void *)NULL,
		ft_lstlast(list)->next);
	ft_lstadd_front(&list, ft_lstnew(tester::new_int(0)));
	tester::expect_eq(report, "ft_lstadd_front head", 0,
		*(int *)list->content);
	tester::expect_eq(report, "ft_lstadd_front links old head", 1,
		*(int *)list->next->content);
	tester::expect_eq(report, "ft_lstsize after add_front", 4,
		ft_lstsize(list));
	ft_lstclear(&list, tester::free_int);
	tester::expect_eq(report, "ft_lstclear resets head", (void *)NULL, list);
	tester::expect_eq(report, "ft_lstsize after clear", 0, ft_lstsize(list));
	{
		t_list	*orphan;

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
	{
		list = build_list({4, 5});
		g_delete_count = 0;
		ft_lstclear(&list, count_delete);
		tester::expect_eq(report, "ft_lstclear calls del twice", 2,
			g_delete_count);
		tester::expect_eq(report, "ft_lstclear two nodes resets head",
			(void *)NULL, list);
		ft_lstclear(&list, count_delete);
		tester::expect_eq(report, "ft_lstclear empty list stays null",
			(void *)NULL, list);
		list = build_list({8});
		ft_lstclear(&list, NULL);
		tester::expect_eq(report, "ft_lstclear null del keeps list", 8,
			*(int *)list->content);
		ft_lstclear(&list, tester::free_int);
	}
}

static void	test_lstiter_map(tester::Report &report)
{
	t_list	*list = build_list({1, 2, 3});

	ft_lstiter(list, increment_int);
	std::vector<int>	values = list_values(list);
	tester::expect_eq(report, "ft_lstiter size", static_cast<size_t>(3),
		values.size());
	tester::expect_eq(report, "ft_lstiter first", 2, values[0]);
	tester::expect_eq(report, "ft_lstiter second", 3, values[1]);
	tester::expect_eq(report, "ft_lstiter third", 4, values[2]);
	ft_lstiter(list, NULL);
	tester::expect_eq(report, "ft_lstiter null function keeps first", 2,
		*(int *)list->content);
	ft_lstclear(&list, tester::free_int);
	list = build_list({10});
	ft_lstiter(list, increment_int);
	tester::expect_eq(report, "ft_lstiter single node", 11,
		*(int *)list->content);
	ft_lstclear(&list, tester::free_int);
	list = build_list({1, 2, 3});
	t_list	*mapped = ft_lstmap(list, double_int, tester::free_int);
	tester::expect_malloc(report, "ft_lstmap", mapped, true);
	values = list_values(mapped);
	tester::expect_eq(report, "ft_lstmap size", static_cast<size_t>(3),
		values.size());
	tester::expect_eq(report, "ft_lstmap first", 2, values[0]);
	tester::expect_eq(report, "ft_lstmap second", 4, values[1]);
	tester::expect_eq(report, "ft_lstmap third", 6, values[2]);
	ft_lstclear(&mapped, tester::free_int);
	ft_lstclear(&list, tester::free_int);
}

static void	test_lstmap_failures(tester::Report &report)
{
	t_list	*list = build_list({1, 2, 3});
	t_list	*mapped = ft_lstmap(list, fail_on_three, counting_free);

	tester::expect_malloc(report, "ft_lstmap failure returns null", mapped,
		false);
	if (mapped)
		ft_lstclear(&mapped, tester::free_int);
	ft_lstclear(&list, tester::free_int);
	list = build_list({1, 2, 3});
	size_t	fail_at = 1;
	while (fail_at <= 3)
	{
		malloc_fail_on(fail_at);
		mapped = ft_lstmap(list, identity_content, no_free);
		malloc_fail_off();
		tester::expect_malloc(report, "ft_lstmap malloc failure", mapped,
			false);
		if (mapped)
			ft_lstclear(&mapped, no_free);
		fail_at++;
	}
	ft_lstclear(&list, tester::free_int);
	{
		t_list	*none = NULL;

		tester::expect_eq(report, "ft_lstmap null list", (t_list *)NULL,
			ft_lstmap(none, double_int, tester::free_int));
		tester::expect_eq(report, "ft_lstclear null list", (void *)NULL, none);
		ft_lstiter(none, increment_int);
		ft_lstdelone(NULL, tester::free_int);
	}
}

void	test_lists(tester::Report &report)
{
	test_lstnew_add_back(report);
	test_lstsize_last_front_clear(report);
	test_lstiter_map(report);
	test_lstmap_failures(report);
}
