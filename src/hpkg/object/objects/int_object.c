/******************************************************************************
 *  @file         int_object.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-09
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        hpkg integer object
 *****************************************************************************/

#include "int_object.h"
#include <stdlib.h>
#include <string.h>

static struct hpkg_object_type *hpkg_type_int()
{
	static struct hpkg_object_type s_integer_type = {
		.tp_name = "int",
		.destructor = NULL,
	};
	return &s_integer_type;
}

struct hpkg_int_object *hpkg_int_new(hpkg_int_t v)
{
	struct hpkg_int_object *p_object =
		(struct hpkg_int_object *)malloc(sizeof(struct hpkg_int_object));

	memset(p_object, 0, sizeof(*p_object));
	p_object->base.obj_type = hpkg_type_int();
	p_object->base.ref = 1;

	p_object->value.v = v;

	return p_object;
}

hpkg_int_t hpkg_int_get(struct hpkg_int_object *p_object)
{
	return p_object->value.v;
}

hpkg_hash_t hpkg_int_hash(struct hpkg_int_object *p_object)
{
}
