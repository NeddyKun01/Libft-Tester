#include "driver.hpp"

static bool	is_rescue_output_option(const std::string &arg)
{
	return (arg == "--summary-only" || arg == "--quiet"
		|| arg == "--json" || arg == "--html" || arg == "--review"
		|| arg == "--coverage"
		|| arg == "--coverage-md" || arg == "--list" || arg == "--help"
		|| arg == "--version" || arg == "--no-color");
}

static bool	is_rescue_scoped_option(const std::string &arg)
{
	return (arg == "--suite" || arg == "--only" || arg == "--explain"
		|| arg == "--hint");
}

static std::vector<std::string>	filter_rescue_args(
	const std::vector<std::string> &args)
{
	std::vector<std::string>	filtered;
	size_t					i = 0;

	while (i < args.size())
	{
		if (is_rescue_scoped_option(args[i]) && i + 1 < args.size())
			i += 2;
		else if (is_rescue_output_option(args[i]))
			i++;
		else
			filtered.push_back(args[i++]);
	}
	return (filtered);
}

void	Driver::print_rescue_failure(std::ostream &out,
	const FunctionInfo &fn, const std::vector<std::string> &args,
	const CommandResult &summary)
{
	std::vector<std::string>	cmd = {rescue_path().string(), "--suite",
		fn.suite, "--only", fn.name, "--no-color", "--verbose"};
	CommandResult			detail;

	cmd.insert(cmd.end(), args.begin(), args.end());
	detail = run_process(cmd, fs::path(), suite_env());
	out << "\n" << red << "+-- Failure details: " << fn.name
		<< reset << "\n";
	out << "| command: " << join_args(cmd) << "\n";
	out << "| exit: " << detail.status << "\n";
	out << "| output:\n";
	if (!detail.output.empty())
		print_prefixed_excerpt(out, detail.output, "|   ", 120);
	else if (!summary.output.empty())
		print_prefixed_excerpt(out, summary.output, "|   ", 120);
	else
		out << "|   No output was produced by the test runner.\n";
	out << "+-- end of " << fn.name << " failure\n\n";
}

void	Driver::print_final_summary(std::ostream &out,
	const std::string &diagnose, const std::string &rescue,
	int diagnose_status, int rescue_status, bool rescue_ran)
{
	section(out, "Final Health Summary");
	out << "Status:       " << yellow << "NEEDS FIXES" << reset << "\n";
	out << "Normal:       " << red << "FAILED" << reset << "\n";
	if (diagnose_status == 0)
		out << "Diagnose:     " << green << "OK" << reset << "\n";
	else
		out << "Diagnose:     " << yellow << "PROBLEMS FOUND" << reset << "\n";
	if (rescue_ran && rescue_status == 0)
		out << "Rescue:       " << green << "OK" << reset << "\n";
	else if (rescue_ran)
		out << "Rescue:       " << red << "FAILED" << reset << "\n";
	else
		out << "Rescue:       " << dim << "SKIPPED" << reset
			<< ", libft.a was not available\n";
	out << "\nProblem counters:\n";
	out << "- structure/build errors: "
		<< value_from_log("structure/build errors", diagnose) << "\n";
	out << "- structure/build warnings: "
		<< value_from_log("structure/build warnings", diagnose) << "\n";
	out << "- missing source files: "
		<< value_from_log("missing source files", diagnose) << "\n";
	out << "- missing header declarations: "
		<< value_from_log("missing header declarations", diagnose) << "\n";
	out << "- missing library symbols: "
		<< value_from_log("missing library symbols", diagnose) << "\n";
	if (rescue_ran)
	{
		out << "\nRescue counters:\n";
		out << "- real symbols found: "
			<< value_from_log("real symbols found", rescue) << "\n";
		out << "- missing symbols skipped: "
			<< value_from_log("missing symbols skipped", rescue) << "\n";
		out << "- failed tested functions: "
			<< value_from_log("failed tested functions", rescue) << "\n";
	}
	out << "\nNext action:\n";
	if (diagnose_status != 0)
		out << "Fix the diagnose problems first, then run make again.\n";
	else if (rescue_status != 0)
		out << "The structure looks usable, but some tested functions still fail.\n";
	else
		out << "Fallback tests passed, but the normal tester still failed. Check the normal messages above.\n";
}

int	Driver::run_rescue_function(std::ostream &out, const FunctionInfo &fn,
	const std::vector<std::string> &args)
{
	std::vector<std::string>	cmd = {rescue_path().string(), "--suite", fn.suite,
		"--only", fn.name, "--summary-only", "--no-color"};
	std::vector<std::string>	safe_args = filter_rescue_args(args);
	CommandResult				result;

	cmd.insert(cmd.end(), safe_args.begin(), safe_args.end());
	result = run_process(cmd, fs::path(), suite_env());
	if (result.status == 0)
	{
		out << pad(fn.name, 17) << "OK\n";
		return (0);
	}
	out << pad(fn.name, 17) << "NOK\n";
	print_rescue_failure(out, fn, safe_args, result);
	return (1);
}

int	Driver::run_rescue(std::ostream &out,
	const std::vector<std::string> &args)
{
	std::set<std::string>	symbols;
	int						found = 0;
	int						missing = 0;
	int						indirect = 0;
	int						failed = 0;

	out << "\nLibft Tester Rescue Test\n";
	out << "root: " << root_dir << "\n";
	out << "suite: " << rescue_path() << "\n\n";
	if (!fs::is_regular_file(libft_path()))
	{
		out << "[ERROR] libft.a not found. Run diagnose first.\n";
		return (1);
	}
	if (build_suite(out, true) != 0)
	{
		out << "[ERROR] rescue runner failed to build.\n";
		return (1);
	}
	symbols = read_symbols();
	out << "Function results\n";
	out << "Function          Status\n";
	out << "--------          ------\n";
	for (const auto &fn : functions())
	{
		if (!symbols.count(fn.name))
		{
			missing++;
			out << pad(fn.name, 17) << "SKIP missing symbol\n";
		}
		else if (!fn.direct_rescue)
		{
			found++;
			indirect++;
			out << pad(fn.name, 17) << "SKIP indirect coverage\n";
		}
		else
		{
			found++;
			if (run_rescue_function(out, fn, args) != 0)
				failed++;
		}
	}
	out << "\nSummary\n";
	out << "real symbols found: " << found << "\n";
	out << "missing symbols skipped: " << missing << "\n";
	out << "indirect-only skipped: " << indirect << "\n";
	out << "failed tested functions: " << failed << "\n";
	return (failed == 0 ? 0 : 1);
}

int	Driver::run_smart(std::ostream &out,
	const std::vector<std::string> &args)
{
	std::ostringstream	normal;
	std::ostringstream	diagnose;
	std::ostringstream	rescue;
	int					normal_status;
	int					diagnose_status;
	int					rescue_status = 1;
	bool				rescue_ran = false;

	out << "\nLibft Tester Smart Run\n";
	out << "root: " << root_dir << "\n";
	if (!args.empty())
		out << "args: " << join_args(args) << "\n";
	normal_status = run_suite(args, normal);
	if (normal_status == 0)
	{
		out << normal.str();
		section(out, "Final Health Summary");
		out << "Status:       " << green << "PASS" << reset << "\n";
		out << "Normal:       " << green << "OK" << reset << "\n";
		out << "Diagnose:     not needed\n";
		out << "Rescue:       not needed\n";
		return (0);
	}
	print_normal_excerpt(out, normal.str());
	section(out, "Diagnose Fallback");
	diagnose_status = run_diagnose(diagnose);
	out << diagnose.str();
	section(out, "Rescue Fallback");
	if (fs::is_regular_file(libft_path()))
	{
		rescue_ran = true;
		rescue_status = run_rescue(rescue, args);
		out << rescue.str();
	}
	else
		out << "Skipped: libft.a was not found, so there are no symbols to test.\n";
	print_final_summary(out, diagnose.str(), rescue.str(),
		diagnose_status, rescue_status, rescue_ran);
	return (1);
}
