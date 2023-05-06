/******************************************************************************
 *  @file         hpkg_handle.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-06
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#include "hpkg_cli_handle.h"
#include "hpkg/base/version.h"
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

	return 0;
}
