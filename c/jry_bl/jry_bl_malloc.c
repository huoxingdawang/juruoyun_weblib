/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_bl_malloc.h"
#if JRY_BL_MALLOC_ENABLE==1
#if JRY_BL_MALLOC_DEBUG_MODE==1
size_t __jry_bl_malloced_size=0;
#endif
void jry_bl_malloc_start()
{
#if JRY_BL_MALLOC_DEBUG_MODE==1
	__jry_bl_malloced_size=0;
#endif	
}
void* jry_bl_malloc(size_t size)
{
	void *ptr=malloc(size);
	if(ptr==NULL)
		jry_bl_exception(JRY_BL_ERROR_MEMORY_ERROR);	
#if JRY_BL_MALLOC_DEBUG_MODE==1
	__jry_bl_malloced_size+=jry_bl_malloc_size(ptr);
#endif
	return ptr;
}
size_t jry_bl_malloc_size(void* ptr)
{
	if(ptr==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
#ifdef __linux__
	return malloc_usable_size(ptr);
#else
	return _msize(ptr);
#endif
}
void* jry_bl_realloc(void* ptr,size_t size)
{
	if(ptr==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
#ifdef __linux__	
	void *ptr2=realloc(ptr,size);
	if(ptr2==NULL)
		jry_bl_exception(JRY_BL_ERROR_MEMORY_ERROR);		
#if JRY_BL_MALLOC_DEBUG_MODE==1
	__jry_bl_malloced_size-=jry_bl_malloc_size(ptr);
	__jry_bl_malloced_size+=jry_bl_malloc_size(ptr2);
#endif		
	return ptr2;
#else
	void * p=jry_bl_malloc(size);
	if(p==NULL)
		jry_bl_exception(JRY_BL_ERROR_MEMORY_ERROR);	
	size_t s=_msize(ptr);
	if(s>size)
		s=size;
	jry_bl_memory_copy(p,ptr,s);
	jry_bl_free(ptr);
	return p;
#endif
}
void jry_bl_free(void * p)
{
	if(p==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
#if JRY_BL_MALLOC_DEBUG_MODE==1
	__jry_bl_malloced_size-=jry_bl_malloc_size(p);
#endif	
	free(p);
}
void jry_bl_memory_copy(void *to,const void * from,size_t len){for(register size_t i=0;i<len;++i)((char*)to)[i]=((char*)from)[i];}
void jry_bl_memory_copy_reverse(void *to,const void * from,size_t len,size_t size){for(register size_t i=0;i<len;i+=size)for(register size_t j=0;j<size;++j)((char*)to)[i+j]=((char*)from)[len-i-size+j];}
#endif
