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

#include "coverage.hpp"
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
	bool		fail_fast = false;
};

static const char	*g_version = "1.0.0-dev";

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
		<< "  --fail-fast         Stop after the first failing suite\n"
		<< "  --verbose           Do not aggregate status tokens\n"
		<< "  --quiet             Show only failures and summary\n"
		<< "  --json              Print machine-readable JSON\n"
		<< "  --no-color          Disable terminal colors\n\n"
		<< "Examples:\n"
		<< "  " << program << " --only ft_split\n"
		<< "  " << program << " --suite lists --fail-fast\n"
		<< "  " << program << " --explain ft_lstmap\n"
		<< "  " << program << " --suite memory --verbose\n"
		<< "  " << program << " --json --no-color\n";
}

static void	print_list(const tester::SuiteRunner &runner)
{
	std::vector<std::string>	suites = runner.suite_names();
	std::vector<std::string>	functions = coverage::function_names();
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
		else if (value == "--fail-fast")
			options.fail_fast = true;
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
		coverage::print_table();
		return (0);
	}
	if (!cli.explain.empty())
		return (coverage::print_explain(cli.explain) ? 0 : 1);
	report = runner.run_all(cli.timeout_ms, cli.suite, cli.fail_fast);
	report = tester::filter_report(report, cli.only);
	if (cli.json)
		tester::print_json_report(report);
	else
		tester::print_summary(report, output_options(cli));
	return (report.failures == 0 ? 0 : 1);
}
