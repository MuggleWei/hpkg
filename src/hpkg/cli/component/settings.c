/******************************************************************************
 *  @file         settings.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-06
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#include "settings.h"
#include <stdio.h>
#include "hpkg/cli/utils/path.h"
#include "muggle/c/base/macro.h"
#include "muggle/c/os/os.h"
#include "muggle/c/os/path.h"
#include "yaml.h"

static const char *hpkg_settings_rel_etc(char *buf, size_t bufsize)
{
	char exedir[MUGGLE_MAX_PATH];
	if (hpkg_path_process_dir(exedir, sizeof(exedir)) == NULL) {
		return NULL;
	}

	char relbuf[MUGGLE_MAX_PATH];
	const char *relpath = "../etc/hpkg/settings.yml";
	if (muggle_path_join(exedir, relpath, relbuf, sizeof(relbuf)) != 0) {
		return NULL;
	}

	if (muggle_path_abspath(buf, buf, sizeof(bufsize)) != 0) {
		return NULL;
	}

	return buf;
}

struct hpkg_settings *hpkg_settings_instance()
{
	static struct hpkg_settings instance;
	return &instance;
}

bool hpkg_settings_load_default(struct hpkg_settings *instance)
{
	const char *candidates[] = {
		"~/.hpkg/settings.yml",
		"/opt/hpkg/etc/hpkg/settings.yml",
		"/usr/local/etc/hpkg/settings.yml",
		"/usr/etc/hpkg/settings.yml",
	};

	int ret = false;
	for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); i++) {
		char buf[MUGGLE_MAX_PATH];
		const char *filepath =
			hpkg_path_expand(candidates[i], buf, sizeof(buf));
		if (filepath == NULL) {
			continue;
		}

		if (!muggle_path_exists(filepath)) {
			continue;
		}

		if (hpkg_settings_load(instance, filepath)) {
			ret = true;
			fprintf(stdout, "Load settings in %s\n", filepath);
			break;
		}
	}

	if (!ret) {
		char buf[MUGGLE_MAX_PATH];
		const char *filepath = hpkg_settings_rel_etc(buf, sizeof(buf));
		if (filepath == NULL) {
			return false;
		}

		if (!muggle_path_exists(filepath)) {
			return false;
		}

		ret = hpkg_settings_load(instance, filepath);
		if (ret) {
			fprintf(stdout, "Load settings in %s\n", filepath);
		}
	}

	return ret;
}

bool hpkg_settings_load(struct hpkg_settings *instance, const char *filepath)
{
	if (!muggle_path_exists(filepath)) {
		fprintf(stderr, "filepath not exists: %s\n", filepath);
		return 0;
	}

	// TODO: parse yaml file to dict object
	return false;
}
