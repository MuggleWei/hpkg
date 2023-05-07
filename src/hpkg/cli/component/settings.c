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
#include "hpkg/cli/utils/path.h"
#include <stdio.h>
#include "muggle/c/os/path.h"
#include "yaml.h"

/**
 * @brief parse settings file
 *
 * @param instance
 * @param filepath
 *
 * @return
 *     - on success returns nonzero
 *     - on failed returns zero
 */
static int hpkg_settings_parse_file(struct hpkg_settings *instance,
									const char *filepath);

/**
 * @brief parse settings
 *
 * @param instance
 * @param p_parser
 *
 * @return 
 *     - on success returns nonzero
 *     - on failed returns zero
 */
static int hpkg_settings_run_parse(struct hpkg_settings *instance,
								   yaml_parser_t *p_parser);

/**
 * @brief prototype of parse settings
 *
 * @param instance
 * @param p_event
 */
typedef void (*hpkg_settings_parse_func)(struct hpkg_settings *instance,
										 yaml_event_t *p_event);

/**
 * @brief parse settings root
 *
 * @param instance
 * @param p_event
 */
static void hpkg_settings_parse_root(struct hpkg_settings *instance,
									 yaml_event_t *p_event);

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

	return hpkg_settings_parse_file(instance, filepath);
}

int hpkg_settings_load_default(struct hpkg_settings *instance)
{
	const char *candidates[] = {
		"~/.hpkg/settings.yml",
		"/opt/hpkg/etc/hpkg/settings.yml",
		"/usr/local/etc/hpkg/settings.yml",
		"/usr/etc/hpkg/settings.yml",
	};

	int ret = 0;
	for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); i++) {
		char buf[MUGGLE_MAX_PATH];
		const char *filepath =
			hpkg_path_expand(candidates[i], buf, sizeof(buf));
		if (filepath == NULL) {
			continue;
		}

		if (hpkg_settings_load(instance, filepath)) {
			ret = 1;
			fprintf(stdout, "Load settings in %s\n", candidates[i]);
			break;
		}
	}

	return ret;
}

int hpkg_settings_parse_file(struct hpkg_settings *instance,
							 const char *filepath)
{
	int ret = 0;

	yaml_parser_t parser;
	FILE *fp = NULL;

	if (!yaml_parser_initialize(&parser)) {
		fprintf(stderr, "Failed initialize yaml parser");
		return 0;
	}

	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Failed open file: %s", filepath);
		goto parse_exit;
	}

	yaml_parser_set_input_file(&parser, fp);

	ret = hpkg_settings_run_parse(instance, &parser);

parse_exit:
	yaml_parser_delete(&parser);

	if (fp) {
		fclose(fp);
		fp = NULL;
	}

	return ret;
}

int hpkg_settings_run_parse(struct hpkg_settings *instance,
							yaml_parser_t *p_parser)
{
	memset(instance, 0, sizeof(*instance));

	hpkg_settings_parse_func func = hpkg_settings_parse_root;

	yaml_event_t event;
	int done = 0;
	while (!done) {
		if (!yaml_parser_parse(p_parser, &event)) {
			break;
		}

		func(instance, &event);

		done = (event.type == YAML_STREAM_END_EVENT);
		yaml_event_delete(&event);
	}

	return 0;
}

void hpkg_settings_parse_root(struct hpkg_settings *instance,
							  yaml_event_t *p_event)
{
	switch (p_event->type) {
	case YAML_SCALAR_EVENT: {
		fprintf(stdout, "%s\n", p_event->data.scalar.value);
	} break;
	default: {
	} break;
	}
}
