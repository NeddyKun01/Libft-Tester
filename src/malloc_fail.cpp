/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_fail.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:44 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:45 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_fail.hpp"

#include <cstdlib>

static bool		g_fail_malloc = false;
static size_t	g_malloc_call = 0;
static size_t	g_malloc_fail_at = 0;

extern "C" void	*__real_malloc(size_t size);

extern "C" void	*__wrap_malloc(size_t size)
{
	if (g_fail_malloc)
	{
		g_malloc_call++;
		if (g_malloc_call == g_malloc_fail_at)
			return (NULL);
	}
	return (__real_malloc(size));
}

void	malloc_fail_on(size_t fail_at)
{
	g_fail_malloc = true;
	g_malloc_call = 0;
	g_malloc_fail_at = fail_at;
}

void	malloc_fail_off(void)
{
	g_fail_malloc = false;
	g_malloc_call = 0;
	g_malloc_fail_at = 0;
}
