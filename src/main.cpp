/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 22:53:40 by neddykun          #+#    #+#             */
/*   Updated: 2026/07/06 22:53:41 by neddykun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_modules.hpp"

struct CliOptions
{
	std::string	only;
	std::string	suite;
	std::string	explain;
	int			timeout_ms = 3000;
	bool		verbose = false;
	bool		quiet = false;
	bool		json = false;
	bool		list = false;
	bool		help = false;
	bool		version = false;
	bool		coverage = false;
};

static const char	*g_version = "1.0.0-dev";

struct CoverageEntry
{
	const char	*name;
	const char	*suite;
	const char	*group;
	const char	*cases;
	const char	*malloc_cases;
};

static std::vector<CoverageEntry>	coverage_entries(void);

static void	register_suites(tester::SuiteRunner &runner)
{
	runner.add("ctype", test_ctype);
	runner.add("memory", test_memory);
	runner.add("atoi", test_atoi);
	runner.add("strings", test_strings);
	runner.add("string_utils", test_string_utils);
	runner.add("output", test_output);
	runner.add("lists", test_lists);
}

static std::vector<std::string>	function_names(void)
{
	std::vector<std::string>	names;
	std::vector<CoverageEntry>	entries;
	size_t						i;

	entries = coverage_entries();
	i = 0;
	while (i < entries.size())
	{
		names.push_back(entries[i].name);
		i++;
	}
	return (names);
}

static std::vector<CoverageEntry>	coverage_entries(void)
{
	std::vector<CoverageEntry>	entries;
	const CoverageEntry			items[] = {
		{"ft_isalpha", "ctype", "classification", "full signed/unsigned char range, letters, non-letters", "none"},
		{"ft_isdigit", "ctype", "classification", "full signed/unsigned char range, digits, non-digits", "none"},
		{"ft_isalnum", "ctype", "classification", "full signed/unsigned char range, letters, digits, symbols", "none"},
		{"ft_isascii", "ctype", "classification", "full signed/unsigned char range, ASCII limits, out of range", "none"},
		{"ft_isprint", "ctype", "classification", "full signed/unsigned char range, printable limits, controls", "none"},
		{"ft_toupper", "ctype", "conversion", "full signed/unsigned char range, lowercase, uppercase, symbols", "none"},
		{"ft_tolower", "ctype", "conversion", "full signed/unsigned char range, uppercase, lowercase, symbols", "none"},
		{"ft_strlen", "strings", "string length", "empty string, normal text, whitespace, punctuation, early NUL", "none"},
		{"ft_strchr", "strings", "string search", "found, first char, missing, NUL terminator, empty string, wrapped char, stops at NUL", "none"},
		{"ft_strrchr", "strings", "string search", "last match, first char, missing, NUL terminator, empty string, wrapped char, stops at NUL", "none"},
		{"ft_strncmp", "strings", "string compare", "zero length, equal, prefix, negative diff, positive diff, shorter string, unsigned chars", "none"},
		{"ft_strnstr", "strings", "bounded search", "empty needle, zero length, exact fit, too short, start match, missing, partial end", "none"},
		{"ft_strlcpy", "strings", "bounded copy", "size zero, size one, full copy, truncation, empty source, return length, buffer bytes", "none"},
		{"ft_strlcat", "strings", "bounded append", "size smaller than dst, truncation, full append, size zero, empty dst, empty src, buffer bytes", "none"},
		{"ft_strdup", "strings", "allocation copy", "normal text, empty string, copied content", "malloc failure"},
		{"ft_memset", "memory", "memory write", "return pointer, fixed fill, zero size, unsigned char cast", "none"},
		{"ft_bzero", "memory", "memory zero", "full clear, partial clear, zero size, one byte, surrounding bytes", "none"},
		{"ft_memcpy", "memory", "memory copy", "return pointer, fixed copy, zero size, unchanged bytes, offset copy", "none"},
		{"ft_memmove", "memory", "overlap copy", "return pointer, forward overlap, backward overlap, zero size, same pointer", "none"},
		{"ft_memchr", "memory", "memory search", "first match, NUL byte, missing, zero size, stops before later match", "none"},
		{"ft_memcmp", "memory", "memory compare", "equal buffers, zero size, unsigned diff, negative diff, equal prefix", "none"},
		{"ft_calloc", "memory", "zero allocation", "overflow protection, zero-filled blocks, one byte allocation", "malloc failure"},
		{"ft_atoi", "atoi", "conversion", "zero, positive, negative, whitespace, signs, INT_MAX, INT_MIN, invalid prefixes, trailing text", "none"},
		{"ft_substr", "string_utils", "allocation substring", "basic slice, long length, out of range, zero length, empty source", "malloc failure"},
		{"ft_strjoin", "string_utils", "allocation join", "normal join, empty left, empty right, both empty", "malloc failure"},
		{"ft_strtrim", "string_utils", "allocation trim", "spaces, custom set, empty set, full trim, no trim, multi-character set", "malloc failure"},
		{"ft_split", "string_utils", "allocation split", "spaces, repeated delimiters, leading/trailing delimiters, empty tokens avoided, NULL terminator", "malloc failure across allocations"},
		{"ft_itoa", "string_utils", "integer to string", "INT_MIN, zero, positive, INT_MAX, negative, selected values", "malloc failure"},
		{"ft_strmapi", "string_utils", "mapped string", "index-aware transform, empty string, allocated result", "malloc failure"},
		{"ft_striteri", "string_utils", "in-place iteration", "uppercase transform, empty string, index use, NULL function, NULL string guard", "none"},
		{"ft_putchar_fd", "output", "fd output", "letter, newline, digit, NUL byte size/content", "none"},
		{"ft_putstr_fd", "output", "fd output", "normal text, empty string, whitespace, NULL string, digits", "none"},
		{"ft_putendl_fd", "output", "fd output", "normal text with newline, empty string, inner newline, NULL string, digits", "none"},
		{"ft_putnbr_fd", "output", "fd output", "INT_MIN, zero, negative, INT_MAX, positive", "none"},
		{"ft_lstnew", "lists", "list allocation", "content pointer, NULL content, next initialized to NULL", "malloc failure"},
		{"ft_lstadd_front", "lists", "list mutation", "normal prepend, empty list, NULL new node, size after prepend", "setup allocations"},
		{"ft_lstsize", "lists", "list query", "NULL list, one node, multiple nodes, after clear", "none"},
		{"ft_lstlast", "lists", "list query", "NULL list, single node, multiple nodes, next NULL on tail", "none"},
		{"ft_lstadd_back", "lists", "list mutation", "NULL new node, empty list, preserves head, links tail, last node", "setup allocations"},
		{"ft_lstdelone", "lists", "list deletion", "covered through clear/map deletion paths and custom deleters", "none"},
		{"ft_lstclear", "lists", "list deletion", "two nodes, resets head, empty list, NULL deleter guard", "none"},
		{"ft_lstiter", "lists", "list iteration", "multiple nodes, single node, NULL function guard, content mutation", "none"},
		{"ft_lstmap", "lists", "list mapping", "mapped copy, size, transformed values, f failure cleanup, NULL list", "malloc failure and cleanup"}
	};
	size_t						i;

	i = 0;
	while (i < sizeof(items) / sizeof(items[0]))
	{
		entries.push_back(items[i]);
		i++;
	}
	return (entries);
}

static void	print_help(const char *program)
{
	std::cout
		<< "Usage: " << program << " [options]\n\n"
		<< "Options:\n"
		<< "  --help              Show this help message\n"
		<< "  --version           Show tester version\n"
		<< "  --list              List suites and functions\n"
		<< "  --coverage          Show documented coverage table\n"
		<< "  --explain NAME      Explain what is tested for a function\n"
		<< "  --suite NAME        Run only suites matching NAME\n"
		<< "  --only NAME         Show only functions matching NAME\n"
		<< "  --timeout MS        Timeout per suite in milliseconds\n"
		<< "  --verbose           Do not aggregate status tokens\n"
		<< "  --quiet             Show only failures and summary\n"
		<< "  --json              Print machine-readable JSON\n"
		<< "  --no-color          Disable terminal colors\n\n"
		<< "Examples:\n"
		<< "  " << program << " --only ft_split\n"
		<< "  " << program << " --explain ft_lstmap\n"
		<< "  " << program << " --suite memory --verbose\n"
		<< "  " << program << " --json --no-color\n";
}

static void	print_coverage_table(void)
{
	std::vector<CoverageEntry>	entries = coverage_entries();
	size_t						i;

	std::cout << "Function             Suite          Group              Malloc\n";
	std::cout << "----------------------------------------------------------------\n";
	i = 0;
	while (i < entries.size())
	{
		std::cout << std::left << std::setw(21) << entries[i].name
			<< std::setw(15) << entries[i].suite
			<< std::setw(23) << entries[i].group
			<< entries[i].malloc_cases << '\n';
		i++;
	}
}

static bool	print_explain(const std::string &filter)
{
	std::vector<CoverageEntry>	entries = coverage_entries();
	bool						found;
	size_t						i;

	found = false;
	i = 0;
	while (i < entries.size())
	{
		if (tester::name_matches(entries[i].name, filter))
		{
			if (found)
				std::cout << '\n';
			std::cout << entries[i].name << "\n";
			std::cout << "  suite:  " << entries[i].suite << "\n";
			std::cout << "  group:  " << entries[i].group << "\n";
			std::cout << "  cases:  " << entries[i].cases << "\n";
			std::cout << "  malloc: " << entries[i].malloc_cases << "\n";
			found = true;
		}
		i++;
	}
	if (!found)
		std::cerr << "No documented coverage found for: " << filter << '\n';
	return (found);
}

static void	print_list(const tester::SuiteRunner &runner)
{
	std::vector<std::string>	suites = runner.suite_names();
	std::vector<std::string>	functions = function_names();
	size_t					i;

	std::cout << "Suites:\n";
	i = 0;
	while (i < suites.size())
	{
		std::cout << "  " << suites[i] << '\n';
		i++;
	}
	std::cout << "\nFunctions:\n";
	i = 0;
	while (i < functions.size())
	{
		std::cout << "  " << functions[i] << '\n';
		i++;
	}
}

static bool	read_value(int argc, char **argv, int &index, std::string &value)
{
	if (index + 1 >= argc)
		return (false);
	index++;
	value = argv[index];
	return (true);
}

static bool	parse_args(int argc, char **argv, CliOptions &options)
{
	int			i;
	std::string	value;

	i = 1;
	while (i < argc)
	{
		value = argv[i];
		if (value == "--help" || value == "-h")
			options.help = true;
		else if (value == "--version")
			options.version = true;
		else if (value == "--list")
			options.list = true;
		else if (value == "--coverage")
			options.coverage = true;
		else if (value == "--verbose" || value == "-v")
			options.verbose = true;
		else if (value == "--quiet" || value == "-q")
			options.quiet = true;
		else if (value == "--json")
			options.json = true;
		else if (value == "--no-color")
			setenv("NO_COLOR", "1", 1);
		else if (value == "--only" && read_value(argc, argv, i, options.only))
			;
		else if (value == "--explain"
			&& read_value(argc, argv, i, options.explain))
			;
		else if (value == "--suite" && read_value(argc, argv, i, options.suite))
			;
		else if (value == "--timeout" && read_value(argc, argv, i, value))
			options.timeout_ms = std::max(1, std::atoi(value.c_str()));
		else
		{
			std::cerr << "Unknown or incomplete option: " << value << '\n';
			return (false);
		}
		i++;
	}
	if (options.json)
		setenv("NO_COLOR", "1", 1);
	return (true);
}

static tester::OutputOptions	output_options(const CliOptions &cli)
{
	tester::OutputOptions	options;

	options.verbose = cli.verbose;
	options.quiet = cli.quiet;
	options.json = cli.json;
	options.filter = cli.only;
	return (options);
}

int	main(int argc, char **argv)
{
	tester::SuiteRunner	runner;
	tester::Report		report;
	CliOptions			cli;

	register_suites(runner);
	if (!parse_args(argc, argv, cli))
	{
		print_help(argv[0]);
		return (2);
	}
	if (cli.help)
	{
		print_help(argv[0]);
		return (0);
	}
	if (cli.version)
	{
		std::cout << "libft_tester " << g_version << '\n';
		return (0);
	}
	if (cli.list)
	{
		print_list(runner);
		return (0);
	}
	if (cli.coverage)
	{
		print_coverage_table();
		return (0);
	}
	if (!cli.explain.empty())
		return (print_explain(cli.explain) ? 0 : 1);
	report = runner.run_all(cli.timeout_ms, cli.suite);
	report = tester::filter_report(report, cli.only);
	if (cli.json)
		tester::print_json_report(report);
	else
		tester::print_summary(report, output_options(cli));
	return (report.failures == 0 ? 0 : 1);
}
