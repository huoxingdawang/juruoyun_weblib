/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_MALLOC_CONFIG_H
#define __JBL_MALLOC_CONFIG_H
#include "jbl_config.h"
#if JBL_MALLOC_ENABLE==1
#include "jbl_ying.h"
#define JBL_MALLOC_FAST				1
#define JBL_MALLOC_COUNT			1
#define JBL_MALLOC_LOG				0
#define JBL_MALLOC_NULL_PTR_CHECK	1
#if __SIZEOF_POINTER__ == 8
	typedef jbl_uint64 jbl_malloc_size_type;
#else
	typedef jbl_uint32 jbl_malloc_size_type;
#endif


#endif
#endif