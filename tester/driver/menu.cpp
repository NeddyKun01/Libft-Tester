#include "driver.hpp"
#include "coverage.hpp"
#include "hints.hpp"

#include <iomanip>

static std::string	menu_shell_quote(const std::string &value)
{
	std::string	quoted;
	size_t		i;

	quoted = "'";
	i = 0;
	while (i < value.size())
	{
		if (value[i] == '\'')
			quoted += "'\\''";
		else
			quoted += value[i];
		i++;
	}
	quoted += "'";
	return (quoted);
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

static std::string	recommended_key(const std::string &action)
{
	if (action == "Change ROOT_DIR")
		return ("r");
	if (action == "Diagnose project")
		return ("5");
	if (action == "Smart test")
		return ("1");
	if (action == "Quick test")
		return ("2");
	return ("3");
}

std::string	Driver::recommended_command()
{
	std::string	action;
	std::string	root;

	action = recommended_action();
	root = menu_shell_quote(root_dir);
	if (action == "Change ROOT_DIR")
		return ("press r, then enter the correct Libft path");
	if (action == "Diagnose project")
		return ("./libft_tester --root " + root + " --diagnose");
	if (action == "Smart test")
		return ("./libft_tester --root " + root + " --smart");
	if (action == "Quick test")
		return ("./libft_tester --root " + root + " --profile quick");
	return ("./libft_tester --root " + root);
}

std::string	Driver::status_file(const fs::path &path)
{
	if (fs::is_regular_file(path))
		return (green + "OK" + reset);
	return (red + "missing" + reset);
}

void	Driver::print_menu_item(const std::string &key,
	const std::string &label, const std::string &hint,
	const std::string &accent)
{
	std::cout << "  " << accent << key << reset << ") "
		<< std::left << std::setw(22) << label
		<< dim << hint << reset << "\n";
}

void	Driver::print_menu_header()
{
	std::string	action;
	std::string	key;

	action = recommended_action();
	key = recommended_key(action);
	std::cout << "\033[H\033[2J";
	std::cout << "\n" << cyan
		<< "+------------------------------------------------------------+\n"
		<< "|                        LIBFT TESTER                        |\n"
		<< "+------------------------------------------------------------+\n"
		<< reset;
	std::cout << " " << bold << "Libft Tester" << reset << " "
		<< dim << "(v" << g_version << ")" << reset << "\n";
	std::cout << " target:    " << root_dir << "\n";
	std::cout << " health:    Makefile " << status_file(makefile_path())
		<< "  libft.h " << status_file(header_path())
		<< "  libft.a " << status_file(libft_path())
		<< "  suite " << status_file(suite_path()) << "\n";
	std::cout << "------------------------------------------------------------\n";
	std::cout << " recommended: [" << cyan << key << reset << "] "
		<< bold << action << reset << "\n";
	std::cout << " why:         " << dim << recommendation_hint()
		<< reset << "\n";
	std::cout << " CLI:         " << dim << recommended_command()
		<< reset << "\n";
	std::cout << "------------------------------------------------------------\n";
}

void	Driver::print_menu()
{
	std::cout << "\n" << bold << "Test Runs" << reset << "\n";
	print_menu_item("1", "Smart test", "best first run, auto fallback", cyan);
	print_menu_item("2", "Quick test", "fast feedback while coding", cyan);
	print_menu_item("3", "Full test", "normal complete suite", cyan);
	print_menu_item("4", "Strict test", "deeper validation before sharing", cyan);
	std::cout << "\n" << bold << "Fix And Inspect" << reset << "\n";
	print_menu_item("5", "Diagnose project", "Makefile/header/source checks", yellow);
	print_menu_item("6", "Rescue test", "test valid symbols even if incomplete", yellow);
	print_menu_item("7", "Leak check", "focused Valgrind run", yellow);
	print_menu_item("8", "Explain or hint", "coverage notes or debugging hint", yellow);
	print_menu_item("d", "Doctor environment", "tools and target sanity check", yellow);
	std::cout << "\n" << bold << "Reports And Setup" << reset << "\n";
	print_menu_item("9", "Generate HTML report", "standalone visual report", green);
	print_menu_item("10", "Review summary", "compact reviewer output", green);
	print_menu_item("h", "Advanced CLI help", "all command-line options", green);
	print_menu_item("r", "Change ROOT_DIR", "point tester at another Libft", green);
	print_menu_item("0", "Exit", "close the tester", green);
	std::cout << "\n";
	std::cout << "Choice: ";
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
		run_and_pause([&](){ (void)args; return (coverage::print_explain(name) ? 0 : 1); });
	else if (choice == "2")
		run_and_pause([&](){ (void)args; return (hints::print_for_function(name) ? 0 : 1); });
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
	result = run_process(cmd, fs::path(), {
		{"LIBFT_TESTER_NO_FORK", "1"},
		{"LIBFT_TESTER_ROOT", root_dir}
	});
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
	result = run_process(cmd, fs::path(), suite_env());
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
		else if (choice == "10")
			run_and_pause([&](){ return run_suite(append(args, {"--review"}), std::cout); });
		else if (choice == "d" || choice == "D")
			run_and_pause([&](){ return run_doctor(std::cout); });
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
