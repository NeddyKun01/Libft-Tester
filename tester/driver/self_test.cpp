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

void	Driver::assert_not_contains(std::ostream &out, int &failures,
	const std::string &needle, const std::string &label,
	const std::string &log)
{
	if (log.find(needle) == std::string::npos)
		pass(out, label);
	else
		fail(out, failures, label, "unexpected: " + needle + "\n" + log);
}

void	Driver::assert_in_order(std::ostream &out, int &failures,
	const std::vector<std::string> &needles, const std::string &label,
	const std::string &log)
{
	size_t	position;
	size_t	i;

	position = 0;
	i = 0;
	while (i < needles.size())
	{
		position = log.find(needles[i], position);
		if (position == std::string::npos)
		{
			fail(out, failures, label, "missing in order: " + needles[i]
				+ "\n" + log);
			return ;
		}
		position += needles[i].size();
		i++;
	}
	pass(out, label);
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

void	Driver::write_html_report_libft(const fs::path &root, bool bad_strlen)
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
		"SRCS=ft_strlen.c ft_split.c\n"
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
	if (bad_strlen)
		write_file(root / "ft_strlen.c",
			"#include \"libft.h\"\n\n"
			"size_t\tft_strlen(const char *s)\n"
			"{\n"
			"\t(void)s;\n"
			"\treturn (0);\n"
			"}\n");
	else
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
	write_file(root / "ft_split.c",
		"#include \"libft.h\"\n\n"
		"char\t**ft_split(char const *s, char c)\n"
		"{\n"
		"\t(void)s;\n"
		"\t(void)c;\n"
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

void	Driver::test_terminal_output_contract(std::ostream &out,
	const fs::path &tmp, int &failures)
{
	fs::path			root = tmp / "terminal_contract";
	std::ostringstream	log;
	std::string			old_root = root_dir;
	int					status;

	write_html_report_libft(root, true);
	root_dir = root.string();
	status = run_suite({"--only", "ft_strlen", "--no-color", "--seed", "42"},
			log);
	root_dir = old_root;
	if (status != 0)
		pass(out, "terminal output contract uses a failing fixture");
	else
		fail(out, failures, "terminal output contract uses a failing fixture",
			log.str());
	assert_contains(out, failures, "Function             OK/Total",
		"terminal output labels scores as OK over total", log.str());
	assert_contains(out, failures, "ft_strlen",
		"terminal output includes the failing function name", log.str());
	assert_contains(out, failures, "2/13",
		"terminal output prints passed checks over total checks", log.str());
	assert_not_contains(out, failures, "Function             Score",
		"terminal output does not use the old score header", log.str());
	assert_in_order(out, failures,
		{"Results", "Failure Details", "Debug Focus", "Summary"},
		"terminal output keeps the debugging sections in order", log.str());
}

void	Driver::test_review_output(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path			root = tmp / "review_contract";
	std::ostringstream	log;
	std::string			old_root = root_dir;
	int					status;

	write_html_report_libft(root, true);
	root_dir = root.string();
	status = run_suite({"--only", "ft_strlen", "--review", "--no-color",
			"--seed", "42"}, log);
	root_dir = old_root;
	if (status != 0)
		pass(out, "review mode returns failing status for failing fixture");
	else
		fail(out, failures, "review mode returns failing status for failing fixture",
			log.str());
	assert_contains(out, failures, "Libft Tester Review",
		"review mode prints review header", log.str());
	assert_contains(out, failures, "verdict: FAIL",
		"review mode prints compact verdict", log.str());
	assert_contains(out, failures, "score: 2/13",
		"review mode prints passed checks over total checks", log.str());
	assert_contains(out, failures, "failed functions: ft_strlen",
		"review mode lists failed functions", log.str());
	assert_contains(out, failures, "crash functions: none",
		"review mode reports no crashes when none happened", log.str());
	assert_contains(out, failures, "Reproduce first failures",
		"review mode prints focused rerun section", log.str());
	assert_contains(out, failures, "--only ft_strlen --verbose --seed 42",
		"review mode includes reproducible rerun command", log.str());
}

void	Driver::test_html_report_output(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path			pass_root = tmp / "html_report_pass";
	fs::path			fail_root = tmp / "html_report_fail";
	std::ostringstream	html;
	std::string			old_root = root_dir;
	int					status;

	write_html_report_libft(pass_root, false);
	root_dir = pass_root.string();
	status = run_suite({"--only", "ft_strlen", "--web", "--no-color",
			"--seed", "42"}, html);
	if (status == 0)
		pass(out, "html report succeeds for passing function output");
	else
		fail(out, failures, "html report succeeds for passing function output",
			html.str());
	assert_contains(out, failures, "Libft Tester Web Dashboard",
		"web report includes dashboard hero", html.str());
	assert_contains(out, failures, "Function Overview",
		"web report includes function overview", html.str());
	assert_contains(out, failures, "id=\"functionSearch\"",
		"web report includes function search", html.str());
	assert_contains(out, failures, "OK/Total",
		"web report labels scores as OK over total", html.str());
	assert_contains(out, failures, "PASS</strong> - 100% pass rate",
		"web report includes pass rate hero", html.str());
	assert_contains(out, failures, "Passed</span><strong class=\"pass\">13/13",
		"web report shows passed checks over total checks", html.str());
	assert_contains(out, failures, "id=\"score-guide\"",
		"web report explains score direction", html.str());
	assert_contains(out, failures, "filterReport('passed')",
		"web report includes a passing filter button", html.str());
	assert_contains(out, failures, "filterReport('malloc')",
		"web report includes a malloc filter button", html.str());
	assert_contains(out, failures, "filterReport('crash')",
		"web report includes a crash filter button", html.str());
	assert_contains(out, failures, "data-result=\"passed\"",
		"web report marks passing rows and cards for filtering", html.str());
	assert_contains(out, failures, "data-tags=\"passed\"",
		"web report marks passing filter tags", html.str());
	assert_contains(out, failures, "<details class=\"card\"",
		"web report uses collapsible function cards", html.str());
	html.str("");
	html.clear();
	write_html_report_libft(fail_root, true);
	root_dir = fail_root.string();
	status = run_suite({"--only", "ft_strlen", "--html", "--no-color",
			"--seed", "42"}, html);
	root_dir = old_root;
	if (status != 0)
		pass(out, "html report fails when selected function fails");
	else
		fail(out, failures, "html report fails when selected function fails",
			html.str());
	assert_contains(out, failures, "Debug Focus",
		"html report includes debug focus for failures", html.str());
	assert_contains(out, failures, "id=\"failures\"",
		"html report includes failure jump target", html.str());
	assert_contains(out, failures, "Copy commands",
		"html report includes copy command control", html.str());
	assert_contains(out, failures, "./libft_tester --root '",
		"html report includes copyable root-aware command", html.str());
	assert_contains(out, failures, "filterReport('failed')",
		"html report includes a failure filter button", html.str());
	assert_contains(out, failures, "data-result=\"failed\"",
		"html report marks failing rows and cards for filtering", html.str());
	assert_contains(out, failures, "data-tags=\"failed\"",
		"html report marks failing filter tags", html.str());
	assert_contains(out, failures, "Likely Fixes",
		"html report includes likely fixes section", html.str());
	assert_contains(out, failures, "Failed</span><strong class=\"fail\">11/13",
		"html report shows failed checks over total checks", html.str());
	assert_contains(out, failures, "NOK",
		"html report includes failed status labels", html.str());
}

void	Driver::test_config_file(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path		root = tmp / "config_libft";
	fs::path		config = tmp / "libft-tester-config.json";
	CommandResult	result;

	write_html_report_libft(root, false);
	write_file(config,
		"{\n"
		"  \"root\": \"" + root.string() + "\",\n"
		"  \"preset\": \"review\",\n"
		"  \"profile\": \"quick\",\n"
		"  \"seed\": 42,\n"
		"  \"no_color\": true\n"
		"}\n");
	result = run_process({(tester_dir / "libft_tester").string(),
			"--config", config.string(), "--only", "ft_strlen"});
	if (result.status == 0)
		pass(out, "config file can provide root and suite defaults");
	else
		fail(out, failures, "config file can provide root and suite defaults",
			result.output);
	assert_contains(out, failures, "Libft Tester Review",
		"config file run reaches review mode", result.output);
	assert_contains(out, failures, "profile: quick | seed: 42",
		"config file applies profile and seed defaults", result.output);
	assert_contains(out, failures, "verdict: PASS",
		"config file run tests the configured target", result.output);
}

void	Driver::test_presets(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path		root = tmp / "preset_libft";
	CommandResult	result;

	write_html_report_libft(root, false);
	result = run_process({(tester_dir / "libft_tester").string(),
			"--root", root.string(), "--preset", "review", "--only",
			"ft_strlen"});
	if (result.status == 0)
		pass(out, "review preset runs successfully");
	else
		fail(out, failures, "review preset runs successfully", result.output);
	assert_contains(out, failures, "Libft Tester Review",
		"review preset enables review output", result.output);
	assert_contains(out, failures, "profile: normal | seed: 42",
		"review preset applies deterministic seed", result.output);
	result = run_process({(tester_dir / "libft_tester").string(),
			"--root", root.string(), "--preset=school", "--only",
			"ft_strlen", "--review"});
	if (result.status == 0)
		pass(out, "school preset runs successfully");
	else
		fail(out, failures, "school preset runs successfully", result.output);
	assert_contains(out, failures, "profile: strict | seed: 42",
		"school preset applies strict profile", result.output);
	result = run_process({(tester_dir / "libft_tester").string(), "--presets"});
	if (result.status == 0)
		pass(out, "preset listing command succeeds");
	else
		fail(out, failures, "preset listing command succeeds", result.output);
	assert_contains(out, failures, "review",
		"preset listing includes review preset", result.output);
	assert_contains(out, failures, "school",
		"preset listing includes school preset", result.output);
	assert_contains(out, failures, "web",
		"preset listing includes web preset", result.output);
}

void	Driver::test_compare(std::ostream &out, const fs::path &tmp,
	int &failures)
{
	fs::path		good_root = tmp / "compare_good";
	fs::path		bad_root = tmp / "compare_bad";
	std::string		old_root = root_dir;
	std::ostringstream	same;
	std::ostringstream	different;
	int				status;

	write_html_report_libft(good_root, false);
	write_html_report_libft(bad_root, true);
	root_dir = good_root.string();
	status = run_compare(same, good_root.string(), {"--only", "ft_strlen",
			"--seed", "42"});
	if (status == 0)
		pass(out, "compare mode passes for matching roots");
	else
		fail(out, failures, "compare mode passes for matching roots", same.str());
	assert_contains(out, failures, "Compare verdict: MATCHING PASS",
		"compare mode reports matching pass", same.str());
	status = run_compare(different, bad_root.string(), {"--only", "ft_strlen",
			"--seed", "42"});
	root_dir = old_root;
	if (status != 0)
		pass(out, "compare mode fails for different roots");
	else
		fail(out, failures, "compare mode fails for different roots",
			different.str());
	assert_contains(out, failures, "Compare verdict: DIFFERENT",
		"compare mode reports different verdict", different.str());
	assert_contains(out, failures, "ft_strlen",
		"compare mode lists differing function", different.str());
	assert_contains(out, failures, "Left : PASS",
		"compare mode shows left summary", different.str());
	assert_contains(out, failures, "Right: FAIL",
		"compare mode shows right summary", different.str());
	CommandResult result = run_process({(tester_dir / "libft_tester").string(),
			"--root", good_root.string(), "--preset", "review", "--compare",
			good_root.string(), "--only", "ft_strlen"});
	if (result.status == 0)
		pass(out, "compare mode works after preset expansion");
	else
		fail(out, failures, "compare mode works after preset expansion",
			result.output);
	assert_contains(out, failures, "Compare verdict: MATCHING PASS",
		"compare mode after preset reports matching pass", result.output);
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
	test_terminal_output_contract(out, tmp, failures);
	test_review_output(out, tmp, failures);
	test_html_report_output(out, tmp, failures);
	test_config_file(out, tmp, failures);
	test_presets(out, tmp, failures);
	test_compare(out, tmp, failures);
	test_doctor(out, tmp, failures);
	fs::remove_all(tmp);
	if (old_mode)
		setenv("LIBFT_TESTER_SELF_TEST", old_value.c_str(), 1);
	else
		unsetenv("LIBFT_TESTER_SELF_TEST");
	out << "\nSelf-test failures: " << failures << "\n";
	return (failures == 0 ? 0 : 1);
}
