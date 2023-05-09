/******************************************************************************
 *  @file         macro.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-09
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        hpkg object macro
 *****************************************************************************/

#ifndef HPKG_OBJECT_MACRO_H_
#define HPKG_OBJECT_MACRO_H_

#include "hpkg/config.h"

// windows export
#if defined(_WIN32) && HPKG_USE_DLL
	#if defined(HPKG_OBJ_EXPORTS)
		#define HPKG_OBJ_EXPORT __declspec(dllexport)
	#else
		#define HPKG_OBJ_EXPORT __declspec(dllimport)
	#endif
#else
	#define HPKG_OBJ_EXPORT
#endif

#endif // !HPKG_OBJECT_MACRO_H_
