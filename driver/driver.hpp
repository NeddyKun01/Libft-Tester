#ifndef DRIVER_HPP
# define DRIVER_HPP

# include <algorithm>
# include <cerrno>
# include <csignal>
# include <cstdlib>
# include <cstring>
# include <filesystem>
# include <fstream>
# include <iostream>
# include <map>
# include <regex>
# include <set>
# include <sstream>
# include <string>
# include <sys/wait.h>
# include <unistd.h>
# include <vector>

namespace fs = std::filesystem;

extern const char	*g_version;

struct FunctionInfo
{
	std::string	name;
	std::string	prototype;
	std::string	suite;
	bool		direct_rescue;
};

struct CommandResult
{
	int			status;
	std::string	output;
};

struct Counters
{
	int	ok = 0;
	int	warnings = 0;
	int	errors = 0;
	int	missing_sources = 0;
	int	missing_headers = 0;
	int	missing_symbols = 0;
};

const std::vector<FunctionInfo>	&functions();
std::string						join_args(const std::vector<std::string> &args);
std::vector<std::string>		split_lines(const std::string &text);
std::string						read_file(const fs::path &path);
bool							write_file(const fs::path &path,
									const std::string &content);
std::string						last_field(const std::string &line);
CommandResult					run_process(const std::vector<std::string> &args,
									const fs::path &cwd = fs::path(),
									const std::map<std::string, std::string> &env = {});

class Driver
{
public:
	Driver(const fs::path &tester, const std::string &root);
	int	print_help();
	int	build_suite(std::ostream &out, bool rescue);
	int	run_suite(const std::vector<std::string> &args, std::ostream &out);
	int	run_diagnose(std::ostream &out);
	int	run_rescue(std::ostream &out, const std::vector<std::string> &args);
	int	run_smart(std::ostream &out, const std::vector<std::string> &args);
	int	run_menu(const std::vector<std::string> &args);
	int	run_self_test(std::ostream &out);
	int	run_doctor(std::ostream &out);

private:
	fs::path	tester_dir;
	std::string	root_dir;
	bool		color;
	std::string	bold;
	std::string	dim;
	std::string	red;
	std::string	green;
	std::string	yellow;
	std::string	cyan;
	std::string	reset;

	fs::path	root_path() const;
	fs::path	libft_path() const;
	fs::path	header_path() const;
	fs::path	makefile_path() const;
	fs::path	model_header() const;
	fs::path	model_makefile() const;
	fs::path	suite_path() const;
	fs::path	rescue_path() const;
	static std::vector<std::string>	with_args(std::vector<std::string> base,
					const std::vector<std::string> &args);
	static std::vector<std::string>	append(std::vector<std::string> base,
					const std::vector<std::string> &args);
	static std::string	pad(const std::string &value, size_t width);
	void	status_line(std::ostream &out, Counters &counters,
					const std::string &status, const std::string &message);
	void	check_structure(std::ostream &out, Counters &counters);
	void	check_build(std::ostream &out, Counters &counters);
	void	check_header_shape(std::ostream &out, Counters &counters);
	bool	has_header_decl(const std::string &name);
	std::set<std::string>	read_symbols();
	void	check_functions(std::ostream &out, Counters &counters,
					const std::set<std::string> &symbols);
	void	print_summary(std::ostream &out, const Counters &counters);
	void	section(std::ostream &out, const std::string &title);
	void	print_prefixed_excerpt(std::ostream &out, const std::string &text,
					const std::string &prefix, size_t max_lines);
	void	print_normal_excerpt(std::ostream &out, const std::string &normal);
	static std::string	value_from_log(const std::string &label,
					const std::string &log);
	void	print_final_summary(std::ostream &out, const std::string &diagnose,
					const std::string &rescue, int diagnose_status,
					int rescue_status, bool rescue_ran);
	int		run_rescue_function(std::ostream &out, const FunctionInfo &fn,
					const std::vector<std::string> &args);
	std::string	recommended_action();
	std::string	recommendation_hint();
	std::string	status_file(const fs::path &path);
	void	print_menu_header();
	void	print_menu();
	template <typename F>
	void	run_and_pause(F action)
	{
		action();
		std::cout << "\nPress Enter to return to the menu...";
		std::string	line;
		std::getline(std::cin, line);
	}
	void	function_tools(const std::vector<std::string> &args);
	void	change_root();
	int		run_leaks(std::ostream &out, const std::vector<std::string> &args);
	int		generate_html(std::ostream &out, const std::vector<std::string> &args);
	void	pass(std::ostream &out, const std::string &label);
	void	fail(std::ostream &out, int &failures, const std::string &label,
					const std::string &log = "");
	void	assert_contains(std::ostream &out, int &failures,
					const std::string &needle, const std::string &label,
					const std::string &log);
	void	write_partial_libft(const fs::path &root);
	void	write_broken_makefile_libft(const fs::path &root);
	void	test_missing_header(std::ostream &out, const fs::path &tmp,
					int &failures);
	void	test_partial_libft(std::ostream &out, const fs::path &tmp,
					int &failures);
	void	test_broken_makefile(std::ostream &out, const fs::path &tmp,
					int &failures);
};

#endif
