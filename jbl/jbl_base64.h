/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_BASE64_H
#define __JBL_BASE64_H
#include "jbl_base64_config.h"
#if JBL_BASE64_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_ying jbl_exception                                   */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_ying.h"
/*******************************************************************************************/
/*                            联动jbl_stream jbl_string                                    */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_stream.h"

#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的base64编解码操作                      */
/*******************************************************************************************/
jbl_string *	jbl_base64_encode					(jbl_string *this,jbl_string *result);	//base64加密
jbl_string *	jbl_base64_decode					(jbl_string *this,jbl_string *result);	//base64解密
#endif

#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现stream的base64编解码操作                       */
/*******************************************************************************************/
extern			const jbl_stream_operater			jbl_stream_base64_encode_operators;						//base64加密流处理器
extern			const jbl_stream_operater 			jbl_stream_base64_decode_operators;						//base64解密流处理器
#define			jbl_stream_base64_encode_new()		jbl_stream_new(&jbl_stream_base64_encode_operators,NULL,JBL_STREAM_EXCEED_LENGTH+8,NULL,0)	//新建base64加密流
#define			jbl_stream_base64_decode_new()		jbl_stream_new(&jbl_stream_base64_decode_operators,NULL,JBL_STREAM_EXCEED_LENGTH+8,NULL,0)	//新建base64解密流

#endif

#endif
#endif