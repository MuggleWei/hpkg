/******************************************************************************
 *  @file         macro.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-06
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#ifndef HPKG_CLI_BASE_MACRO_H_
#define HPKG_CLI_BASE_MACRO_H_

#include "hpkg/config.h"

// windows export
#if defined(_WIN32) && HPKG_USE_DLL
	#if defined(HPKG_CLI_EXPORTS)
		#define HPKG_CLI_EXPORT __declspec(dllexport)
	#else
		#define HPKG_CLI_EXPORT __declspec(dllimport)
	#endif
#else
	#define HPKG_CLI_EXPORT
#endif

#endif // !HPKG_CLI_BASE_MACRO_H_
