/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profiles.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 15:10:00 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/07 15:10:00 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "profiles.hpp"

#include <iomanip>
#include <iostream>

namespace profiles
{
	std::vector<Settings>	all(void)
	{
		std::vector<Settings>	result;
		const Settings			items[] = {
			{"quick", 1, 1500, true, "fast feedback while coding"},
			{"normal", 1, 3000, false, "default balanced run"},
			{"strict", 10, 6000, false, "reproducible deeper local run"},
			{"brutal", 25, 9000, false, "heavy stress run before pushing"}
		};
		size_t					i;

		i = 0;
		while (i < sizeof(items) / sizeof(items[0]))
		{
			result.push_back(items[i]);
			i++;
		}
		return (result);
	}

	bool	find(const std::string &name, Settings &settings)
	{
		std::vector<Settings>	items;
		size_t					i;

		items = all();
		i = 0;
		while (i < items.size())
		{
			if (items[i].name == name)
			{
				settings = items[i];
				return (true);
			}
			i++;
		}
		return (false);
	}

	void	print_table(void)
	{
		std::vector<Settings>	items;
		size_t					i;

		items = all();
		std::cout << "Profile  Repeats  Timeout  Fail-fast  Description\n";
		std::cout << "------------------------------------------------------------\n";
		i = 0;
		while (i < items.size())
		{
			std::cout << std::left << std::setw(9) << items[i].name
				<< std::setw(9) << items[i].repeat_count
				<< std::setw(9) << items[i].timeout_ms
				<< std::setw(11) << (items[i].fail_fast ? "yes" : "no")
				<< items[i].description << '\n';
			i++;
		}
	}
}
