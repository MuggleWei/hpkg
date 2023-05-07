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
#include "muggle/c/os/path.h"

struct hpkg_settings *hpkg_settings_instance()
{
	static struct hpkg_settings instance;
	return &instance;
}

int hpkg_settings_load(struct hpkg_settings *instance, const char *filepath)
{
	if (!muggle_path_exists(filepath)) {
		fprintf(stderr, "filepath not exists: %s\n", filepath);
		return 0;
	}

	// TODO:

	return 1;
}

int hpkg_settings_load_default(struct hpkg_settings *instance)
{
	// TODO:
	return 1;
}
