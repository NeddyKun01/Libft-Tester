/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_tests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:47 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:48 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_fail.hpp"
#include "test_modules.hpp"

void	test_memory(tester::Report &report)
{
	{
		unsigned char	buffer[16];
		unsigned char	ref[16];

		std::memset(ref, 0xAB, sizeof(ref));
		tester::expect_eq(report, "ft_memset return", (void *)buffer,
			ft_memset(buffer, 0xAB, sizeof(buffer)));
		tester::expect_mem(report, "ft_memset fills bytes", ref, buffer,
			sizeof(buffer));
		ft_bzero(buffer, sizeof(buffer));
		std::memset(ref, 0, sizeof(ref));
		tester::expect_mem(report, "ft_bzero clears bytes", ref, buffer,
			sizeof(buffer));
		std::memset(buffer, 0xAA, sizeof(buffer));
		std::memset(ref, 0xAA, sizeof(ref));
		ft_bzero(buffer + 4, 5);
		std::memset(ref + 4, 0, 5);
		tester::expect_mem(report, "ft_bzero partial clear", ref, buffer,
			sizeof(buffer));
		tester::expect_eq(report, "ft_bzero keeps byte before",
			static_cast<unsigned char>(0xAA), buffer[3]);
		tester::expect_eq(report, "ft_bzero keeps byte after",
			static_cast<unsigned char>(0xAA), buffer[9]);
		buffer[0] = 42;
		ft_bzero(buffer, 0);
		tester::expect_eq(report, "ft_bzero zero size",
			static_cast<unsigned char>(42), buffer[0]);
		buffer[1] = 99;
		ft_bzero(buffer + 1, 1);
		tester::expect_eq(report, "ft_bzero one byte",
			static_cast<unsigned char>(0), buffer[1]);
		tester::expect_eq(report, "ft_memset zero size", (void *)buffer,
			ft_memset(buffer, 7, 0));
		tester::expect_eq(report, "ft_memset zero size content",
			static_cast<unsigned char>(42), buffer[0]);
		ft_memset(buffer, 300, 3);
		std::memset(ref, 44, sizeof(ref));
		tester::expect_mem(report, "ft_memset unsigned char cast", ref,
			buffer, 3);
	}
	{
		unsigned char	source[] = {0, 1, 2, 3, 255, 4};
		unsigned char	destination[] = {9, 9, 9, 9, 9, 9};
		unsigned char	reference[] = {0, 1, 2, 3, 255, 4};

		tester::expect_eq(report, "ft_memcpy return", (void *)destination,
			ft_memcpy(destination, source, sizeof(source)));
		tester::expect_mem(report, "ft_memcpy fixed", reference, destination,
			sizeof(reference));
		tester::expect_eq(report, "ft_memcpy zero size", (void *)destination,
			ft_memcpy(destination, source, 0));
		tester::expect_mem(report, "ft_memcpy zero size unchanged", reference,
			destination, sizeof(reference));
		std::memset(destination, 9, sizeof(destination));
		std::memset(reference, 9, sizeof(reference));
		std::memcpy(reference + 2, source + 1, 3);
		tester::expect_eq(report, "ft_memcpy offset return",
			(void *)(destination + 2),
			ft_memcpy(destination + 2, source + 1, 3));
		tester::expect_mem(report, "ft_memcpy offset copy", reference,
			destination, sizeof(reference));
	}
	{
		unsigned char	forward[] = "abcdef";
		unsigned char	backward[] = "abcdef";
		unsigned char	forward_ref[] = "abcdef";
		unsigned char	backward_ref[] = "abcdef";
		unsigned char	same[] = "libft";

		tester::expect_eq(report, "ft_memmove return", (void *)(forward + 1),
			ft_memmove(forward + 1, forward, 4));
		std::memmove(forward_ref + 1, forward_ref, 4);
		tester::expect_mem(report, "ft_memmove forward overlap", forward_ref,
			forward, sizeof(forward));
		ft_memmove(backward, backward + 1, 4);
		std::memmove(backward_ref, backward_ref + 1, 4);
		tester::expect_mem(report, "ft_memmove backward overlap", backward_ref,
			backward, sizeof(backward));
		tester::expect_eq(report, "ft_memmove zero size", (void *)backward,
			ft_memmove(backward, backward + 1, 0));
		tester::expect_eq(report, "ft_memmove same pointer", (void *)same,
			ft_memmove(same, same, sizeof(same)));
		tester::expect_eq(report, "ft_memmove same pointer text",
			std::string("libft"), std::string((char *)same));
	}
	{
		unsigned char	data[] = {0, 1, 2, 3, 2, 1, 0};

		tester::expect_eq(report, "ft_memchr first match", (void *)(data + 2),
			ft_memchr(data, 2, sizeof(data)));
		tester::expect_eq(report, "ft_memchr nul byte", (void *)data,
			ft_memchr(data, 0, sizeof(data)));
		tester::expect_eq(report, "ft_memchr missing", (void *)NULL,
			ft_memchr(data, 9, sizeof(data)));
		tester::expect_eq(report, "ft_memchr zero size", (void *)NULL,
			ft_memchr(data, 0, 0));
		tester::expect_eq(report, "ft_memchr stops before later match",
			(void *)NULL, ft_memchr(data, 3, 3));
	}
	{
		unsigned char	left[] = {0, 1, 2, 255};
		unsigned char	right[] = {0, 1, 2, 0};
		unsigned char	shorter[] = {0, 1, 3, 0};

		tester::expect_eq(report, "ft_memcmp equal", 0,
			ft_memcmp(left, left, sizeof(left)));
		tester::expect_eq(report, "ft_memcmp zero size", 0,
			ft_memcmp(left, right, 0));
		tester::expect(report, "ft_memcmp unsigned diff",
			ft_memcmp(left, right, sizeof(left)) > 0);
		tester::expect(report, "ft_memcmp negative diff",
			ft_memcmp(left, shorter, 3) < 0);
		tester::expect_eq(report, "ft_memcmp equal prefix", 0,
			ft_memcmp(left, shorter, 2));
	}
	{
		void	*overflow = ft_calloc(SIZE_MAX / 2 + 1, 2);
		tester::expect_malloc(report, "ft_calloc overflow", overflow, false);
	}
	{
		void	*memory;

		malloc_fail_on(1);
		memory = ft_calloc(4, sizeof(int));
		malloc_fail_off();
		tester::expect_malloc(report, "ft_calloc malloc failure", memory, false);
		std::free(memory);
	}
	{
		int		*numbers = static_cast<int *>(ft_calloc(8, sizeof(int)));
		bool	zeroed = true;
		size_t	i;

		tester::expect_malloc(report, "ft_calloc zero fill", numbers, true);
		if (numbers)
		{
			i = 0;
			while (i < 8)
			{
				zeroed = zeroed && numbers[i] == 0;
				i++;
			}
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
