/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_malloc.h"
#if JBL_MALLOC_ENABLE==1
/*******************************************************************************************/
/*                            依赖 jbl_ying jbl_bitset jbl_exception                         */
/*******************************************************************************************/
#include "jbl_ying.h"
#include "jbl_bitset.h"
#include "jbl_exception.h"
#include "jbl_pthread.h"
/*******************************************************************************************/
/*                            联动 jbl_stream                                               */
/*******************************************************************************************/
#include "jbl_stream.h"
/*******************************************************************************************/
/*                            以下函数完成内存复制操作                                    */
/*******************************************************************************************/
inline void jbl_memory_copy(void *to,const void * from,jbl_malloc_size_type len){for(register jbl_malloc_size_type i=0;i<len;((unsigned char*)to)[i]=((unsigned char*)from)[i],++i);}
void jbl_memory_copy_reverse(void *to,const void * from,jbl_malloc_size_type len,jbl_malloc_size_type size){for(register jbl_malloc_size_type i=0;i<len;i+=size)for(register jbl_malloc_size_type j=0;j<size;++j)((char*)to)[i+j]=((char*)from)[len-i-size+j];}
jbl_uint64 __jbl_malloc_get_ignore_size()//该函数获取了输出流所占空间,用于程序结束时统计内存占用时忽略
{
	jbl_uint64 size=0;
#if JBL_STREAM_ENABLE==1
	jbl_stream* tmp=jbl_stream_stdout;
start:;
	size+=jbl_malloc_size(tmp);
	while(jbl_gc_is_ref(tmp))
	{
		tmp=((jbl_reference *)tmp)->ptr;
		size+=jbl_malloc_size(tmp);
	}
	if(tmp->nxt){tmp=tmp->nxt;goto start;}
#endif
	return size;
}
#if JBL_MALLOC_FAST==0
/*******************************************************************************************/
/*                            系统文件包含                                                   */
/*******************************************************************************************/
#ifdef _WIN32
	#include <malloc.h>
#elif __APPLE__
	#include <malloc/malloc.h>
	#include <stdlib.h>
#elif __linux__
	#include <malloc.h>
#endif
#if JBL_MALLOC_LOG ==1
#include "jbl_log.h"
#endif
/*******************************************************************************************/
/*                            结构体定义                                                   */
/*******************************************************************************************/
typedef struct __jbl_malloc_heap_struct
{
	jbl_pthread_lock_define;
	jbl_malloc_size_type size;
	jbl_malloc_size_type peak;
}jbl_malloc_heap_struct;
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
#if JBL_MALLOC_COUNT==1
	jbl_malloc_heap_struct jbl_malloc_heap;
#endif
#if JBL_MALLOC_COUNT==1
jbl_malloc_size_type	jbl_malloc_used_size	(){return jbl_malloc_heap.size;}
#endif
#if JBL_MALLOC_LOG ==0
#undef jbl_log
#define jbl_log(x,...)
#endif
/*******************************************************************************************/
/*                            以下函数完成内存管理组件启动和停止                           */
/*******************************************************************************************/
void jbl_malloc_start()
{
	jbl_pthread_lock_init(&jbl_malloc_heap);
#if JBL_MALLOC_COUNT==1
	jbl_malloc_heap.size=0;
	jbl_malloc_heap.peak=0;
#endif
}
void jbl_malloc_stop()
{
#if JBL_STREAM_ENABLE==1
#if JBL_MALLOC_COUNT==1
//输出内存统计
	jbl_stream_push_chars(jbl_stream_stdout,UC"\n\n");
	jbl_stream_push_chars(jbl_stream_stdout,UC"Memory            :");jbl_stream_push_uint(jbl_stream_stdout,jbl_malloc_heap.size-__jbl_malloc_get_ignore_size());jbl_stream_push_chars(jbl_stream_stdout,UC"B\n");	
	jbl_stream_push_chars(jbl_stream_stdout,UC"Max memory        :");jbl_stream_push_uint(jbl_stream_stdout,jbl_malloc_heap.peak);jbl_stream_push_chars(jbl_stream_stdout,UC"B(");jbl_stream_push_double(jbl_stream_stdout,(double)jbl_malloc_heap.peak/1048576);jbl_stream_push_chars(jbl_stream_stdout,UC"M)\n");	
#endif
//统计完成关流
	jbl_stream_do(jbl_stream_stdout,jbl_stream_force);
	jbl_stream_stdout=jbl_stream_free(jbl_stream_stdout);//强推，关闭
#endif
}
/*******************************************************************************************/
/*                            以下函数完成内存管理组件基本操作                           */
/*******************************************************************************************/
JBL_INLINE void* jbl_malloc(jbl_malloc_size_type size)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!size)jbl_exception("MEMORY ERROR");
#endif
	void *ptr=malloc(size);
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("MEMORY ERROR");
#endif
	jbl_log(UC "addr:0X%X\tsize:%d",ptr,jbl_malloc_size(ptr));
#if JBL_MALLOC_COUNT ==1
	jbl_pthread_lock_wrlock(&jbl_malloc_heap);
	jbl_malloc_heap.size+=jbl_malloc_size(ptr),jbl_max_update(jbl_malloc_heap.peak,jbl_malloc_heap.size);
	jbl_pthread_lock_unlock(&jbl_malloc_heap);
#endif
	return ptr;
}
JBL_INLINE jbl_malloc_size_type jbl_malloc_size(void* ptr)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("NULL POINTER");	
#endif	
#ifdef _WIN32
	return _msize(ptr);
#elif __APPLE__
	return malloc_size(ptr);
#elif __linux__
	return malloc_usable_size(ptr);
#endif	
}
void* jbl_realloc(void* ptr,jbl_malloc_size_type size)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("NULL POINTER");	
	if(!size)jbl_exception("MEMORY ERROR");		
#endif
#ifdef _WIN32
	void * ptr2=malloc(size);
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr2)jbl_exception("MEMORY ERROR");
#endif
	jbl_malloc_size_type size_old=_msize(ptr);
#if JBL_MALLOC_COUNT ==1
	jbl_pthread_lock_wrlock(&jbl_malloc_heap);
	jbl_malloc_heap.size-=size_old;
	jbl_malloc_heap.size+=_msize(ptr2),jbl_max_update(jbl_malloc_heap.peak,jbl_malloc_heap.size);	
	jbl_pthread_lock_unlock(&jbl_malloc_heap);
#endif
	jbl_min_update(size_old,size);
	jbl_memory_copy(ptr2,ptr,size_old);
	free(ptr);
	jbl_log(UC "addr:0X%X\tto addr:0X%X\tsize:%d",ptr,ptr2,_msize(ptr2));
	return ptr2;
#elif defined(__APPLE__) || defined(__linux__)
#if JBL_MALLOC_COUNT ==1
	jbl_pthread_lock_wrlock(&jbl_malloc_heap);
	jbl_malloc_heap.size-=jbl_malloc_size(ptr);
	jbl_pthread_lock_unlock(&jbl_malloc_heap);
#endif
	void *ptr2=realloc(ptr,size);
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr2)jbl_exception("MEMORY ERROR");
#endif
#if JBL_MALLOC_COUNT ==1
	jbl_pthread_lock_wrlock(&jbl_malloc_heap);
	jbl_malloc_heap.size+=jbl_malloc_size(ptr2),jbl_max_update(jbl_malloc_heap.peak,jbl_malloc_heap.size);
	jbl_pthread_lock_unlock(&jbl_malloc_heap);
#endif
	jbl_log(UC "addr:0X%X\tto addr:0X%X\tsize:%d",ptr,ptr2,jbl_malloc_size(ptr2));
	return ptr2;
#endif
}
JBL_INLINE void jbl_free(void * ptr)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("NULL POINTER");	
#endif
	jbl_log(UC "addr:0X%X",ptr);
#if JBL_MALLOC_COUNT ==1
	jbl_pthread_lock_wrlock(&jbl_malloc_heap);
	jbl_malloc_heap.size-=jbl_malloc_size(ptr);
	jbl_pthread_lock_unlock(&jbl_malloc_heap);
#endif
	free(ptr);
}
#endif
#endif
