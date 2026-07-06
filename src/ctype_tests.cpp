/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctype_tests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:34 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:35 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_modules.hpp"

static bool	is_alpha_ref(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static bool	is_digit_ref(int c)
{
	return (c >= '0' && c <= '9');
}

static bool	is_alnum_ref(int c)
{
	return (is_alpha_ref(c) || is_digit_ref(c));
}

static bool	is_ascii_ref(int c)
{
	return (c >= 0 && c <= 127);
}

static bool	is_print_ref(int c)
{
	return (c >= 32 && c <= 126);
}

static int	toupper_ref(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static int	tolower_ref(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

void	test_ctype(tester::Report &report)
{
	for (int c = -128; c <= 255; ++c)
	{
		tester::expect_eq(report, "ft_isalpha", is_alpha_ref(c), ft_isalpha(c));
		tester::expect_eq(report, "ft_isdigit", is_digit_ref(c), ft_isdigit(c));
		tester::expect_eq(report, "ft_isalnum", is_alnum_ref(c), ft_isalnum(c));
		tester::expect_eq(report, "ft_isascii", is_ascii_ref(c), ft_isascii(c));
		tester::expect_eq(report, "ft_isprint", is_print_ref(c), ft_isprint(c));
		tester::expect_eq(report, "ft_toupper", toupper_ref(c), ft_toupper(c));
		tester::expect_eq(report, "ft_tolower", tolower_ref(c), ft_tolower(c));
	}
}
