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
}
