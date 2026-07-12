#include "driver.hpp"

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

void	Driver::write_bad_strlen_libft(const fs::path &root)
{
	write_partial_libft(root);
	write_file(root / "ft_strlen.c",
		"#include \"libft.h\"\n\n"
		"size_t\tft_strlen(const char *s)\n"
		"{\n"
		"\t(void)s;\n"
		"\treturn (0);\n"
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

void	Driver::test_rescue_failure_output(std::ostream &out,
	const fs::path &tmp, int &failures)
{
	fs::path			root = tmp / "bad_strlen";
	std::ostringstream	rescue;
	std::string			old_root = root_dir;
	int					status;

	write_bad_strlen_libft(root);
	run_process({"make", "-s", "-C", root.string(), "fclean"});
	run_process({"make", "-s", "-C", root.string()});
	root_dir = root.string();
	status = run_rescue(rescue, {"--seed", "42"});
	root_dir = old_root;
	if (status != 0)
		pass(out, "rescue mode fails when an existing function is wrong");
	else
		fail(out, failures, "rescue mode fails when an existing function is wrong",
			rescue.str());
	assert_contains(out, failures, "ft_strlen        NOK",
		"rescue mode marks the broken function as NOK", rescue.str());
	assert_contains(out, failures, "Failure details: ft_strlen",
		"rescue mode prints inline failure details", rescue.str());
	assert_contains(out, failures, "Failure Details",
		"rescue mode includes the detailed failed checks", rescue.str());
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

void	Driver::test_doctor(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path			root;
	std::ostringstream	log;
	std::string			old_root = root_dir;
	int					status;

	root = tmp / "doctor_missing_root";
	root_dir = root.string();
	status = run_doctor(log);
	root_dir = old_root;
	if (status != 0)
		pass(out, "doctor fails when ROOT_DIR is missing");
	else
		fail(out, failures, "doctor fails when ROOT_DIR is missing", log.str());
	assert_contains(out, failures, "ROOT_DIR does not exist",
		"doctor explains missing ROOT_DIR", log.str());
	assert_contains(out, failures, "Fix: pass the correct target path",
		"doctor suggests fixing ROOT_DIR", log.str());
	assert_contains(out, failures, "Next action: fix required errors",
		"doctor gives next action for missing ROOT_DIR", log.str());
	log.str("");
	log.clear();
	root = tmp / "doctor_missing_makefile";
	fs::create_directories(root);
	fs::copy_file(model_header(), root / "libft.h",
		fs::copy_options::overwrite_existing);
	root_dir = root.string();
	status = run_doctor(log);
	root_dir = old_root;
	if (status != 0)
		pass(out, "doctor fails when Makefile is missing");
	else
		fail(out, failures, "doctor fails when Makefile is missing", log.str());
	assert_contains(out, failures, "Makefile missing",
		"doctor explains missing Makefile", log.str());
	assert_contains(out, failures, "Fix: restore the target Makefile",
		"doctor suggests fixing missing Makefile", log.str());
	log.str("");
	log.clear();
	root = tmp / "doctor_missing_header";
	fs::create_directories(root);
	fs::copy_file(model_makefile(), root / "Makefile",
		fs::copy_options::overwrite_existing);
	root_dir = root.string();
	status = run_doctor(log);
	root_dir = old_root;
	if (status != 0)
		pass(out, "doctor fails when libft.h is missing");
	else
		fail(out, failures, "doctor fails when libft.h is missing", log.str());
	assert_contains(out, failures, "libft.h missing",
		"doctor explains missing libft.h", log.str());
	assert_contains(out, failures, "Fix: restore the target header",
		"doctor suggests fixing missing libft.h", log.str());
	log.str("");
	log.clear();
	root = tmp / "doctor_partial_libft";
	write_partial_libft(root);
	root_dir = root.string();
	status = run_doctor(log);
	root_dir = old_root;
	if (status == 0)
		pass(out, "doctor accepts missing libft.a as warning");
	else
		fail(out, failures, "doctor accepts missing libft.a as warning", log.str());
	assert_contains(out, failures, "libft.a missing",
		"doctor reports missing libft.a warning", log.str());
	assert_contains(out, failures, "Fix: run `make` inside ROOT_DIR",
		"doctor suggests building missing libft.a", log.str());
	assert_contains(out, failures, "Doctor verdict: OK",
		"doctor keeps warning-only projects usable", log.str());
	assert_contains(out, failures, "Next action: you can run `make ROOT_DIR=",
		"doctor gives next action for warnings", log.str());
	log.str("");
	log.clear();
	root = tmp / "doctor_valid_libft";
	write_partial_libft(root);
	run_process({"make", "-s", "-C", root.string()});
	root_dir = root.string();
	status = run_doctor(log);
	root_dir = old_root;
	if (status == 0)
		pass(out, "doctor passes when required project files exist");
	else
		fail(out, failures, "doctor passes when required project files exist", log.str());
	assert_contains(out, failures, "libft.a found",
		"doctor sees generated libft.a", log.str());
	assert_contains(out, failures, "Next action: you can run `make ROOT_DIR=",
		"doctor still suggests safe next action with optional warnings", log.str());
}

int	Driver::run_self_test(std::ostream &out)
{
	fs::path	tmp = fs::temp_directory_path()
		/ ("libft_tester_self_test_" + std::to_string(getpid()));
	int			failures = 0;
	const char	*old_mode = std::getenv("LIBFT_TESTER_SELF_TEST");
	std::string	old_value;

	if (old_mode)
		old_value = old_mode;
	setenv("LIBFT_TESTER_SELF_TEST", "1", 1);
	fs::remove_all(tmp);
	fs::create_directories(tmp);
	out << "\nLibft Tester Self-Test\n";
	out << "tmp: " << tmp << "\n\n";
	test_missing_header(out, tmp, failures);
	test_partial_libft(out, tmp, failures);
	test_rescue_failure_output(out, tmp, failures);
	test_broken_makefile(out, tmp, failures);
	test_doctor(out, tmp, failures);
	fs::remove_all(tmp);
	if (old_mode)
		setenv("LIBFT_TESTER_SELF_TEST", old_value.c_str(), 1);
	else
		unsetenv("LIBFT_TESTER_SELF_TEST");
	out << "\nSelf-test failures: " << failures << "\n";
	return (failures == 0 ? 0 : 1);
}
