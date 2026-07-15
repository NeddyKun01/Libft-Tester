#include "driver.hpp"

#include <iomanip>

struct CompareFunction
{
	std::string	name;
	int			passed = 0;
	int			total = 0;
	int			failures = 0;
};

struct CompareReport
{
	std::string	root;
	int			status = 1;
	int			checks = 0;
	int			failures = 0;
	bool		passed = false;
	std::string	output;
	std::vector<CompareFunction>	functions;
};

static bool	compare_output_option(const std::string &arg)
{
	return (arg == "--json" || arg == "--web" || arg == "--html" || arg == "--review"
		|| arg == "--summary-only" || arg == "--quiet" || arg == "-q"
		|| arg == "--verbose" || arg == "-v");
}

static std::vector<std::string>	compare_suite_args(
	const std::vector<std::string> &args)
{
	std::vector<std::string>	filtered;
	size_t					i;

	i = 0;
	while (i < args.size())
	{
		if (compare_output_option(args[i]))
			i++;
		else
			filtered.push_back(args[i++]);
	}
	filtered.push_back("--json");
	filtered.push_back("--no-color");
	return (filtered);
}

static int	json_int(const std::string &json, const std::string &key)
{
	std::regex	pattern("\\\"" + key + "\\\":([0-9]+)");
	std::smatch	match;

	if (!std::regex_search(json, match, pattern))
		return (0);
	return (std::atoi(match[1].str().c_str()));
}

static bool	json_bool_value(const std::string &json, const std::string &key)
{
	std::regex	pattern("\\\"" + key + "\\\":(true|false)");
	std::smatch	match;

	if (!std::regex_search(json, match, pattern))
		return (false);
	return (match[1].str() == "true");
}

static int	count_value(const std::string &counts, const std::string &key)
{
	std::regex	pattern("\\\"" + key + "\\\":([0-9]+)");
	std::smatch	match;

	if (!std::regex_search(counts, match, pattern))
		return (0);
	return (std::atoi(match[1].str().c_str()));
}

static CompareReport	parse_compare_report(const std::string &root,
	const CommandResult &result)
{
	CompareReport	report;
	std::regex		function_pattern("\\{\\\"name\\\":\\\"([^\\\"]+)\\\",\\\"passed\\\":([0-9]+),\\\"total\\\":([0-9]+),\\\"counts\\\":\\{([^}]*)\\}");
	auto			begin = std::sregex_iterator(result.output.begin(),
					result.output.end(), function_pattern);
	auto			end = std::sregex_iterator();

	report.root = root;
	report.status = result.status;
	report.output = result.output;
	report.checks = json_int(result.output, "checks");
	report.failures = json_int(result.output, "failures");
	report.passed = json_bool_value(result.output, "passed");
	for (auto it = begin; it != end; ++it)
	{
		CompareFunction function;
		std::string counts = (*it)[4].str();

		function.name = (*it)[1].str();
		function.passed = std::atoi((*it)[2].str().c_str());
		function.total = std::atoi((*it)[3].str().c_str());
		function.failures = count_value(counts, "nok")
			+ count_value(counts, "mnok") + count_value(counts, "segv")
			+ count_value(counts, "bus") + count_value(counts, "abrt")
			+ count_value(counts, "fpe") + count_value(counts, "timeout");
		report.functions.push_back(function);
	}
	return (report);
}

static const CompareFunction	*find_compare_function(
	const CompareReport &report, const std::string &name)
{
	for (const auto &function : report.functions)
		if (function.name == name)
			return (&function);
	return (NULL);
}

static std::string	compare_score(const CompareFunction *function)
{
	std::ostringstream	out;

	if (!function)
		return ("missing");
	out << function->passed << "/" << function->total;
	if (function->failures == 0 && function->passed == function->total)
		out << " PASS";
	else
		out << " FAIL";
	return (out.str());
}

static void	print_compare_side(std::ostream &out, const std::string &label,
	const CompareReport &report)
{
	out << label << ": " << (report.passed ? "PASS" : "FAIL")
		<< " | score " << report.checks - report.failures << "/"
		<< report.checks << " | failures " << report.failures
		<< " | root " << report.root << "\n";
}

int	Driver::run_compare(std::ostream &out, const std::string &other_root,
	const std::vector<std::string> &args)
{
	std::vector<std::string>	suite_args;
	CompareReport			left;
	CompareReport			right;
	std::set<std::string>		seen;
	int						differences;

	suite_args = compare_suite_args(args);
	std::string left_root = root_dir;
	auto run_side = [&](const std::string &target_root) {
		std::ostringstream	build_log;
		CommandResult		result;
		CompareReport		report;
		std::string			old_root;

		old_root = root_dir;
		root_dir = target_root;
		if (build_suite(build_log, false) != 0)
		{
			report.root = target_root;
			report.status = 2;
			report.output = build_log.str();
			out << "[ERROR] failed to build suite for " << target_root << "\n";
			print_prefixed_excerpt(out, report.output, "  ", 20);
			root_dir = old_root;
			return (report);
		}
		result = run_process(with_args({suite_path().string()}, suite_args),
			fs::path(), suite_env());
		report = parse_compare_report(target_root, result);
		root_dir = old_root;
		return (report);
	};
	out << "\nLibft Tester Compare\n";
	out << "left:  " << left_root << "\n";
	out << "right: " << other_root << "\n";
	out << "args:  " << join_args(suite_args) << "\n\n";
	left = run_side(left_root);
	right = run_side(other_root);
	if (left.status == 2 || right.status == 2)
		return (1);
	print_compare_side(out, "Left ", left);
	print_compare_side(out, "Right", right);
	out << "\nDifferences\n";
	out << "Function             Left            Right\n";
	out << "------------------------------------------------------------\n";
	differences = 0;
	for (const auto &function : left.functions)
	{
		const CompareFunction	*other = find_compare_function(right, function.name);
		std::string				left_score = compare_score(&function);
		std::string				right_score = compare_score(other);

		seen.insert(function.name);
		if (left_score != right_score)
		{
			out << pad(function.name, 21) << std::left << std::setw(16)
				<< left_score << right_score << "\n";
			differences++;
		}
	}
	for (const auto &function : right.functions)
	{
		if (seen.count(function.name))
			continue ;
		out << pad(function.name, 21) << std::left << std::setw(16)
			<< "missing" << compare_score(&function) << "\n";
		differences++;
	}
	if (differences == 0)
		out << "No function-level score differences.\n";
	out << "\nCompare verdict: ";
	if (left.passed && right.passed && differences == 0)
	{
		out << green << "MATCHING PASS" << reset << "\n";
		return (0);
	}
	if (differences == 0)
		out << yellow << "SAME RESULT, NEEDS FIXES" << reset << "\n";
	else
		out << yellow << "DIFFERENT" << reset << "\n";
	return (1);
}
