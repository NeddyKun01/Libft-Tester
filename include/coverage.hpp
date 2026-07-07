/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coverage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 10:20:00 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/07 10:20:00 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COVERAGE_HPP
# define COVERAGE_HPP

# include <string>
# include <vector>

namespace coverage
{
	struct Entry
	{
		const char	*name;
		const char	*suite;
		const char	*group;
		const char	*cases;
		const char	*malloc_cases;
	};

	std::vector<Entry>			entries(void);
	std::vector<std::string>	function_names(void);
	void						print_table(void);
	bool						print_explain(const std::string &filter);
}

#endif
