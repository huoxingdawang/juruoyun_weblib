/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_STRING_CC_H
#define __JBL_STRING_CC_H
#include "jbl_string_config.h"
#if JBL_STRING_CC_ENABLE==1
#include "jbl_string.h"
/*******************************************************************************************/
/*                            以下函数完成字符串编码转换扩展组件启动和停止                         */
/*******************************************************************************************/
void					jbl_string_cc_start						();
void					jbl_string_cc_stop						();
/*******************************************************************************************/
/*                            以下函数实现utf8 gb2312 unicode互转                              */
/*******************************************************************************************/

jbl_string*				jbl_string_to_gb2312_from_utf8			(jbl_string* this,jbl_string* that);				//把字符串转换为gb2312编码
jbl_string*				jbl_string_to_utf8_from_gb2312			(jbl_string* this,jbl_string* that);				//把字符串转成utf8编码
#define					jbl_string_get_unicode_from_utf8(this)	jbl_string_get_unicode_from_utf8_start(this,0)		//从一个utf8字符串中获取一个unicode
jbl_uint32				jbl_string_get_unicode_from_utf8_start	(jbl_string *this,jbl_string_size_type *start);		//从一个utf8字符串start位开始获取一个unicode
jbl_string *			jbl_string_add_utf8_from_unicode		(jbl_string *this,jbl_uint32 unicode);				//把unicode转换成utf8并添加到字符串末尾
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现stream的转码操作                                     */
/*******************************************************************************************/
extern			const jbl_stream_operater			jbl_stream_utf8_to_gb2312_operators;				//utf8到gb2312转码流处理器
extern			const jbl_stream_operater 			jbl_stream_gb2312_to_utf8_operators;				//gb2312到utf8转码流处理器
#define			jbl_stream_utf8_to_gb2312_new()		jbl_stream_new(&jbl_stream_utf8_to_gb2312_operators,NULL,JBL_STREAM_EXCEED_LENGTH+8,NULL,0)	//新建utf8到gb2312转码流处理器
#define			jbl_stream_gb2312_to_utf8_new()		jbl_stream_new(&jbl_stream_gb2312_to_utf8_operators,NULL,JBL_STREAM_EXCEED_LENGTH+8,NULL,0)	//新建gb2312到utf8转码流处理器
#if JBL_VAR_ENABLE == 1
#define			jbl_Vstream_utf8_to_gb2312_new()	jbl_Vstream_new(&jbl_stream_utf8_to_gb2312_operators,NULL,JBL_STREAM_EXCEED_LENGTH+8,NULL,0)	//新建var格式的utf8到gb2312转码流处理器
#define			jbl_Vstream_gb2312_to_utf8_new()	jbl_Vstream_new(&jbl_stream_gb2312_to_utf8_operators,NULL,JBL_STREAM_EXCEED_LENGTH+8,NULL,0)	//新建var格式的gb2312到utf8转码流处理器
#endif

#endif



#endif
#endif
