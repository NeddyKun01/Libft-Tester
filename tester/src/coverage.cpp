/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coverage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 10:20:00 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/07 10:20:00 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coverage.hpp"

#include <cctype>
#include <iomanip>
#include <iostream>

namespace coverage
{
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

	static bool	name_matches(const std::string &name,
		const std::string &filter)
	{
		if (filter.empty())
			return (true);
		return (lower_copy(name).find(lower_copy(filter)) != std::string::npos);
	}

	std::vector<Entry>	entries(void)
	{
		std::vector<Entry>	result;
		const Entry			items[] = {
			{"ft_isalpha", "ctype", "classification", "full signed/unsigned char range, letters, non-letters", "none"},
			{"ft_isdigit", "ctype", "classification", "full signed/unsigned char range, digits, non-digits", "none"},
			{"ft_isalnum", "ctype", "classification", "full signed/unsigned char range, letters, digits, symbols", "none"},
			{"ft_isascii", "ctype", "classification", "full signed/unsigned char range, ASCII limits, out of range", "none"},
			{"ft_isprint", "ctype", "classification", "full signed/unsigned char range, printable limits, controls", "none"},
			{"ft_toupper", "ctype", "conversion", "full signed/unsigned char range, lowercase, uppercase, symbols", "none"},
			{"ft_tolower", "ctype", "conversion", "full signed/unsigned char range, uppercase, lowercase, symbols", "none"},
			{"ft_strlen", "strings", "string length", "empty string, normal text, whitespace, punctuation, early NUL, seeded random strings", "none"},
			{"ft_strchr", "strings", "string search", "found, first char, missing, NUL terminator, empty string, wrapped char, stops at NUL, seeded random chars", "none"},
			{"ft_strrchr", "strings", "string search", "last match, first char, missing, NUL terminator, empty string, wrapped char, stops at NUL, seeded random chars", "none"},
			{"ft_strncmp", "strings", "string compare", "zero length, equal, prefix, negative diff, positive diff, shorter string, unsigned chars, seeded random signs", "none"},
			{"ft_strnstr", "strings", "bounded search", "empty needle, zero length, exact fit, too short, start match, missing, partial end, seeded random offsets", "none"},
			{"ft_strlcpy", "strings", "bounded copy", "size zero, size one, full copy, truncation, empty source, return length, buffer bytes", "none"},
			{"ft_strlcat", "strings", "bounded append", "size smaller than dst, truncation, full append, size zero, empty dst, empty src, buffer bytes", "none"},
			{"ft_strdup", "strings", "allocation copy", "normal text, empty string, copied content", "malloc failure"},
			{"ft_memset", "memory", "memory write", "return pointer, fixed fill, zero size, unsigned char cast, seeded random sizes/values", "none"},
			{"ft_bzero", "memory", "memory zero", "full clear, partial clear, zero size, one byte, surrounding bytes", "none"},
			{"ft_memcpy", "memory", "memory copy", "return pointer, fixed copy, zero size, unchanged bytes, offset copy, seeded random bytes", "none"},
			{"ft_memmove", "memory", "overlap copy", "return pointer, forward overlap, backward overlap, zero size, same pointer", "none"},
			{"ft_memchr", "memory", "memory search", "first match, NUL byte, missing, zero size, stops before later match, seeded random values", "none"},
			{"ft_memcmp", "memory", "memory compare", "equal buffers, zero size, unsigned diff, negative diff, equal prefix, seeded random signs", "none"},
			{"ft_calloc", "memory", "zero allocation", "overflow protection, zero-filled blocks, one byte allocation", "malloc failure"},
			{"ft_atoi", "atoi", "conversion", "zero, positive, negative, whitespace, signs, INT_MAX, INT_MIN, invalid prefixes, trailing text", "none"},
			{"ft_substr", "string_utils", "allocation substring", "basic slice, long length, out of range, zero length, empty source", "malloc failure"},
			{"ft_strjoin", "string_utils", "allocation join", "normal join, empty left, empty right, both empty", "malloc failure"},
			{"ft_strtrim", "string_utils", "allocation trim", "spaces, custom set, empty set, full trim, no trim, multi-character set", "malloc failure"},
			{"ft_split", "string_utils", "allocation split", "spaces, repeated delimiters, leading/trailing delimiters, empty tokens avoided, NULL terminator", "malloc failure across allocations"},
			{"ft_itoa", "string_utils", "integer to string", "INT_MIN, zero, positive, INT_MAX, negative, selected values", "malloc failure"},
			{"ft_strmapi", "string_utils", "mapped string", "index-aware transform, empty string, allocated result", "malloc failure"},
			{"ft_striteri", "string_utils", "in-place iteration", "uppercase transform, empty string, index use, NULL function, NULL string guard", "none"},
			{"ft_putchar_fd", "output", "fd output", "letter, newline, digit, NUL byte size/content", "none"},
			{"ft_putstr_fd", "output", "fd output", "normal text, empty string, whitespace, NULL string, digits", "none"},
			{"ft_putendl_fd", "output", "fd output", "normal text with newline, empty string, inner newline, NULL string, digits", "none"},
			{"ft_putnbr_fd", "output", "fd output", "INT_MIN, zero, negative, INT_MAX, positive", "none"},
			{"ft_lstnew", "lists", "list allocation", "content pointer, NULL content, next initialized to NULL", "malloc failure"},
			{"ft_lstadd_front", "lists", "list mutation", "normal prepend, empty list, NULL new node, size after prepend", "setup allocations"},
			{"ft_lstsize", "lists", "list query", "NULL list, one node, multiple nodes, after clear", "none"},
			{"ft_lstlast", "lists", "list query", "NULL list, single node, multiple nodes, next NULL on tail", "none"},
			{"ft_lstadd_back", "lists", "list mutation", "NULL new node, empty list, preserves head, links tail, last node", "setup allocations"},
			{"ft_lstdelone", "lists", "list deletion", "covered through clear/map deletion paths and custom deleters", "none"},
			{"ft_lstclear", "lists", "list deletion", "two nodes, resets head, empty list, NULL deleter guard", "none"},
			{"ft_lstiter", "lists", "list iteration", "multiple nodes, single node, NULL function guard, content mutation", "none"},
			{"ft_lstmap", "lists", "list mapping", "mapped copy, size, transformed values, f failure cleanup, NULL list", "malloc failure and cleanup"}
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

	std::vector<std::string>	function_names(void)
	{
		std::vector<std::string>	names;
		std::vector<Entry>		all;
		size_t					i;

		all = entries();
		i = 0;
		while (i < all.size())
		{
			names.push_back(all[i].name);
			i++;
		}
		return (names);
	}

	void	print_table(void)
	{
		std::vector<Entry>	all;
		size_t				i;

		all = entries();
		std::cout << "Function             Suite          Group              Malloc\n";
		std::cout << "----------------------------------------------------------------\n";
		i = 0;
		while (i < all.size())
		{
			std::cout << std::left << std::setw(21) << all[i].name
				<< std::setw(15) << all[i].suite
				<< std::setw(23) << all[i].group
				<< all[i].malloc_cases << '\n';
			i++;
		}
	}

	void	print_markdown(void)
	{
		std::vector<Entry>	all;
		size_t				i;

		all = entries();
		std::cout << "# Coverage Table\n\n";
		std::cout << "This table is generated from `tester/src/coverage.cpp`.\n\n";
		std::cout << "```sh\n";
		std::cout << "./libft_tester --coverage\n";
		std::cout << "./libft_tester --explain ft_split\n";
		std::cout << "./libft_tester --coverage-md > docs/COVERAGE.md\n";
		std::cout << "```\n\n";
		std::cout << "| Function | Suite | Group | Main cases | Malloc |\n";
		std::cout << "| --- | --- | --- | --- | --- |\n";
		i = 0;
		while (i < all.size())
		{
			std::cout << "| `" << all[i].name << "` | `" << all[i].suite
				<< "` | " << all[i].group << " | " << all[i].cases
				<< " | " << all[i].malloc_cases << " |\n";
			i++;
		}
	}

	bool	print_explain(const std::string &filter)
	{
		std::vector<Entry>	all;
		bool				found;
		size_t				i;

		all = entries();
		found = false;
		i = 0;
		while (i < all.size())
		{
			if (name_matches(all[i].name, filter))
			{
				if (found)
					std::cout << '\n';
				std::cout << all[i].name << "\n";
				std::cout << "  suite:  " << all[i].suite << "\n";
				std::cout << "  group:  " << all[i].group << "\n";
				std::cout << "  cases:  " << all[i].cases << "\n";
				std::cout << "  malloc: " << all[i].malloc_cases << "\n";
				found = true;
			}
			i++;
		}
		if (!found)
			std::cerr << "No documented coverage found for: " << filter << '\n';
		return (found);
	}
}
