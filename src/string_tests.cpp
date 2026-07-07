/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_tests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:52 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:53 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_fail.hpp"
#include "test_modules.hpp"

static long	pointer_offset(const char *base, const char *position)
{
	if (!position)
		return (-1);
	return (static_cast<long>(position - base));
}

static bool	same_sign(int left, int right)
{
	return ((left == 0 && right == 0)
		|| (left < 0 && right < 0)
		|| (left > 0 && right > 0));
}

static void	test_random_string_search(tester::Report &report,
	const std::string &sample, std::mt19937 &rng)
{
	std::uniform_int_distribution<int>	char_dist(32, 126);
	const char							*c_sample;
	int									needle;

	c_sample = sample.c_str();
	needle = char_dist(rng);
	tester::expect_eq(report, "ft_strlen random", sample.size(),
		ft_strlen(c_sample));
	tester::expect_eq(report, "ft_strchr random",
		std::strchr(c_sample, needle), ft_strchr(c_sample, needle));
	tester::expect_eq(report, "ft_strrchr random",
		std::strrchr(c_sample, needle), ft_strrchr(c_sample, needle));
}

static void	test_random_strnstr(tester::Report &report,
	const std::string &haystack, std::mt19937 &rng)
{
	std::uniform_int_distribution<int>	pos_dist;
	std::uniform_int_distribution<int>	len_dist;
	std::string							needle;
	const char							*reference;
	const char							*result;
	size_t								start;
	size_t								len;

	if (haystack.empty())
		return ;
	pos_dist = std::uniform_int_distribution<int>(
		0, static_cast<int>(haystack.size() - 1));
	start = static_cast<size_t>(pos_dist(rng));
	len_dist = std::uniform_int_distribution<int>(
		0, static_cast<int>(haystack.size() - start));
	len = static_cast<size_t>(len_dist(rng));
	needle = haystack.substr(start, len);
	len = static_cast<size_t>(std::uniform_int_distribution<int>(
		0, static_cast<int>(haystack.size() + 3))(rng));
	reference = tester::strnstr_reference(haystack.c_str(), needle.c_str(), len);
	result = ft_strnstr(haystack.c_str(), needle.c_str(), len);
	tester::expect_eq(report, "ft_strnstr random offset",
		pointer_offset(haystack.c_str(), reference),
		pointer_offset(haystack.c_str(), result));
}

static void	test_strings_randomized(tester::Report &report)
{
	std::mt19937						&rng = tester::random_engine();
	std::uniform_int_distribution<int>	len_dist(0, 32);
	int									round;

	round = 0;
	while (round < 8)
	{
		std::string	left = tester::random_printable(rng, 32);
		std::string	right = tester::random_printable(rng, 32);
		size_t		len = static_cast<size_t>(len_dist(rng));

		test_random_string_search(report, left, rng);
		tester::expect(report, "ft_strncmp random sign",
			same_sign(std::strncmp(left.c_str(), right.c_str(), len),
				ft_strncmp(left.c_str(), right.c_str(), len)));
		test_random_strnstr(report, left, rng);
		round++;
	}
}

void	test_strings(tester::Report &report)
{
	{
		const char	*sample = "hello world";
		const char	*empty = "";
		const char	*big = "abcdef";
		const char	*small = "abc";
		const char	hidden[] = {'a', 'b', '\0', 'c', '\0'};

		tester::expect_eq(report, "ft_strlen empty", static_cast<size_t>(0),
			ft_strlen(""));
		tester::expect_eq(report, "ft_strlen normal", std::strlen(sample),
			ft_strlen(sample));
		tester::expect_eq(report, "ft_strlen whitespace",
			std::strlen(" \t\n"), ft_strlen(" \t\n"));
		tester::expect_eq(report, "ft_strlen punctuation",
			std::strlen("42 Lisboa!"), ft_strlen("42 Lisboa!"));
		tester::expect_eq(report, "ft_strlen stops at nul",
			static_cast<size_t>(2), ft_strlen(hidden));
		tester::expect_eq(report, "ft_strchr found",
			std::strchr(sample, 'o'), ft_strchr(sample, 'o'));
		tester::expect_eq(report, "ft_strchr first char",
			std::strchr(sample, 'h'), ft_strchr(sample, 'h'));
		tester::expect_eq(report, "ft_strchr missing",
			std::strchr(sample, 'z'), ft_strchr(sample, 'z'));
		tester::expect_eq(report, "ft_strchr nul",
			std::strchr(sample, '\0'), ft_strchr(sample, '\0'));
		tester::expect_eq(report, "ft_strchr empty nul",
			std::strchr(empty, '\0'), ft_strchr(empty, '\0'));
		tester::expect_eq(report, "ft_strchr wrapped char",
			std::strchr(sample, 'h' + 256), ft_strchr(sample, 'h' + 256));
		tester::expect_eq(report, "ft_strchr stops at nul",
			(char *)NULL, ft_strchr(hidden, 'c'));
		tester::expect_eq(report, "ft_strrchr found",
			std::strrchr(sample, 'o'), ft_strrchr(sample, 'o'));
		tester::expect_eq(report, "ft_strrchr first char",
			std::strrchr(sample, 'h'), ft_strrchr(sample, 'h'));
		tester::expect_eq(report, "ft_strrchr missing",
			std::strrchr(sample, 'z'), ft_strrchr(sample, 'z'));
		tester::expect_eq(report, "ft_strrchr nul",
			std::strrchr(sample, '\0'), ft_strrchr(sample, '\0'));
		tester::expect_eq(report, "ft_strrchr empty nul",
			std::strrchr(empty, '\0'), ft_strrchr(empty, '\0'));
		tester::expect_eq(report, "ft_strrchr wrapped char",
			std::strrchr(sample, 'h' + 256), ft_strrchr(sample, 'h' + 256));
		tester::expect_eq(report, "ft_strrchr stops at nul",
			(char *)NULL, ft_strrchr(hidden, 'c'));
		tester::expect_eq(report, "ft_strncmp zero size", 0,
			ft_strncmp("abc", "xyz", 0));
		tester::expect_eq(report, "ft_strncmp equal", 0,
			ft_strncmp("libft", "libft", 5));
		tester::expect_eq(report, "ft_strncmp prefix",
			std::strncmp("abc", "abd", 2), ft_strncmp("abc", "abd", 2));
		tester::expect(report, "ft_strncmp negative",
			ft_strncmp("abc", "abd", 3) < 0);
		tester::expect(report, "ft_strncmp positive",
			ft_strncmp("abd", "abc", 3) > 0);
		tester::expect(report, "ft_strncmp shorter",
			ft_strncmp("abc", "abcd", 4) < 0);
		tester::expect_eq(report, "ft_strncmp unsigned",
			std::strncmp("\xff", "\x01", 1), ft_strncmp("\xff", "\x01", 1));
		tester::expect_eq(report, "ft_strnstr empty needle",
			small, ft_strnstr(small, "", 3));
		tester::expect_eq(report, "ft_strnstr empty needle zero len",
			big, ft_strnstr(big, "", 0));
		tester::expect_eq(report, "ft_strnstr exact fit",
			big + 2, ft_strnstr(big, "cde", 5));
		tester::expect_eq(report, "ft_strnstr too short", (char *)NULL,
			ft_strnstr("abcdef", "cde", 4));
		tester::expect_eq(report, "ft_strnstr start",
			big, ft_strnstr(big, "abc", 3));
		tester::expect_eq(report, "ft_strnstr missing",
			(char *)NULL, ft_strnstr(big, "gh", 6));
		tester::expect_eq(report, "ft_strnstr partial needle at end",
			(char *)NULL, ft_strnstr(big, "efg", 6));
	}
	{
		char	buffer[16];
		char	reference[16];

		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		tester::expect_eq(report, "ft_strlcpy size zero length",
			tester::strlcpy_reference(reference, "hello", 0),
			ft_strlcpy(buffer, "hello", 0));
		tester::expect_mem(report, "ft_strlcpy size zero buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		tester::expect_eq(report, "ft_strlcpy size one length",
			tester::strlcpy_reference(reference, "hello", 1),
			ft_strlcpy(buffer, "hello", 1));
		tester::expect_mem(report, "ft_strlcpy size one buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		tester::expect_eq(report, "ft_strlcpy full length",
			tester::strlcpy_reference(reference, "hello", sizeof(buffer)),
			ft_strlcpy(buffer, "hello", sizeof(buffer)));
		tester::expect_mem(report, "ft_strlcpy full buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		tester::expect_eq(report, "ft_strlcpy trunc length",
			tester::strlcpy_reference(reference, "hello", 4),
			ft_strlcpy(buffer, "hello", 4));
		tester::expect_mem(report, "ft_strlcpy trunc buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		tester::expect_eq(report, "ft_strlcpy empty source length",
			tester::strlcpy_reference(reference, "", sizeof(buffer)),
			ft_strlcpy(buffer, "", sizeof(buffer)));
		tester::expect_mem(report, "ft_strlcpy empty source buffer", reference,
			buffer, sizeof(buffer));
	}
	{
		char	buffer[16];
		char	reference[16];

		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		std::strcpy(buffer, "abc");
		std::strcpy(reference, "abc");
		tester::expect_eq(report, "ft_strlcat size smaller length",
			tester::strlcat_reference(reference, "XYZ", 2),
			ft_strlcat(buffer, "XYZ", 2));
		tester::expect_mem(report, "ft_strlcat size smaller buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		std::strcpy(buffer, "abc");
		std::strcpy(reference, "abc");
		tester::expect_eq(report, "ft_strlcat trunc length",
			tester::strlcat_reference(reference, "XYZ", 5),
			ft_strlcat(buffer, "XYZ", 5));
		tester::expect_mem(report, "ft_strlcat trunc buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		std::strcpy(buffer, "abc");
		std::strcpy(reference, "abc");
		tester::expect_eq(report, "ft_strlcat full length",
			tester::strlcat_reference(reference, "XYZ", sizeof(buffer)),
			ft_strlcat(buffer, "XYZ", sizeof(buffer)));
		tester::expect_mem(report, "ft_strlcat full buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		std::strcpy(buffer, "abc");
		std::strcpy(reference, "abc");
		tester::expect_eq(report, "ft_strlcat size zero length",
			tester::strlcat_reference(reference, "XYZ", 0),
			ft_strlcat(buffer, "XYZ", 0));
		tester::expect_mem(report, "ft_strlcat size zero buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		std::strcpy(buffer, "");
		std::strcpy(reference, "");
		tester::expect_eq(report, "ft_strlcat empty dst length",
			tester::strlcat_reference(reference, "XYZ", sizeof(buffer)),
			ft_strlcat(buffer, "XYZ", sizeof(buffer)));
		tester::expect_mem(report, "ft_strlcat empty dst buffer", reference,
			buffer, sizeof(buffer));
		std::memset(buffer, 0xCC, sizeof(buffer));
		std::memset(reference, 0xCC, sizeof(reference));
		std::strcpy(buffer, "abc");
		std::strcpy(reference, "abc");
		tester::expect_eq(report, "ft_strlcat empty src length",
			tester::strlcat_reference(reference, "", sizeof(buffer)),
			ft_strlcat(buffer, "", sizeof(buffer)));
		tester::expect_mem(report, "ft_strlcat empty src buffer", reference,
			buffer, sizeof(buffer));
	}
	{
		char	*copy = ft_strdup("libft");

		tester::expect_malloc(report, "ft_strdup", copy, true);
		if (copy)
		{
			tester::expect_eq(report, "ft_strdup text", std::string("libft"),
				std::string(copy));
			std::free(copy);
		}
		copy = ft_strdup("");
		tester::expect_malloc(report, "ft_strdup empty", copy, true);
		if (copy)
		{
			tester::expect_eq(report, "ft_strdup empty text", std::string(""),
				std::string(copy));
			std::free(copy);
		}
		malloc_fail_on(1);
		copy = ft_strdup("malloc");
		malloc_fail_off();
		tester::expect_malloc(report, "ft_strdup malloc failure", copy, false);
		std::free(copy);
	}
	test_strings_randomized(report);
}
