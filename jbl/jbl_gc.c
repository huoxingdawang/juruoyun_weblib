/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_gc.h"
#if JBL_GC_ENABLE==1
#include "jbl_pthread.h"


JBL_INLINE void* jbl_gc_plus	(void *this){if(this)((jbl_reference*)this)->gc+=16;return this;};
JBL_INLINE void* jbl_gc_minus	(void *this){if(this)((jbl_reference*)this)->gc-=16;return this;};


void * jbl_refer(void *ptr)
{
	if(ptr==NULL)jbl_exception("NULL POINTER");
	jbl_reference *pptr=*((jbl_reference**)(ptr));
	if(pptr==NULL)jbl_exception("NULL POINTER");
	jbl_reference *this;
	this=jbl_malloc((sizeof(jbl_reference)));
	jbl_gc_init(this);
	jbl_gc_plus(this);//增加引用计数
	jbl_gc_plus(this);//增加引用计数
	jbl_gc_set_ref(this);
	jbl_var_set_operators(this,jbl_var_get_operators(pptr));
	this->ptr=pptr;
	(*((jbl_reference**)(ptr)))=this;
	return this;
}
void * jbl_derefer(void *ptr)
{
	if(ptr==NULL)jbl_exception("NULL POINTER");
	if(!jbl_gc_is_ref((jbl_reference*)ptr))
		return ptr;
	char flag=1;
	while(jbl_gc_is_ref((const jbl_reference*)ptr))
	{
		void *data=((const jbl_reference*)ptr)->ptr;
		if(flag)
		{
			jbl_gc_minus((jbl_reference*)ptr);//减掉ref_cnt
			if(jbl_gc_refcnt((jbl_reference*)ptr))
				flag=0;
			jbl_free(ptr);
		}
		ptr=data;
	}
//	jbl_gc_plus((jbl_reference*)ptr);
	return ptr;
}
JBL_INLINE void *jbl_refer_pull(void *ptr)
{
	while(ptr&&jbl_gc_is_ref((jbl_reference*)ptr))ptr=((jbl_reference*)ptr)->ptr;
	return (void*)ptr;
}
JBL_INLINE void * jbl_refer_pull_keep_father(void *ptr,jbl_reference** ref)
{
	jbl_reference* reff=NULL;
	while(ptr&&jbl_gc_is_ref((jbl_reference*)ptr))reff=(jbl_reference*)ptr,ptr=((jbl_reference*)ptr)->ptr;
	if(ref)*ref=reff;
	return (void*)ptr;
}
#if JBL_PTHREAD_ENABLE==1
void *			jbl_refer_pull_wrlock				(void *ptr){while(ptr&&jbl_gc_is_ref((jbl_reference*)ptr))jbl_pthread_lock_wrlock(((jbl_reference*)ptr)),ptr=((jbl_reference*)ptr)->ptr;if(ptr)jbl_pthread_lock_wrlock(((jbl_reference*)ptr));return (void*)ptr;}
void *			jbl_refer_pull_rdlock				(void *ptr){while(ptr&&jbl_gc_is_ref((jbl_reference*)ptr))jbl_pthread_lock_rdlock(((jbl_reference*)ptr)),ptr=((jbl_reference*)ptr)->ptr;if(ptr)jbl_pthread_lock_rdlock(((jbl_reference*)ptr));return (void*)ptr;}
void *			jbl_refer_pull_unlock				(void *ptr){while(ptr&&jbl_gc_is_ref((jbl_reference*)ptr))jbl_pthread_lock_unlock(((jbl_reference*)ptr)),ptr=((jbl_reference*)ptr)->ptr;if(ptr)jbl_pthread_lock_unlock(((jbl_reference*)ptr));return (void*)ptr;}
void *			jbl_refer_pull_keep_father_wrlock	(void *ptr,jbl_reference** ref){jbl_reference* reff=NULL;while(ptr&&jbl_gc_is_ref((jbl_reference*)ptr))reff=(jbl_reference*)ptr,jbl_pthread_lock_wrlock(reff),ptr=((jbl_reference*)ptr)->ptr;if(ref)*ref=reff;if(ptr)jbl_pthread_lock_wrlock(((jbl_reference*)ptr));return (void*)ptr;}
void *			jbl_refer_pull_keep_father_rdlock	(void *ptr,jbl_reference** ref){jbl_reference* reff=NULL;while(ptr&&jbl_gc_is_ref((jbl_reference*)ptr))reff=(jbl_reference*)ptr,jbl_pthread_lock_rdlock(reff),ptr=((jbl_reference*)ptr)->ptr;if(ref)*ref=reff;if(ptr)jbl_pthread_lock_rdlock(((jbl_reference*)ptr));return (void*)ptr;}
#endif
#endif
