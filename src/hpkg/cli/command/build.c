#include "build.h"
#include "muggle/c/muggle_c.h"

#define HPKG_BUILD_ARG_TASK_NAME 1001
#define HPKG_BUILD_ARG_TASK_ID 1002
#define HPKG_BUILD_ARG_WORKDIR 1002

int hpkg_cli_run_build(int argc, char **argv)
{
	static const char *str_usage =
		"Options: \n"
		"-c, --config string     [REQUIRED] build config file\n"
		"-m, --mode string       [OPTIONAL] dev or task, use dev by default\n"
		"  , --task-name string  [OPTIONAL] build task name, if empty, use value of 'name' field in config file\n"
		"  , --task-id string    [OPTIONAL] build task id, if empty, set 'yyyymmddHHMMSSxxxx' as task-id\n"
		"  , --work-dir string   [OPTIONAL] working directory(by default, use current working directory)\n"
		"-p, --param list        [OPTIONAL] build parameters, e.g. --params foo=123 -p bar=456\n"
		"-s, --settings string   [OPTIONAL] manual set settings.xml\n";

	int c;
	while (1) {
		int option_index = 0;
		static struct option long_options[] = {
			{ "config", required_argument, NULL, 'c' },
			{ "mode", required_argument, NULL, 'm' },
			{ "task-name", required_argument, NULL, HPKG_BUILD_ARG_TASK_NAME },
			{ "task-id", required_argument, NULL, HPKG_BUILD_ARG_TASK_ID },
			{ "work-dir", required_argument, NULL, HPKG_BUILD_ARG_WORKDIR },
			{ "param", required_argument, NULL, 'p' },
			{ "settings", required_argument, NULL, 's' },
			{ NULL, 0, NULL, 0 }
		};

		c = getopt_long(argc, argv, "c:", long_options, &option_index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 'c': {
			// TODO:
		} break;
		case 'm': {
			// TODO:
		} break;
		default: {
			fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
		} break;
		}
	}

	return 0;
}
