/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_STRING_H
#define __JRY_BL_STRING_H
#include "jry_bl_string_config.h"
#if JRY_BL_STRING_ENABLE==1
#include "jry_bl_exception.h"
#include "jry_bl_malloc.h"
#include "jry_bl_ying.h"
#if JRY_BL_USE_STDIO==1
#include <stdio.h>
#endif
#if JRY_BL_USE_STDARG==1
#include <stdarg.h>
#endif

typedef struct __jry_bl_string
{
	jry_bl_string_size_type	len;
	jry_bl_string_size_type	size;
	unsigned char *			s;
}jry_bl_string;
jry_bl_string_size_type	jry_bl_strlen										(char *a);
void					jry_bl_string_init									(jry_bl_string *this);
void					jry_bl_string_free									(jry_bl_string *this);
void					jry_bl_string_clear									(jry_bl_string *this);
void					jry_bl_string_parse									(jry_bl_string *this);
#define					jry_bl_string_extend(a,b)							jry_bl_string_extend_to((a),(jry_bl_string_get_length((a))+(b)))
void					jry_bl_string_extend_to								(jry_bl_string *this,jry_bl_string_size_type size);
jry_bl_uint64			jry_bl_string_hash									(jry_bl_string *this);
unsigned char			jry_bl_string_get									(jry_bl_string *this,jry_bl_string_size_type i);
#define					jry_bl_string_get1(this,i)							(((i)<0||(i)>=(this)->len)?0:(this)->s[(i)])
unsigned char			jry_bl_string_set									(jry_bl_string *this,jry_bl_string_size_type i,unsigned char a);
#define					jry_bl_string_get_char_pointer(this)				((this)->s)
#define					jry_bl_string_get_length(this) 						((this)->len)
#define					jry_bl_string_get_size(this) 						((this)->size)
#define					jry_bl_string_const(s,len)							{len,0,s}
void					jry_bl_string_add_string							(jry_bl_string *this,jry_bl_string *in);
#define					jry_bl_string_add_char_pointer(x,y)					jry_bl_string_add_char_pointer_length(x,y,jry_bl_strlen(y))
void					jry_bl_string_add_char_pointer_length				(jry_bl_string *this,unsigned char *in,jry_bl_string_size_type len);
void					jry_bl_string_add_char								(jry_bl_string *this,unsigned char in);
#define					jry_bl_string_add_char1(this,in)					(this)->s[(this)->len]=(in),++(this)->len
void					jry_bl_string_add_int64								(jry_bl_string *this,jry_bl_int64 in);
void					jry_bl_string_add_uint64							(jry_bl_string *this,jry_bl_uint64 in);
#define					jry_bl_string_add_double(this,in)					jry_bl_string_add_double_length(this,in,10)
void					jry_bl_string_add_double_length						(jry_bl_string *this,double in,unsigned char len);
void					jry_bl_string_add_unicode_as_utf8					(jry_bl_string *this,unsigned long unicode);
#define					jry_bl_string_delete_1(this)						((this->len)>0?(--(this)->len):(0))
#define					jry_bl_string_equal(a,b)							jry_bl_string_copy(a,b,JRY_BL_COPY)
#define					jry_bl_string_equal_light(a,b)						jry_bl_string_copy(a,b,JRY_BL_COPY_LIGHT)
#define					jry_bl_string_equal_light_move(a,b)					jry_bl_string_copy(a,b,JRY_BL_COPY_LIGHT_MOVE)
#define					jry_bl_string_equal_string(a,b)						jry_bl_string_copy(a,b,JRY_BL_COPY)
#define					jry_bl_string_equal_string_light(a,b)				jry_bl_string_copy(a,b,JRY_BL_COPY_LIGHT)
#define					jry_bl_string_equal_string_light_move(a,b)			jry_bl_string_copy(a,b,JRY_BL_COPY_LIGHT_MOVE)
#define					jry_bl_string_equal_char_pointer(this,in)			jry_bl_string_clear(this),jry_bl_string_add_char_pointer(this,in)
#define					jry_bl_string_equal_char_pointer_length(this,in,len)jry_bl_string_clear(this),jry_bl_string_add_char_pointer_length(this,in,len)
#define					jry_bl_string_equal_char_pointer_light(a,b)			jry_bl_string_equal_char_pointer_light_length(a,b,jry_bl_strlen(b))
#define					jry_bl_string_equal_char_pointer_light_length(a,b,c)jry_bl_string_free(a),(a)->len=c,(a)->s=b
#define					jry_bl_string_equal_char(this,in)					jry_bl_string_clear(this),jry_bl_string_add_char(this,in)
#define					jry_bl_string_equal_int64(this,in)					jry_bl_string_clear(this),jry_bl_string_add_int64(this,in)
#define					jry_bl_string_equal_uint64(this,in)					jry_bl_string_clear(this),jry_bl_string_add_uint64(this,in)
#define					jry_bl_string_equal_double(this,in)					jry_bl_string_clear(this),jry_bl_string_add_double(this,in)
#define					jry_bl_string_equal_double_length(this,in,l)		jry_bl_string_clear(this),jry_bl_string_add_double_length(this,in,l)
void					jry_bl_string_copy									(jry_bl_string *this,jry_bl_string *in,jry_bl_uint8 copytype);
char					jry_bl_string_space_ship							(jry_bl_string *this,jry_bl_string *that);
#define					jry_bl_string_if_big(x,y)							(jry_bl_string_space_ship(x,y)>0)
#define					jry_bl_string_if_equal(x,y)							(jry_bl_string_space_ship(x,y)==0)
#define					jry_bl_string_if_small(x,y) 						(jry_bl_string_space_ship(x,y)<0)
#define					jry_bl_string_if_equal_small(x,y)					(jry_bl_string_space_ship(x,y)<=0)
#define					jry_bl_string_if_equal_big(x,y) 					(jry_bl_string_space_ship(x,y)>=0)
#define					jry_bl_string_get_int64(this)						jry_bl_string_get_int64_start_v(this,0)
jry_bl_int64			jry_bl_string_get_int64_start						(jry_bl_string *this,jry_bl_string_size_type *start);
jry_bl_int64			jry_bl_string_get_int64_start_v						(jry_bl_string *this,jry_bl_string_size_type start);
#define					jry_bl_string_get_uint64(this)						jry_bl_string_get_uint64_start_v(this,0)
jry_bl_uint64			jry_bl_string_get_uint64_start						(jry_bl_string *this,jry_bl_string_size_type *start);
jry_bl_uint64			jry_bl_string_get_uint64_start_v					(jry_bl_string *this,jry_bl_string_size_type start);
#define					jry_bl_string_get_double(this)						jry_bl_string_get_double_start_v(this,0)
double					jry_bl_string_get_double_start						(jry_bl_string *this,jry_bl_string_size_type *start);
double					jry_bl_string_get_double_start_v					(jry_bl_string *this,jry_bl_string_size_type start);
#define					jry_bl_string_get_hex(this)							jry_bl_string_get_hex_start_v(this,0)
jry_bl_uint64			jry_bl_string_get_hex_start							(jry_bl_string *this,jry_bl_string_size_type *start);
jry_bl_uint64			jry_bl_string_get_hex_start_v						(jry_bl_string *this,jry_bl_string_size_type start);
#define					jry_bl_string_to_json(x,y)							jry_bl_string_to_json_ex(x,y,0)
void					jry_bl_string_to_json_ex							(jry_bl_string *this,jry_bl_string *result,jry_bl_uint8 type);
#define					jry_bl_string_from_json(this,in)					jry_bl_string_from_json_start(this,in,0)
jry_bl_string_size_type	jry_bl_string_from_json_start						(jry_bl_string *this,jry_bl_string *in,jry_bl_string_size_type start);
#define					jry_bl_string_find_char(this,in)					jry_bl_string_find_char_start(this,in,0)
jry_bl_string_size_type	jry_bl_string_find_char_start						(jry_bl_string *this,unsigned char in,jry_bl_string_size_type start);
#if JRY_BL_USE_STDIO==1
void 					jry_bl_string_print									(jry_bl_string *this,FILE * file);
#define					jry_bl_string_view(x,y) 							jry_bl_string_view_ex(x,y,#x " @ "__FILE__,__LINE__,jry_bl_view_default_tabs_num)
void 					jry_bl_string_view_ex								(jry_bl_string *this,FILE * file,char*str,int a,int tabs);
void					jry_bl_string_add_file								(jry_bl_string *this,FILE * file);
void					jry_bl_string_add_file_end_by						(jry_bl_string *this,FILE * file,unsigned char end);
#define					jry_bl_string_equal_file(this,file)					jry_bl_string_clear(this),jry_bl_string_add_file(this,file)
#define					jry_bl_string_equal_file_end_by(this,file,end)		jry_bl_string_clear(this),jry_bl_string_add_file_end_by(this,file,end)
#endif
#if JRY_BL_USE_STDARG==1
void					jry_bl_string_inits									(int n,...);
void					jry_bl_string_frees									(int n,...);
void					jry_bl_string_clears								(int n,...);
#if JRY_BL_USE_STDIO==1
void					jry_bl_string_views									(FILE * file,int n,...);
#endif
#endif
#if JRY_BL_VAR_ENABLE==1
typedef struct __jry_bl_var jry_bl_var;
#define 				jry_bl_var_get_string(this)					((jry_bl_string*)(this)->data.p)
void					jry_bl_var_equal_string						(jry_bl_var *this,jry_bl_string *that);
void					jry_bl_var_equal_string_light				(jry_bl_var *this,jry_bl_string *that);
void					jry_bl_var_equal_string_light_move			(jry_bl_var *this,jry_bl_string *that);
void					jry_bl_var_equal_string_pointer				(jry_bl_var *this,jry_bl_string *that);
void					jry_bl_string_equal_var						(jry_bl_string *this,jry_bl_var *that);
void					jry_bl_string_equal_var_light				(jry_bl_string *this,jry_bl_var *that);
void					jry_bl_string_equal_var_light_move			(jry_bl_string *this,jry_bl_var *that);
#endif
#if JRY_BL_LINK_LIST_ENABLE==1
typedef struct __jry_bl_link_list jry_bl_link_list;
jry_bl_string_size_type	jry_bl_string_cut_start								(jry_bl_string *this,jry_bl_link_list *list,char cut,jry_bl_string_size_type start);
#define					jry_bl_string_cut(x,y,z)							jry_bl_string_cut_start(x,y,z,0)
#endif

#endif	
#endif
