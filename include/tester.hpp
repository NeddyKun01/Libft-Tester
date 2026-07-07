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
			<< "Function             Score      Progress     Status\n"
			<< "------------------------------------------------------------\n"
			<< paint(reset);
		i = 0;
		while (i < report.functions.size())
		{
			const FunctionReport	&function = report.functions[i];

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

	inline void	print_failure_details(const Report &report)
	{
		size_t	i;
		size_t	j;

		if (report.failures == 0)
			return ;
		std::cout << "\n" << paint(bold) << "Failure Details"
			<< paint(reset) << "\n";
		i = 0;
		while (i < report.functions.size())
		{
			j = 0;
			while (j < report.functions[i].checks.size())
			{
				const CheckResult	&check = report.functions[i].checks[j];

				if (!check.success)
				{
					std::cout << paint(red) << check.status << paint(reset) << " "
						<< check.label << '\n';
					if (!check.details.empty())
						std::cout << "  " << check.details << '\n';
				}
				j++;
			}
			i++;
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

	inline void	print_summary(const Report &report,
		const OutputOptions &options = OutputOptions())
	{
		int	passed;
		int	percent;

		passed = report.checks - report.failures;
		percent = 100;
		if (report.checks > 0)
			percent = (passed * 100) / report.checks;
		if (!options.quiet && !options.summary_only)
		{
			print_banner();
			if (report.seed != 0)
				std::cout << paint(dim) << "seed: " << report.seed
					<< " | repeats: " << report.repeat_count
					<< " | duration: " << report.duration_ms << "ms"
					<< paint(reset) << "\n";
			if (!options.filter.empty())
				std::cout << paint(dim) << "filter: " << options.filter
					<< paint(reset) << "\n";
		}
		if (!options.quiet && !options.summary_only)
			print_function_results(report, options);
		if (!options.summary_only)
			print_failure_details(report);
		std::cout << "\n" << paint(bold) << "Summary" << paint(reset) << "\n  ";
		if (options.summary_only && report.seed != 0)
			std::cout << "seed: " << report.seed
				<< " | repeats: " << report.repeat_count
				<< " | duration: " << report.duration_ms << "ms\n  ";
		print_status_count("OK", report.ok_count, true);
		print_status_count("MOK", report.mok_count, true);
		print_status_count("NOK", report.nok_count, false);
		print_status_count("MNOK", report.mnok_count, false);
		std::cout << "\n  checks: " << report.checks
			<< " | failures: ";
		if (report.failures == 0)
			std::cout << paint(green) << report.failures << paint(reset)
				<< " | verdict: " << paint(green) << "PASS"
				<< paint(reset) << '\n';
		else
			std::cout << paint(red) << report.failures << paint(reset)
				<< " | verdict: " << paint(red) << "FAIL"
				<< paint(reset) << " | check failure details above\n";
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
				add_raw_check(report, suite.name + " pipe", "NOK", false,
					"pipe() failed");
				return ;
			}
			pid = fork();
			if (pid == -1)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				add_raw_check(report, suite.name + " fork", "NOK", false,
					"fork() failed");
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
					add_raw_check(report, suite.name + " timeout", "TIMEOUT",
						false, "suite exceeded timeout");
					done = true;
				}
				else
					usleep(1000);
			}
			read_available(pipe_fd[0], data);
			close(pipe_fd[0]);
			parse_report_data(report, data);
			if (WIFSIGNALED(status) && WTERMSIG(status) != SIGKILL)
				add_raw_check(report, suite.name + " signal",
					signal_status(WTERMSIG(status)), false,
					"suite stopped by signal");
			else if (data.empty() && !WIFEXITED(status))
				add_raw_check(report, suite.name + " no report", "NOK",
					false, "suite did not return a report");
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
