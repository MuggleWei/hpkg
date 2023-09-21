/******************************************************************************
 *  @file         hpkg_handle.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-06
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#include "cli_handle.h"
#include "hpkg/cli/base/version.h"
#include "hpkg/cli/command/build.h"
#include "hpkg/cli/component/settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *s_str_usage =
	"Usage: %s COMMAND [OPTIONS]\n"
	"\n"
	"Commands:\n"
	"  build   build pacakge\n"
	"  search  search package\n"
	"  pull    pull package\n"
	"  pack    pack package\n"
	"  push    upload package\n"
	"  flush   sync local db and local package directory\n";

int hpkg_cli_run(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, s_str_usage, argv[0]);
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		fprintf(stdout, s_str_usage, argv[0]);
		exit(EXIT_SUCCESS);
	}

	if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
		fprintf(stdout,
				"hpkg (Happy Package)\nVersion: %s\nCompile Datetime: %s\n",
				hpkg_version(), hpkg_compile_datetime_iso8601());
		exit(EXIT_SUCCESS);
	}

	// struct hpkg_settings *p_settings = hpkg_settings_instance();
	// if (!hpkg_settings_load_default(p_settings)) {
	//     fprintf(stderr, "Failed load settings\n");
	//     exit(EXIT_FAILURE);
	// }

	// TODO:
	if (strcmp(argv[1], "build") == 0) {
		return hpkg_cli_run_build(argc - 1, argv + 1);
	} else {
		// TODO:
		fprintf(stderr, "sub command '%s' not implement yet", argv[1]);
		return 1;
	}

	return 0;
}
