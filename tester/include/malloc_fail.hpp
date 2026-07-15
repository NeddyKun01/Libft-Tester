/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_fail.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: libft-tester <opensource@example.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:54:20 by libft-tester          #+#    #+#             */
/*   Updated: 2026/07/06 22:54:21 by libft-tester         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_FAIL_HPP
# define MALLOC_FAIL_HPP

# include <cstddef>

void	malloc_fail_on(size_t fail_at);
void	malloc_fail_off(void);

#endif
