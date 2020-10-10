/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/   
#include "jbl_var.h"
#if JBL_VAR_ENABLE==1
#include "jbl_exception.h"
#include "jbl_malloc.h"
#include "jbl_string.h"
#include "jbl_stream.h"
#include "jbl_pthread.h"

struct __jbl_var
{
	jbl_gc 					gc;
	const jbl_var_operators	*var_ops;
	jbl_pthread_lock_define		;
};
JBL_INLINE void * jbl_var_set_operators(void * this,const jbl_var_operators *ops)
{
	if(!this)jbl_exception("NULL POINTER");	
	((struct __jbl_var *)this)->var_ops=ops;
	return this;
}
JBL_INLINE const jbl_var_operators * jbl_var_get_operators(void * this)
{
	if(!this)return NULL;
	return ((struct __jbl_var *)this)->var_ops;
}
JBL_INLINE void * jbl_var_free(void * this)
{
	const jbl_var_operators * ops=jbl_var_get_operators(this);
	if(ops&&ops->free)return ops->free(this);
	return NULL;
}
JBL_INLINE void * jbl_var_copy(void * this)
{
	const jbl_var_operators * ops=jbl_var_get_operators(this);
	if(ops&&ops->copy)return ops->copy(this);
	return NULL;
}
JBL_INLINE char jbl_var_space_ship(void * this,void * that)
{
	const jbl_var_operators * ops1=jbl_var_get_operators(this);
	const jbl_var_operators * ops2=jbl_var_get_operators(that);
	if(this==that){return 0;}if(!ops1){return -1;}if(!ops2){return 1;}if(ops1<ops2){return -1;}if(ops1>ops2){return 1;}
	if(ops1->space_ship)return ops1->space_ship(this,that);
	return -1;
}

/*
jbl_var *jbl_var_copy_as(void * that,const jbl_var_operators *ops)
{
	if(that==NULL)return NULL;
	if(jbl_gc_is_var((jbl_reference*)that))
		return jbl_var_copy(that);
	if(jbl_gc_is_ref((jbl_reference*)that))
	{
		void *data=that;
		jbl_gc_plus((jbl_reference*)that);	//refer不会加ref_cnt，手动补上
		jbl_refer_as_var(&data,ops);
		jbl_gc_minus((jbl_reference*)data);	//出来的data ref_cnt一定是2
		return (jbl_var*)(data);
	}
	jbl_reference *this=(jbl_reference*)(((char*)(jbl_malloc((sizeof(jbl_reference))+(sizeof(jbl_var))))+(sizeof(jbl_var))));	
	jbl_gc_init(this);
	jbl_gc_plus(this);//增加引用计数
	jbl_gc_set_pvar(this);
	jbl_gc_set_var(this);		
	jbl_var_set_operators((jbl_var*)this,ops);
	this->ptr=that;
	jbl_gc_plus((jbl_reference*)that);
	return (jbl_var*)(this);
}
*/
#if JBL_STRING_ENABLE==1 && JBL_JSON_ENABLE==1
jbl_string * jbl_var_json_encode(void * this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	const jbl_var_operators * ops=jbl_var_get_operators(this);
	if(!ops||!ops->json_encode)
	{
		if(out==NULL)out=jbl_string_new();
		out=jbl_refer_pull(out);
		if(format&&tabs>=0)for(jbl_int16 i=0;i<tabs;out=jbl_string_add_char(out,'\t'),++i);	
		return jbl_string_add_chars_length(out,(unsigned char *)"null",4);
	}
	return ops->json_encode(this,out,format,tabs);
}
#endif




#if JBL_STREAM_ENABLE==1
void* jbl_var_view_put(void* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	if(!this)goto print;
	const jbl_var_operators * ops=jbl_var_get_operators(this);
	if(ops&&ops->view_put)ops->view_put(this,out,format,tabs,line,varname,func,file);
	else goto print;
	return this;
print:;
	jbl_stream_view_put_format(this,out,format,tabs,UC"unprintable var",line,varname,func,file);jbl_stream_push_char(out,'\n');
	return this;
}
#if JBL_JSON_ENABLE==1
void jbl_var_json_put(void * this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(!out)jbl_exception("NULL POINTER");
	const jbl_var_operators * ops=jbl_var_get_operators(this);
	if(!ops||!ops->json_encode){if(format&&tabs>=0)for(jbl_int16 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);jbl_stream_push_chars(out,UC"null");return;}
	return ops->json_put(this,out,format,tabs);
}
#endif
#endif

#endif