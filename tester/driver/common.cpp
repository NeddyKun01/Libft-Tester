#include "driver.hpp"

const char	*g_version = "1.7.0";

const std::vector<FunctionInfo>	&functions()
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

std::string	join_args(const std::vector<std::string> &args)
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

std::vector<std::string>	split_lines(const std::string &text)
{
	std::vector<std::string>	lines;
	std::istringstream		input(text);
	std::string				line;

	while (std::getline(input, line))
		lines.push_back(line);
	return (lines);
}

std::string	read_file(const fs::path &path)
{
	std::ifstream		file(path);
	std::ostringstream	buffer;

	buffer << file.rdbuf();
	return (buffer.str());
}

bool	write_file(const fs::path &path, const std::string &content)
{
	std::ofstream	file(path);

	if (!file)
		return (false);
	file << content;
	return (true);
}

std::string	last_field(const std::string &line)
{
	std::istringstream	input(line);
	std::string			field;
	std::string			last;

	while (input >> field)
		last = field;
	return (last);
}

CommandResult	run_process(const std::vector<std::string> &args,
	const fs::path &cwd, const std::map<std::string, std::string> &env)
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
fs::path	Driver::model_header() const { return (tester_dir / "tester" / "templates" / "libft.h"); }
fs::path	Driver::model_makefile() const { return (tester_dir / "tester" / "templates" / "Makefile"); }

static std::string	root_hash(const std::string &root)
{
	unsigned long long	hash = 1469598103934665603ULL;
	std::ostringstream	out;

	for (unsigned char c : root)
	{
		hash ^= c;
		hash *= 1099511628211ULL;
	}
	out << hash;
	return (out.str());
}

fs::path	Driver::build_path() const
{
	const char	*self_test;

	self_test = std::getenv("LIBFT_TESTER_SELF_TEST");
	if (self_test && std::string(self_test) == "1")
		return (tester_dir / "tester" / "build-self-test" / root_hash(root_dir));
	return (tester_dir / "tester" / "build" / root_hash(root_dir));
}

fs::path	Driver::suite_path() const { return (build_path() / "libft_suite"); }
fs::path	Driver::rescue_path() const { return (build_path() / "rescue" / "libft_suite_rescue"); }

std::map<std::string, std::string>	Driver::suite_env() const
{
	std::map<std::string, std::string>	env;

	env["LIBFT_TESTER_ROOT"] = root_dir;
	return (env);
}

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
		<< "  ./libft_tester --root ../libft\n"
		<< "  ./libft_tester --root ../libft --summary-only\n"
		<< "  make build\n\n"
		<< "Driver options:\n"
		<< "  --root PATH         Target Libft directory, default: ..\n"
		<< "  --config PATH       Optional .libft-tester.json path\n"
		<< "  --menu              Open the interactive menu\n"
		<< "  --smart             Run normal tests, then diagnose/rescue if needed\n"
		<< "  --diagnose          Check Makefile, header, sources, and symbols\n"
		<< "  --rescue            Test only symbols found in libft.a\n"
		<< "  --doctor            Check tester tools and target project shape\n"
		<< "  --self-test         Validate tester fallback behavior\n"
		<< "  --version           Show tester version\n"
		<< "  --help              Show this help message\n\n"
		<< "Info options, no target build needed:\n"
		<< "  --coverage, --coverage-md\n"
		<< "  --explain NAME, --hint NAME\n\n"
		<< "Optional config file:\n"
		<< "  .libft-tester.json may define root, profile, seed, and no_color.\n"
		<< "  CLI options always override config values.\n\n"
		<< "Test options, forwarded after building the internal suite:\n"
		<< "  --profile quick|normal|strict|brutal\n"
		<< "  --summary-only, --review, --only NAME, --suite NAME, --json, --html\n"
		<< "  --fail-fast, --seed N, --repeat N, --verbose\n\n"
		<< "Recommended examples:\n"
		<< "  ./libft_tester --root ../libft\n"
		<< "  ./libft_tester --root ../libft --only ft_split --verbose\n"
		<< "  ./libft_tester --root ../libft --profile strict --seed 42\n"
		<< "  ./libft_tester --hint ft_split\n"
		<< "  ./libft_tester --coverage\n";
	return (0);
}

int	Driver::build_suite(std::ostream &out, bool rescue)
{
	std::vector<std::string>	cmd = {
		"make", "-s", "-j4", "-C", tester_dir.string(),
		rescue ? "rescue-suite" : "suite",
		"ROOT_DIR=" + root_dir,
		"BUILD_DIR=" + build_path().string()
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
	CommandResult result = run_process(with_args({suite_path().string()}, args),
		fs::path(), suite_env());
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
