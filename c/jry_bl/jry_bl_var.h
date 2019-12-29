/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_VAR_H
#define __JRY_BL_VAR_H
#include "jry_bl_var_config.h"
#if JRY_BL_VAR_ENABLE==1
#include "jry_bl_exception.h"
#include "jry_bl_malloc.h"
#include "jry_bl_ying.h"
#if JRY_BL_USE_STDARG==1
#include <stdarg.h>
#endif
#define					jry_bl_var_flag_pointer(this)					((this)->f.f.pointer)

#define JRY_BL_VAR_TYPE_UNUSE		0
#define JRY_BL_VAR_TYPE_NULL		1
#define JRY_BL_VAR_TYPE_INT64		2
#define JRY_BL_VAR_TYPE_UINT64		3
#define JRY_BL_VAR_TYPE_DOUBLE		4
#define JRY_BL_VAR_TYPE_TRUE		5
#define JRY_BL_VAR_TYPE_FALSE		6
#define JRY_BL_VAR_TYPE_CHAR		7
#define JRY_BL_VAR_TYPE_POINTER		8
#define JRY_BL_VAR_TYPE_VAR			9	
#if JRY_BL_STRING_ENABLE==1
#define JRY_BL_VAR_TYPE_STRING		10
#include "jry_bl_string.h"
#endif
#if JRY_BL_LINK_LIST_ENABLE==1
#define JRY_BL_VAR_TYPE_LINK_LIST	11
typedef struct __jry_bl_link_list jry_bl_link_list;
#endif
#if JRY_BL_HASH_TABLE_ENABLE==1
#define JRY_BL_VAR_TYPE_HASH_TABLE	12
typedef struct __jry_bl_hash_table jry_bl_hash_table;
#endif

typedef struct __jry_bl_var
{
	union
	{
		jry_bl_int64 ll;
		jry_bl_uint64 ull;
		double d;
		char c;
		void* p;
	}data;
	union
	{
		int ff;
		struct
		{
			jry_bl_var_type_type type;
			jry_bl_uint8 pointer:1;
		}f;
	}f;
}jry_bl_var;

typedef struct __jry_bl_var_functions_struct
{
	size_t size;
	void (*init)(void*);
	void (*free)(jry_bl_string*);
	void (*copy)(jry_bl_string*,jry_bl_string*,jry_bl_uint8);
	char (*space_ship)(jry_bl_string*,jry_bl_string*);
	void (*to_json)(jry_bl_string*,jry_bl_string*,jry_bl_uint8);
	void (*view_ex)(jry_bl_string*,FILE*,char*,int,int);
}jry_bl_var_functions_struct;

extern const jry_bl_var_functions_struct jry_bl_var_functions[4];
extern jry_bl_var_functions_struct* jry_bl_var_fs[jry_bl_var_fs_size];
extern jry_bl_var_functions_struct jry_bl_var_tmp_functions[jry_bl_var_tmp_size];
#if jry_bl_var_tmp_size!=0
#define jry_bl_var_tmp_register(type,a,b,c,d,e,f,g,h,i)	jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].size=a, \
														jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].init=b,	\
														jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].free=c,	\
														jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].equal=d,	\
														jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].equal_light=e,	\
														jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].equal_light_move=f,	\
														jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].space_ship=g,	\
														jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].to_json=h,	\
														jry_bl_var_tmp_functions[((type)&(1<<jry_bl_var_type_bit))].view_ex=i
#endif
void					jry_bl_var_init						(jry_bl_var *this);
void					jry_bl_var_free						(jry_bl_var *this);
void					jry_bl_var_init_as					(jry_bl_var *this,jry_bl_var_type_type type);
void					jry_bl_var_copy						(jry_bl_var *this,jry_bl_var *that,jry_bl_uint8 copytype);
#define					jry_bl_var_get_type(a)				((a)->f.f.type)
#define					jry_bl_var_equal(a,b)				jry_bl_var_copy(a,b,JRY_BL_COPY)
#define					jry_bl_var_equal_light(a,b)			jry_bl_var_copy(a,b,JRY_BL_COPY_LIGHT)
#define					jry_bl_var_equal_light_move(a,b)	jry_bl_var_copy(a,b,JRY_BL_COPY_LIGHT_MOVE)
char					jry_bl_var_space_ship				(jry_bl_var *this,jry_bl_var *that);
#define					jry_bl_var_if_big(x,y)				(jry_bl_var_space_ship(x,y)>0)
#define					jry_bl_var_if_equal(x,y)			(jry_bl_var_space_ship(x,y)==0)
#define					jry_bl_var_if_small(x,y) 			(jry_bl_var_space_ship(x,y)<0)
#define					jry_bl_var_if_equal_small(x,y)		(jry_bl_var_space_ship(x,y)<=0)
#define					jry_bl_var_if_equal_big(x,y) 		(jry_bl_var_space_ship(x,y)>=0)
#define 				jry_bl_var_get_int64(this)			((this)->data.ll)
#define 				jry_bl_var_get_uint64(this)			((this)->data.ull)
#define 				jry_bl_var_get_double(this)			((this)->data.d)
#define 				jry_bl_var_get_char(this)			((this)->data.x)
#define 				jry_bl_var_get_pointer(this)		((this)->data.p)
#define 				jry_bl_var_equal_int64(this,a)		jry_bl_var_init_as((this),JRY_BL_VAR_TYPE_INT64)	,((this)->data.ll=(a))
#define 				jry_bl_var_equal_uint64(this,a)		jry_bl_var_init_as((this),JRY_BL_VAR_TYPE_UINT64)	,((this)->data.ull=(a))
#define 				jry_bl_var_equal_double(this,a)		jry_bl_var_init_as((this),JRY_BL_VAR_TYPE_DOUBLE)	,((this)->data.d=(a))
#define 				jry_bl_var_equal_char(this,a)		jry_bl_var_init_as((this),JRY_BL_VAR_TYPE_CHAR)		,((this)->data.x=(a))
#define 				jry_bl_var_equal_pointer(this,a)	jry_bl_var_init_as((this),JRY_BL_VAR_TYPE_POINTER)	,((this)->data.p=(a))
#define 				jry_bl_var_equal_true(this)			jry_bl_var_init_as((this),JRY_BL_VAR_TYPE_TRUE)
#define 				jry_bl_var_equal_false(this)		jry_bl_var_init_as((this),JRY_BL_VAR_TYPE_FALSE)
void					jry_bl_var_turn						(jry_bl_var *this,jry_bl_var_type_type type);
#if JRY_BL_STRING_ENABLE==1
void					jry_bl_var_to_json_ex				(jry_bl_var *this,jry_bl_string *result,jry_bl_uint8 type);
#define					jry_bl_var_to_json(x,y)				jry_bl_var_to_json_ex(x,y,0)
jry_bl_string_size_type	jry_bl_var_from_json_start			(jry_bl_var *this,jry_bl_string *in,jry_bl_string_size_type start);
#define					jry_bl_var_from_json(this,in)		jry_bl_var_from_json_start(this,in,0)
#endif

#if JRY_BL_USE_STDIO==1
#define					jry_bl_var_view(x,y) 				jry_bl_var_view_ex(x,y,#x " @ "__FILE__,__LINE__,jry_bl_view_default_tabs_num)
void					jry_bl_var_view_ex					(jry_bl_var *this,FILE * file,char*str,int a,int tabs);
#endif
#if JRY_BL_USE_STDARG==1
void					jry_bl_var_inits					(int n,...);
void					jry_bl_var_frees					(int n,...);
#if JRY_BL_USE_STDIO==1
void					jry_bl_var_views					(FILE * file,int n,...);
#endif
#endif
#endif
#endif