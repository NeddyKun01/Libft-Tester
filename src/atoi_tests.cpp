/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi_tests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:29 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:30 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_modules.hpp"

void	test_atoi(tester::Report &report)
{
	tester::expect_eq(report, "ft_atoi zero", 0, ft_atoi("0"));
	tester::expect_eq(report, "ft_atoi positive", 42, ft_atoi("42"));
	tester::expect_eq(report, "ft_atoi negative", -42, ft_atoi("-42"));
	tester::expect_eq(report, "ft_atoi leading spaces", 123,
		ft_atoi("   \t\n 123"));
	tester::expect_eq(report, "ft_atoi sign and zeros", -7,
		ft_atoi("   -0007abc"));
	tester::expect_eq(report, "ft_atoi int max", INT_MAX,
		ft_atoi("2147483647"));
	tester::expect_eq(report, "ft_atoi int min", INT_MIN,
		ft_atoi("-2147483648"));
	tester::expect_eq(report, "ft_atoi plus sign", 99, ft_atoi("+99"));
	tester::expect_eq(report, "ft_atoi double sign", 0, ft_atoi("--42"));
	tester::expect_eq(report, "ft_atoi mixed signs", 0, ft_atoi("+-42"));
	tester::expect_eq(report, "ft_atoi only spaces", 0,
		ft_atoi(" \t\n\v\f\r"));
	tester::expect_eq(report, "ft_atoi stops at text", 123,
		ft_atoi("123abc456"));
	tester::expect_eq(report, "ft_atoi starts with text", 0,
		ft_atoi("abc123"));
	tester::expect_eq(report, "ft_atoi tab before sign", -12,
		ft_atoi("\t-12"));
	tester::expect_eq(report, "ft_atoi form feed whitespace", 15,
		ft_atoi("\f15"));
	tester::expect_eq(report, "ft_atoi space between sign and digit", 0,
		ft_atoi("- 42"));
	tester::expect_eq(report, "ft_atoi leading plus zeros", 7,
		ft_atoi("+0007"));
	tester::expect_eq(report, "ft_atoi negative zero", 0,
		ft_atoi("-0"));
}
