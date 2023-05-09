/******************************************************************************
 *  @file         type.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-09
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        hpkg object type
 *****************************************************************************/

#ifndef HPKG_OBJECT_OBJECTS_TYPE_H_
#define HPKG_OBJECT_OBJECTS_TYPE_H_

#include "hpkg/object/macro.h"

HPKG_EXTERN_C_BEGIN

typedef void (*hpkg_func_destructor)(void *data);

struct hpkg_object_type {
	const char *tp_name;
	hpkg_func_destructor destructor;
};

HPKG_EXTERN_C_END

#endif // !HPKG_OBJECT_OBJECTS_TYPE_H_
