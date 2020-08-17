/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_MALLOC_H
#define __JBL_MALLOC_H
#include "jbl_malloc_config.h"
#if JBL_MALLOC_ENABLE==1
/*******************************************************************************************/
/*                            依赖 jbl_ying jbl_bitset jbl_exception                         */
/*******************************************************************************************/
#include "jbl_ying.h"
#include "jbl_bitset.h"
#include "jbl_exception.h"
/*******************************************************************************************/
/*                            联动 jbl_stream                                               */
/*******************************************************************************************/
#include "jbl_stream.h"
#if JBL_MALLOC_COUNT==1
jbl_malloc_size_type	jbl_malloc_used_size	();
#endif
/*******************************************************************************************/
/*                            以下函数完成内存复制操作                           */
/*******************************************************************************************/
void					jbl_memory_copy			(void *to,const void * from,jbl_malloc_size_type len);								//复制
void					jbl_memory_copy_reverse	(void *to,const void * from,jbl_malloc_size_type len,jbl_malloc_size_type size);	//倒序复制
/*******************************************************************************************/
/*                            以下函数完成内存管理组件启动和停止                           */
/*******************************************************************************************/
void					jbl_malloc_start		();										//启动内存管理
void					jbl_malloc_stop			();										//关闭内存管理
/*******************************************************************************************/
/*                            以下函数完成内存管理组件基本操作                           */
/*******************************************************************************************/
void*					jbl_malloc				(jbl_malloc_size_type size);			//申请一段内存
void*					jbl_realloc				(void* ptr,jbl_malloc_size_type size);	//重新申请一段内存
void					jbl_free				(void* ptr);							//释放一段内存
jbl_malloc_size_type	jbl_malloc_size			(void* ptr);							//获取一段内存的大小
#else
#define jbl_malloc_start()	1
#define jbl_malloc_stop()	1
#endif
#endif