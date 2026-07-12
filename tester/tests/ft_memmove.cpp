#include "test_helpers.hpp"

void	test_ft_memmove(tester::Report &report)
{
	unsigned char	forward[] = "abcdef";
	unsigned char	backward[] = "abcdef";
	unsigned char	forward_ref[] = "abcdef";
	unsigned char	backward_ref[] = "abcdef";
	unsigned char	same[] = "libft";

	tester::expect_eq(report, "ft_memmove return", (void *)(forward + 1),
		ft_memmove(forward + 1, forward, 4));
	std::memmove(forward_ref + 1, forward_ref, 4);
	tester::expect_mem(report, "ft_memmove forward overlap", forward_ref,
		forward, sizeof(forward));
	ft_memmove(backward, backward + 1, 4);
	std::memmove(backward_ref, backward_ref + 1, 4);
	tester::expect_mem(report, "ft_memmove backward overlap", backward_ref,
		backward, sizeof(backward));
	tester::expect_eq(report, "ft_memmove zero size", (void *)backward,
		ft_memmove(backward, backward + 1, 0));
	tester::expect_eq(report, "ft_memmove same pointer", (void *)same,
		ft_memmove(same, same, sizeof(same)));
	tester::expect_eq(report, "ft_memmove same pointer text",
		std::string("libft"), std::string((char *)same));
}
