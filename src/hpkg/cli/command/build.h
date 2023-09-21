/******************************************************************************
 *  @file         build.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-06-20
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#ifndef HPKG_CLI_COMMAND_BUILD_H_
#define HPKG_CLI_COMMAND_BUILD_H_

#include "hpkg/cli/base/macro.h"

HPKG_EXTERN_C_BEGIN

/**
 * @brief run happy package build command
 *
 * @param argc
 * @param argv
 *
 * @return
 *   - return 0 on success
 *   - otherwise failed
 */
HPKG_CLI_EXPORT
int hpkg_cli_run_build(int argc, char **argv);

HPKG_EXTERN_C_END

#endif // !HPKG_CLI_COMMAND_BUILD_H_
