/******************************************************************************
 *  @file         hash_func.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-09
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        hpkg hash functions
 *****************************************************************************/

#ifndef HPKG_OBJECT_HASH_FUNCTION_H_
#define HPKG_OBJECT_HASH_FUNCTION_H_

#include <stdint.h>
#include "hpkg/object/macro.h"

HPKG_EXTERN_C_BEGIN

//-----------------------------------------------------------------------------
// This function copy form MurmurHash
//
// MurmurHash2 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.
//-----------------------------------------------------------------------------
HPKG_OBJ_EXPORT
uint64_t hpkg_MurmurHash64A(const void *key, int len, uint64_t seed);

HPKG_EXTERN_C_END

#endif // !HPKG_OBJECT_HASH_FUNCTION_H_
