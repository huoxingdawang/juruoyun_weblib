/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_VAR_H
#define __JBL_VAR_H
#include "jbl_var_config.h"
#if JBL_VAR_ENABLE==1
#include "jbl_ying.h"
#if JBL_STREAM_ENABLE==1
#include "jbl_stream.h"	
#endif
#if JBL_STRING_ENABLE==1
#include "jbl_string.h"	
#endif
typedef struct __jbl_var_operators
{
	void*	(*free)(void*);
	void*	(*copy)(void*);
	char	(*space_ship)(const void*,const void*);
#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
	jbl_string*	(*json_encode)(const void*,jbl_string *,char,jbl_int32);
#endif
#endif	
#if JBL_STREAM_ENABLE==1
	void	(*view_put)(const void*,jbl_stream *,jbl_int32,char*,jbl_int32);
#if JBL_JSON_ENABLE==1
	void	(*json_put)(const void*,jbl_stream *,char,jbl_int32);
#endif
#endif
}jbl_var_operators;

#if JBL_STRING_ENABLE==1
	#if JBL_STREAM_ENABLE==1
		#if JBL_JSON_ENABLE==1
			//STRING 开 STREAM 开 JSON 开
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
				(jbl_string*(*)(const void*,jbl_string *,char,jbl_int32))json_encode,				\
				(void(*)(const void*,jbl_stream *,jbl_int32,char*,jbl_int32))view_put,				\
				(void(*)(const void*,jbl_stream *,char,jbl_int32))json_put,							\
			};
		#else
			//STRING 开 STREAM 开 JSON 关
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
				(void(*)(const void*,jbl_stream *,jbl_int32,char*,jbl_int32))view_put,				\
			};
		#endif
	#else
		#if JBL_JSON_ENABLE==1
			//STRING 开 STREAM 关 JSON 开
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
				(jbl_string*(*)(const void*,jbl_string *,char,jbl_int32))json_encode,				\
			};
		#else
			//STRING 开 STREAM 关 JSON 关
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
			};
		#endif
	#endif
#else
	#if JBL_STREAM_ENABLE==1
		#if JBL_JSON_ENABLE==1
			//STRING 关 STREAM 开 JSON 开
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
				(void(*)(const void*,jbl_stream *,jbl_int32,char*,jbl_int32))view_put,				\
				(void(*)(const void*,jbl_stream *,char,jbl_int32))json_put,							\
			};
		#else
			//STRING 关 STREAM 开 JSON 关
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
				(void(*)(const void*,jbl_stream *,jbl_int32,char*,jbl_int32))view_put,				\
			};
		#endif
	#else
		#if JBL_JSON_ENABLE==1
			//STRING 关 STREAM 关 JSON 开
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
			};
		#else
			//STRING 关 STREAM 关 JSON 关
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
			};
		#endif
	#endif
#endif



typedef struct __jbl_var
{
	const jbl_var_operators *ops;
}jbl_var;
typedef struct __jbl_var_data
{
	jbl_gc gc;
	union
	{
		jbl_uint64 u;
		jbl_int64 i;
		double d;
	};
}jbl_var_data;

#define		jbl_V(x)						((jbl_var*)x)														//按照var使用一个变量
jbl_var *	jbl_var_set_operators			(jbl_var * this,const jbl_var_operators *ops);						//设置一个var的操作器
const jbl_var_operators *	jbl_var_get_operators			(const jbl_var * this);													//获取一个var的操作器
jbl_var *	jbl_var_free					(jbl_var * this);													//释放一个var
jbl_var *	jbl_var_copy					(jbl_var * this);													//复制一个var
char		jbl_var_space_ship				(const jbl_var * this,const jbl_var * that);						//var的太空船操作符
jbl_var *jbl_var_copy_as(void * that,const jbl_var_operators *ops);
#if JBL_STREAM_ENABLE==1
void		jbl_var_view_put				(const jbl_var * this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs);	//从out浏览一个var
#define		jbl_var_view(x)					jbl_var_view_put(x,jbl_stream_stdout,__LINE__,#x " @ "__FILE__,JBL_VIEW_DEFAULT_TABS),jbl_stream_push_char(jbl_stream_stdout,'\n')	//浏览一个var
#if JBL_JSON_ENABLE==1
void 		jbl_var_json_put				(const jbl_var * this,jbl_stream *out,char format,jbl_int32 tabs);	//从从out JSON格式化一个var
#endif
#endif

#if JBL_STRING_ENABLE==1

#if JBL_JSON_ENABLE==1
jbl_string *	jbl_var_json_encode			(const jbl_var * this,jbl_string *out,char format,jbl_int32 tabs);	//JSON编码一个var
jbl_var    *	jbl_var_json_decode			(jbl_var *this,const jbl_string* in,jbl_string_size_type *start);	//JSON解码一个var
#endif

#endif
extern const	jbl_var_operators			jbl_uint_operators;
extern const	jbl_var_operators			jbl_int_operators;
extern const	jbl_var_operators			jbl_double_operators;
extern const	jbl_var_operators			jbl_ntf_operators;

jbl_var *		jbl_Vuint_new				();													//新建一个var格式的uint
jbl_var *		jbl_Vuint_set				(jbl_var * this,jbl_uint64 data);					//设置一个var格式的uint
jbl_var *		jbl_Vuint_copy				(jbl_var * that);									//复制一个var格式的uint
char			jbl_Vuint_space_ship		(jbl_var * this,jbl_var * that);					//比较两个var格式的uint
#define			Vis_jbl_uint(x)				(jbl_var_get_operators(x)==&jbl_uint_operators)		//判断一个var是不是uint

jbl_var *		jbl_Vint_new				();													//新建一个var格式的int
jbl_var *		jbl_Vint_set				(jbl_var * this,jbl_int64 data);					//设置一个var格式的int
jbl_var *		jbl_Vint_copy				(jbl_var * that);									//复制一个var格式的int
char			jbl_Vint_space_ship			(jbl_var * this,jbl_var * that);					//比较两个var格式的int
#define			Vis_jbl_int(x)				(jbl_var_get_operators(x)==&jbl_int_operators)		//判断一个var是不是int

jbl_var *		jbl_Vdouble_new				();													//新建一个var格式的double
jbl_var *		jbl_Vdouble_set				(jbl_var * this,double data);						//设置一个var格式的double
jbl_var *		jbl_Vdouble_copy			(jbl_var * that);									//复制一个var格式的double
char			jbl_Vdouble_space_ship		(jbl_var * this,jbl_var * that);					//比较两个var格式的double
#define			Vis_jbl_double(x)			(jbl_var_get_operators(x)==&jbl_double_operators)	//判断一个var是不是double

jbl_var *		jbl_Vnull_new				();													//新建一个var格式的null
jbl_var *		jbl_Vtrue_new				();													//新建一个var格式的true
jbl_var *		jbl_Vfalse_new				();													//新建一个var格式的false
jbl_var *		jbl_Vntf_copy				(jbl_var * that);									//复制一个var格式的null或true或false
char			jbl_Vntf_space_ship			(jbl_var * this,jbl_var * that);					//比较两个var格式的null或true或false (null<false<true)
#define			Vis_jbl_ntf(x)				(jbl_var_get_operators(x)==&jbl_ntf_operators)		//判断一个var是不是null或true或false
#define			Vis_jbl_null(x)				((!x)||Vis_jbl_ntf(x)&&((jbl_var_data*)jbl_refer_pull(x))->u==0)	//判断一个varv是不是null
#define			Vis_jbl_true(x)				(Vis_jbl_ntf(x)&&((jbl_var_data*)jbl_refer_pull(x))->u==2)			//判断一个varv是不是true
#define			Vis_jbl_false(x)			(Vis_jbl_ntf(x)&&((jbl_var_data*)jbl_refer_pull(x))->u==1)			//判断一个varv是不是false

#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
jbl_string * 	jbl_Vuint_json_encode		(const jbl_var* this,jbl_string *out,char format,jbl_int32 tabs);	//JSON编码一个var格式的uint
jbl_string * 	jbl_Vint_json_encode		(const jbl_var* this,jbl_string *out,char format,jbl_int32 tabs);	//JSON编码一个var格式的int
jbl_string * 	jbl_Vdouble_json_encode		(const jbl_var* this,jbl_string *out,char format,jbl_int32 tabs);	//JSON编码一个var格式的double
jbl_string *	jbl_Vntf_json_encode		(const jbl_var* this,jbl_string *out,char format,jbl_int32 tabs);	//JSON编码一个var格式的null或true或false
#endif
#endif
#if JBL_STREAM_ENABLE==1
void 			jbl_Vuint_view_put			(const jbl_var* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs);	//从从out 浏览一个var格式的uint
void 			jbl_Vint_view_put			(const jbl_var* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs);	//从从out 浏览一个var格式的int
void 			jbl_Vdouble_view_put		(const jbl_var* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs);	//从从out 浏览一个var格式的double
void			jbl_Vntf_view_put			(const jbl_var* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs);	//从从out 浏览一个var格式的null或true或false
#if JBL_JSON_ENABLE==1
void 			jbl_Vuint_json_put			(const jbl_var* this,jbl_stream *out,char format,jbl_int32 tabs);	//从从out JSON格式化一个var格式的uint
void 			jbl_Vint_json_put			(const jbl_var* this,jbl_stream *out,char format,jbl_int32 tabs);	//从从out JSON格式化一个var格式的int
void 			jbl_Vdouble_json_put		(const jbl_var* this,jbl_stream *out,char format,jbl_int32 tabs);	//从从out JSON格式化一个var格式的double
void			jbl_Vntf_json_put			(const jbl_var* this,jbl_stream *out,char format,jbl_int32 tabs);	//从从out JSON格式化一个var格式的null或true或false
#endif
#endif

#endif
#endif