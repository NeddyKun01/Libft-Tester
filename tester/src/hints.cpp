/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hints.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: libft-tester <opensource@example.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 15:10:00 by libft-tester          #+#    #+#             */
/*   Updated: 2026/07/07 15:10:00 by libft-tester         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hints.hpp"

#include <cctype>
#include <iostream>
#include <vector>

namespace hints
{
	struct Hint
	{
		const char	*key;
		const char	*text;
	};

	static std::string	lower_copy(const std::string &value)
	{
		std::string	result;
		size_t		i;

		result = value;
		i = 0;
		while (i < result.size())
		{
			result[i] = static_cast<char>(std::tolower(
				static_cast<unsigned char>(result[i])));
			i++;
		}
		return (result);
	}

	static std::vector<Hint>	all(void)
	{
		std::vector<Hint>	result;
		const Hint			items[] = {
			{"ft_strlen", "Count bytes until the first NUL byte; do not include the terminator."},
			{"ft_strchr", "Search must include the final NUL byte and cast the searched char to unsigned char."},
			{"ft_strrchr", "Keep the last matching address, including matches on the final NUL byte."},
			{"ft_strncmp", "Compare as unsigned char and stop at n bytes or at the first difference/NUL."},
			{"ft_strnstr", "Never read past len; an empty needle returns big immediately."},
			{"ft_strlcpy", "Return the full source length and NUL-terminate only when size is greater than zero."},
			{"ft_strlcat", "Return initial dst length plus src length, or size plus src length if dst is not NUL inside size."},
			{"ft_strdup", "Allocate strlen(s) + 1 bytes, copy the terminator, and return NULL on malloc failure."},
			{"ft_memset", "Write exactly n bytes using the unsigned char value of c."},
			{"ft_bzero", "Set exactly n bytes to zero and leave surrounding bytes untouched."},
			{"ft_memcpy", "Copy exactly n bytes; overlap is undefined, but n == 0 must not change memory."},
			{"ft_memmove", "Handle overlapping ranges by choosing the safe copy direction."},
			{"ft_memchr", "Search byte by byte as unsigned char and stop exactly after n bytes."},
			{"ft_memcmp", "Return the difference between the first different unsigned bytes."},
			{"ft_calloc", "Check multiplication overflow before malloc and zero every allocated byte."},
			{"ft_atoi", "Skip standard whitespace, handle one sign, stop at the first non-digit."},
			{"ft_substr", "If start is past the source length, return an allocated empty string."},
			{"ft_strjoin", "Allocate len(s1) + len(s2) + 1 and copy both strings plus the terminator."},
			{"ft_strtrim", "Trim only characters present in set from the beginning and the end."},
			{"ft_split", "Skip repeated delimiters, allocate a NULL-terminated array, and free all previous allocations on failure."},
			{"ft_itoa", "Handle zero, negatives, INT_MIN, and allocate space for sign plus terminator."},
			{"ft_strmapi", "Allocate a new string, call f(index, char), and keep the original string untouched."},
			{"ft_striteri", "Modify the string in place and safely ignore NULL string/function if you chose that guard."},
			{"ft_putchar_fd", "Write exactly one byte to the provided file descriptor."},
			{"ft_putstr_fd", "Write the string bytes without adding a newline."},
			{"ft_putendl_fd", "Write the string and then exactly one newline."},
			{"ft_putnbr_fd", "Handle INT_MIN without overflowing a signed int."},
			{"ft_lstnew", "Allocate one node, store the content pointer, and initialize next to NULL."},
			{"ft_lstadd_front", "If the new node exists, link it before the current head and update the head pointer."},
			{"ft_lstsize", "Count nodes until NULL; a NULL list has size zero."},
			{"ft_lstlast", "Walk until next is NULL and return that last node."},
			{"ft_lstadd_back", "If the list is empty, the new node becomes the head; otherwise link it after the last node."},
			{"ft_lstdelone", "Call del on the content, then free the node itself."},
			{"ft_lstclear", "Delete every node, call del for each content, and set the list pointer to NULL."},
			{"ft_lstiter", "Apply f to every content pointer in order."},
			{"ft_lstmap", "Create a new list with f(content); if any allocation fails, clear everything already created."}
		};
		size_t				i;

		i = 0;
		while (i < sizeof(items) / sizeof(items[0]))
		{
			result.push_back(items[i]);
			i++;
		}
		return (result);
	}

	std::string	for_label(const std::string &label)
	{
		std::vector<Hint>	items;
		std::string			lower_label;
		size_t				i;

		items = all();
		lower_label = lower_copy(label);
		i = 0;
		while (i < items.size())
		{
			if (lower_label.find(items[i].key) != std::string::npos)
				return (items[i].text);
			i++;
		}
		return ("");
	}

	bool	print_for_function(const std::string &filter)
	{
		std::vector<Hint>	items;
		std::string			lower_filter;
		bool				found;
		size_t				i;

		items = all();
		lower_filter = lower_copy(filter);
		found = false;
		i = 0;
		while (i < items.size())
		{
			std::string	key = items[i].key;

			if (lower_filter.empty()
				|| lower_copy(key).find(lower_filter) != std::string::npos)
			{
				std::cout << items[i].key << "\n  " << items[i].text << "\n";
				found = true;
			}
			i++;
		}
		if (!found)
			std::cerr << "No hint found for: " << filter << '\n';
		return (found);
	}
}
