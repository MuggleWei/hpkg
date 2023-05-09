/******************************************************************************
 *  @file         object.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-09
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        hpkg object
 *
 *  NOTE:
 *  1. Use object function, don't set/get member variable directly
 *  1. Keep object immutable
 *****************************************************************************/

#ifndef HPKG_OBJECT_OBJECTS_OBJECT_H_
#define HPKG_OBJECT_OBJECTS_OBJECT_H_

#include "hpkg/object/macro.h"
#include "hpkg/object/predefine.h"
#include "hpkg/object/objects/type.h"

HPKG_EXTERN_C_BEGIN

/**
 * @brief hpkg object
 */
struct hpkg_object {
	struct hpkg_object_type *obj_type;
	hpkg_size_t ref;
	hpkg_size_t hash_val;
};

#define HPKG_OBJECT_HEAD struct hpkg_object base;

/**
 * @brief get object type name
 *
 * @param obj
 *
 * @return 
 */
HPKG_OBJ_EXPORT
const char *hpkg_object_tp_name(struct hpkg_object *obj);

/**
 * @brief retain object
 *
 * @param obj
 *
 * @return reference count after retain
 */
HPKG_OBJ_EXPORT
hpkg_size_t hpkg_object_retain(struct hpkg_object *obj);

/**
 * @brief release object
 *
 * @param obj
 *
 * @return reference count after release
 */
HPKG_OBJ_EXPORT
hpkg_size_t hpkg_object_release(struct hpkg_object *obj);

HPKG_EXTERN_C_END

#endif // !HPKG_OBJECT_OBJECTS_OBJECT_H_
