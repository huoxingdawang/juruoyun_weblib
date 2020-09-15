/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_STRING_CONFIG_H
#define __JBL_STRING_CONFIG_H
#include "jbl_config.h"
#if JBL_STRING_ENABLE==1
#include "jbl_ying.h"
#if __SIZEOF_POINTER__ == 8
	typedef jbl_uint64 			jbl_string_size_type;		//字符串长度类型
	typedef jbl_uint64 			jbl_string_hash_type;		//字符串哈希类型
#else
	typedef jbl_uint32 			jbl_string_size_type;		//字符串长度类型
	typedef jbl_uint32 			jbl_string_hash_type;		//字符串哈希类型
#endif

#define JBL_STRING_MIN_LENGTH						8		//最小长度
#define JBL_STRING_USE_CACHE						1		//是否使用缓冲,依赖于ht的sys模式
#define JBL_STRING_USE_CACHE_WHEN_JSON_DECODE		1		//JSONdecode时是否使用缓冲
#define JBL_STRING_CACHE_MAX_LENGTH					128		//参与缓冲字符串最大长度
#define JBL_STRING_CACHE_DEBUG						1		//调试模式

#define JBL_STRING_CC_ENABLE						1		//jbl_string_cc.c
#define JBL_STRING_CC_DEBUG							1		//jbl_string_cc.c
#define JBL_STRING_CC_INNER_DB_ENABLE				0		//使用内联表
#define JBL_STRING_CC_OUT_DB_DIR					"jbl/jbl_string_cc_table.db"		//加载表的地址

#endif
#endif
