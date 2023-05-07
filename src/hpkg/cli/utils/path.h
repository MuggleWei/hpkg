/******************************************************************************
 *  @file         path.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-07
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#ifndef HPKG_CLI_UTILS_PATH_H_
#define HPKG_CLI_UTILS_PATH_H_

#include <stddef.h>
#include "hpkg/cli/base/macro.h"

HPKG_EXTERN_C_BEGIN

/**
 * @brief expand path to absolute path
 * e.g.
 *   when user home is /home/mugglewei, then
 *   expand ~/.hpkg/settigs -> /home/mugglewei/.hpkg/settings
 *
 * @param filepath
 * @param buf
 * @param bufsize
 *
 * @return
 *     - on success, return result of expand
 *     - on failed, return NULL
 */
HPKG_CLI_EXPORT
const char *hpkg_path_expand(const char *filepath, char *buf, size_t bufsize);

HPKG_EXTERN_C_END

#endif // !HPKG_CLI_UTILS_PATH_H_
