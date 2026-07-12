#include "test_helpers.hpp"

void	test_ft_calloc(tester::Report &report)
{
	void	*overflow;
	void	*memory;

	overflow = ft_calloc(SIZE_MAX / 2 + 1, 2);
	tester::expect_malloc(report, "ft_calloc overflow", overflow, false);
	malloc_fail_on(1);
	memory = ft_calloc(4, sizeof(int));
	malloc_fail_off();
	tester::expect_malloc(report, "ft_calloc malloc failure", memory, false);
	std::free(memory);
	{
		int		*numbers = static_cast<int *>(ft_calloc(8, sizeof(int)));
		bool	zeroed = true;
		size_t	i = 0;

		tester::expect_malloc(report, "ft_calloc zero fill", numbers, true);
		while (numbers && i < 8)
		{
			zeroed = zeroed && numbers[i] == 0;
			i++;
		}
		if (numbers)
		{
			tester::expect(report, "ft_calloc zero fill content", zeroed);
			std::free(numbers);
		}
	}
	{
		unsigned char	*byte;

		byte = static_cast<unsigned char *>(ft_calloc(1, 1));
		tester::expect_malloc(report, "ft_calloc one byte", byte, true);
		if (byte)
		{
			tester::expect_eq(report, "ft_calloc one byte content",
				static_cast<unsigned char>(0), *byte);
			std::free(byte);
		}
	}
}
