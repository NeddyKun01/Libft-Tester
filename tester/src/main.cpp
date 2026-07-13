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
#include "hints.hpp"
#include "profiles.hpp"
#include "test_modules.hpp"

struct CliOptions
{
	std::string	only;
	std::string	suite;
	std::string	explain;
	std::string	hint;
	std::string	profile = "normal";
	int			timeout_ms = 3000;
	int			repeat_count = 1;
	unsigned int	seed = 0;
	bool		verbose = false;
	bool		quiet = false;
	bool		json = false;
	bool		html = false;
	bool		list = false;
	bool		help = false;
	bool		profiles = false;
	bool		version = false;
	bool		coverage = false;
	bool		coverage_md = false;
	bool		fail_fast = false;
	bool		summary_only = false;
	bool		has_seed = false;
	bool		timeout_set = false;
	bool		repeat_set = false;
	bool		fail_fast_set = false;
};

static const char	*g_version = "1.6.0";

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
		<< "  --coverage-md       Print documented coverage as Markdown\n"
		<< "  --profiles          List run profiles\n"
		<< "  --explain NAME      Explain what is tested for a function\n"
		<< "  --hint NAME         Show debugging hints for a function\n"
		<< "  --suite NAME        Run only suites matching NAME\n"
		<< "  --only NAME         Show only functions matching NAME\n"
		<< "  --profile NAME      Use quick, normal, strict, or brutal\n"
		<< "  --timeout MS        Timeout per suite in milliseconds\n"
		<< "  --repeat N          Run the selected suites N times\n"
		<< "  --seed N            Reproduce pseudo-random tests with seed N\n"
		<< "  --strict            Shortcut for --profile strict\n"
		<< "  --fail-fast         Stop after the first failing suite\n"
		<< "  --summary-only      Print only the final summary\n"
		<< "  --verbose           Do not aggregate status tokens\n"
		<< "  --quiet             Show only failures and summary\n"
		<< "  --json              Print machine-readable JSON\n"
		<< "  --html              Print a standalone HTML report\n"
		<< "  --no-color          Disable terminal colors\n\n"
		<< "Examples:\n"
		<< "  " << program << " --only ft_split\n"
		<< "  " << program << " --suite lists --fail-fast\n"
		<< "  " << program << " --profile brutal --summary-only\n"
		<< "  " << program << " --repeat 10 --seed 42\n"
		<< "  " << program << " --hint ft_split\n"
		<< "  " << program << " --explain ft_lstmap\n"
		<< "  " << program << " --suite memory --verbose\n"
		<< "  " << program << " --json --no-color\n"
		<< "  " << program << " --html --no-color > report.html\n";
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

static bool	parse_int_value(const std::string &value, int &number)
{
	char	*end;
	long	parsed;

	end = NULL;
	parsed = std::strtol(value.c_str(), &end, 10);
	if (!end || *end != '\0' || parsed < 1 || parsed > INT_MAX)
		return (false);
	number = static_cast<int>(parsed);
	return (true);
}

static bool	parse_seed_value(const std::string &value, unsigned int &seed)
{
	char			*end;
	unsigned long	parsed;

	end = NULL;
	parsed = std::strtoul(value.c_str(), &end, 10);
	if (!end || *end != '\0' || parsed > UINT_MAX)
		return (false);
	seed = static_cast<unsigned int>(parsed);
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
		else if (value == "--profiles")
			options.profiles = true;
		else if (value == "--coverage")
			options.coverage = true;
		else if (value == "--coverage-md")
			options.coverage_md = true;
		else if (value == "--fail-fast")
		{
			options.fail_fast = true;
			options.fail_fast_set = true;
		}
		else if (value == "--strict")
			options.profile = "strict";
		else if (value == "--verbose" || value == "-v")
			options.verbose = true;
		else if (value == "--quiet" || value == "-q")
			options.quiet = true;
		else if (value == "--json")
			options.json = true;
		else if (value == "--html")
			options.html = true;
		else if (value == "--no-color")
			setenv("NO_COLOR", "1", 1);
		else if (value == "--summary-only")
			options.summary_only = true;
		else if (value == "--only" && read_value(argc, argv, i, options.only))
			;
		else if (value == "--explain"
			&& read_value(argc, argv, i, options.explain))
			;
		else if (value == "--hint" && read_value(argc, argv, i, options.hint))
			;
		else if (value == "--profile"
			&& read_value(argc, argv, i, options.profile))
			;
		else if (value == "--suite" && read_value(argc, argv, i, options.suite))
			;
		else if (value == "--timeout" && read_value(argc, argv, i, value))
		{
			if (!parse_int_value(value, options.timeout_ms))
				return (false);
			options.timeout_set = true;
		}
		else if (value == "--repeat" && read_value(argc, argv, i, value))
		{
			if (!parse_int_value(value, options.repeat_count))
				return (false);
			options.repeat_set = true;
		}
		else if (value == "--seed" && read_value(argc, argv, i, value))
		{
			if (!parse_seed_value(value, options.seed))
				return (false);
			options.has_seed = true;
		}
		else
		{
			std::cerr << "Unknown or incomplete option: " << value << '\n';
			return (false);
		}
		i++;
	}
	if (options.json)
		setenv("NO_COLOR", "1", 1);
	if (options.html)
		setenv("NO_COLOR", "1", 1);
	return (true);
}

static bool	apply_profile(CliOptions &options)
{
	profiles::Settings	settings;

	if (!profiles::find(options.profile, settings))
	{
		std::cerr << "Unknown profile: " << options.profile << '\n';
		profiles::print_table();
		return (false);
	}
	if (!options.repeat_set)
		options.repeat_count = settings.repeat_count;
	if (!options.timeout_set)
		options.timeout_ms = settings.timeout_ms;
	if (!options.fail_fast_set)
		options.fail_fast = settings.fail_fast;
	return (true);
}

static tester::OutputOptions	output_options(const CliOptions &cli)
{
	tester::OutputOptions	options;

	options.verbose = cli.verbose;
	options.quiet = cli.quiet;
	options.json = cli.json;
	options.summary_only = cli.summary_only;
	options.filter = cli.only;
	return (options);
}

static void	fill_report_metadata(tester::Report &report, const CliOptions &cli,
	long long duration_ms, int executed_repeats)
{
	report.version = g_version;
	report.profile = cli.profile;
	report.seed = cli.seed;
	report.repeat_count = executed_repeats;
	report.timeout_ms = cli.timeout_ms;
	report.duration_ms = duration_ms;
	report.fail_fast = cli.fail_fast;
	report.suite_filter = cli.suite;
	report.function_filter = cli.only;
}

static tester::Report	run_repeated(tester::SuiteRunner &runner,
	const CliOptions &cli, int &executed_repeats)
{
	tester::Report	report;
	int				i;

	i = 0;
	while (i < cli.repeat_count)
	{
		tester::Report	iteration;

		tester::set_random_seed(cli.seed + static_cast<unsigned int>(i));
		iteration = runner.run_all(cli.timeout_ms, cli.suite, cli.fail_fast);
		tester::append_report(report, iteration);
		i++;
		if (cli.fail_fast && report.failures > 0)
			break ;
	}
	executed_repeats = i;
	return (report);
}

int	main(int argc, char **argv)
{
	tester::SuiteRunner	runner;
	tester::Report		report;
	CliOptions			cli;
	std::chrono::steady_clock::time_point	start;
	int					executed_repeats;
	long long			duration_ms;

	register_suites(runner);
	if (!parse_args(argc, argv, cli))
	{
		print_help(argv[0]);
		return (2);
	}
	if (!apply_profile(cli))
		return (2);
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
	if (cli.profiles)
	{
		profiles::print_table();
		return (0);
	}
	if (cli.coverage)
	{
		coverage::print_table();
		return (0);
	}
	if (cli.coverage_md)
	{
		coverage::print_markdown();
		return (0);
	}
	if (!cli.explain.empty())
		return (coverage::print_explain(cli.explain) ? 0 : 1);
	if (!cli.hint.empty())
		return (hints::print_for_function(cli.hint) ? 0 : 1);
	if (!cli.has_seed)
		cli.seed = tester::default_seed();
	start = std::chrono::steady_clock::now();
	report = run_repeated(runner, cli, executed_repeats);
	duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now() - start).count();
	fill_report_metadata(report, cli, duration_ms, executed_repeats);
	report = tester::filter_report(report, cli.only);
	if (cli.json)
		tester::print_json_report(report);
	else if (cli.html)
		tester::print_html_report(report);
	else
		tester::print_summary(report, output_options(cli));
	return (report.failures == 0 ? 0 : 1);
}
