/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:54:33 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:54:34 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTER_HPP
# define TESTER_HPP

# include "hints.hpp"

# include <algorithm>
# include <climits>
# include <cstddef>
# include <cstdint>
# include <cstdlib>
# include <cstring>
# include <functional>
# include <chrono>
# include <csignal>
# include <cctype>
# include <fcntl.h>
# include <iomanip>
# include <iostream>
# include <random>
# include <sstream>
# include <string>
# include <vector>
# include <cstdio>
# include <sys/wait.h>
# include <unistd.h>

extern "C"
{
# define new list_new
# include "libft.h"
# undef new
}

namespace tester
{
	inline const char	*const green = "\033[32m";
	inline const char	*const red = "\033[31m";
	inline const char	*const cyan = "\033[36m";
	inline const char	*const bold = "\033[1m";
	inline const char	*const dim = "\033[2m";
	inline const char	*const reset = "\033[0m";

	inline bool	use_color(void)
	{
		return (std::getenv("NO_COLOR") == NULL);
	}

	inline const char	*paint(const char *color)
	{
		if (!use_color())
			return ("");
		return (color);
	}

	struct CheckResult
	{
		std::string	status;
		std::string	label;
		std::string	details;
		bool		success = false;
	};

	struct FunctionReport
	{
		std::string					name;
		std::vector<CheckResult>	checks;
		int							passed = 0;
	};

	struct Report
	{
		int							checks = 0;
		int							failures = 0;
		int							ok_count = 0;
		int							nok_count = 0;
		int							mok_count = 0;
		int							mnok_count = 0;
		int							repeat_count = 1;
		int							timeout_ms = 3000;
		long long					duration_ms = 0;
		unsigned int				seed = 0;
		bool						fail_fast = false;
		std::string					version;
		std::string					profile;
		std::string					suite_filter;
		std::string					function_filter;
		std::vector<FunctionReport>	functions;
	};

	struct OutputOptions
	{
		bool		verbose = false;
		bool		quiet = false;
		bool		json = false;
		bool		summary_only = false;
		bool		review = false;
		std::string	filter;
	};

	typedef void	(*SuiteFn)(Report &report);

	struct Suite
	{
		std::string	name;
		SuiteFn		run;
	};

	inline void	section(const std::string &name)
	{
		(void)name;
	}

	inline void	subsection(const std::string &name)
	{
		(void)name;
	}

	inline std::string	text(const std::string &value)
	{
		return (std::string("\"") + value + "\"");
	}

	inline std::string	text(const char *value)
	{
		if (!value)
			return ("(null)");
		return (text(std::string(value)));
	}

	inline std::string	text(char *value)
	{
		return (text(static_cast<const char *>(value)));
	}

	inline std::string	text(char value)
	{
		if (value == '\0')
			return ("'\\0'");
		if (value == '\n')
			return ("'\\n'");
		if (value == '\t')
			return ("'\\t'");
		return (std::string("'") + value + "'");
	}

	inline std::string	text(bool value)
	{
		return (value ? "true" : "false");
	}

	inline std::string	text(const void *value)
	{
		std::ostringstream	stream;

		stream << value;
		return (stream.str());
	}

	template <typename T>
	inline std::string	text(const T &value)
	{
		std::ostringstream	stream;

		stream << value;
		return (stream.str());
	}

	inline std::string	hex_dump(const void *data, size_t size)
	{
		const unsigned char	*bytes;
		std::ostringstream	stream;
		size_t				i;

		bytes = static_cast<const unsigned char *>(data);
		i = 0;
		while (i < size)
		{
			stream << std::uppercase << std::setw(2) << std::setfill('0')
				<< std::hex << static_cast<int>(bytes[i]);
			if (i + 1 < size)
				stream << ' ';
			i++;
		}
		return (stream.str());
	}

	inline std::string	function_name(const std::string &label)
	{
		size_t	end;

		end = label.find(' ');
		if (end == std::string::npos)
			return (label);
		return (label.substr(0, end));
	}

	inline FunctionReport	&function_report(Report &report,
		const std::string &name)
	{
		size_t	i;

		i = 0;
		while (i < report.functions.size())
		{
			if (report.functions[i].name == name)
				return (report.functions[i]);
			i++;
		}
		report.functions.push_back(FunctionReport());
		report.functions.back().name = name;
		return (report.functions.back());
	}

	inline const char	*status_color(bool success)
	{
		if (success)
			return (paint(green));
		return (paint(red));
	}

	inline void	record_check(Report &report, const std::string &label,
		const std::string &status, bool success, const std::string &details)
	{
		FunctionReport	&function = function_report(report, function_name(label));
		CheckResult		check;

		check.status = status;
		check.label = label;
		check.details = details;
		check.success = success;
		function.checks.push_back(check);
		if (success)
			function.passed++;
	}

	inline void	count_status(Report &report, const std::string &status,
		bool success)
	{
		report.checks++;
		if (status == "OK")
			report.ok_count++;
		else if (status == "MOK")
			report.mok_count++;
		else if (status == "MNOK")
			report.mnok_count++;
		else
			report.nok_count++;
		if (!success)
			report.failures++;
	}

	inline void	add_raw_check(Report &report, const std::string &label,
		const std::string &status, bool success, const std::string &details)
	{
		count_status(report, status, success);
		record_check(report, label, status, success, details);
	}

	inline std::string	lower_copy(const std::string &value)
	{
		std::string	result;
		size_t		i;

		result = value;
		i = 0;
		while (i < result.size())
		{
			result[i] = static_cast<char>(std::tolower(
				static_cast<unsigned char>(result[i])));
			i++;
		}
		return (result);
	}

	inline bool	name_matches(const std::string &name,
		const std::string &filter)
	{
		if (filter.empty())
			return (true);
		return (lower_copy(name).find(lower_copy(filter)) != std::string::npos);
	}

	inline Report	filter_report(const Report &report,
		const std::string &filter)
	{
		Report	filtered;
		size_t	i;
		size_t	j;

		if (filter.empty())
			return (report);
		filtered.repeat_count = report.repeat_count;
		filtered.timeout_ms = report.timeout_ms;
		filtered.duration_ms = report.duration_ms;
		filtered.seed = report.seed;
		filtered.fail_fast = report.fail_fast;
		filtered.version = report.version;
		filtered.profile = report.profile;
		filtered.suite_filter = report.suite_filter;
		filtered.function_filter = filter;
		i = 0;
		while (i < report.functions.size())
		{
			if (name_matches(report.functions[i].name, filter))
			{
				j = 0;
				while (j < report.functions[i].checks.size())
				{
					const CheckResult	&check = report.functions[i].checks[j];

					add_raw_check(filtered, check.label, check.status,
						check.success, check.details);
					j++;
				}
			}
			i++;
		}
		if (filtered.functions.empty())
			add_raw_check(filtered, "filter no match", "NOK", false,
				std::string("no function matched: ") + filter);
		return (filtered);
	}

	inline void	append_report(Report &target, const Report &source)
	{
		size_t	i;
		size_t	j;

		i = 0;
		while (i < source.functions.size())
		{
			j = 0;
			while (j < source.functions[i].checks.size())
			{
				const CheckResult	&check = source.functions[i].checks[j];

				add_raw_check(target, check.label, check.status,
					check.success, check.details);
				j++;
			}
			i++;
		}
	}

	inline unsigned int	default_seed(void)
	{
		return (static_cast<unsigned int>(
			std::chrono::high_resolution_clock::now()
				.time_since_epoch().count()));
	}

	inline unsigned int	&current_seed(void)
	{
		static unsigned int	seed = default_seed();

		return (seed);
	}

	inline std::mt19937	&random_engine(void)
	{
		static std::mt19937	engine(current_seed());

		return (engine);
	}

	inline void	set_random_seed(unsigned int seed)
	{
		current_seed() = seed;
		random_engine().seed(seed);
	}

	inline bool	expect(Report &report, const std::string &label, bool condition)
	{
		report.checks++;
		if (condition)
			report.ok_count++;
		else
			report.nok_count++;
		record_check(report, label, condition ? "OK" : "NOK", condition, "");
		if (!condition)
			report.failures++;
		return (condition);
	}

	template <typename Expected, typename Actual>
	inline bool	expect_eq(Report &report, const std::string &label,
		const Expected &expected, const Actual &actual)
	{
		bool	condition;
		std::string	details;

		report.checks++;
		condition = (expected == actual);
		if (condition)
			report.ok_count++;
		else
			report.nok_count++;
		if (!condition)
		{
			report.failures++;
			details = std::string("expected: ") + text(expected)
				+ "\nactual:   " + text(actual);
		}
		record_check(report, label, condition ? "OK" : "NOK", condition,
			details);
		return (condition);
	}

	inline bool	expect_mem(Report &report, const std::string &label,
		const void *expected, const void *actual, size_t size)
	{
		bool	condition;
		std::string	details;

		report.checks++;
		condition = (std::memcmp(expected, actual, size) == 0);
		if (condition)
			report.ok_count++;
		else
			report.nok_count++;
		if (!condition)
		{
			report.failures++;
			details = std::string("expected: ") + hex_dump(expected, size)
				+ "\nactual:   " + hex_dump(actual, size);
		}
		record_check(report, label, condition ? "OK" : "NOK", condition,
			details);
		return (condition);
	}

	inline bool	expect_malloc(Report &report, const std::string &label,
		const void *ptr, bool should_exist)
	{
		bool	condition;
		bool	exists;

		report.checks++;
		exists = (ptr != NULL);
		condition = (exists == should_exist);
		if (condition)
			report.mok_count++;
		else
			report.mnok_count++;
		record_check(report, label, condition ? "MOK" : "MNOK", condition, "");
		if (!condition)
			report.failures++;
		return (condition);
	}

	inline void	print_status_token(const CheckResult &check)
	{
		std::cout << status_color(check.success) << check.status
			<< paint(reset) << " ";
	}

	inline bool	function_has_failure(const FunctionReport &function)
	{
		return (function.passed != static_cast<int>(function.checks.size()));
	}

	inline bool	is_runner_issue(const CheckResult &check)
	{
		return (check.label == "runner issue");
	}

	inline bool	is_crash_status(const std::string &status)
	{
		return (status == "SEGV" || status == "BUS" || status == "ABRT"
			|| status == "FPE" || status == "TIMEOUT");
	}

	inline bool	is_malloc_status(const std::string &status)
	{
		return (status == "MOK" || status == "MNOK");
	}

	inline bool	function_has_status(const FunctionReport &function,
		bool (*predicate)(const std::string &))
	{
		size_t	i;

		i = 0;
		while (i < function.checks.size())
		{
			if (predicate(function.checks[i].status))
				return (true);
			i++;
		}
		return (false);
	}

	inline bool	function_has_crash(const FunctionReport &function)
	{
		return (function_has_status(function, is_crash_status));
	}

	inline bool	function_has_malloc_check(const FunctionReport &function)
	{
		return (function_has_status(function, is_malloc_status));
	}

	inline bool	function_has_malloc_failure(const FunctionReport &function)
	{
		size_t	i;

		i = 0;
		while (i < function.checks.size())
		{
			if (function.checks[i].status == "MNOK")
				return (true);
			i++;
		}
		return (false);
	}

	inline bool	function_is_runner_issues(const FunctionReport &function)
	{
		size_t	i;

		if (function.checks.empty())
			return (false);
		i = 0;
		while (i < function.checks.size())
		{
			if (!is_runner_issue(function.checks[i]))
				return (false);
			i++;
		}
		return (true);
	}

	struct StatusCounts
	{
		size_t	ok = 0;
		size_t	nok = 0;
		size_t	mok = 0;
		size_t	mnok = 0;
		size_t	segv = 0;
		size_t	bus = 0;
		size_t	abrt = 0;
		size_t	fpe = 0;
		size_t	timeout = 0;
	};

	inline StatusCounts	status_counts(const FunctionReport &function)
	{
		StatusCounts	counts;
		size_t			i;

		i = 0;
		while (i < function.checks.size())
		{
			if (function.checks[i].status == "OK")
				counts.ok++;
			else if (function.checks[i].status == "NOK")
				counts.nok++;
			else if (function.checks[i].status == "MOK")
				counts.mok++;
			else if (function.checks[i].status == "MNOK")
				counts.mnok++;
			else if (function.checks[i].status == "SEGV")
				counts.segv++;
			else if (function.checks[i].status == "BUS")
				counts.bus++;
			else if (function.checks[i].status == "ABRT")
				counts.abrt++;
			else if (function.checks[i].status == "FPE")
				counts.fpe++;
			else if (function.checks[i].status == "TIMEOUT")
				counts.timeout++;
			i++;
		}
		return (counts);
	}

	inline void	print_status_count(const std::string &status, size_t count,
		bool success)
	{
		if (count == 0)
			return ;
		std::cout << status_color(success) << status << "x" << count
			<< paint(reset) << " ";
	}

	inline void	print_aggregated_statuses(const FunctionReport &function)
	{
		StatusCounts	counts;

		counts = status_counts(function);
		print_status_count("OK", counts.ok, true);
		print_status_count("NOK", counts.nok, false);
		print_status_count("MOK", counts.mok, true);
		print_status_count("MNOK", counts.mnok, false);
		print_status_count("SEGV", counts.segv, false);
		print_status_count("BUS", counts.bus, false);
		print_status_count("ABRT", counts.abrt, false);
		print_status_count("FPE", counts.fpe, false);
		print_status_count("TIMEOUT", counts.timeout, false);
	}

	inline void	print_banner(void)
	{
		std::cout << paint(cyan)
			<< "\n+------------------------------------------------------------+\n"
			<< "|                        LIBFT TESTER                        |\n"
			<< "+------------------------------------------------------------+\n"
			<< paint(reset);
		std::cout << paint(dim)
			<< "runner: fork-isolated suites | malloc-fail: enabled"
			<< " | leaks: make leaks\n"
			<< "legend: OK pass | MOK malloc expectation pass"
			<< " | NOK/MNOK fail | SEGV/BUS/ABRT/FPE/TIMEOUT crash\n"
			<< paint(reset);
	}

	inline std::string	progress_bar(const FunctionReport &function)
	{
		size_t	total;
		size_t	filled;
		size_t	i;
		std::string	bar;

		total = function.checks.size();
		if (total == 0)
			return ("[----------]");
		filled = static_cast<size_t>((function.passed * 10) / total);
		bar = "[";
		i = 0;
		while (i < 10)
		{
			if (i < filled)
				bar += "#";
			else
				bar += "-";
			i++;
		}
		bar += "]";
		return (bar);
	}

	inline void	print_score(const FunctionReport &function)
	{
		std::ostringstream	score;

		score << function.passed << "/" << function.checks.size();
		if (function_has_failure(function))
			std::cout << paint(red);
		else
			std::cout << paint(green);
		std::cout << std::right << std::setw(9) << score.str()
			<< paint(reset);
	}

	inline void	print_detailed_statuses(const FunctionReport &function)
	{
		size_t	j;

		j = 0;
		while (j < function.checks.size())
		{
			print_status_token(function.checks[j]);
			j++;
		}
	}

	inline void	print_function_statuses(const FunctionReport &function,
		const OutputOptions &options)
	{
		if (!options.verbose && function.checks.size() > 12)
			print_aggregated_statuses(function);
		else
			print_detailed_statuses(function);
	}

	inline void	print_function_results(const Report &report,
		const OutputOptions &options)
	{
		size_t	i;

		std::cout << "\n" << paint(bold) << "Results" << paint(reset) << "\n";
		std::cout << paint(dim)
			<< "Function             OK/Total   Progress     Status\n"
			<< "------------------------------------------------------------\n"
			<< paint(reset);
		i = 0;
		while (i < report.functions.size())
		{
			const FunctionReport	&function = report.functions[i];

			if (function_is_runner_issues(function))
			{
				i++;
				continue ;
			}
			std::cout << paint(cyan) << std::left << std::setw(20)
				<< function.name << paint(reset);
			print_score(function);
			std::cout << "  " << progress_bar(function) << "  ";
			print_function_statuses(function, options);
			if (function_has_failure(function))
				std::cout << paint(dim) << "details below" << paint(reset);
			std::cout << '\n';
			i++;
		}
	}

	inline void	print_runner_issues(const Report &report)
	{
		size_t	i;
		size_t	j;
		bool	printed = false;

		i = 0;
		while (i < report.functions.size())
		{
			j = 0;
			while (j < report.functions[i].checks.size())
			{
				const CheckResult	&check = report.functions[i].checks[j];

				if (is_runner_issue(check) && !check.success)
				{
					if (!printed)
					{
						std::cout << "\n" << paint(bold) << "Runner Issues"
							<< paint(reset) << "\n";
						std::cout << paint(dim)
							<< "These are tester/suite crashes, not Libft "
							<< "function names.\n" << paint(reset);
						printed = true;
					}
					std::cout << paint(red) << check.status << paint(reset)
						<< " " << check.details << '\n';
				}
				j++;
			}
			i++;
		}
	}

	inline void	print_indented_block(const std::string &text,
		const std::string &prefix)
	{
		std::istringstream	input(text);
		std::string			line;

		while (std::getline(input, line))
			std::cout << prefix << line << '\n';
	}

	inline void	print_failure_details(const Report &report)
	{
		size_t	i;
		size_t	j;
		bool	printed = false;

		if (report.failures == 0)
			return ;
		i = 0;
		while (i < report.functions.size())
		{
			j = 0;
			while (j < report.functions[i].checks.size())
			{
				const CheckResult	&check = report.functions[i].checks[j];

				if (!check.success && !is_runner_issue(check))
				{
					if (!printed)
					{
						std::cout << "\n" << paint(bold) << "Failure Details"
							<< paint(reset) << "\n";
						printed = true;
					}
					std::cout << paint(red) << check.status << paint(reset) << " "
						<< check.label << '\n';
					if (!check.details.empty())
						print_indented_block(check.details, "  ");
					std::string	hint = hints::for_label(check.label);
					if (!hint.empty())
						std::cout << paint(dim) << "  hint: " << hint
							<< paint(reset) << '\n';
				}
				j++;
			}
			i++;
		}
	}

	inline std::vector<std::string>	failed_functions(const Report &report)
	{
		std::vector<std::string>	names;
		size_t					i;

		i = 0;
		while (i < report.functions.size())
		{
			if (function_has_failure(report.functions[i])
				&& !function_is_runner_issues(report.functions[i]))
				names.push_back(report.functions[i].name);
			i++;
		}
		return (names);
	}

	inline void	print_name_list(const std::vector<std::string> &names)
	{
		size_t	i;

		i = 0;
		while (i < names.size())
		{
			if (i > 0)
				std::cout << ", ";
			std::cout << names[i];
			i++;
		}
	}

	inline std::string	shell_quote(const std::string &value)
	{
		std::string	quoted;
		size_t		i;

		quoted = "'";
		i = 0;
		while (i < value.size())
		{
			if (value[i] == '\'')
				quoted += "'\\''";
			else
				quoted += value[i];
			i++;
		}
		quoted += "'";
		return (quoted);
	}

	inline std::string	root_cli_arg(void)
	{
		const char	*root;

		root = std::getenv("LIBFT_TESTER_ROOT");
		if (!root || root[0] == '\0')
			return ("");
		return (std::string(" --root ") + shell_quote(root));
	}

	inline std::string	seed_cli_arg(unsigned int seed)
	{
		std::ostringstream	out;

		if (seed == 0)
			return ("");
		out << " --seed " << seed;
		return (out.str());
	}

	inline void	print_focus_commands(const std::vector<std::string> &names,
		const std::string &root_arg, const std::string &seed_arg)
	{
		size_t	limit;
		size_t	i;

		limit = std::min<size_t>(names.size(), 3);
		i = 0;
		while (i < limit)
		{
			std::cout << "    ./libft_tester" << root_arg << " --only "
				<< names[i] << " --verbose" << seed_arg << "\n";
			i++;
		}
		if (names.size() > limit)
			std::cout << paint(dim) << "    ... and "
				<< (names.size() - limit) << " more failed function(s)\n"
				<< paint(reset);
	}

	inline void	print_debug_next_steps(const Report &report)
	{
		std::vector<std::string>	names;
		std::string				root_arg;
		std::string				seed_arg;

		names = failed_functions(report);
		if (names.empty())
			return ;
		root_arg = root_cli_arg();
		seed_arg = seed_cli_arg(report.seed);
		std::cout << "\n" << paint(bold) << "Debug Focus"
			<< paint(reset) << "\n";
		std::cout << "  failed functions: " << paint(red);
		print_name_list(names);
		std::cout << paint(reset) << "\n";
		std::cout << "  try next:\n";
		print_focus_commands(names, root_arg, seed_arg);
		std::cout << "    ./libft_tester --hint " << names[0] << "\n";
		if (root_arg.empty())
			std::cout << paint(dim)
				<< "  note: add --root PATH if your Libft is not in ../libft.\n"
				<< paint(reset);
		if (names.size() > 1)
			std::cout << paint(dim)
				<< "  tip: fix one function at a time, then rerun the same "
				<< "seed.\n" << paint(reset);
	}

	inline int	count_runner_issues(const Report &report)
	{
		size_t	i;
		size_t	j;
		int		count = 0;

		i = 0;
		while (i < report.functions.size())
		{
			j = 0;
			while (j < report.functions[i].checks.size())
			{
				if (is_runner_issue(report.functions[i].checks[j])
					&& !report.functions[i].checks[j].success)
					count++;
				j++;
			}
			i++;
		}
		return (count);
	}

	inline int	count_functions_matching(const Report &report,
		bool (*predicate)(const FunctionReport &))
	{
		int		count;
		size_t	i;

		count = 0;
		i = 0;
		while (i < report.functions.size())
		{
			if (!function_is_runner_issues(report.functions[i])
				&& predicate(report.functions[i]))
				count++;
			i++;
		}
		return (count);
	}

	inline bool	function_passed(const FunctionReport &function)
	{
		return (!function_has_failure(function));
	}

	inline std::vector<std::string>	functions_matching(const Report &report,
		bool (*predicate)(const FunctionReport &))
	{
		std::vector<std::string>	names;
		size_t					i;

		i = 0;
		while (i < report.functions.size())
		{
			if (!function_is_runner_issues(report.functions[i])
				&& predicate(report.functions[i]))
				names.push_back(report.functions[i].name);
			i++;
		}
		return (names);
	}

	inline void	print_review_name_line(const std::string &title,
		const std::vector<std::string> &names)
	{
		std::cout << "  " << title << ": ";
		if (names.empty())
			std::cout << paint(green) << "none" << paint(reset);
		else
		{
			std::cout << paint(red);
			print_name_list(names);
			std::cout << paint(reset);
		}
		std::cout << "\n";
	}

	inline void	print_review_report(const Report &report)
	{
		int							passed;
		int							percent;
		std::vector<std::string>	failed;
		std::vector<std::string>	crashes;
		std::vector<std::string>	malloc_failures;
		std::string					root_arg;
		std::string					seed_arg;

		passed = report.checks - report.failures;
		percent = 100;
		if (report.checks > 0)
			percent = (passed * 100) / report.checks;
		failed = failed_functions(report);
		crashes = functions_matching(report, function_has_crash);
		malloc_failures = functions_matching(report, function_has_malloc_failure);
		root_arg = root_cli_arg();
		seed_arg = seed_cli_arg(report.seed);
		std::cout << paint(bold) << "Libft Tester Review" << paint(reset) << "\n";
		std::cout << "  verdict: "
			<< (report.failures == 0 ? paint(green) : paint(red))
			<< (report.failures == 0 ? "PASS" : "FAIL") << paint(reset)
			<< "\n";
		std::cout << "  score: " << passed << "/" << report.checks
			<< " (" << percent << "%)"
			<< " | profile: " << report.profile
			<< " | seed: " << report.seed << "\n";
		std::cout << "  status: OKx" << report.ok_count
			<< " MOKx" << report.mok_count
			<< " NOKx" << report.nok_count
			<< " MNOKx" << report.mnok_count << "\n";
		print_review_name_line("failed functions", failed);
		print_review_name_line("crash functions", crashes);
		print_review_name_line("malloc failures", malloc_failures);
		if (!failed.empty())
		{
			std::cout << "\nReproduce first failures:\n";
			print_focus_commands(failed, root_arg, seed_arg);
			std::cout << "    ./libft_tester --hint " << failed[0] << "\n";
		}
	}

	inline std::string	json_escape(const std::string &value)
	{
		std::string	result;
		size_t		i;

		result.reserve(value.size());
		i = 0;
		while (i < value.size())
		{
			if (value[i] == '\\')
				result += "\\\\";
			else if (value[i] == '"')
				result += "\\\"";
			else if (value[i] == '\n')
				result += "\\n";
			else if (value[i] == '\t')
				result += "\\t";
			else
				result += value[i];
			i++;
		}
		return (result);
	}

	inline std::string	html_escape(const std::string &value)
	{
		std::string	result;
		size_t		i;

		result.reserve(value.size());
		i = 0;
		while (i < value.size())
		{
			if (value[i] == '&')
				result += "&amp;";
			else if (value[i] == '<')
				result += "&lt;";
			else if (value[i] == '>')
				result += "&gt;";
			else if (value[i] == '"')
				result += "&quot;";
			else
				result += value[i];
			i++;
		}
		return (result);
	}

	inline void	print_json_counts(const StatusCounts &counts)
	{
		std::cout << "\"ok\":" << counts.ok
			<< ",\"nok\":" << counts.nok
			<< ",\"mok\":" << counts.mok
			<< ",\"mnok\":" << counts.mnok
			<< ",\"segv\":" << counts.segv
			<< ",\"bus\":" << counts.bus
			<< ",\"abrt\":" << counts.abrt
			<< ",\"fpe\":" << counts.fpe
			<< ",\"timeout\":" << counts.timeout;
	}

	inline void	print_json_check(const CheckResult &check)
	{
		std::cout << "{\"status\":\"" << json_escape(check.status)
			<< "\",\"success\":" << (check.success ? "true" : "false")
			<< ",\"label\":\"" << json_escape(check.label)
			<< "\",\"details\":\"" << json_escape(check.details) << "\"}";
	}

	inline void	print_json_report(const Report &report)
	{
		size_t	i;

		std::cout << "{";
		std::cout << "\"version\":\"" << json_escape(report.version)
			<< "\",\"profile\":\"" << json_escape(report.profile)
			<< "\",\"seed\":" << report.seed
			<< ",\"repeats\":" << report.repeat_count
			<< ",\"timeout_ms\":" << report.timeout_ms
			<< ",\"duration_ms\":" << report.duration_ms
			<< ",\"suite_filter\":\"" << json_escape(report.suite_filter)
			<< "\",\"function_filter\":\""
			<< json_escape(report.function_filter)
			<< "\",\"fail_fast\":" << (report.fail_fast ? "true" : "false")
			<< ",\"checks\":" << report.checks
			<< ",\"failures\":" << report.failures
			<< ",\"ok\":" << report.ok_count
			<< ",\"nok\":" << report.nok_count
			<< ",\"mok\":" << report.mok_count
			<< ",\"mnok\":" << report.mnok_count
			<< ",\"passed\":" << (report.failures == 0 ? "true" : "false")
			<< ",\"functions\":[";
		i = 0;
		while (i < report.functions.size())
		{
			const FunctionReport	&function = report.functions[i];
			StatusCounts			counts = status_counts(function);

			if (i > 0)
				std::cout << ",";
			std::cout << "{\"name\":\"" << json_escape(function.name)
				<< "\",\"passed\":" << function.passed
				<< ",\"total\":" << function.checks.size()
				<< ",\"counts\":{";
			print_json_counts(counts);
			std::cout << "},\"checks\":[";
			size_t	j = 0;
			while (j < function.checks.size())
			{
				if (j > 0)
					std::cout << ",";
				print_json_check(function.checks[j]);
				j++;
			}
			std::cout << "]}";
			i++;
		}
		std::cout << "]}\n";
	}

	inline void	print_html_status(const CheckResult &check)
	{
		std::cout << "<span class=\"status "
			<< (check.success ? "pass" : "fail") << "\">"
			<< html_escape(check.status) << "</span>";
	}

	inline int	pass_percent(int passed, size_t total)
	{
		if (total == 0)
			return (100);
		return ((passed * 100) / static_cast<int>(total));
	}

	inline void	print_html_progress(int percent)
	{
		std::cout << "<div class=\"bar\"><span style=\"width:"
			<< percent << "%\"></span></div>";
	}

	inline std::string	html_id(const std::string &value)
	{
		std::string	id;
		size_t		i;

		id = "fn-";
		i = 0;
		while (i < value.size())
		{
			if (std::isalnum(static_cast<unsigned char>(value[i])))
				id += static_cast<char>(std::tolower(
					static_cast<unsigned char>(value[i])));
			else
				id += "-";
			i++;
		}
		return (id);
	}

	inline void	print_html_copy_block(const std::string &content)
	{
		std::cout << "<button class=\"copy\" onclick=\"copyCommand(this)\">";
		std::cout << "Copy commands</button><pre class=\"commands\">";
		std::cout << html_escape(content);
		std::cout << "</pre>";
	}

	inline std::string	html_rerun_command(const std::string &name,
		unsigned int seed, bool verbose)
	{
		std::string	command;

		command = "./libft_tester" + root_cli_arg() + " --only " + name;
		if (verbose)
			command += " --verbose";
		command += seed_cli_arg(seed);
		return (command);
	}

	inline void	print_html_debug_focus(const Report &report)
	{
		std::vector<std::string>	names;
		std::string				root_arg;
		std::string				seed_arg;
		std::string				commands;
		size_t					limit;
		size_t					i;

		names = failed_functions(report);
		if (names.empty())
			return ;
		root_arg = root_cli_arg();
		seed_arg = seed_cli_arg(report.seed);
		limit = std::min<size_t>(names.size(), 3);
		std::cout << "<section class=\"panel\" id=\"debug-focus\"><h2>Debug Focus</h2>";
		std::cout << "<p class=\"muted\">Fix one function at a time, then "
			<< "rerun the same seed.</p>";
		i = 0;
		while (i < limit)
		{
			commands += "./libft_tester" + root_arg + " --only "
				+ names[i] + " --verbose" + seed_arg + "\n";
			i++;
		}
		if (names.size() > limit)
			commands += "... and "
				+ std::to_string(names.size() - limit)
				+ " more failed function(s)\n";
		commands += "./libft_tester --hint " + names[0];
		print_html_copy_block(commands);
		std::cout << "</section>";
	}

	inline void	print_html_failed_summary(const Report &report)
	{
		std::vector<std::string>	names;
		size_t					i;

		names = failed_functions(report);
		if (names.empty())
			return ;
		std::cout << "<section class=\"panel\" id=\"failures\">";
		std::cout << "<h2>Failed Functions</h2><div class=\"failed-list\">";
		i = 0;
		while (i < names.size())
		{
			std::cout << "<a href=\"#" << html_escape(html_id(names[i]))
				<< "\">" << html_escape(names[i]) << "</a>";
			i++;
		}
		std::cout << "</div></section>";
	}

	inline void	print_html_likely_fixes(const Report &report)
	{
		std::vector<std::string>	names;
		size_t					i;

		names = failed_functions(report);
		if (names.empty())
			return ;
		std::cout << "<section class=\"panel\" id=\"likely-fixes\">";
		std::cout << "<h2>Likely Fixes</h2>";
		std::cout << "<p class=\"muted\">Start with the first failing function. ";
		std::cout << "Each hint is intentionally short so you can inspect your ";
		std::cout << "own implementation.</p>";
		i = 0;
		while (i < names.size())
		{
			std::string	hint = hints::for_label(names[i]);

			std::cout << "<div class=\"fix\"><strong>"
				<< html_escape(names[i]) << "</strong>";
			if (!hint.empty())
				std::cout << "<p>" << html_escape(hint) << "</p>";
			print_html_copy_block(html_rerun_command(names[i], report.seed, true)
				+ "\n./libft_tester --hint " + names[i]);
			std::cout << "</div>";
			i++;
		}
		std::cout << "</section>";
	}

	inline void	print_html_score_guide(void)
	{
		std::cout << "<section class=\"panel\" id=\"score-guide\">";
		std::cout << "<h2>Score Guide</h2>";
		std::cout << "<p class=\"muted\">Every X/Y score means passed/total. ";
		std::cout << "Status pills such as OKx5, MOKx4, and MNOKx1 are ";
		std::cout << "counters, not ratios.</p></section>";
	}

	inline std::string	html_function_tags(const FunctionReport &function)
	{
		std::string	tags;

		tags = function_has_failure(function) ? "failed" : "passed";
		if (function_has_malloc_check(function))
			tags += " malloc";
		if (function_has_crash(function))
			tags += " crash";
		return (tags);
	}

	inline void	print_html_filter_button(const std::string &mode,
		const std::string &label, int count)
	{
		std::cout << "<button class=\"filter\" onclick=\"filterReport('"
			<< html_escape(mode) << "')\">" << html_escape(label)
			<< " <span>" << count << "</span></button>";
	}

	inline void	print_html_function_table(const Report &report)
	{
		size_t	i;

		std::cout << "<section class=\"panel\"><h2>Function Overview</h2>";
		std::cout << "<table><thead><tr><th>Function</th><th>OK/Total</th>";
		std::cout << "<th>Progress</th><th>Status counts</th></tr></thead><tbody>";
		i = 0;
		while (i < report.functions.size())
		{
			const FunctionReport	&function = report.functions[i];
			StatusCounts			counts = status_counts(function);
			int						percent = pass_percent(function.passed,
										function.checks.size());

			std::cout << "<tr data-result=\""
				<< (function_has_failure(function) ? "failed" : "passed")
				<< "\" data-tags=\"" << html_escape(html_function_tags(function))
				<< "\"><td><a href=\"#"
				<< html_escape(html_id(function.name)) << "\"><strong>"
				<< html_escape(function.name)
				<< "</strong></a></td><td class=\""
				<< (function_has_failure(function) ? "fail" : "pass") << "\">"
				<< function.passed << "/" << function.checks.size() << "</td><td>";
			print_html_progress(percent);
			std::cout << "</td><td>";
			if (counts.ok > 0)
				std::cout << "<span class=\"pill pass\">OKx" << counts.ok
					<< "</span>";
			if (counts.mok > 0)
				std::cout << "<span class=\"pill pass\">MOKx" << counts.mok
					<< "</span>";
			if (counts.nok > 0)
				std::cout << "<span class=\"pill fail\">NOKx" << counts.nok
					<< "</span>";
			if (counts.mnok > 0)
				std::cout << "<span class=\"pill fail\">MNOKx" << counts.mnok
					<< "</span>";
			if (counts.segv > 0)
				std::cout << "<span class=\"pill fail\">SEGVx" << counts.segv
					<< "</span>";
			if (counts.timeout > 0)
				std::cout << "<span class=\"pill fail\">TIMEOUTx"
					<< counts.timeout << "</span>";
			std::cout << "</td></tr>";
			i++;
		}
		std::cout << "</tbody></table></section>";
	}

	inline void	print_html_report(const Report &report)
	{
		size_t	i;
		int		passed;
		int		percent;
		int		passed_functions;
		int		failed_functions_count;
		int		malloc_functions;
		int		crash_functions;

		passed = report.checks - report.failures;
		percent = 100;
		if (report.checks > 0)
			percent = (passed * 100) / report.checks;
		passed_functions = count_functions_matching(report, function_passed);
		failed_functions_count = count_functions_matching(report, function_has_failure);
		malloc_functions = count_functions_matching(report, function_has_malloc_check);
		crash_functions = count_functions_matching(report, function_has_crash);
		std::cout << "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\">";
		std::cout << "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">";
		std::cout << "<title>Libft Tester Report</title><style>";
		std::cout << ":root{--bg:#0f1411;--card:#18211d;--panel:#101713;";
		std::cout << "--text:#f3f4e8;--muted:#aab5a7;--ok:#67d68f;";
		std::cout << "--bad:#ff6b5f;--line:#2c3a32;--gold:#e2c044;}";
		std::cout << "body{margin:0;background:radial-gradient(circle at 10% 0%,";
		std::cout << "#254734,#0f1411 44%,#0b0f0d);color:var(--text);";
		std::cout << "font-family:ui-monospace,SFMono-Regular,Menlo,monospace;}";
		std::cout << "main{max-width:1220px;margin:0 auto;padding:34px 18px 48px;}";
		std::cout << ".hero,.panel,.card{border:1px solid var(--line);";
		std::cout << "background:linear-gradient(145deg,rgba(24,33,29,.96),";
		std::cout << "rgba(16,23,19,.92));box-shadow:0 24px 80px #0009;}";
		std::cout << ".hero{border-radius:26px;padding:28px;position:relative;overflow:hidden;}";
		std::cout << ".hero:after{content:\"\";position:absolute;right:-80px;top:-90px;";
		std::cout << "width:240px;height:240px;border-radius:50%;background:#e2c04422;}";
		std::cout << "h1{margin:0 0 8px;font-size:38px;letter-spacing:-1px}";
		std::cout << "h2{margin:0 0 14px;font-size:20px}.muted{color:var(--muted)}";
		std::cout << ".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(145px,1fr));";
		std::cout << "gap:12px;margin:22px 0}.metric,.panel{background:var(--panel);";
		std::cout << "border:1px solid var(--line);border-radius:18px;padding:16px}";
		std::cout << ".metric strong{display:block;font-size:26px}.pass{color:var(--ok)}";
		std::cout << ".fail{color:var(--bad)}.bar{height:9px;background:#0a0f0c;";
		std::cout << "border-radius:999px;overflow:hidden;border:1px solid var(--line)}";
		std::cout << ".bar span{display:block;height:100%;background:linear-gradient(90deg,";
		std::cout << "var(--ok),var(--gold));}.panel{margin-top:16px}";
		std::cout << "table{width:100%;border-collapse:collapse}th,td{padding:10px;";
		std::cout << "border-bottom:1px solid var(--line);text-align:left}th{color:var(--muted)}";
		std::cout << "a{color:var(--gold);text-decoration:none}a:hover{text-decoration:underline}";
		std::cout << ".pill,.status{display:inline-block;margin:3px 5px 3px 0;";
		std::cout << "font-weight:700}.pill{border:1px solid currentColor;border-radius:999px;";
		std::cout << "padding:2px 8px}.card{margin-top:16px;border-radius:18px;overflow:hidden}";
		std::cout << ".head{display:flex;justify-content:space-between;gap:12px;";
		std::cout << "padding:14px 16px;border-bottom:1px solid var(--line);cursor:pointer}";
		std::cout << ".checks{padding:14px 16px}details{margin-top:10px}";
		std::cout << ".failure{margin:10px 0;padding:12px;border-left:3px solid var(--bad);";
		std::cout << "background:#0a0f0c}.hint{color:var(--muted)}pre.commands,pre{";
		std::cout << "white-space:pre-wrap;background:#0a0f0c;border:1px solid var(--line);";
		std::cout << "border-radius:14px;padding:12px;overflow:auto}.copy,.jump{";
		std::cout << "display:inline-block;margin:4px 0 10px;padding:8px 12px;";
		std::cout << "border-radius:999px;border:1px solid var(--gold);";
		std::cout << "background:#e2c04418;color:var(--gold);font-weight:700}";
		std::cout << ".actions{display:flex;flex-wrap:wrap;gap:8px;margin-top:12px}";
		std::cout << ".filter{display:inline-block;padding:8px 12px;border-radius:999px;";
		std::cout << "border:1px solid var(--line);background:#0a0f0c;color:var(--text);";
		std::cout << "font-weight:700;cursor:pointer}.filter:hover{border-color:var(--gold)}";
		std::cout << ".filter span{color:var(--muted)}.fix{margin-top:12px;";
		std::cout << "padding:12px;border:1px solid var(--line);border-radius:14px;";
		std::cout << "background:#0a0f0c}.fix p{margin:8px 0;color:var(--muted)}";
		std::cout << ".failed-list{display:flex;flex-wrap:wrap;gap:8px}.failed-list a{";
		std::cout << "border:1px solid var(--bad);border-radius:999px;padding:6px 10px;";
		std::cout << "color:var(--bad)}summary{list-style:none}summary::-webkit-details-marker{display:none}";
		std::cout << ".card:not([open]) .head{border-bottom:0}";
		std::cout << "</style><script>function copyCommand(b){var p=b.nextElementSibling;";
		std::cout << "navigator.clipboard&&navigator.clipboard.writeText(p.innerText);";
		std::cout << "b.innerText='Copied';setTimeout(function(){b.innerText='Copy commands'},1200);}";
		std::cout << "function filterReport(m){document.querySelectorAll('[data-tags]').";
		std::cout << "forEach(function(e){var t=(e.dataset.tags||'').split(' ');";
		std::cout << "e.style.display=(m==='all'||t.indexOf(m)!==-1)";
		std::cout << "?'':'none';});}";
		std::cout << "</script>";
		std::cout << "</head><body><main>";
		std::cout << "<section class=\"hero\"><h1>Libft Tester Report</h1>";
		std::cout << "<p class=\"muted\">version " << html_escape(report.version)
			<< " | profile " << html_escape(report.profile)
			<< " | seed " << report.seed
			<< " | repeats " << report.repeat_count
			<< " | duration " << report.duration_ms << "ms</p>";
		std::cout << "<p class=\"" << (report.failures == 0 ? "pass" : "fail")
			<< "\"><strong>" << (report.failures == 0 ? "PASS" : "FAIL")
			<< "</strong> - " << percent << "% pass rate</p>";
		std::cout << "<div class=\"actions\">";
		if (report.failures > 0)
			std::cout << "<a class=\"jump\" href=\"#failures\">Jump to failures</a>";
		print_html_filter_button("all", "All", passed_functions + failed_functions_count);
		print_html_filter_button("failed", "Failed", failed_functions_count);
		print_html_filter_button("passed", "Passed", passed_functions);
		print_html_filter_button("malloc", "Malloc", malloc_functions);
		print_html_filter_button("crash", "Crash", crash_functions);
		std::cout << "</div>";
		std::cout << "<div class=\"grid\">";
		std::cout << "<div class=\"metric\"><span>Verdict</span><strong class=\""
			<< (report.failures == 0 ? "pass\">PASS" : "fail\">FAIL")
			<< "</strong></div>";
		std::cout << "<div class=\"metric\"><span>Checks</span><strong>"
			<< report.checks << "</strong></div>";
		std::cout << "<div class=\"metric\"><span>Passed</span><strong class=\"pass\">"
			<< passed << "/" << report.checks << "</strong></div>";
		std::cout << "<div class=\"metric\"><span>Failed</span><strong class=\""
			<< (report.failures == 0 ? "pass" : "fail") << "\">"
			<< report.failures << "/" << report.checks << "</strong></div>";
		std::cout << "</div></section>";
		print_html_score_guide();
		print_html_debug_focus(report);
		print_html_failed_summary(report);
		print_html_likely_fixes(report);
		print_html_function_table(report);
		i = 0;
		while (i < report.functions.size())
		{
			const FunctionReport	&function = report.functions[i];
			size_t					j = 0;
			bool					open_card;

			open_card = (report.failures == 0 || function_has_failure(function));
			std::cout << "<details class=\"card\" data-result=\""
				<< (function_has_failure(function) ? "failed" : "passed")
				<< "\" data-tags=\"" << html_escape(html_function_tags(function))
				<< "\" id=\""
				<< html_escape(html_id(function.name)) << "\"";
			if (open_card)
				std::cout << " open";
			std::cout << "><summary class=\"head\"><strong>"
				<< html_escape(function.name) << "</strong><span class=\""
				<< (function_has_failure(function) ? "fail" : "pass") << "\">"
				<< function.passed << "/" << function.checks.size()
				<< "</span></summary><div class=\"checks\">";
			while (j < function.checks.size())
			{
				print_html_status(function.checks[j]);
				j++;
			}
			if (function_has_failure(function))
			{
				print_html_copy_block(html_rerun_command(function.name,
					report.seed, true));
				std::cout << "<details open><summary>Failure details</summary>";
				j = 0;
				while (j < function.checks.size())
				{
					const CheckResult	&check = function.checks[j];

					if (!check.success)
					{
						std::string	hint = hints::for_label(check.label);

						std::cout << "<div class=\"failure\"><strong>"
							<< html_escape(check.status) << " "
							<< html_escape(check.label) << "</strong>";
						if (!check.details.empty())
							std::cout << "<pre>" << html_escape(check.details)
								<< "</pre>";
						if (!hint.empty())
							std::cout << "<p class=\"hint\">hint: "
								<< html_escape(hint) << "</p>";
						std::cout << "</div>";
					}
					j++;
				}
				std::cout << "</details>";
			}
			std::cout << "</div></details>";
			i++;
		}
		std::cout << "</main></body></html>\n";
	}

	inline void	print_summary(const Report &report,
		const OutputOptions &options = OutputOptions())
	{
		int	passed;
		int	percent;
		int	runner_issues;

		passed = report.checks - report.failures;
		runner_issues = count_runner_issues(report);
		percent = 100;
		if (report.checks > 0)
			percent = (passed * 100) / report.checks;
		if (!options.quiet && !options.summary_only)
		{
			print_banner();
			if (report.seed != 0)
				std::cout << paint(dim) << "seed: " << report.seed
					<< " | repeats: " << report.repeat_count
					<< " | profile: " << report.profile
					<< " | duration: " << report.duration_ms << "ms"
					<< paint(reset) << "\n";
			if (!options.filter.empty())
				std::cout << paint(dim) << "filter: " << options.filter
					<< paint(reset) << "\n";
		}
		if (!options.quiet && !options.summary_only)
			print_function_results(report, options);
		if (!options.summary_only)
			print_runner_issues(report);
		if (!options.summary_only)
			print_failure_details(report);
		if (!options.summary_only && report.failures > 0)
			print_debug_next_steps(report);
		std::cout << "\n" << paint(bold) << "Summary" << paint(reset) << "\n  ";
		if (options.summary_only && report.seed != 0)
			std::cout << "seed: " << report.seed
				<< " | repeats: " << report.repeat_count
				<< " | profile: " << report.profile
				<< " | duration: " << report.duration_ms << "ms\n  ";
		print_status_count("OK", report.ok_count, true);
		print_status_count("MOK", report.mok_count, true);
		print_status_count("NOK", report.nok_count, false);
		print_status_count("MNOK", report.mnok_count, false);
		if (runner_issues > 0)
			std::cout << "\n  function failures: "
				<< (report.failures - runner_issues)
				<< " | runner issues: " << runner_issues;
		std::cout << "\n  checks: " << report.checks
			<< " | failures: ";
		if (report.failures == 0)
			std::cout << paint(green) << report.failures << paint(reset)
				<< " | verdict: " << paint(green) << "PASS"
				<< paint(reset) << '\n';
		else
			std::cout << paint(red) << report.failures << paint(reset)
				<< " | verdict: " << paint(red) << "FAIL"
				<< paint(reset) << " | see details above\n";
		std::cout << "  pass rate: " << percent << "%\n";
	}

	inline std::string	escape_field(const std::string &value)
	{
		std::string	result;
		size_t		i;

		result.reserve(value.size());
		i = 0;
		while (i < value.size())
		{
			if (value[i] == '\\')
				result += "\\\\";
			else if (value[i] == '\n')
				result += "\\n";
			else if (value[i] == '\t')
				result += "\\t";
			else if (value[i] == '|')
				result += "\\p";
			else
				result += value[i];
			i++;
		}
		return (result);
	}

	inline std::string	unescape_field(const std::string &value)
	{
		std::string	result;
		size_t		i;

		result.reserve(value.size());
		i = 0;
		while (i < value.size())
		{
			if (value[i] == '\\' && i + 1 < value.size())
			{
				i++;
				if (value[i] == 'n')
					result += '\n';
				else if (value[i] == 't')
					result += '\t';
				else if (value[i] == 'p')
					result += '|';
				else
					result += value[i];
			}
			else
				result += value[i];
			i++;
		}
		return (result);
	}

	inline void	write_all(int fd, const std::string &data)
	{
		const char	*buffer;
		size_t		written;
		ssize_t		count;

		buffer = data.c_str();
		written = 0;
		while (written < data.size())
		{
			count = write(fd, buffer + written, data.size() - written);
			if (count <= 0)
				return ;
			written += static_cast<size_t>(count);
		}
	}

	inline void	serialize_report(int fd, const Report &report)
	{
		size_t	i;
		size_t	j;

		i = 0;
		while (i < report.functions.size())
		{
			j = 0;
			while (j < report.functions[i].checks.size())
			{
				const CheckResult	&check = report.functions[i].checks[j];
				std::string			line;

				line = std::string("CHECK|") + escape_field(check.status)
					+ "|" + (check.success ? "1" : "0")
					+ "|" + escape_field(check.label)
					+ "|" + escape_field(check.details) + "\n";
				write_all(fd, line);
				j++;
			}
			i++;
		}
		write_all(fd, "END\n");
	}

	inline std::vector<std::string>	split_serialized_line(
		const std::string &line)
	{
		std::vector<std::string>	fields;
		std::string				field;
		size_t					i;
		bool					escaped;

		i = 0;
		escaped = false;
		while (i < line.size())
		{
			if (escaped)
			{
				field += '\\';
				field += line[i];
				escaped = false;
			}
			else if (line[i] == '\\')
				escaped = true;
			else if (line[i] == '|')
			{
				fields.push_back(field);
				field.clear();
			}
			else
				field += line[i];
			i++;
		}
		if (escaped)
			field += '\\';
		fields.push_back(field);
		return (fields);
	}

	inline void	parse_report_data(Report &report, const std::string &data)
	{
		std::istringstream	stream(data);
		std::string			line;

		while (std::getline(stream, line))
		{
			std::vector<std::string>	fields;

			if (line == "END")
				break ;
			fields = split_serialized_line(line);
			if (fields.size() == 5 && fields[0] == "CHECK")
				add_raw_check(report, unescape_field(fields[3]),
					unescape_field(fields[1]), fields[2] == "1",
					unescape_field(fields[4]));
		}
	}

	inline std::string	signal_status(int signal)
	{
		if (signal == SIGSEGV)
			return ("SEGV");
		if (signal == SIGBUS)
			return ("BUS");
		if (signal == SIGABRT)
			return ("ABRT");
		if (signal == SIGFPE)
			return ("FPE");
		return (std::string("SIG") + text(signal));
	}

	inline std::string	suite_signal_details(const std::string &suite,
		int signal)
	{
		return ("suite \"" + suite + "\" stopped with "
			+ signal_status(signal)
			+ " before it could report the exact function. This usually means "
			+ "one function used inside that suite crashed; it is not a Libft "
			+ "function named \"" + suite + "\".");
	}

	inline void	read_available(int fd, std::string &data)
	{
		char	buffer[4096];
		ssize_t	count;

		while ((count = read(fd, buffer, sizeof(buffer))) > 0)
			data.append(buffer, buffer + count);
	}

	class SuiteRunner
	{
	public:
		void	add(const std::string &name, SuiteFn run)
		{
			Suite	suite;

			suite.name = name;
			suite.run = run;
			m_suites.push_back(suite);
		}

		std::vector<std::string>	suite_names(void) const
		{
			std::vector<std::string>	names;
			size_t						i;

			i = 0;
			while (i < m_suites.size())
			{
				names.push_back(m_suites[i].name);
				i++;
			}
			return (names);
		}

		Report	run_all(int timeout_ms = 3000,
			const std::string &suite_filter = "", bool fail_fast = false) const
		{
			Report	report;
			size_t	i;

			if (std::getenv("LIBFT_TESTER_NO_FORK"))
			{
				run_all_in_process(report, suite_filter, fail_fast);
				return (report);
			}
			i = 0;
			while (i < m_suites.size())
			{
				if (suite_filter.empty()
					|| name_matches(m_suites[i].name, suite_filter))
				{
					run_suite(m_suites[i], timeout_ms, report);
					if (fail_fast && report.failures > 0)
						break ;
				}
				i++;
			}
			if (!suite_filter.empty() && report.functions.empty())
				add_raw_check(report, "suite no match", "NOK", false,
					std::string("no suite matched: ") + suite_filter);
			return (report);
		}

	private:
		std::vector<Suite>	m_suites;

		void	run_all_in_process(Report &report,
			const std::string &suite_filter, bool fail_fast) const
		{
			size_t	i;

			i = 0;
			while (i < m_suites.size())
			{
				if (suite_filter.empty()
					|| name_matches(m_suites[i].name, suite_filter))
				{
					m_suites[i].run(report);
					if (fail_fast && report.failures > 0)
						break ;
				}
				i++;
			}
			if (!suite_filter.empty() && report.functions.empty())
				add_raw_check(report, "suite no match", "NOK", false,
					std::string("no suite matched: ") + suite_filter);
		}

		void	run_suite(const Suite &suite, int timeout_ms,
			Report &report) const
		{
			int		pipe_fd[2];
			pid_t	pid;

			if (pipe(pipe_fd) == -1)
			{
				add_raw_check(report, "runner issue", "NOK", false,
					"internal pipe setup failed while preparing suite \""
					+ suite.name + "\"");
				return ;
			}
			pid = fork();
			if (pid == -1)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				add_raw_check(report, "runner issue", "NOK", false,
					"internal fork setup failed while preparing suite \""
					+ suite.name + "\"");
				return ;
			}
			if (pid == 0)
				run_child_suite(suite, pipe_fd);
			run_parent_suite(suite, timeout_ms, pipe_fd, pid, report);
		}

		static void	run_child_suite(const Suite &suite, int pipe_fd[2])
		{
			Report	child_report;

			close(pipe_fd[0]);
			suite.run(child_report);
			serialize_report(pipe_fd[1], child_report);
			close(pipe_fd[1]);
			std::exit(child_report.failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
		}

		static void	run_parent_suite(const Suite &suite, int timeout_ms,
			int pipe_fd[2], pid_t pid, Report &report)
		{
			std::chrono::steady_clock::time_point	start;
			std::string								data;
			int										status;
			bool									done;

			close(pipe_fd[1]);
			fcntl(pipe_fd[0], F_SETFL, O_NONBLOCK);
			start = std::chrono::steady_clock::now();
			done = false;
			status = 0;
			while (!done)
			{
				pid_t	result = waitpid(pid, &status, WNOHANG);

				read_available(pipe_fd[0], data);
				if (result == pid)
					done = true;
				else if (timeout_reached(start, timeout_ms))
				{
					kill(pid, SIGKILL);
					waitpid(pid, &status, 0);
					add_raw_check(report, "runner issue", "TIMEOUT", false,
						"suite \"" + suite.name + "\" exceeded timeout");
					done = true;
				}
				else
					usleep(1000);
			}
			read_available(pipe_fd[0], data);
			close(pipe_fd[0]);
			parse_report_data(report, data);
			if (WIFSIGNALED(status) && WTERMSIG(status) != SIGKILL)
				add_raw_check(report, "runner issue",
					signal_status(WTERMSIG(status)), false,
					suite_signal_details(suite.name, WTERMSIG(status)));
			else if (data.empty() && !WIFEXITED(status))
				add_raw_check(report, "runner issue", "NOK", false,
					"suite \"" + suite.name + "\" did not return a report");
		}

		static bool	timeout_reached(
			const std::chrono::steady_clock::time_point &start, int timeout_ms)
		{
			std::chrono::steady_clock::time_point	now;

			now = std::chrono::steady_clock::now();
			return (std::chrono::duration_cast<std::chrono::milliseconds>(
				now - start).count() > timeout_ms);
		}
	};

	inline bool	isolated_timeout_reached(
		const std::chrono::steady_clock::time_point &start, int timeout_ms)
	{
		std::chrono::steady_clock::time_point	now;

		now = std::chrono::steady_clock::now();
		return (std::chrono::duration_cast<std::chrono::milliseconds>(
			now - start).count() > timeout_ms);
	}

	template <typename Fn>
	inline void	run_isolated(Report &report, const std::string &name, Fn fn,
		int timeout_ms = 3000)
	{
		int										pipe_fd[2];
		pid_t									pid;
		std::chrono::steady_clock::time_point	start;
		std::string								data;
		int										status;
		bool									done;

		if (pipe(pipe_fd) == -1)
		{
			add_raw_check(report, name + " runner", "NOK", false,
				"internal pipe setup failed while isolating this function");
			return ;
		}
		pid = fork();
		if (pid == -1)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			add_raw_check(report, name + " runner", "NOK", false,
				"internal fork setup failed while isolating this function");
			return ;
		}
		if (pid == 0)
		{
			Report	child_report;

			close(pipe_fd[0]);
			fn(child_report);
			serialize_report(pipe_fd[1], child_report);
			close(pipe_fd[1]);
			std::exit(child_report.failures == 0
				? EXIT_SUCCESS : EXIT_FAILURE);
		}
		close(pipe_fd[1]);
		fcntl(pipe_fd[0], F_SETFL, O_NONBLOCK);
		start = std::chrono::steady_clock::now();
		done = false;
		status = 0;
		while (!done)
		{
			pid_t	result = waitpid(pid, &status, WNOHANG);

			read_available(pipe_fd[0], data);
			if (result == pid)
				done = true;
			else if (isolated_timeout_reached(start, timeout_ms))
			{
				kill(pid, SIGKILL);
				waitpid(pid, &status, 0);
				add_raw_check(report, name + " timeout", "TIMEOUT", false,
					"function tests exceeded timeout");
				done = true;
			}
			else
				usleep(1000);
		}
		read_available(pipe_fd[0], data);
		close(pipe_fd[0]);
		parse_report_data(report, data);
		if (WIFSIGNALED(status) && WTERMSIG(status) != SIGKILL)
			add_raw_check(report, name + " crash",
				signal_status(WTERMSIG(status)), false,
				"crashed while running isolated tests for " + name);
		else if (data.empty() && !WIFEXITED(status))
			add_raw_check(report, name + " no report", "NOK", false,
				"isolated function tests did not return a report");
	}

	inline void	allocation_section()
	{
		subsection("Allocation Checks");
	}

	template <typename Writer>
	inline std::vector<unsigned char>	capture_fd_raw(Writer writer)
	{
		int					pipe_fd[2];
		std::vector<unsigned char>	result;
		char				buffer[256];
		ssize_t				count;

		if (pipe(pipe_fd) == -1)
		{
			std::perror("pipe");
			std::exit(EXIT_FAILURE);
		}
		writer(pipe_fd[1]);
		close(pipe_fd[1]);
		while ((count = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
			result.insert(result.end(), buffer, buffer + count);
		close(pipe_fd[0]);
		return (result);
	}

	template <typename Writer>
	inline std::string	capture_fd_text(Writer writer)
	{
		std::vector<unsigned char>	raw;

		raw = capture_fd_raw(writer);
		return (std::string(raw.begin(), raw.end()));
	}

	inline int	*new_int(int value)
	{
		int	*number;

		number = static_cast<int *>(std::malloc(sizeof(int)));
		if (number)
			*number = value;
		return (number);
	}

	inline void	free_int(void *content)
	{
		std::free(content);
	}

	inline void	free_split(char **split)
	{
		size_t	i;

		i = 0;
		while (split && split[i])
		{
			std::free(split[i]);
			i++;
		}
		std::free(split);
	}

	inline std::string	random_printable(std::mt19937 &rng, size_t max_len)
	{
		std::uniform_int_distribution<int>	length_dist(0, static_cast<int>(max_len));
		std::uniform_int_distribution<int>	char_dist(32, 126);
		size_t								length;
		std::string							value;
		size_t								i;

		length = static_cast<size_t>(length_dist(rng));
		value.reserve(length);
		i = 0;
		while (i < length)
		{
			value.push_back(static_cast<char>(char_dist(rng)));
			i++;
		}
		return (value);
	}

	inline std::string	random_ascii_set(std::mt19937 &rng, size_t max_len)
	{
		std::uniform_int_distribution<int>	length_dist(0, static_cast<int>(max_len));
		std::uniform_int_distribution<int>	char_dist(32, 126);
		size_t								length;
		std::string							value;
		size_t								i;

		length = static_cast<size_t>(length_dist(rng));
		value.reserve(length);
		i = 0;
		while (i < length)
		{
			value.push_back(static_cast<char>(char_dist(rng)));
			i++;
		}
		return (value);
	}

	inline std::vector<std::string>	split_reference(const std::string &input,
		char delimiter)
	{
		std::vector<std::string>	tokens;
		size_t						i;
		size_t						start;

		i = 0;
		while (i < input.size())
		{
			while (i < input.size() && input[i] == delimiter)
				i++;
			start = i;
			while (i < input.size() && input[i] != delimiter)
				i++;
			if (i > start)
				tokens.push_back(input.substr(start, i - start));
		}
		return (tokens);
	}

	inline size_t	split_count(char **split)
	{
		size_t	count;

		count = 0;
		while (split && split[count])
			count++;
		return (count);
	}

	inline std::string	trim_reference(const std::string &input,
		const std::string &set)
	{
		size_t	start;
		size_t	end;

		start = 0;
		end = input.size();
		while (start < end && set.find(input[start]) != std::string::npos)
			start++;
		while (end > start && set.find(input[end - 1]) != std::string::npos)
			end--;
		return (input.substr(start, end - start));
	}

	inline const char	*strnstr_reference(const char *big,
		const char *little, size_t len)
	{
		size_t	little_len;
		size_t	i;

		if (!*little)
			return (big);
		little_len = std::strlen(little);
		if (little_len > len)
			return (NULL);
		i = 0;
		while (big[i] && i + little_len <= len)
		{
			if (std::strncmp(big + i, little, little_len) == 0)
				return (big + i);
			i++;
		}
		return (NULL);
	}

	inline size_t	strlcpy_reference(char *dst, const char *src, size_t size)
	{
		size_t	src_len;
		size_t	copy_len;

		src_len = std::strlen(src);
		if (size == 0)
			return (src_len);
		copy_len = std::min(src_len, size - 1);
		std::memcpy(dst, src, copy_len);
		dst[copy_len] = '\0';
		return (src_len);
	}

	inline size_t	strlcat_reference(char *dst, const char *src, size_t size)
	{
		size_t	dst_len;
		size_t	src_len;
		size_t	copy_len;

		dst_len = std::strlen(dst);
		src_len = std::strlen(src);
		if (dst_len >= size)
			return (size + src_len);
		copy_len = std::min(src_len, size - dst_len - 1);
		std::memcpy(dst + dst_len, src, copy_len);
		dst[dst_len + copy_len] = '\0';
		return (dst_len + src_len);
	}
}

#endif
