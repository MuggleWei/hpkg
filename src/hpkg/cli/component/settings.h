/******************************************************************************
 *  @file         settings.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-06
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#ifndef HPKG_CLI_COMPONENT_SETTINGS_H_
#define HPKG_CLI_COMPONENT_SETTINGS_H_

#include <stdint.h>
#include "hpkg/cli/base/macro.h"

HPKG_EXTERN_C_BEGIN

struct hpkg_settings {
	uint16_t log_console_level;
	uint16_t log_file_level;
	char *local_db;
	char *local_src;
	char *local_pkg;
};

/**
 * @brief get settings instance
 *
 * @return 
 */
HPKG_CLI_EXPORT
struct hpkg_settings *hpkg_settings_instance();

/**
 * @brief load settings file
 *
 * @param instance
 * @param filepath
 *
 * @return
 *     - on success returns nonzero
 *     - on failed returns zero
 */
HPKG_CLI_EXPORT
int hpkg_settings_load(struct hpkg_settings *instance, const char *filepath);

/**
 * @brief load settings from default filepaths
 *
 * @param instance
 *
 * @return
 *     - on success returns nonzero
 *     - on failed returns zero
 */
HPKG_CLI_EXPORT
int hpkg_settings_load_default(struct hpkg_settings *instance);

HPKG_EXTERN_C_END

#endif // !HPKG_CLI_COMPONENT_SETTINGS_H_
