#include "driver.hpp"

void	Driver::check_structure(std::ostream &out, Counters &counters)
{
	if (fs::is_regular_file(makefile_path()))
		status_line(out, counters, "OK", "Makefile found");
	else
	{
		status_line(out, counters, "ERROR", "Makefile missing");
		out << "           Fix: compare with " << model_makefile() << "\n";
	}
	if (fs::is_regular_file(header_path()))
		status_line(out, counters, "OK", "libft.h found");
	else
	{
		status_line(out, counters, "ERROR", "libft.h missing");
		out << "           Fix: compare with " << model_header() << "\n";
	}
}

void	Driver::check_build(std::ostream &out, Counters &counters)
{
	out << "\nBuild check\n";
	if (!fs::is_regular_file(makefile_path()))
	{
		status_line(out, counters, "WARN", "Skipping make because Makefile is missing");
		return;
	}
	CommandResult result = run_process({"make", "-s", "-C", root_dir});
	if (result.status == 0)
		status_line(out, counters, "OK", "make completed successfully");
	else
	{
		status_line(out, counters, "ERROR", "make failed");
		out << "           First build messages:\n";
		print_prefixed_excerpt(out, result.output, "           ", 12);
	}
	if (fs::is_regular_file(libft_path()))
		status_line(out, counters, "OK", "libft.a found");
	else
	{
		status_line(out, counters, "ERROR", "libft.a missing after make");
		out << "           Fix: Makefile must build a library named libft.a\n";
	}
}

void	Driver::check_header_shape(std::ostream &out, Counters &counters)
{
	std::string	header;

	out << "\nHeader shape\n";
	if (!fs::is_regular_file(header_path()))
	{
		status_line(out, counters, "WARN", "Skipping header checks because libft.h is missing");
		return;
	}
	header = read_file(header_path());
	if (std::regex_search(header, std::regex("typedef[[:space:]]+struct[[:space:]]+s_list|t_list")))
		status_line(out, counters, "OK", "t_list appears to be declared");
	else
	{
		status_line(out, counters, "WARN", "t_list declaration not found");
		out << "           Fix: add the t_list struct used by list functions\n";
	}
	if (std::regex_search(header, std::regex("#[[:space:]]*ifndef|#[[:space:]]*pragma[[:space:]]+once")))
		status_line(out, counters, "OK", "include guard or pragma once found");
	else
	{
		status_line(out, counters, "WARN", "include guard not found");
		out << "           Fix: add a normal include guard to libft.h\n";
	}
}

bool	Driver::has_header_decl(const std::string &name)
{
	if (!fs::is_regular_file(header_path()))
		return (false);
	return (std::regex_search(read_file(header_path()),
		std::regex("(^|[^A-Za-z0-9_])" + name + "[[:space:]]*\\(")));
}

std::set<std::string>	Driver::read_symbols()
{
	std::set<std::string>	symbols;

	if (!fs::is_regular_file(libft_path()))
		return (symbols);
	CommandResult result = run_process({"nm", "-g", libft_path().string()});
	for (const auto &line : split_lines(result.output))
	{
		std::string symbol = last_field(line);
		if (!symbol.empty())
			symbols.insert(symbol);
	}
	return (symbols);
}

void	Driver::check_functions(std::ostream &out, Counters &counters,
	const std::set<std::string> &symbols)
{
	out << "\nFunction matrix\n";
	out << "Function          Source  Header  Symbol  Status           Suggested fix\n";
	out << "--------          ------  ------  ------  ------           -------------\n";
	for (const auto &fn : functions())
	{
		bool		source = fs::is_regular_file(root_path() / (fn.name + ".c"));
		bool		header = has_header_decl(fn.name);
		bool		symbol = symbols.count(fn.name) > 0;
		std::string	status = "OK";
		std::string	fix = "-";

		if (!source)
		{
			status = "MISSING_SOURCE";
			fix = "create " + fn.name + ".c";
			counters.missing_sources++;
		}
		if (!header)
		{
			if (status == "OK")
				status = "MISSING_HEADER";
			if (fix == "-")
				fix = "add prototype: " + fn.prototype;
			counters.missing_headers++;
		}
		if (!symbol)
		{
			if (status == "OK")
				status = "MISSING_SYMBOL";
			if (fix == "-")
				fix = source ? "add " + fn.name + ".c to SRCS/Makefile"
					: "implement " + fn.name + ".c and build it";
			counters.missing_symbols++;
		}
		out << pad(fn.name, 17) << pad(source ? "yes" : "no", 8)
			<< pad(header ? "yes" : "no", 8)
			<< pad(symbol ? "yes" : "no", 8)
			<< pad(status, 17) << fix << "\n";
	}
}

void	Driver::print_summary(std::ostream &out, const Counters &counters)
{
	out << "\nSummary\n";
	out << "structure/build errors: " << counters.errors << "\n";
	out << "structure/build warnings: " << counters.warnings << "\n";
	out << "missing source files: " << counters.missing_sources << "\n";
	out << "missing header declarations: " << counters.missing_headers << "\n";
	out << "missing library symbols: " << counters.missing_symbols << "\n";
	out << "\nRecommended next step\n";
	if (counters.errors == 0 && counters.missing_headers == 0
		&& counters.missing_symbols == 0)
		out << "Run the normal tester: make ROOT_DIR=" << root_dir << "\n";
	else
	{
		out << "Fix the reported Makefile/header/source issues first.\n";
		out << "Use the known-good models only as references:\n";
		out << "- " << model_header() << "\n";
		out << "- " << model_makefile() << "\n";
	}
}

int	Driver::run_diagnose(std::ostream &out)
{
	Counters				counters;
	std::set<std::string>	symbols;

	out << "\nLibft Tester Diagnose\n";
	out << "root: " << root_dir << "\n";
	out << "model header: " << model_header() << "\n";
	out << "model makefile: " << model_makefile() << "\n\n";
	if (!fs::is_directory(root_path()))
	{
		status_line(out, counters, "ERROR", "ROOT_DIR does not exist: " + root_dir);
		print_summary(out, counters);
		return (1);
	}
	status_line(out, counters, "OK", "ROOT_DIR exists");
	check_structure(out, counters);
	check_build(out, counters);
	symbols = read_symbols();
	check_header_shape(out, counters);
	check_functions(out, counters, symbols);
	print_summary(out, counters);
	if (counters.errors > 0 || counters.missing_sources > 0
		|| counters.missing_headers > 0 || counters.missing_symbols > 0)
		return (1);
	return (0);
}
