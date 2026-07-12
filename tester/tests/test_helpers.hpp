#ifndef TEST_HELPERS_HPP
# define TEST_HELPERS_HPP

# include "malloc_fail.hpp"
# include "test_modules.hpp"

namespace test_helpers
{
	inline bool	is_alpha_ref(int c)
	{
		return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
	}

	inline bool	is_digit_ref(int c)
	{
		return (c >= '0' && c <= '9');
	}

	inline bool	is_alnum_ref(int c)
	{
		return (is_alpha_ref(c) || is_digit_ref(c));
	}

	inline bool	is_ascii_ref(int c)
	{
		return (c >= 0 && c <= 127);
	}

	inline bool	is_print_ref(int c)
	{
		return (c >= 32 && c <= 126);
	}

	inline int	toupper_ref(int c)
	{
		if (c >= 'a' && c <= 'z')
			return (c - 32);
		return (c);
	}

	inline int	tolower_ref(int c)
	{
		if (c >= 'A' && c <= 'Z')
			return (c + 32);
		return (c);
	}

	inline long	pointer_offset(const char *base, const char *position)
	{
		if (!position)
			return (-1);
		return (static_cast<long>(position - base));
	}

	inline bool	same_sign(int left, int right)
	{
		return ((left == 0 && right == 0)
			|| (left < 0 && right < 0)
			|| (left > 0 && right > 0));
	}

	inline void	fill_random_bytes(std::vector<unsigned char> &bytes,
		std::mt19937 &rng)
	{
		std::uniform_int_distribution<int>	byte_dist(0, 255);
		size_t								i = 0;

		while (i < bytes.size())
		{
			bytes[i] = static_cast<unsigned char>(byte_dist(rng));
			i++;
		}
	}

	inline char	shift_by_index(unsigned int index, char c)
	{
		return (static_cast<char>(c + static_cast<char>(index)));
	}

	inline void	make_upper(unsigned int, char *c)
	{
		if (*c >= 'a' && *c <= 'z')
			*c = static_cast<char>(ft_toupper(*c));
	}

	inline void	write_index_digit(unsigned int index, char *c)
	{
		*c = static_cast<char>('0' + index);
	}

	inline int	&delete_count(void)
	{
		static int	count = 0;

		return (count);
	}

	inline void	increment_int(void *content)
	{
		(*(int *)content)++;
	}

	inline void	*double_int(void *content)
	{
		return (tester::new_int((*(int *)content) * 2));
	}

	inline void	*fail_on_three(void *content)
	{
		if (*(int *)content == 3)
			return (NULL);
		return (tester::new_int(*(int *)content));
	}

	inline void	*identity_content(void *content)
	{
		return (content);
	}

	inline void	no_free(void *content)
	{
		(void)content;
	}

	inline void	count_delete(void *content)
	{
		delete_count()++;
		tester::free_int(content);
	}

	inline void	counting_free(void *content)
	{
		tester::free_int(content);
	}

	inline std::vector<int>	list_values(t_list *list)
	{
		std::vector<int>	values;

		while (list)
		{
			values.push_back(*(int *)list->content);
			list = list->next;
		}
		return (values);
	}

	inline t_list	*build_list(const std::vector<int> &values)
	{
		t_list	*list = NULL;
		size_t	i = 0;

		while (i < values.size())
		{
			ft_lstadd_back(&list, ft_lstnew(tester::new_int(values[i])));
			i++;
		}
		return (list);
	}
}

#endif
