
/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_HASH_TABLE_H
#define __JRY_BL_HASH_TABLE_H
#include "jry_bl_hash_table_config.h"
#if JRY_BL_HASH_TABLE_ENABLE==1
#include "jry_bl_exception.h"
#include "jry_bl_malloc.h"
#include "jry_bl_string.h"
#include "jry_bl_ying.h"
#include "jry_bl_var.h"
#if JRY_BL_USE_STDARG==1
#include <stdarg.h>
#endif
typedef struct __jry_bl_hash_table_data
{
	jry_bl_uint64				h;
	jry_bl_hash_table_size_type	nxt;	
	jry_bl_string				k;
	jry_bl_var					v;
}jry_bl_hash_table_data;
typedef struct __jry_bl_hash_table
{
	jry_bl_hash_table_size_type size;
	jry_bl_hash_table_size_type len;
	jry_bl_hash_table_size_type nxt;
	jry_bl_hash_table_data		*data;
}jry_bl_hash_table;
void					jry_bl_hash_table_init								(jry_bl_hash_table *this);
void					jry_bl_hash_table_free								(jry_bl_hash_table *this);
void					jry_bl_hash_table_extend_to							(jry_bl_hash_table *this,jry_bl_hash_table_size_type size);
#define					jry_bl_hash_table_extend(x,y)						jry_bl_hash_table_extend_to((x),(y)+(x)->nxt)
void					jry_bl_hash_table_insert							(jry_bl_hash_table *this,jry_bl_string *k,jry_bl_var *v,jry_bl_uint8 kcpt,jry_bl_uint8 vcpt);
void					jry_bl_hash_table_get								(const jry_bl_hash_table *this,const jry_bl_string *k,jry_bl_var *v,jry_bl_uint8 vcpt);
void					jry_bl_hash_table_unset								(jry_bl_hash_table *this,const jry_bl_string *k);
char					jry_bl_hash_table_space_ship						(const jry_bl_hash_table *this,const jry_bl_hash_table *that);
#define					jry_bl_hash_table_if_big(x,y)						(jry_bl_hash_table_space_ship(x,y)>0)
#define					jry_bl_hash_table_if_equal(x,y)						(jry_bl_hash_table_space_ship(x,y)==0)
#define					jry_bl_hash_table_if_small(x,y) 					(jry_bl_hash_table_space_ship(x,y)<0)
#define					jry_bl_hash_table_if_equal_small(x,y)				(jry_bl_hash_table_space_ship(x,y)<=0)
#define					jry_bl_hash_table_if_equal_big(x,y) 				(jry_bl_hash_table_space_ship(x,y)>=0)
void					jry_bl_hash_table_clear								(jry_bl_hash_table *this);
void					jry_bl_hash_table_copy								(jry_bl_hash_table *this,jry_bl_hash_table *that,jry_bl_uint8 copytype);
#define					jry_bl_hash_table_to_json(x,y)						jry_bl_hash_table_to_json_ex(x,y,0)
void					jry_bl_hash_table_to_json_ex						(const jry_bl_hash_table *this,jry_bl_string *out,jry_bl_uint8 type);
void					jry_bl_hash_table_merge								(jry_bl_hash_table *this,jry_bl_hash_table *that,jry_bl_uint8 copytype);
jry_bl_string_size_type jry_bl_hash_table_from_json_start					(jry_bl_hash_table *this,const jry_bl_string *in,jry_bl_string_size_type start);
#define					jry_bl_hash_table_from_json(this,in)				jry_bl_hash_table_from_json_start(this,in,0)


void					jry_bl_hash_table_rehash							(jry_bl_hash_table *this);

#define					jry_bl_hash_table_get_key(x)						(&((x)->k))
#define					jry_bl_hash_table_get_var(x)						(&((x)->v))

#define					jry_bl_hash_table_foreach(this,i)					for(jry_bl_hash_table_data *i=&(this)->data[0];i!=&(this)->data[(this)->nxt];i++)if(jry_bl_var_get_type(&i->v)!=JRY_BL_VAR_TYPE_UNUSE)
#define 				jry_bl_hash_table_insert_str(x,k,v,s1,s2,v2)		jry_bl_string_equal_char_pointer_light((s1),(k));jry_bl_string_equal_char_pointer_light((s2),(v));jry_bl_var_equal_string_light_move((v2),(s2));jry_bl_hash_table_insert((x),(s1),(v2),(JRY_BL_COPY_LIGHT_MOVE),(JRY_BL_COPY_LIGHT_MOVE));	
#define 				jry_bl_hash_table_insert_str_int64(x,k,v,s1,v2)		jry_bl_string_equal_char_pointer_light((s1),(k));jry_bl_var_equal_int64((v2),(v));jry_bl_hash_table_insert((x),(s1),(v2),(JRY_BL_COPY_LIGHT_MOVE),(JRY_BL_COPY_LIGHT_MOVE));	
#define 				jry_bl_hash_table_insert_str_uint64(x,k,v,s1,v2)	jry_bl_string_equal_char_pointer_light((s1),(k));jry_bl_var_equal_uint64((v2),(v));jry_bl_hash_table_insert((x),(s1),(v2),(JRY_BL_COPY_LIGHT_MOVE),(JRY_BL_COPY_LIGHT_MOVE));	
#define 				jry_bl_hash_table_insert_str_double(x,k,v,s1,v2)	jry_bl_string_equal_char_pointer_light((s1),(k));jry_bl_var_equal_double((v2),(v));jry_bl_hash_table_insert((x),(s1),(v2),(JRY_BL_COPY_LIGHT_MOVE),(JRY_BL_COPY_LIGHT_MOVE));	
#define 				jry_bl_hash_table_insert_str_true(x,k,s1,v2)		jry_bl_string_equal_char_pointer_light((s1),(k));jry_bl_var_equal_true((v2));jry_bl_hash_table_insert((x),(s1),(v2),(JRY_BL_COPY_LIGHT_MOVE),(JRY_BL_COPY_LIGHT_MOVE));	
#define 				jry_bl_hash_table_insert_str_false(x,k,s1,v2)		jry_bl_string_equal_char_pointer_light((s1),(k));jry_bl_var_equal_false((v2));jry_bl_hash_table_insert((x),(s1),(v2),(JRY_BL_COPY_LIGHT_MOVE),(JRY_BL_COPY_LIGHT_MOVE));	
#if JRY_BL_USE_STDIO==1
#define					jry_bl_hash_table_view(x,y) 						jry_bl_hash_table_view_ex(x,y,#x " @ "__FILE__,__LINE__,jry_bl_view_default_tabs_num)
void 					jry_bl_hash_table_view_ex							(const jry_bl_hash_table *this,FILE * file,char*str,int a,int tabs);
#endif
#if JRY_BL_USE_STDARG==1
void					jry_bl_hash_table_inits								(int n,...);
void					jry_bl_hash_table_frees								(int n,...);
void					jry_bl_hash_table_clears							(int n,...);
#endif

#endif
#endif