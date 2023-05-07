/******************************************************************************
 *  @file         version.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-06
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#ifndef HPKG_CLI_BASE_VERSION_H_
#define HPKG_CLI_BASE_VERSION_H_

#include "hpkg/cli/base/macro.h"

HPKG_EXTERN_C_BEGIN

HPKG_CLI_EXPORT
const char *hpkg_version();

HPKG_CLI_EXPORT
const char *hpkg_compile_datetime();

HPKG_CLI_EXPORT
const char *hpkg_compile_datetime_iso8601();

HPKG_EXTERN_C_END

#endif // !HPKG_CLI_BASE_VERSION_H_
