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
#include "jbl_stream.h"	
typedef struct __jbl_string jbl_string;
typedef struct __jbl_var_operators
{
	void*	(*free)(void*);
	void*	(*copy)(void*);
	char	(*space_ship)(const void*,const void*);
#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
	jbl_string*	(*json_encode)(const void*,jbl_string *,jbl_uint8,jbl_uint32);
#endif
#endif	
#if JBL_STREAM_ENABLE==1
	void *	(*view_put)(void*,jbl_stream *,jbl_uint8,jbl_uint32,jbl_uint32 line,unsigned char*,unsigned char *,unsigned char *);
#if JBL_JSON_ENABLE==1
	void	(*json_put)(const void*,jbl_stream *,jbl_uint8,jbl_uint32);
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
				(jbl_string*(*)(const void*,jbl_string *,jbl_uint8,jbl_uint32))json_encode,				\
				(void* (*)(void*,jbl_stream *,jbl_uint8,jbl_uint32,jbl_uint32 line,unsigned char*,unsigned char *,unsigned char *))view_put,				\
				(void(*)(const void*,jbl_stream *,jbl_uint8,jbl_uint32))json_put,							\
			};
		#else
			//STRING 开 STREAM 开 JSON 关
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
				(void* (*)(void*,jbl_stream *,jbl_uint8,jbl_uint32,jbl_uint32 line,unsigned char*,unsigned char *,unsigned char *))view_put,				\
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
				(jbl_string*(*)(const void*,jbl_string *,jbl_uint8,jbl_uint32))json_encode,				\
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
				(void* (*)(void*,jbl_stream *,jbl_uint8,jbl_uint32,jbl_uint32 line,unsigned char*,unsigned char *,unsigned char *))view_put,				\
				(void(*)(const void*,jbl_stream *,jbl_uint8,jbl_uint32))json_put,							\
			};
		#else
			//STRING 关 STREAM 开 JSON 关
			#define jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)	\
			const jbl_var_operators name={															\
				(void* (*)(void *))free,															\
				(void* (*)(void *))copy,															\
				(char  (*)(const void*,const void*))space_ship,										\
				(void* (*)(void*,jbl_stream *,jbl_uint8,jbl_uint32,jbl_uint32 line,unsigned char*,unsigned char *,unsigned char *))view_put,				\
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
#define						jbl_var_operators_extern(name)	extern	const		jbl_var_operators name


void *						jbl_var_set_operators		(void * this,const jbl_var_operators *ops);		//设置一个var的操作器
const jbl_var_operators *	jbl_var_get_operators		(void * this);									//获取一个var的操作器
void *						jbl_var_free				(void * this);									//释放一个var
void *						jbl_var_copy				(void * this);									//复制一个var
char						jbl_var_space_ship			(void * this,void * that);						//var的太空船操作符
#define						jbl_var_is(x,y)				(jbl_var_get_operators(x)==&y)

#if JBL_STREAM_ENABLE==1
void *						jbl_var_view_put			(void* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个var
#define						jbl_var_view(x)				jbl_var_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个var
#if JBL_JSON_ENABLE==1
void 						jbl_var_json_put			(void * this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);	//从从out JSON格式化一个var
#endif
#endif

#if JBL_STRING_ENABLE==1 && JBL_JSON_ENABLE==1
jbl_string *				jbl_var_json_encode			(void * this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);	//JSON编码一个var
#endif





#include "jbl_var_data.h"
#else
#define						jbl_var_set_operators(x,y)	
#define						jbl_var_get_operators(x,y)	(NULL)
#define						jbl_var_is(x,y)				(1)
#define						jbl_var_operators_new(name,free,copy,space_ship,json_encode,view_put,json_put)
#define						jbl_var_operators_extern(name)
#endif
#endif