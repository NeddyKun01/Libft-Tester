/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_tests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:50 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:51 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_modules.hpp"

void	test_output(tester::Report &report)
{
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putchar_fd('A', fd);
		});
		tester::expect_eq(report, "ft_putchar_fd", std::string("A"), captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putchar_fd('\n', fd);
		});
		tester::expect_eq(report, "ft_putchar_fd newline", std::string("\n"),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putchar_fd('0', fd);
		});
		tester::expect_eq(report, "ft_putchar_fd digit", std::string("0"),
			captured);
	}
	{
		std::vector<unsigned char>	raw = tester::capture_fd_raw([](int fd)
		{
			ft_putchar_fd('\0', fd);
		});
		tester::expect_eq(report, "ft_putchar_fd null size",
			static_cast<size_t>(1), raw.size());
		tester::expect(report, "ft_putchar_fd null byte",
			!raw.empty() && raw[0] == 0);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putstr_fd((char *)"hello", fd);
		});
		tester::expect_eq(report, "ft_putstr_fd", std::string("hello"),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putstr_fd((char *)"", fd);
		});
		tester::expect_eq(report, "ft_putstr_fd empty", std::string(""),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putstr_fd((char *)" \t\n", fd);
		});
		tester::expect_eq(report, "ft_putstr_fd whitespace",
			std::string(" \t\n"), captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putstr_fd(NULL, fd);
		});
		tester::expect_eq(report, "ft_putstr_fd null", std::string(""),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putstr_fd((char *)"42", fd);
		});
		tester::expect_eq(report, "ft_putstr_fd digits", std::string("42"),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putendl_fd((char *)"hello", fd);
		});
		tester::expect_eq(report, "ft_putendl_fd", std::string("hello\n"),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putendl_fd((char *)"", fd);
		});
		tester::expect_eq(report, "ft_putendl_fd empty", std::string("\n"),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putendl_fd((char *)"line\ninside", fd);
		});
		tester::expect_eq(report, "ft_putendl_fd keeps inner newline",
			std::string("line\ninside\n"), captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putendl_fd(NULL, fd);
		});
		tester::expect_eq(report, "ft_putendl_fd null", std::string(""),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putendl_fd((char *)"42", fd);
		});
		tester::expect_eq(report, "ft_putendl_fd digits", std::string("42\n"),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putnbr_fd(INT_MIN, fd);
		});
		tester::expect_eq(report, "ft_putnbr_fd INT_MIN",
			std::to_string(INT_MIN), captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putnbr_fd(0, fd);
		});
		tester::expect_eq(report, "ft_putnbr_fd zero", std::string("0"),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putnbr_fd(-42, fd);
		});
		tester::expect_eq(report, "ft_putnbr_fd negative", std::string("-42"),
			captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putnbr_fd(INT_MAX, fd);
		});
		tester::expect_eq(report, "ft_putnbr_fd INT_MAX",
			std::to_string(INT_MAX), captured);
	}
	{
		std::string	captured = tester::capture_fd_text([](int fd)
		{
			ft_putnbr_fd(42, fd);
		});
		tester::expect_eq(report, "ft_putnbr_fd positive", std::string("42"),
			captured);
	}
}
