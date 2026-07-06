/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils_tests.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:55 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:56 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_fail.hpp"
#include "test_modules.hpp"

static char	shift_by_index(unsigned int index, char c)
{
	return (static_cast<char>(c + static_cast<char>(index)));
}

static void	make_upper(unsigned int, char *c)
{
	if (*c >= 'a' && *c <= 'z')
		*c = static_cast<char>(ft_toupper(*c));
}

static void	write_index_digit(unsigned int index, char *c)
{
	*c = static_cast<char>('0' + index);
}

static void	check_split(tester::Report &report, const char *input,
	char delimiter, const std::vector<std::string> &expected)
{
	char	**split;
	size_t	i;

	split = ft_split(input, delimiter);
	tester::expect_malloc(report, "ft_split allocation", split, true);
	tester::expect_eq(report, "ft_split token count", expected.size(),
		tester::split_count(split));
	i = 0;
	while (i < expected.size() && split && split[i])
	{
		tester::expect_eq(report, "ft_split token", expected[i],
			std::string(split[i]));
		i++;
	}
	tester::expect(report, "ft_split terminator", split && split[i] == NULL);
	tester::free_split(split);
}

static void	test_substr(tester::Report &report)
{
	char	*result;

	result = ft_substr("abcdef", 1, 3);
	tester::expect_malloc(report, "ft_substr basic", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr basic text", std::string("bcd"),
			std::string(result));
	std::free(result);
	result = ft_substr("abcdef", 2, 99);
	tester::expect_malloc(report, "ft_substr long len", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr long len text",
			std::string("cdef"), std::string(result));
	std::free(result);
	result = ft_substr("abcdef", 99, 3);
	tester::expect_malloc(report, "ft_substr out of range", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr out of range text",
			std::string(""), std::string(result));
	std::free(result);
	result = ft_substr("abcdef", 0, 0);
	tester::expect_malloc(report, "ft_substr zero len", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr zero len text", std::string(""),
			std::string(result));
	std::free(result);
	result = ft_substr("", 0, 10);
	tester::expect_malloc(report, "ft_substr empty source", result, true);
	if (result)
		tester::expect_eq(report, "ft_substr empty source text",
			std::string(""), std::string(result));
	std::free(result);
	malloc_fail_on(1);
	result = ft_substr("abcdef", 1, 3);
	malloc_fail_off();
	tester::expect_malloc(report, "ft_substr malloc failure", result, false);
	std::free(result);
}

static void	test_join_trim(tester::Report &report)
{
	char		*join;
	char		*result;
	std::string	trimmed;

	join = ft_strjoin("hello", " world");
	tester::expect_malloc(report, "ft_strjoin", join, true);
	if (join)
		tester::expect_eq(report, "ft_strjoin text",
			std::string("hello world"), std::string(join));
	std::free(join);
	join = ft_strjoin("", "tail");
	tester::expect_malloc(report, "ft_strjoin empty left", join, true);
	if (join)
		tester::expect_eq(report, "ft_strjoin empty left text",
			std::string("tail"), std::string(join));
	std::free(join);
	join = ft_strjoin("head", "");
	tester::expect_malloc(report, "ft_strjoin empty right", join, true);
	if (join)
		tester::expect_eq(report, "ft_strjoin empty right text",
			std::string("head"), std::string(join));
	std::free(join);
	join = ft_strjoin("", "");
	tester::expect_malloc(report, "ft_strjoin both empty", join, true);
	if (join)
		tester::expect_eq(report, "ft_strjoin both empty text",
			std::string(""), std::string(join));
	std::free(join);
	malloc_fail_on(1);
	join = ft_strjoin("left", "right");
	malloc_fail_off();
	tester::expect_malloc(report, "ft_strjoin malloc failure", join, false);
	std::free(join);
	trimmed = tester::trim_reference("   hello   ", " ");
	result = ft_strtrim("   hello   ", " ");
	tester::expect_malloc(report, "ft_strtrim spaces", result, true);
	if (result)
		tester::expect_eq(report, "ft_strtrim spaces text", trimmed,
			std::string(result));
	std::free(result);
	trimmed = tester::trim_reference("--libft--", "-");
	result = ft_strtrim("--libft--", "-");
	tester::expect_malloc(report, "ft_strtrim dashes", result, true);
	if (result)
		tester::expect_eq(report, "ft_strtrim dashes text", trimmed,
			std::string(result));
	std::free(result);
	result = ft_strtrim("hello", "");
	tester::expect_malloc(report, "ft_strtrim empty set", result, true);
	if (result)
		tester::expect_eq(report, "ft_strtrim empty set text",
			std::string("hello"), std::string(result));
	std::free(result);
	result = ft_strtrim("xxxx", "x");
	tester::expect_malloc(report, "ft_strtrim full trim", result, true);
	if (result)
		tester::expect_eq(report, "ft_strtrim full trim text",
			std::string(""), std::string(result));
	std::free(result);
	result = ft_strtrim("hello", "xyz");
	tester::expect_malloc(report, "ft_strtrim no trim", result, true);
	if (result)
		tester::expect_eq(report, "ft_strtrim no trim text",
			std::string("hello"), std::string(result));
	std::free(result);
	result = ft_strtrim("xyhelloyx", "xy");
	tester::expect_malloc(report, "ft_strtrim multi set", result, true);
	if (result)
		tester::expect_eq(report, "ft_strtrim multi set text",
			std::string("hello"), std::string(result));
	std::free(result);
	malloc_fail_on(1);
	result = ft_strtrim("  hello  ", " ");
	malloc_fail_off();
	tester::expect_malloc(report, "ft_strtrim malloc failure", result, false);
	std::free(result);
}

static void	test_split_itoa(tester::Report &report)
{
	size_t	fail_at;
	char	*result;

	check_split(report, "one  two   three", ' ', {"one", "two", "three"});
	check_split(report, ",,,", ',', {});
	check_split(report, "", ',', {});
	check_split(report, ",one,two,", ',', {"one", "two"});
	check_split(report, "abc", ',', {"abc"});
	check_split(report, "abc", '\0', {"abc"});
	check_split(report, "a\tb\t\tc", '\t', {"a", "b", "c"});
	fail_at = 1;
	while (fail_at <= 4)
	{
		char	**split;

		malloc_fail_on(fail_at);
		split = ft_split("one two three", ' ');
		malloc_fail_off();
		tester::expect_malloc(report, "ft_split malloc failure", split, false);
		tester::free_split(split);
		fail_at++;
	}
	result = ft_itoa(INT_MIN);
	tester::expect_malloc(report, "ft_itoa INT_MIN", result, true);
	if (result)
		tester::expect_eq(report, "ft_itoa INT_MIN text",
			std::to_string(INT_MIN), std::string(result));
	std::free(result);
	result = ft_itoa(0);
	tester::expect_malloc(report, "ft_itoa zero", result, true);
	if (result)
		tester::expect_eq(report, "ft_itoa zero text", std::string("0"),
			std::string(result));
	std::free(result);
	result = ft_itoa(123456789);
	tester::expect_malloc(report, "ft_itoa positive", result, true);
	if (result)
		tester::expect_eq(report, "ft_itoa positive text",
			std::to_string(123456789), std::string(result));
	std::free(result);
	result = ft_itoa(INT_MAX);
	tester::expect_malloc(report, "ft_itoa INT_MAX", result, true);
	if (result)
		tester::expect_eq(report, "ft_itoa INT_MAX text",
			std::to_string(INT_MAX), std::string(result));
	std::free(result);
	result = ft_itoa(-987654321);
	tester::expect_malloc(report, "ft_itoa negative", result, true);
	if (result)
		tester::expect_eq(report, "ft_itoa negative text",
			std::to_string(-987654321), std::string(result));
	std::free(result);
	for (int value : {-1, 1, 9, 10, -10, 1000, -1000})
	{
		result = ft_itoa(value);
		tester::expect_malloc(report, "ft_itoa selected value", result, true);
		if (result)
			tester::expect_eq(report, "ft_itoa selected value text",
				std::to_string(value), std::string(result));
		std::free(result);
	}
	malloc_fail_on(1);
	result = ft_itoa(42);
	malloc_fail_off();
	tester::expect_malloc(report, "ft_itoa malloc failure", result, false);
	std::free(result);
}

static void	test_mapping(tester::Report &report)
{
	char	*mapped;
	char	buffer[] = "libft";
	char	indexed[] = "abcde";
	char	unchanged[] = "safe";

	mapped = ft_strmapi("abcd", shift_by_index);
	tester::expect_malloc(report, "ft_strmapi", mapped, true);
	if (mapped)
		tester::expect_eq(report, "ft_strmapi text", std::string("aceg"),
			std::string(mapped));
	std::free(mapped);
	mapped = ft_strmapi("", shift_by_index);
	tester::expect_malloc(report, "ft_strmapi empty", mapped, true);
	if (mapped)
		tester::expect_eq(report, "ft_strmapi empty text", std::string(""),
			std::string(mapped));
	std::free(mapped);
	malloc_fail_on(1);
	mapped = ft_strmapi("abcd", shift_by_index);
	malloc_fail_off();
	tester::expect_malloc(report, "ft_strmapi malloc failure", mapped, false);
	std::free(mapped);
	ft_striteri(buffer, make_upper);
	tester::expect_eq(report, "ft_striteri", std::string("LIBFT"),
		std::string(buffer));
	buffer[0] = '\0';
	ft_striteri(buffer, make_upper);
	tester::expect_eq(report, "ft_striteri empty", std::string(""),
		std::string(buffer));
	ft_striteri(indexed, write_index_digit);
	tester::expect_eq(report, "ft_striteri index", std::string("01234"),
		std::string(indexed));
	ft_striteri(unchanged, NULL);
	tester::expect_eq(report, "ft_striteri null function", std::string("safe"),
		std::string(unchanged));
	ft_striteri(NULL, make_upper);
	tester::expect(report, "ft_striteri null string", true);
}

void	test_string_utils(tester::Report &report)
{
	test_substr(report);
	test_join_trim(report);
	test_split_itoa(report);
	test_mapping(report);
}
