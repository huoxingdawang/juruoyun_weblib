/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_VAR_DATA_H
#define __JBL_VAR_DATA_H
#include "jbl_var_config.h"
#if JBL_VAR_ENABLE==1
#include "jbl_ying.h"
#include "jbl_stream.h"	
#include "jbl_var.h"	
typedef struct __jbl_var_data
{
	jbl_gc gc;
#if JBL_VAR_ENABLE==1
	jbl_var_operators *		var_ops;
#endif
	union
	{
		jbl_uint64 u;
		jbl_int64 i;
		double d;
	};
}jbl_var_data;
extern const	jbl_var_operators			jbl_uint_operators;
extern const	jbl_var_operators			jbl_int_operators;
extern const	jbl_var_operators			jbl_double_operators;
extern const	jbl_var_operators			jbl_ntf_operators;

jbl_var_data *	jbl_Vuint_new				();													//新建一个var格式的uint
jbl_var_data *	jbl_Vuint_set				(jbl_var_data * this,jbl_uint64 data);					//设置一个var格式的uint
jbl_var_data *	jbl_Vuint_copy				(jbl_var_data * that);									//复制一个var格式的uint
char			jbl_Vuint_space_ship		(jbl_var_data * this,jbl_var_data * that);					//比较两个var格式的uint
#define			Vis_jbl_uint(x)				(jbl_var_get_operators(x)==&jbl_uint_operators)		//判断一个var是不是uint

jbl_var_data *	jbl_Vint_new				();													//新建一个var格式的int
jbl_var_data *	jbl_Vint_set				(jbl_var_data * this,jbl_int64 data);					//设置一个var格式的int
jbl_var_data *	jbl_Vint_copy				(jbl_var_data * that);									//复制一个var格式的int
char			jbl_Vint_space_ship			(jbl_var_data * this,jbl_var_data * that);					//比较两个var格式的int
#define			Vis_jbl_int(x)				(jbl_var_get_operators(x)==&jbl_int_operators)		//判断一个var是不是int

jbl_var_data *	jbl_Vdouble_new				();													//新建一个var格式的double
jbl_var_data *	jbl_Vdouble_set				(jbl_var_data * this,double data);						//设置一个var格式的double
jbl_var_data *	jbl_Vdouble_copy			(jbl_var_data * that);									//复制一个var格式的double
char			jbl_Vdouble_space_ship		(jbl_var_data * this,jbl_var_data * that);					//比较两个var格式的double
#define			Vis_jbl_double(x)			(jbl_var_get_operators(x)==&jbl_double_operators)	//判断一个var是不是double

jbl_var_data *	jbl_Vnull_new				();													//新建一个var格式的null
jbl_var_data *	jbl_Vtrue_new				();													//新建一个var格式的true
jbl_var_data *	jbl_Vfalse_new				();													//新建一个var格式的false
jbl_var_data *	jbl_Vntf_copy				(jbl_var_data * that);									//复制一个var格式的null或true或false
char			jbl_Vntf_space_ship			(jbl_var_data * this,jbl_var_data * that);					//比较两个var格式的null或true或false (null<false<true)
#define			Vis_jbl_ntf(x)				(jbl_var_get_operators(x)==&jbl_ntf_operators)		//判断一个var是不是null或true或false
#define			Vis_jbl_null(x)				((!x)||Vis_jbl_ntf(x)&&((jbl_var_data*)jbl_refer_pull(x))->u==0)	//判断一个varv是不是null
#define			Vis_jbl_true(x)				(Vis_jbl_ntf(x)&&((jbl_var_data*)jbl_refer_pull(x))->u==2)			//判断一个varv是不是true
#define			Vis_jbl_false(x)			(Vis_jbl_ntf(x)&&((jbl_var_data*)jbl_refer_pull(x))->u==1)			//判断一个varv是不是false

#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
jbl_string * 	jbl_Vuint_json_encode		(jbl_var_data* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);	//JSON编码一个var格式的uint
jbl_string * 	jbl_Vint_json_encode		(jbl_var_data* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);	//JSON编码一个var格式的int
jbl_string * 	jbl_Vdouble_json_encode		(jbl_var_data* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);	//JSON编码一个var格式的double
jbl_string *	jbl_Vntf_json_encode		(jbl_var_data* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);	//JSON编码一个var格式的null或true或false
#endif
#endif
#if JBL_STREAM_ENABLE==1
jbl_var_data * 	jbl_Vuint_view_put			(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从从out 浏览一个var格式的uint
jbl_var_data * 	jbl_Vint_view_put			(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从从out 浏览一个var格式的int
jbl_var_data * 	jbl_Vdouble_view_put		(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从从out 浏览一个var格式的double
jbl_var_data *	jbl_Vntf_view_put			(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从从out 浏览一个var格式的null或true或false
#if JBL_JSON_ENABLE==1
void 			jbl_Vuint_json_put			(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);	//从从out JSON格式化一个var格式的uint
void 			jbl_Vint_json_put			(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);	//从从out JSON格式化一个var格式的int
void 			jbl_Vdouble_json_put		(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);	//从从out JSON格式化一个var格式的double
void			jbl_Vntf_json_put			(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);	//从从out JSON格式化一个var格式的null或true或false
#endif
#endif

#endif
#endif