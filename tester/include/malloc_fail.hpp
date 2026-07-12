/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_fail.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:54:20 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:54:21 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_FAIL_HPP
# define MALLOC_FAIL_HPP

# include <cstddef>

void	malloc_fail_on(size_t fail_at);
void	malloc_fail_off(void);

#endif
