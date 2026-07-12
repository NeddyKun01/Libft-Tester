#include "driver.hpp"

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
	if (args[0] == "--doctor")
		return (driver.run_doctor(std::cout));
	if (args[0] == "--self-test")
		return (driver.run_self_test(std::cout));
	return (driver.run_suite(args, std::cout));
}
