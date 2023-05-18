/******************************************************************************
 *  @file         int_object.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-09
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        hpkg integer object
 *****************************************************************************/

#ifndef HPKG_OBJECT_OBJECTS_INT_OBJECT_H_
#define HPKG_OBJECT_OBJECTS_INT_OBJECT_H_

#include "hpkg/object/macro.h"
#include "hpkg/object/predefine.h"
#include "hpkg/object/objects/type.h"
#include "hpkg/object/objects/object.h"

HPKG_EXTERN_C_BEGIN

struct hpkg_int_value {
	hpkg_int_t v;
};

struct hpkg_int_object {
	HPKG_OBJECT_HEAD;
	struct hpkg_int_value value;
};

HPKG_OBJ_EXPORT
struct hpkg_int_object *hpkg_int_new(hpkg_int_t v);

HPKG_OBJ_EXPORT
hpkg_int_t hpkg_int_get(struct hpkg_int_object *p_object);

HPKG_OBJ_EXPORT
hpkg_hash_t hpkg_int_hash(struct hpkg_int_object *p_object);

HPKG_EXTERN_C_END

#endif // !HPKG_OBJECT_OBJECTS_INT_OBJECT_H_
