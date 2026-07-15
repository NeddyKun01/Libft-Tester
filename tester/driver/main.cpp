#include "driver.hpp"
#include "coverage.hpp"
#include "hints.hpp"

static fs::path	executable_dir(const char *argv0)
{
	fs::path	path(argv0);

	if (path.has_parent_path())
		return (fs::weakly_canonical(path.parent_path()));
	return (fs::current_path());
}

struct DriverConfig
{
	std::string	path;
	std::string	root;
	std::string	profile;
	std::string	seed;
	bool		has_root = false;
	bool		has_profile = false;
	bool		has_seed = false;
	bool		no_color = false;
};

static bool	json_string(const std::string &content, const std::string &key,
	std::string &value)
{
	std::regex	pattern("\"" + key + "\"[ \t\r\n]*:[ \t\r\n]*\"([^\"]*)\"");
	std::smatch	match;

	if (!std::regex_search(content, match, pattern))
		return (false);
	value = match[1].str();
	return (true);
}

static bool	json_bool(const std::string &content, const std::string &key,
	bool &value)
{
	std::regex	pattern("\"" + key + "\"[ \t\r\n]*:[ \t\r\n]*(true|false)");
	std::smatch	match;

	if (!std::regex_search(content, match, pattern))
		return (false);
	value = (match[1].str() == "true");
	return (true);
}

static bool	json_number(const std::string &content, const std::string &key,
	std::string &value)
{
	std::regex	pattern("\"" + key + "\"[ \t\r\n]*:[ \t\r\n]*([0-9]+)");
	std::smatch	match;

	if (!std::regex_search(content, match, pattern))
		return (false);
	value = match[1].str();
	return (true);
}

static fs::path	find_config_path(const fs::path &tester_dir)
{
	fs::path	local = fs::current_path() / ".libft-tester.json";
	fs::path	inside = tester_dir / ".libft-tester.json";

	if (fs::exists(local))
		return (local);
	if (fs::exists(inside))
		return (inside);
	return (fs::path());
}

static DriverConfig	load_config(const fs::path &path)
{
	DriverConfig	config;
	std::string		content;

	if (path.empty() || !fs::exists(path))
		return (config);
	content = read_file(path);
	config.path = path.string();
	config.has_root = json_string(content, "root", config.root);
	config.has_profile = json_string(content, "profile", config.profile);
	config.has_seed = json_number(content, "seed", config.seed);
	json_bool(content, "no_color", config.no_color);
	return (config);
}

static bool	arg_has_option(const std::vector<std::string> &args,
	const std::string &name)
{
	size_t	i;

	i = 0;
	while (i < args.size())
	{
		if (args[i] == name || args[i].rfind(name + "=", 0) == 0)
			return (true);
		i++;
	}
	return (false);
}

static void	apply_config_args(std::vector<std::string> &args,
	const DriverConfig &config)
{
	if (args.empty())
		return ;
	if (config.has_profile && !arg_has_option(args, "--profile")
		&& !arg_has_option(args, "--strict"))
	{
		args.push_back("--profile");
		args.push_back(config.profile);
	}
	if (config.has_seed && !arg_has_option(args, "--seed"))
	{
		args.push_back("--seed");
		args.push_back(config.seed);
	}
	if (config.no_color && !arg_has_option(args, "--no-color"))
		args.push_back("--no-color");
}

static std::vector<std::string>	parse_args(int argc, char **argv,
	std::string &root, std::string &config_path, bool &root_set)
{
	std::vector<std::string>	args;
	int						i = 1;

	while (i < argc)
	{
		std::string value(argv[i]);
		if (value == "--root" && i + 1 < argc)
		{
			root = argv[i + 1];
			root_set = true;
			i += 2;
		}
		else if (value.rfind("--root=", 0) == 0)
		{
			root = value.substr(7);
			root_set = true;
			i++;
		}
		else if (value == "--config" && i + 1 < argc)
		{
			config_path = argv[i + 1];
			i += 2;
		}
		else if (value.rfind("--config=", 0) == 0)
		{
			config_path = value.substr(9);
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
	fs::path					tester_dir = executable_dir(argv[0]);
	std::string					root = "..";
	std::string					config_path;
	bool						root_set = false;
	std::vector<std::string>	args;
	DriverConfig				config;
	Driver						driver(tester_dir, root);

	args = parse_args(argc, argv, root, config_path, root_set);
	if (config_path.empty())
		config = load_config(find_config_path(tester_dir));
	else
	{
		if (!fs::exists(config_path))
		{
			std::cerr << "Config file not found: " << config_path << "\n";
			return (2);
		}
		config = load_config(config_path);
	}
	if (config.has_root && !root_set)
		root = config.root;
	if (config.no_color)
		setenv("NO_COLOR", "1", 1);
	apply_config_args(args, config);
	driver = Driver(tester_dir, root);

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
	if (args[0] == "--doctor")
		return (driver.run_doctor(std::cout));
	if (args[0] == "--self-test")
		return (driver.run_self_test(std::cout));
	if (args[0] == "--coverage")
	{
		coverage::print_table();
		return (0);
	}
	if (args[0] == "--coverage-md")
	{
		coverage::print_markdown();
		return (0);
	}
	if (args[0] == "--explain" && args.size() > 1)
		return (coverage::print_explain(args[1]) ? 0 : 1);
	if (args[0] == "--hint" && args.size() > 1)
		return (hints::print_for_function(args[1]) ? 0 : 1);
	return (driver.run_suite(args, std::cout));
}
