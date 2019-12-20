/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_YING_H
#define __JRY_BL_YING_H
#include "jry_bl_ying_config.h"
#if JRY_BL_YING_ENABLE==1
#ifndef NULL
	#define NULL 0
#endif
#ifndef false
	#define false 0
#endif
#ifndef true
	#define true 1
#endif
#define jry_bl_view_default_tabs_num		1
#define jry_bl_start()	\
jry_bl_malloc_start()
#define JRY_BL_PRINT_COLORFUL_BLACK			0
#define JRY_BL_PRINT_COLORFUL_RED			1
#define JRY_BL_PRINT_COLORFUL_LIGHT_GREEN	2
#define JRY_BL_PRINT_COLORFUL_YELLOW		3
#define JRY_BL_PRINT_COLORFUL_PURPLE		4
#define JRY_BL_PRINT_COLORFUL_GREEN			5
#define JRY_BL_PRINT_COLORFUL_WHITE			6
typedef unsigned char		jry_bl_uint8;
typedef unsigned long		jry_bl_uint16;
typedef int					jry_bl_int32;
typedef unsigned int		jry_bl_uint32;
typedef long long			jry_bl_int64;
typedef unsigned long long	jry_bl_uint64;
#ifndef JRY_BL_COPY
#define JRY_BL_COPY 0
#endif
#ifndef JRY_BL_COPY_LIGHT
#define JRY_BL_COPY_LIGHT 1
#endif
#ifndef JRY_BL_COPY_LIGHT_MOVE
#define JRY_BL_COPY_LIGHT_MOVE 2
#endif

#ifdef __linux__
	#define jry_bl_print_colorful(f,s,bc,fc) fprintf(f,"\x1b[%d;%dm%s\x1b[0m",bc+40,fc+30,s)
	typedef jry_bl_uint64 jry_bl_pointer_int;
#else
	#define jry_bl_print_colorful(f,s,bc,fc) fprintf(f,s)
	typedef jry_bl_uint32 jry_bl_pointer_int;
#endif
jry_bl_int64 jry_bl_ceil(long double i);
#endif
#endif