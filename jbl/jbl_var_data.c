/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/   
#include "jbl_var_data.h"
#if JBL_VAR_ENABLE==1
#include "jbl_exception.h"
#include "jbl_malloc.h"
#include "jbl_string.h"
jbl_var_data * __jbl_var_data_free(jbl_var_data *this)
{
	if(this==NULL)return NULL;
	jbl_gc_minus(((jbl_var_data*)this));
	if(!jbl_gc_refcnt(((jbl_var_data*)this)))
	{
		((jbl_gc_is_ref(((jbl_var_data*)this)))?__jbl_var_data_free((jbl_var_data*)(((jbl_reference*)this)->ptr)):0);
		jbl_free((char*)this-sizeof(jbl_var_data));
	}
	return NULL;
}
jbl_var_data * __jbl_var_data_new(const jbl_var_operators *ops)
{
	jbl_var_data *this=(jbl_var_data*)(((char*)(jbl_malloc((sizeof(jbl_var_data))+(sizeof(jbl_var_data))))+(sizeof(jbl_var_data))));
	jbl_gc_init(((jbl_var_data*)this));
	jbl_gc_plus(((jbl_var_data*)this));//增加引用计数
	jbl_var_set_operators(this,ops);
	return this;
}
JBL_INLINE jbl_var_data * jbl_Vuint_new(){return __jbl_var_data_new(&jbl_uint_operators);}
JBL_INLINE jbl_var_data * jbl_Vuint_set(jbl_var_data * this,jbl_uint64 data)
{
	if(!this)this=jbl_Vuint_new();
	((jbl_var_data*)jbl_refer_pull(this))->u=data;
	return this;
}
JBL_INLINE jbl_var_data * jbl_Vuint_copy(jbl_var_data * that)
{
	return jbl_Vuint_set(NULL,((jbl_var_data*)jbl_refer_pull(that))->u);
}
JBL_INLINE char jbl_Vuint_space_ship(jbl_var_data * this,jbl_var_data * that)
{
	jbl_uint64 a=((jbl_var_data*)jbl_refer_pull(this))->u,b=((jbl_var_data*)jbl_refer_pull(that))->u;
	return (a>b?1:(a<b?-1:0));
}
#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
jbl_string* jbl_Vuint_json_encode(jbl_var_data* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	out=jbl_string_json_put_format(this=jbl_refer_pull(this),out,format,tabs);if(!this)return out;
	out=jbl_string_add_uint(out,((jbl_var_data*)this)->u);
	if(format&2){out=jbl_string_add_char(out,',');}if((format&1)||(format&4)){out=jbl_string_add_char(out,'\n');}
	return out;
}
#endif 
#endif 
#if JBL_STREAM_ENABLE==1
jbl_var_data* jbl_Vuint_view_put(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jbl_var_data *thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"uint64",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_char(out,'\t');
	jbl_stream_push_uint(out,((jbl_var_data*)thi)->u);
	jbl_stream_push_char(out,'\n');
	return this;
}
#if JBL_JSON_ENABLE==1
void jbl_Vuint_json_put(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,tabs))return;
	jbl_stream_push_uint(out,((jbl_var_data*)this)->u);
	if(format&2){jbl_stream_push_char(out,',');}if((format&1)||(format&4)){jbl_stream_push_char(out,'\n');}
}
#endif
#endif
jbl_var_operators_new(jbl_uint_operators,__jbl_var_data_free,jbl_Vuint_copy,jbl_Vuint_space_ship,jbl_Vuint_json_encode,jbl_Vuint_view_put,jbl_Vuint_json_put);
JBL_INLINE jbl_var_data * jbl_Vint_new(){return __jbl_var_data_new(&jbl_int_operators);}
JBL_INLINE jbl_var_data * jbl_Vint_set(jbl_var_data * this,jbl_int64 data)
{
	if(!this)this=jbl_Vint_new();
	((jbl_var_data*)jbl_refer_pull(this))->i=data;
	return this;
}
JBL_INLINE jbl_var_data * jbl_Vint_copy(jbl_var_data * that)
{
	return jbl_Vint_set(NULL,((jbl_var_data*)jbl_refer_pull(that))->i);
}
JBL_INLINE char jbl_Vint_space_ship(jbl_var_data * this,jbl_var_data * that)
{
	jbl_int64 a=((jbl_var_data*)jbl_refer_pull(this))->i,b=((jbl_var_data*)jbl_refer_pull(that))->i;
	return (a>b?1:(a<b?-1:0));
}
#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
jbl_string* jbl_Vint_json_encode(jbl_var_data* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	out=jbl_string_json_put_format(this=jbl_refer_pull(this),out,format,tabs);if(!this)return out;
	out=jbl_string_add_int(out,((jbl_var_data*)this)->i);
	if(format&2){out=jbl_string_add_char(out,',');}if((format&1)||(format&4)){out=jbl_string_add_char(out,'\n');}
	return out;	
}
#endif 
#endif 
#if JBL_STREAM_ENABLE==1
jbl_var_data* jbl_Vint_view_put(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jbl_var_data *thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"int64",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_char(out,'\t');
	jbl_stream_push_int(out,((jbl_var_data*)thi)->i);
	jbl_stream_push_char(out,'\n');
	return this;
}
#if JBL_JSON_ENABLE==1
void jbl_Vint_json_put(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,tabs))return;
	jbl_stream_push_int(out,((jbl_var_data*)this)->i);
	if(format&2){jbl_stream_push_char(out,',');}if((format&1)||(format&4)){jbl_stream_push_char(out,'\n');}
}
#endif
#endif
jbl_var_operators_new(jbl_int_operators,__jbl_var_data_free,jbl_Vint_copy,jbl_Vint_space_ship,jbl_Vint_json_encode,jbl_Vint_view_put,jbl_Vint_json_put);
JBL_INLINE jbl_var_data * jbl_Vdouble_new(){return __jbl_var_data_new(&jbl_double_operators);}
JBL_INLINE jbl_var_data * jbl_Vdouble_set(jbl_var_data * this,double data)
{
	if(!this)this=jbl_Vdouble_new();
	((jbl_var_data*)jbl_refer_pull(this))->d=data;
	return this;
}
JBL_INLINE jbl_var_data * jbl_Vdouble_copy(jbl_var_data * that)
{
	return jbl_Vdouble_set(NULL,((jbl_var_data*)jbl_refer_pull(that))->d);
}
JBL_INLINE char jbl_Vdouble_space_ship(jbl_var_data * this,jbl_var_data * that)
{
	double a=((jbl_var_data*)jbl_refer_pull(this))->d,b=((jbl_var_data*)jbl_refer_pull(that))->d;
	return (a>b?1:(a<b?-1:0));
}
#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
jbl_string* jbl_Vdouble_json_encode(jbl_var_data* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	out=jbl_string_json_put_format(this=jbl_refer_pull(this),out,format,tabs);if(!this)return out;
	out=jbl_string_add_double(out,((jbl_var_data*)this)->d);
	if(format&2){out=jbl_string_add_char(out,',');}if((format&1)||(format&4)){out=jbl_string_add_char(out,'\n');}
	return out;	
}
#endif 
#endif 
#if JBL_STREAM_ENABLE==1
jbl_var_data* jbl_Vdouble_view_put(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jbl_var_data *thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"double",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_char(out,'\t');
	jbl_stream_push_double(out,((jbl_var_data*)thi)->d);
	jbl_stream_push_char(out,'\n');
	return this;
}
#if JBL_JSON_ENABLE==1
void jbl_Vdouble_json_put(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,tabs))return;
	jbl_stream_push_double(out,((jbl_var_data*)this)->d);
	if(format&2){jbl_stream_push_char(out,',');}if((format&1)||(format&4)){jbl_stream_push_char(out,'\n');}
}
#endif
#endif
jbl_var_operators_new(jbl_double_operators,__jbl_var_data_free,jbl_Vdouble_copy,jbl_Vdouble_space_ship,jbl_Vdouble_json_encode,jbl_Vdouble_view_put,jbl_Vdouble_json_put);
JBL_INLINE jbl_var_data * jbl_Vnull_new(){jbl_var_data *this=__jbl_var_data_new(&jbl_ntf_operators);((jbl_var_data*)this)->u=0;return this;}
JBL_INLINE jbl_var_data * jbl_Vtrue_new(){jbl_var_data *this=__jbl_var_data_new(&jbl_ntf_operators);((jbl_var_data*)this)->u=2;return this;}
JBL_INLINE jbl_var_data * jbl_Vfalse_new(){jbl_var_data *this=__jbl_var_data_new(&jbl_ntf_operators);((jbl_var_data*)this)->u=1;return this;}

// NULL true 和 false类型共享同一组操作符,合称ntf,具体类型由u中值确定,0为NULL,1为false,2为true
JBL_INLINE jbl_var_data * jbl_Vntf_copy(jbl_var_data * that)
{
	jbl_var_data *this=jbl_Vnull_new();
	((jbl_var_data*)jbl_refer_pull(this))->u=((jbl_var_data*)jbl_refer_pull(that))->u;
	return this;
}
JBL_INLINE char jbl_Vntf_space_ship(jbl_var_data * this,jbl_var_data * that)
{
	jbl_uint64 a=((jbl_var_data*)jbl_refer_pull(this))->u,b=((jbl_var_data*)jbl_refer_pull(that))->u;
	return (a>b?1:(a<b?-1:0));
}
#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
jbl_string* jbl_Vntf_json_encode(jbl_var_data* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	out=jbl_string_json_put_format(this=jbl_refer_pull(this),out,format,tabs);if(!this)return out;
	switch(((jbl_var_data*)jbl_refer_pull(this))->u)
	{
		case 0:	out=jbl_string_add_chars(out,UC"null")	;break;
		case 2:	out=jbl_string_add_chars(out,UC"true")	;break;
		case 1:	out=jbl_string_add_chars(out,UC"false")	;break;
		default:out=jbl_string_add_chars(out,UC"null")	;break;
	}
	if(format&2){out=jbl_string_add_char(out,',');}if((format&1)||(format&4)){out=jbl_string_add_char(out,'\n');}
	return out;		
}
#endif 
#endif 
#if JBL_STREAM_ENABLE==1
jbl_var_data* jbl_Vntf_view_put(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	switch(((jbl_var_data*)jbl_refer_pull(this))->u)
	{
		case 0: jbl_stream_view_put_format(this,out,format,tabs,UC"null",line,varname,func,file)	;break;
		case 2: jbl_stream_view_put_format(this,out,format,tabs,UC"true",line,varname,func,file)	;break;
		case 1: jbl_stream_view_put_format(this,out,format,tabs,UC"false",line,varname,func,file)	;break;
	}
	jbl_stream_push_char(out,'\n');
	return this;
}
#if JBL_JSON_ENABLE==1
void jbl_Vntf_json_put(jbl_var_data* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,tabs))return;
	switch(((jbl_var_data*)jbl_refer_pull(this))->u)
	{
		case 0:	jbl_stream_push_chars(out,UC"null")	;break;
		case 2:	jbl_stream_push_chars(out,UC"true")	;break;
		case 1:	jbl_stream_push_chars(out,UC"false");break;
	}
	if(format&2){jbl_stream_push_char(out,',');}if((format&1)||(format&4)){jbl_stream_push_char(out,'\n');}
}
#endif
#endif
jbl_var_operators_new(jbl_ntf_operators,__jbl_var_data_free,jbl_Vntf_copy,jbl_Vntf_space_ship,jbl_Vntf_json_encode,jbl_Vntf_view_put,jbl_Vntf_json_put);

#endif