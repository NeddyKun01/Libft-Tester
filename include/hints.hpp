/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hints.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 15:10:00 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/07 15:10:00 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HINTS_HPP
# define HINTS_HPP

# include <string>

namespace hints
{
	std::string	for_label(const std::string &label);
	bool		print_for_function(const std::string &filter);
}

#endif
