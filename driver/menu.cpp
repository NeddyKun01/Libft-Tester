#include "driver.hpp"

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
	std::cout << "  " << yellow << "8" << reset << ") Explain or hint a function\n";
	std::cout << "  " << yellow << "d" << reset << ") Doctor environment\n\n";
	std::cout << bold << "Reports and help" << reset << "\n";
	std::cout << "  " << green << "9" << reset << ") Generate HTML report\n";
	std::cout << "  " << green << "h" << reset << ") Advanced CLI help\n";
	std::cout << "  " << green << "r" << reset << ") Change ROOT_DIR\n";
	std::cout << "  " << green << "0" << reset << ") Exit\n\n";
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
