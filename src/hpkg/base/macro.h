/******************************************************************************
 *  @file         macro.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-06
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#ifndef HPKG_BASE_MACRO_H_
#define HPKG_BASE_MACRO_H_

#include "hpkg/config.h"

// windows export
#if defined(_WIN32) && HPKG_USE_DLL
	#if defined(HPKG_EXPORTS)
		#define HPKG_EXPORT __declspec(dllexport)
	#else
		#define HPKG_EXPORT __declspec(dllimport)
	#endif
#else
	#define HPKG_EXPORT
#endif

// extern c
#ifdef __cplusplus
	#ifndef HPKG_EXTERN_C_BEGIN
		#define HPKG_EXTERN_C_BEGIN extern "C" {
	#endif
	#ifndef HPKG_EXTERN_C_END
		#define HPKG_EXTERN_C_END }
	#endif
#else
	#ifndef HPKG_EXTERN_C_BEGIN
		#define HPKG_EXTERN_C_BEGIN
	#endif
	#ifndef HPKG_EXTERN_C_END
		#define HPKG_EXTERN_C_END
	#endif
#endif

#endif // !HPKG_BASE_MACRO_H_
