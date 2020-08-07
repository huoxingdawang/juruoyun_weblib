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
#if JBL_VAR_ENABLE==1
#include "jbl_var.h"
#endif


inline void* jbl_gc_plus	(void *this){((jbl_reference*)this)->gc+=16;return this;};
inline void* jbl_gc_minus	(void *this){((jbl_reference*)this)->gc-=16;return this;};


#if JBL_VAR_ENABLE==1
void * jbl_refer_as_var(void *ptr,const jbl_var_operators *ops)
#else
void * jbl_refer(void *ptr)
#endif
{
	if(ptr==NULL)jbl_exception("NULL POINTER");
	jbl_reference *pptr=*((jbl_reference**)(ptr));
	if(pptr==NULL)jbl_exception("NULL POINTER");
	jbl_reference *this;
#if JBL_VAR_ENABLE==1
	if(jbl_gc_is_var(pptr)||ops)
		this=(jbl_reference*)((char*)(jbl_malloc((sizeof(jbl_reference))+(sizeof(jbl_var)))+(sizeof(jbl_var))));	
	else
#endif	
		this=jbl_malloc((sizeof(jbl_reference)));
	jbl_gc_init(this);
	jbl_gc_plus(this);//增加引用计数
	jbl_gc_plus(this);//增加引用计数
	jbl_gc_set_ref(this);
#if JBL_VAR_ENABLE==1
	if(jbl_gc_is_var(pptr))
	{
		jbl_gc_set_var(this);		
		jbl_var_set_operators((jbl_var*)this,jbl_var_get_operators((jbl_var*)pptr));
	}
	else if(ops)
	{
		jbl_gc_set_var(this);		
		jbl_var_set_operators((jbl_var*)this,ops);
	}
#endif	
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
#if JBL_VAR_ENABLE==1
			if(jbl_gc_is_var((jbl_reference*)ptr))		
				jbl_free(((char*)ptr)-(sizeof(jbl_var)));
			else
#endif
				jbl_free(ptr);
		}
		ptr=data;
	}
//	jbl_gc_plus((jbl_reference*)ptr);
	return ptr;
}
inline void *jbl_refer_pull(const void *ptr)
{
	while(ptr&&(jbl_gc_is_ref((const jbl_reference*)ptr)||jbl_gc_is_pvar((const jbl_reference*)ptr)))ptr=((const jbl_reference*)ptr)->ptr;
	return (void*)ptr;
}
inline void * jbl_refer_pull_keep_father(const void *ptr,jbl_reference** ref)
{
	if(!ptr)return NULL;
	jbl_reference* reff=NULL;
	while(jbl_gc_is_ref((const jbl_reference*)ptr))reff=(jbl_reference*)ptr,ptr=((const jbl_reference*)ptr)->ptr;
	if(ref)*ref=reff;
	return (void*)ptr;
}
#endif
