/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coverage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: libft-tester <opensource@example.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 10:20:00 by libft-tester          #+#    #+#             */
/*   Updated: 2026/07/07 10:20:00 by libft-tester         ###   ########.fr       */
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
	void						print_markdown(void);
	bool						print_explain(const std::string &filter);
}

#endif
