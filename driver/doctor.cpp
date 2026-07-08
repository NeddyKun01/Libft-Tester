#include "driver.hpp"

static std::vector<std::string>	path_entries()
{
	std::vector<std::string>	entries;
	const char				*path = std::getenv("PATH");
	std::stringstream		stream(path ? path : "");
	std::string				entry;

	while (std::getline(stream, entry, ':'))
	{
		if (entry.empty())
			entry = ".";
		entries.push_back(entry);
	}
	return (entries);
}

static bool	find_executable(const std::string &name, fs::path &found)
{
	if (name.find('/') != std::string::npos)
	{
		found = name;
		return (access(found.c_str(), X_OK) == 0);
	}
	for (const auto &entry : path_entries())
	{
		fs::path candidate = fs::path(entry) / name;
		if (access(candidate.c_str(), X_OK) == 0)
		{
			found = candidate;
			return (true);
		}
	}
	return (false);
}

static void	print_tool(std::ostream &out, const std::string &name,
	bool required, int &errors, int &warnings)
{
	fs::path	found;
	bool		ok = find_executable(name, found);

	if (ok)
	{
		out << "[OK]    " << name << " found at " << found << "\n";
		return;
	}
	if (required)
	{
		errors++;
		out << "[ERROR] " << name << " missing\n";
	}
	else
	{
		warnings++;
		out << "[WARN]  " << name << " missing, optional feature unavailable\n";
	}
}

static void	print_file_check(std::ostream &out, const std::string &label,
	const fs::path &path, bool required, int &errors, int &warnings)
{
	if (fs::exists(path))
	{
		out << "[OK]    " << label << " found: " << path << "\n";
		return;
	}
	if (required)
	{
		errors++;
		out << "[ERROR] " << label << " missing: " << path << "\n";
	}
	else
	{
		warnings++;
		out << "[WARN]  " << label << " missing: " << path << "\n";
	}
}

int	Driver::run_doctor(std::ostream &out)
{
	int	errors = 0;
	int	warnings = 0;

	out << "\nLibft Tester Doctor\n";
	out << "version: " << g_version << "\n";
	out << "tester:  " << tester_dir << "\n";
	out << "root:    " << root_dir << "\n\n";
	out << "Required tools\n";
	print_tool(out, "make", true, errors, warnings);
	print_tool(out, "c++", true, errors, warnings);
	print_tool(out, "cc", true, errors, warnings);
	print_tool(out, "ar", true, errors, warnings);
	print_tool(out, "nm", true, errors, warnings);
	out << "\nOptional tools\n";
	print_tool(out, "valgrind", false, errors, warnings);
	out << "\nTarget project\n";
	if (fs::is_directory(root_path()))
		out << "[OK]    ROOT_DIR exists: " << root_path() << "\n";
	else
	{
		errors++;
		out << "[ERROR] ROOT_DIR does not exist: " << root_path() << "\n";
	}
	print_file_check(out, "Makefile", makefile_path(), true, errors, warnings);
	print_file_check(out, "libft.h", header_path(), true, errors, warnings);
	print_file_check(out, "libft.a", libft_path(), false, errors, warnings);
	out << "\nTester internals\n";
	print_file_check(out, "model header", model_header(), true, errors, warnings);
	print_file_check(out, "model makefile", model_makefile(), true, errors, warnings);
	print_file_check(out, "internal suite", suite_path(), false, errors, warnings);
	out << "\nSummary\n";
	out << "errors: " << errors << "\n";
	out << "warnings: " << warnings << "\n";
	if (errors == 0)
	{
		out << "\nDoctor verdict: OK\n";
		if (warnings > 0)
			out << "Optional warnings exist, but the tester can still run core checks.\n";
		return (0);
	}
	out << "\nDoctor verdict: NEEDS FIXES\n";
	out << "Fix required tools or target project files, then run doctor again.\n";
	return (1);
}
