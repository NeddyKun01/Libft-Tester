#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

static const char	*g_version = "1.2.0";

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

static const std::vector<FunctionInfo>	&functions()
{
	static const std::vector<FunctionInfo>	items = {
		{"ft_isalpha", "int ft_isalpha(int c);", "ctype", true},
		{"ft_isdigit", "int ft_isdigit(int c);", "ctype", true},
		{"ft_isalnum", "int ft_isalnum(int c);", "ctype", true},
		{"ft_isascii", "int ft_isascii(int c);", "ctype", true},
		{"ft_isprint", "int ft_isprint(int c);", "ctype", true},
		{"ft_strlen", "size_t ft_strlen(const char *s);", "strings", true},
		{"ft_memset", "void *ft_memset(void *s, int c, size_t n);", "memory", true},
		{"ft_bzero", "void ft_bzero(void *s, size_t n);", "memory", true},
		{"ft_memcpy", "void *ft_memcpy(void *dst, const void *src, size_t n);", "memory", true},
		{"ft_memmove", "void *ft_memmove(void *dst, const void *src, size_t len);", "memory", true},
		{"ft_strlcpy", "size_t ft_strlcpy(char *dst, const char *src, size_t dstsize);", "strings", true},
		{"ft_strlcat", "size_t ft_strlcat(char *dst, const char *src, size_t dstsize);", "strings", true},
		{"ft_toupper", "int ft_toupper(int c);", "ctype", true},
		{"ft_tolower", "int ft_tolower(int c);", "ctype", true},
		{"ft_strchr", "char *ft_strchr(const char *s, int c);", "strings", true},
		{"ft_strrchr", "char *ft_strrchr(const char *s, int c);", "strings", true},
		{"ft_strncmp", "int ft_strncmp(const char *s1, const char *s2, size_t n);", "strings", true},
		{"ft_memchr", "void *ft_memchr(const void *s, int c, size_t n);", "memory", true},
		{"ft_memcmp", "int ft_memcmp(const void *s1, const void *s2, size_t n);", "memory", true},
		{"ft_strnstr", "char *ft_strnstr(const char *big, const char *little, size_t len);", "strings", true},
		{"ft_atoi", "int ft_atoi(const char *nptr);", "atoi", true},
		{"ft_calloc", "void *ft_calloc(size_t count, size_t size);", "memory", true},
		{"ft_strdup", "char *ft_strdup(const char *s);", "strings", true},
		{"ft_substr", "char *ft_substr(char const *s, unsigned int start, size_t len);", "string_utils", true},
		{"ft_strjoin", "char *ft_strjoin(char const *s1, char const *s2);", "string_utils", true},
		{"ft_strtrim", "char *ft_strtrim(char const *s1, char const *set);", "string_utils", true},
		{"ft_split", "char **ft_split(char const *s, char c);", "string_utils", true},
		{"ft_itoa", "char *ft_itoa(int n);", "string_utils", true},
		{"ft_strmapi", "char *ft_strmapi(char const *s, char (*f)(unsigned int, char));", "string_utils", true},
		{"ft_striteri", "void ft_striteri(char *s, void (*f)(unsigned int, char *));", "string_utils", true},
		{"ft_putchar_fd", "void ft_putchar_fd(char c, int fd);", "output", true},
		{"ft_putstr_fd", "void ft_putstr_fd(char *s, int fd);", "output", true},
		{"ft_putendl_fd", "void ft_putendl_fd(char *s, int fd);", "output", true},
		{"ft_putnbr_fd", "void ft_putnbr_fd(int n, int fd);", "output", true},
		{"ft_lstnew", "t_list *ft_lstnew(void *content);", "lists", true},
		{"ft_lstadd_front", "void ft_lstadd_front(t_list **lst, t_list *new);", "lists", true},
		{"ft_lstsize", "int ft_lstsize(t_list *lst);", "lists", true},
		{"ft_lstlast", "t_list *ft_lstlast(t_list *lst);", "lists", true},
		{"ft_lstadd_back", "void ft_lstadd_back(t_list **lst, t_list *new);", "lists", true},
		{"ft_lstdelone", "void ft_lstdelone(t_list *lst, void (*del)(void *));", "lists", false},
		{"ft_lstclear", "void ft_lstclear(t_list **lst, void (*del)(void *));", "lists", true},
		{"ft_lstiter", "void ft_lstiter(t_list *lst, void (*f)(void *));", "lists", true},
		{"ft_lstmap", "t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));", "lists", true},
	};
	return (items);
}

static std::string	join_args(const std::vector<std::string> &args)
{
	std::string	out;
	size_t		i = 0;

	while (i < args.size())
	{
		if (i > 0)
			out += " ";
		out += args[i];
		i++;
	}
	return (out);
}

static std::vector<std::string>	split_lines(const std::string &text)
{
	std::vector<std::string>	lines;
	std::istringstream		input(text);
	std::string				line;

	while (std::getline(input, line))
		lines.push_back(line);
	return (lines);
}

static std::string	read_file(const fs::path &path)
{
	std::ifstream		file(path);
	std::ostringstream	buffer;

	buffer << file.rdbuf();
	return (buffer.str());
}

static bool	write_file(const fs::path &path, const std::string &content)
{
	std::ofstream	file(path);

	if (!file)
		return (false);
	file << content;
	return (true);
}

static std::string	last_field(const std::string &line)
{
	std::istringstream	input(line);
	std::string			field;
	std::string			last;

	while (input >> field)
		last = field;
	return (last);
}

static CommandResult	run_process(const std::vector<std::string> &args,
	const fs::path &cwd = fs::path(),
	const std::map<std::string, std::string> &env = {})
{
	int					pipe_fd[2];
	pid_t				pid;
	CommandResult		result;
	std::vector<char *>	argv;
	char				buffer[4096];
	ssize_t				count;
	int					status;

	result.status = 127;
	if (args.empty() || pipe(pipe_fd) == -1)
	{
		result.output = "failed to create process pipe\n";
		return (result);
	}
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		result.output = "failed to fork process\n";
		return (result);
	}
	if (pid == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		dup2(pipe_fd[1], STDERR_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		if (!cwd.empty())
			chdir(cwd.c_str());
		for (const auto &item : env)
			setenv(item.first.c_str(), item.second.c_str(), 1);
		for (const auto &arg : args)
			argv.push_back(const_cast<char *>(arg.c_str()));
		argv.push_back(NULL);
		execvp(argv[0], argv.data());
		std::cerr << "failed to execute " << argv[0] << ": "
			<< std::strerror(errno) << "\n";
		_exit(127);
	}
	close(pipe_fd[1]);
	while ((count = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
		result.output.append(buffer, (size_t)count);
	close(pipe_fd[0]);
	if (waitpid(pid, &status, 0) == -1)
		result.status = 127;
	else if (WIFEXITED(status))
		result.status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		result.status = 128 + WTERMSIG(status);
	else
		result.status = 1;
	return (result);
}

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
	void	run_and_pause(F action);
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

Driver::Driver(const fs::path &tester, const std::string &root)
	: tester_dir(fs::weakly_canonical(tester)),
	  root_dir(root),
	  color(isatty(STDOUT_FILENO) && std::getenv("NO_COLOR") == NULL
		  && (!std::getenv("TERM") || std::string(std::getenv("TERM")) != "dumb"))
{
	if (color)
	{
		bold = "\033[1m";
		dim = "\033[2m";
		red = "\033[31m";
		green = "\033[32m";
		yellow = "\033[33m";
		cyan = "\033[36m";
		reset = "\033[0m";
	}
}

fs::path	Driver::root_path() const { return (fs::path(root_dir)); }
fs::path	Driver::libft_path() const { return (root_path() / "libft.a"); }
fs::path	Driver::header_path() const { return (root_path() / "libft.h"); }
fs::path	Driver::makefile_path() const { return (root_path() / "Makefile"); }
fs::path	Driver::model_header() const { return (tester_dir / "templates" / "libft.h"); }
fs::path	Driver::model_makefile() const { return (tester_dir / "templates" / "Makefile"); }
fs::path	Driver::suite_path() const { return (tester_dir / "build" / "libft_suite"); }
fs::path	Driver::rescue_path() const { return (tester_dir / "build" / "rescue" / "libft_suite_rescue"); }

std::vector<std::string>	Driver::with_args(std::vector<std::string> base,
	const std::vector<std::string> &args)
{
	base.insert(base.end(), args.begin(), args.end());
	return (base);
}

std::vector<std::string>	Driver::append(std::vector<std::string> base,
	const std::vector<std::string> &args)
{
	base.insert(base.end(), args.begin(), args.end());
	return (base);
}

std::string	Driver::pad(const std::string &value, size_t width)
{
	if (value.size() >= width)
		return (value + " ");
	return (value + std::string(width - value.size(), ' '));
}

int	Driver::print_help()
{
	std::cout
		<< "Libft Tester " << g_version << "\n\n"
		<< "Default use:\n"
		<< "  make ROOT_DIR=../libft\n"
		<< "  ./libft_tester --root ../libft --summary-only\n\n"
		<< "Driver options:\n"
		<< "  --root PATH         Target Libft directory, default: ..\n"
		<< "  --menu              Open the interactive menu\n"
		<< "  --smart             Run normal tests, then diagnose/rescue if needed\n"
		<< "  --diagnose          Check Makefile, header, sources, and symbols\n"
		<< "  --rescue            Test only symbols found in libft.a\n"
		<< "  --self-test         Validate tester fallback behavior\n"
		<< "  --version           Show tester version\n"
		<< "  --help              Show this help message\n\n"
		<< "Suite options are forwarded after building the internal suite:\n"
		<< "  --profile quick|normal|strict|brutal\n"
		<< "  --summary-only, --only NAME, --suite NAME, --json, --html\n"
		<< "  --coverage, --explain NAME, --hint NAME, --fail-fast\n";
	return (0);
}

int	Driver::build_suite(std::ostream &out, bool rescue)
{
	std::vector<std::string>	cmd = {
		"make", "-s", "-C", tester_dir.string(),
		rescue ? "rescue-suite" : "suite",
		"ROOT_DIR=" + root_dir
	};
	CommandResult			result = run_process(cmd);

	if (result.status != 0)
		out << result.output;
	return (result.status);
}

int	Driver::run_suite(const std::vector<std::string> &args, std::ostream &out)
{
	int	status = build_suite(out, false);

	if (status != 0)
		return (status);
	CommandResult result = run_process(with_args({suite_path().string()}, args));
	out << result.output;
	return (result.status);
}

void	Driver::status_line(std::ostream &out, Counters &counters,
	const std::string &status, const std::string &message)
{
	if (status == "OK")
		counters.ok++;
	else if (status == "WARN")
		counters.warnings++;
	else if (status == "ERROR")
		counters.errors++;
	out << "[" << status << "]"
		<< std::string(8 - std::min<size_t>(status.size() + 2, 8), ' ')
		<< message << "\n";
}

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

void	Driver::section(std::ostream &out, const std::string &title)
{
	out << "\n" << bold << title << reset << "\n";
	out << "------------------------------------------------------------\n";
}

void	Driver::print_prefixed_excerpt(std::ostream &out,
	const std::string &text, const std::string &prefix, size_t max_lines)
{
	std::vector<std::string>	lines = split_lines(text);
	size_t					i = 0;

	while (i < lines.size() && i < max_lines)
	{
		out << prefix << lines[i] << "\n";
		i++;
	}
}

void	Driver::print_normal_excerpt(std::ostream &out,
	const std::string &normal)
{
	section(out, "Normal Run Result");
	out << "The normal tester did not finish successfully.\n";
	out << "This can happen because of build/header/link errors or real test failures.\n";
	out << "\nFirst messages from the normal run:\n";
	print_prefixed_excerpt(out, normal, "  ", 40);
}

std::string	Driver::value_from_log(const std::string &label,
	const std::string &log)
{
	std::string	prefix = label + ": ";
	std::string	value;

	for (const auto &line : split_lines(log))
		if (line.rfind(prefix, 0) == 0)
			value = line.substr(prefix.size());
	return (value.empty() ? "unknown" : value);
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
	fs::path					log_file = tester_dir / "build" / "rescue" / (fn.name + ".log");
	std::vector<std::string>	cmd = {rescue_path().string(), "--suite", fn.suite,
		"--only", fn.name, "--summary-only", "--no-color"};
	CommandResult				result;

	cmd.insert(cmd.end(), args.begin(), args.end());
	result = run_process(cmd);
	write_file(log_file, result.output);
	if (result.status == 0)
	{
		out << pad(fn.name, 17) << "OK\n";
		return (0);
	}
	out << pad(fn.name, 17) << "NOK see " << log_file << "\n";
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

std::string	Driver::recommended_action()
{
	if (!fs::is_directory(root_path()))
		return ("Change ROOT_DIR");
	if (!fs::is_regular_file(makefile_path()) || !fs::is_regular_file(header_path()))
		return ("Diagnose project");
	if (!fs::is_regular_file(libft_path()))
		return ("Smart test");
	if (!fs::is_regular_file(suite_path()))
		return ("Quick test");
	return ("Full test");
}

std::string	Driver::recommendation_hint()
{
	std::string action = recommended_action();

	if (action == "Change ROOT_DIR")
		return ("The target folder does not exist.");
	if (action == "Diagnose project")
		return ("Project files are missing or incomplete.");
	if (action == "Smart test")
		return ("Build/check the project and fall back if needed.");
	if (action == "Quick test")
		return ("The internal suite is not built yet; start with fast feedback.");
	return ("Project shape looks ready for the full suite.");
}

std::string	Driver::status_file(const fs::path &path)
{
	if (fs::is_regular_file(path))
		return (green + "OK" + reset);
	return (red + "missing" + reset);
}

void	Driver::print_menu_header()
{
	std::cout << "\033[H\033[2J";
	std::cout << "\n============================================================\n";
	std::cout << " " << bold << "Libft Tester" << reset << " "
		<< dim << "(v" << g_version << ")" << reset << "\n";
	std::cout << "============================================================\n";
	std::cout << " root:      " << root_dir << "\n";
	std::cout << " Makefile:  " << status_file(makefile_path()) << "\n";
	std::cout << " libft.h:   " << status_file(header_path()) << "\n";
	std::cout << " libft.a:   " << status_file(libft_path()) << "\n";
	std::cout << " suite:     " << status_file(suite_path()) << "\n";
	std::cout << "------------------------------------------------------------\n";
	std::cout << " " << bold << "Recommended:" << reset << " "
		<< cyan << recommended_action() << reset << "\n";
	std::cout << " " << dim << recommendation_hint() << reset << "\n";
	std::cout << "============================================================\n";
}

void	Driver::print_menu()
{
	std::cout << "\n" << bold << "Recommended" << reset << "\n";
	std::cout << "  " << cyan << "1" << reset << ") Smart test      "
		<< dim << "best default, auto fallback" << reset << "\n";
	std::cout << "  " << cyan << "2" << reset << ") Quick test      "
		<< dim << "fast feedback" << reset << "\n";
	std::cout << "  " << cyan << "3" << reset << ") Full test       "
		<< dim << "normal suite" << reset << "\n";
	std::cout << "  " << cyan << "4" << reset << ") Strict test     "
		<< dim << "stronger validation" << reset << "\n\n";
	std::cout << bold << "Debugging" << reset << "\n";
	std::cout << "  " << yellow << "5" << reset << ") Diagnose project\n";
	std::cout << "  " << yellow << "6" << reset << ") Rescue test\n";
	std::cout << "  " << yellow << "7" << reset << ") Leak check\n";
	std::cout << "  " << yellow << "8" << reset << ") Explain or hint a function\n\n";
	std::cout << bold << "Reports and help" << reset << "\n";
	std::cout << "  " << green << "9" << reset << ") Generate HTML report\n";
	std::cout << "  " << green << "h" << reset << ") Advanced CLI help\n";
	std::cout << "  " << green << "r" << reset << ") Change ROOT_DIR\n";
	std::cout << "  " << green << "0" << reset << ") Exit\n\n";
	std::cout << "Choice: ";
}

template <typename F>
void	Driver::run_and_pause(F action)
{
	action();
	std::cout << "\nPress Enter to return to the menu...";
	std::string	line;
	std::getline(std::cin, line);
}

void	Driver::function_tools(const std::vector<std::string> &args)
{
	std::string	name;
	std::string	choice;

	std::cout << "Function name, for example ft_split: ";
	std::getline(std::cin, name);
	if (name.empty())
		return;
	std::cout << "\n  1) Explain tested cases\n";
	std::cout << "  2) Show debugging hint\n\nChoice: ";
	std::getline(std::cin, choice);
	if (choice == "1")
		run_and_pause([&](){ return run_suite(append(args, {"--explain", name}), std::cout); });
	else if (choice == "2")
		run_and_pause([&](){ return run_suite(append(args, {"--hint", name}), std::cout); });
	else
		run_and_pause([&](){ std::cout << "Invalid choice.\n"; return 1; });
}

void	Driver::change_root()
{
	std::string	new_root;

	std::cout << "New ROOT_DIR: ";
	std::getline(std::cin, new_root);
	if (!new_root.empty())
		root_dir = new_root;
}

int	Driver::run_leaks(std::ostream &out,
	const std::vector<std::string> &args)
{
	int						status = build_suite(out, false);
	std::vector<std::string>	cmd = {"valgrind", "--leak-check=full",
		"--show-leak-kinds=all", "--track-origins=yes",
		"--errors-for-leak-kinds=all", "--error-exitcode=42",
		suite_path().string(), "--only", "ft_split", "--no-color"};
	CommandResult			result;

	if (status != 0)
		return (status);
	cmd.insert(cmd.end(), args.begin(), args.end());
	result = run_process(cmd, fs::path(), {{"LIBFT_TESTER_NO_FORK", "1"}});
	out << result.output;
	return (result.status);
}

int	Driver::generate_html(std::ostream &out,
	const std::vector<std::string> &args)
{
	int						status = build_suite(out, false);
	std::vector<std::string>	cmd = {suite_path().string(), "--html", "--no-color"};
	CommandResult			result;
	fs::path				report = tester_dir / "libft-test-report.html";

	if (status != 0)
		return (status);
	cmd.insert(cmd.end(), args.begin(), args.end());
	result = run_process(cmd);
	write_file(report, result.output);
	out << "HTML report written to " << report << "\n";
	return (result.status);
}

int	Driver::run_menu(const std::vector<std::string> &args)
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (run_smart(std::cout, args));
	while (true)
	{
		std::string	choice;

		print_menu_header();
		print_menu();
		if (!std::getline(std::cin, choice))
			return (1);
		if (choice == "1")
			run_and_pause([&](){ return run_smart(std::cout, args); });
		else if (choice == "2")
			run_and_pause([&](){ return run_suite(append(args, {"--profile", "quick"}), std::cout); });
		else if (choice == "3")
			run_and_pause([&](){ return run_suite(args, std::cout); });
		else if (choice == "4")
			run_and_pause([&](){ return run_suite(append(args, {"--profile", "strict"}), std::cout); });
		else if (choice == "5")
			run_and_pause([&](){ return run_diagnose(std::cout); });
		else if (choice == "6")
			run_and_pause([&](){ return run_rescue(std::cout, args); });
		else if (choice == "7")
			run_and_pause([&](){ return run_leaks(std::cout, args); });
		else if (choice == "8")
			function_tools(args);
		else if (choice == "9")
			run_and_pause([&](){ return generate_html(std::cout, args); });
		else if (choice == "h" || choice == "H")
			run_and_pause([&](){ return print_help(); });
		else if (choice == "r" || choice == "R")
			change_root();
		else if (choice == "0")
		{
			std::cout << "Bye.\n";
			return (0);
		}
		else
			run_and_pause([&](){ std::cout << "Invalid choice.\n"; return 1; });
	}
}

void	Driver::pass(std::ostream &out, const std::string &label)
{
	out << "[OK]   " << label << "\n";
}

void	Driver::fail(std::ostream &out, int &failures,
	const std::string &label, const std::string &log)
{
	failures++;
	out << "[NOK]  " << label << "\n";
	if (!log.empty())
		print_prefixed_excerpt(out, log, "       ", 30);
}

void	Driver::assert_contains(std::ostream &out, int &failures,
	const std::string &needle, const std::string &label,
	const std::string &log)
{
	if (log.find(needle) != std::string::npos)
		pass(out, label);
	else
		fail(out, failures, label, "missing: " + needle + "\n" + log);
}

void	Driver::write_partial_libft(const fs::path &root)
{
	fs::create_directories(root);
	fs::copy_file(model_header(), root / "libft.h",
		fs::copy_options::overwrite_existing);
	write_file(root / "Makefile",
		"NAME=libft.a\n"
		"CC=cc\n"
		"CFLAGS=-Wall -Wextra -Werror\n"
		"AR=ar rcs\n"
		"RM=rm -f\n"
		"SRCS=ft_strlen.c\n"
		"OBJS=$(SRCS:.c=.o)\n"
		"all: $(NAME)\n"
		"$(NAME): $(OBJS)\n"
		"\t$(AR) $(NAME) $(OBJS)\n"
		"%.o: %.c libft.h\n"
		"\t$(CC) $(CFLAGS) -c $< -o $@\n"
		"clean:\n"
		"\t$(RM) $(OBJS)\n"
		"fclean: clean\n"
		"\t$(RM) $(NAME)\n"
		"re: fclean all\n"
		".PHONY: all clean fclean re\n");
	write_file(root / "ft_strlen.c",
		"#include \"libft.h\"\n\n"
		"size_t\tft_strlen(const char *s)\n"
		"{\n"
		"\tsize_t\ti;\n\n"
		"\ti = 0;\n"
		"\twhile (s[i])\n"
		"\t\ti++;\n"
		"\treturn (i);\n"
		"}\n");
}

void	Driver::write_broken_makefile_libft(const fs::path &root)
{
	fs::create_directories(root);
	write_file(root / "Makefile",
		"NAME=libft.a\n\n"
		"all:\n"
		"\t@echo \"intentional broken Makefile\"\n"
		"\t@exit 2\n\n"
		"clean:\n\n"
		"fclean: clean\n\n"
		"re: fclean all\n\n"
		".PHONY: all clean fclean re\n");
	write_file(root / "libft.h",
		"#ifndef LIBFT_H\n"
		"# define LIBFT_H\n\n"
		"# include <stddef.h>\n\n"
		"typedef struct s_list\n"
		"{\n"
		"\tvoid\t\t\t*content;\n"
		"\tstruct s_list\t*next;\n"
		"}\tt_list;\n\n"
		"size_t\tft_strlen(const char *s);\n\n"
		"#endif\n");
	write_file(root / "ft_strlen.c",
		"#include \"libft.h\"\n\n"
		"size_t\tft_strlen(const char *s)\n"
		"{\n"
		"\tsize_t\ti;\n\n"
		"\ti = 0;\n"
		"\twhile (s[i])\n"
		"\t\ti++;\n"
		"\treturn (i);\n"
		"}\n");
}

void	Driver::test_missing_header(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path			root = tmp / "missing_header";
	std::ostringstream	log;
	std::string			old_root = root_dir;
	int					status;

	fs::create_directories(root);
	fs::copy_file(model_makefile(), root / "Makefile",
		fs::copy_options::overwrite_existing);
	root_dir = root.string();
	status = run_diagnose(log);
	root_dir = old_root;
	if (status != 0)
		pass(out, "diagnose fails when libft.h is missing");
	else
		fail(out, failures, "diagnose fails when libft.h is missing", log.str());
	assert_contains(out, failures, "libft.h missing",
		"diagnose explains missing header", log.str());
}

void	Driver::test_partial_libft(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path			root = tmp / "partial_libft";
	std::ostringstream	diagnose;
	std::ostringstream	rescue;
	std::ostringstream	smart;
	std::string			old_root = root_dir;
	int					status;

	write_partial_libft(root);
	root_dir = root.string();
	status = run_diagnose(diagnose);
	if (status != 0)
		pass(out, "diagnose fails for partial libft");
	else
		fail(out, failures, "diagnose fails for partial libft", diagnose.str());
	assert_contains(out, failures, "ft_strlen        yes     yes     yes     OK",
		"diagnose sees existing ft_strlen", diagnose.str());
	assert_contains(out, failures, "ft_split         no      yes     no",
		"diagnose reports missing ft_split source/symbol", diagnose.str());
	status = run_rescue(rescue, {"--seed", "42"});
	if (status == 0)
		pass(out, "rescue mode succeeds for partial libft");
	else
		fail(out, failures, "rescue mode succeeds for partial libft", rescue.str());
	assert_contains(out, failures, "ft_strlen        OK",
		"rescue mode runs existing ft_strlen", rescue.str());
	assert_contains(out, failures, "ft_split         SKIP missing symbol",
		"rescue mode skips missing ft_split", rescue.str());
	status = run_smart(smart, {"--seed", "42"});
	if (status != 0)
		pass(out, "smart run fails when Libft is structurally incomplete");
	else
		fail(out, failures, "smart run fails when Libft is structurally incomplete", smart.str());
	assert_contains(out, failures, "Final Health Summary",
		"smart run prints final health summary", smart.str());
	assert_contains(out, failures, "Diagnose:     PROBLEMS FOUND",
		"smart run reports diagnose problems", smart.str());
	assert_contains(out, failures, "Rescue:       OK",
		"smart run still runs valid rescue checks", smart.str());
	root_dir = old_root;
}

void	Driver::test_broken_makefile(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path			root = tmp / "broken_makefile";
	std::ostringstream	smart;
	std::string			old_root = root_dir;
	int					status;

	write_broken_makefile_libft(root);
	root_dir = root.string();
	status = run_smart(smart, {"--seed", "42"});
	if (status != 0)
		pass(out, "smart run fails when Makefile cannot build libft.a");
	else
		fail(out, failures, "smart run fails when Makefile cannot build libft.a", smart.str());
	assert_contains(out, failures, "make failed",
		"smart run shows Makefile build failure", smart.str());
	assert_contains(out, failures, "Rescue:       SKIPPED, libft.a was not available",
		"smart run skips rescue when archive is missing", smart.str());
	assert_contains(out, failures, "Final Health Summary",
		"smart run summarizes broken Makefile case", smart.str());
	assert_contains(out, failures, "Libft Tester Smart Run",
		"menu non-interactive fallback prints smart run", smart.str());
	root_dir = old_root;
}

int	Driver::run_self_test(std::ostream &out)
{
	fs::path	tmp = fs::temp_directory_path()
		/ ("libft_tester_self_test_" + std::to_string(getpid()));
	int			failures = 0;

	fs::remove_all(tmp);
	fs::create_directories(tmp);
	out << "\nLibft Tester Self-Test\n";
	out << "tmp: " << tmp << "\n\n";
	test_missing_header(out, tmp, failures);
	test_partial_libft(out, tmp, failures);
	test_broken_makefile(out, tmp, failures);
	fs::remove_all(tmp);
	out << "\nSelf-test failures: " << failures << "\n";
	return (failures == 0 ? 0 : 1);
}

static fs::path	executable_dir(const char *argv0)
{
	fs::path	path(argv0);

	if (path.has_parent_path())
		return (fs::weakly_canonical(path.parent_path()));
	return (fs::current_path());
}

static std::vector<std::string>	parse_args(int argc, char **argv,
	std::string &root)
{
	std::vector<std::string>	args;
	int						i = 1;

	while (i < argc)
	{
		std::string value(argv[i]);
		if (value == "--root" && i + 1 < argc)
		{
			root = argv[i + 1];
			i += 2;
		}
		else if (value.rfind("--root=", 0) == 0)
		{
			root = value.substr(7);
			i++;
		}
		else
		{
			args.push_back(value);
			i++;
		}
	}
	return (args);
}

int	main(int argc, char **argv)
{
	std::string					root = "..";
	std::vector<std::string>	args = parse_args(argc, argv, root);
	Driver						driver(executable_dir(argv[0]), root);

	if (args.empty())
		return (driver.run_menu(args));
	if (args[0] == "--version")
	{
		std::cout << "libft_tester " << g_version << "\n";
		return (0);
	}
	if (args[0] == "--help")
		return (driver.print_help());
	if (args[0] == "--menu")
		return (driver.run_menu(std::vector<std::string>(args.begin() + 1, args.end())));
	if (args[0] == "--smart")
		return (driver.run_smart(std::cout, std::vector<std::string>(args.begin() + 1, args.end())));
	if (args[0] == "--diagnose")
		return (driver.run_diagnose(std::cout));
	if (args[0] == "--rescue")
		return (driver.run_rescue(std::cout, std::vector<std::string>(args.begin() + 1, args.end())));
	if (args[0] == "--self-test")
		return (driver.run_self_test(std::cout));
	return (driver.run_suite(args, std::cout));
}
