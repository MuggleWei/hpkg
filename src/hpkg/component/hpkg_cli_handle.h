/******************************************************************************
 *  @file         hpkg_handle.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-06
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#ifndef HPKG_COMPONENT_HPKG_CLI_HANDLE_H_
#define HPKG_COMPONENT_HPKG_CLI_HANDLE_H_

#include "hpkg/base/macro.h"

HPKG_EXTERN_C_BEGIN

/**
 * @brief run happy package
 *
 * @param argc
 * @param argv
 *
 * @return 
 */
HPKG_EXPORT
int hpkg_cli_run(int argc, char **argv);

HPKG_EXTERN_C_END

#endif // !HPKG_COMPONENT_HPKG_HANDLE_H_
