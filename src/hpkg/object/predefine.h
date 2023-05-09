/******************************************************************************
 *  @file         define.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-09
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        hpkg object define
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "hpkg/object/macro.h"

HPKG_EXTERN_C_BEGIN

typedef int64_t hpkg_int_t;
typedef double hpkg_float_t;
typedef bool hpkg_bool_t;
typedef char *hpkg_str_t;
typedef size_t hpkg_size_t;
typedef uint64_t hpkg_hash_t;

HPKG_EXTERN_C_END
