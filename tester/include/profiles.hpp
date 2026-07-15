/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profiles.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: libft-tester <opensource@example.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 15:10:00 by libft-tester          #+#    #+#             */
/*   Updated: 2026/07/07 15:10:00 by libft-tester         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROFILES_HPP
# define PROFILES_HPP

# include <string>
# include <vector>

namespace profiles
{
	struct Settings
	{
		const char	*name;
		int			repeat_count;
		int			timeout_ms;
		bool		fail_fast;
		const char	*description;
	};

	std::vector<Settings>	all(void);
	bool					find(const std::string &name, Settings &settings);
	void					print_table(void);
}

#endif
