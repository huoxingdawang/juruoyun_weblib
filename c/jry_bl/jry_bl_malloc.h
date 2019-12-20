/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_MALLOC_H
#define __JRY_BL_MALLOC_H
#include "jry_bl_malloc_config.h"
#if JRY_BL_MALLOC_ENABLE==1
#include "jry_bl_exception.h"
#include "jry_bl_ying.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
void jry_bl_malloc_start();
#if JRY_BL_MALLOC_DEBUG_MODE==1
extern size_t __jry_bl_malloced_size;
#define jry_bl_malloced_size ((jry_bl_int64)__jry_bl_malloced_size)
#else
#define jry_bl_malloced_size ((jry_bl_int64)0)
#endif
void* jry_bl_malloc(size_t size);
void* jry_bl_realloc(void* ptr,size_t size);
void jry_bl_free(void * p);
size_t jry_bl_malloc_size(void* ptr);
void jry_bl_memory_copy(void *to,const void * from,size_t len);
void jry_bl_memory_copy_reverse(void *to,const void * from,size_t len,size_t size);
#else
#define jry_bl_malloc_init()
#endif
#endif
