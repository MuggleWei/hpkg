/******************************************************************************
 *  @file         object.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-09
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        hpkg object
 *****************************************************************************/

#include "object.h"
#include <stdlib.h>

const char *hpkg_object_tp_name(struct hpkg_object *obj)
{
	return obj->obj_type->tp_name;
}

hpkg_size_t hpkg_object_retain(struct hpkg_object *obj)
{
	return ++obj->ref;
}

hpkg_size_t hpkg_object_release(struct hpkg_object *obj)
{
	uint16_t ret = --obj->ref;

	if (ret == 0) {
		if (obj->obj_type->destructor) {
			obj->obj_type->destructor(obj);
		}
		free(obj);
	}

	return ret;
}
