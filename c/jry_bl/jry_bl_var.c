/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/   
#include "jry_bl_var.h"
#if JRY_BL_VAR_ENABLE==1
#if JRY_BL_STRING_ENABLE==1	
#include "jry_bl_string.h"
#endif
#if JRY_BL_LINK_LIST_ENABLE==1	
#include "jry_bl_link_list.h"
#endif
#define jry_bl_var_get_type_low(x)	((x->f.f.type)&(1<<jry_bl_var_type_bit))
#define jry_bl_var_get_type_high(x)	((x->f.f.type)>>jry_bl_var_type_bit)
const jry_bl_var_functions_struct jry_bl_var_functions[3]=
{
	{(sizeof (jry_bl_var)),jry_bl_var_init,jry_bl_var_free,jry_bl_var_copy,jry_bl_var_space_ship,jry_bl_var_to_json,jry_bl_var_view_ex},
#if JRY_BL_STRING_ENABLE==1
	{(sizeof (jry_bl_string)),jry_bl_string_init,jry_bl_string_free,jry_bl_string_copy,jry_bl_string_space_ship,jry_bl_string_to_json,jry_bl_string_view_ex},
#else
	{0,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
#endif
#if JRY_BL_LINK_LIST_ENABLE==1	
	{(sizeof (jry_bl_link_list)),jry_bl_link_list_init,jry_bl_link_list_free,jry_bl_link_list_copy,jry_bl_link_list_space_ship,jry_bl_link_list_to_json,jry_bl_link_list_view_ex},
#else
	{0,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
#endif
};
jry_bl_var_functions_struct jry_bl_var_tmp_functions[jry_bl_var_tmp_size];
jry_bl_var_functions_struct* jry_bl_var_fs[jry_bl_var_fs_size]={jry_bl_var_functions,jry_bl_var_tmp_functions};
inline void jry_bl_var_init(jry_bl_var *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	this->f.f.type=JRY_BL_VAR_TYPE_NULL;
	this->f.ff=0;
}
inline void jry_bl_var_init_as(jry_bl_var *this,jry_bl_var_type_type type)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this->f.f.type!=JRY_BL_VAR_TYPE_NULL)
		jry_bl_var_free(this);
	this->f.f.pointer=0;
	this->f.f.type=type;
	if(jry_bl_var_get_type_high(this)!=0)
		this->data.p=jry_bl_malloc(jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].size),jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].init(this->data.p);
	else
		if(this->f.f.type>=8)
			this->data.p=jry_bl_malloc(jry_bl_var_functions[this->f.f.type-8].size),jry_bl_var_functions[this->f.f.type-8].init(this->data.p);
}
void jry_bl_var_free(jry_bl_var *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(jry_bl_var_get_type_high(this)!=0)
		jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].free(this->data.p);
	else
		if(this->f.f.type>=8)
			jry_bl_var_functions[this->f.f.type-8].free(this->data.p);
	if((!this->f.f.pointer)&&this->f.f.type>=8)
		jry_bl_free(this->data.p);
	this->f.f.type=JRY_BL_VAR_TYPE_NULL;
	this->f.f.pointer=0;
}
void jry_bl_var_copy(jry_bl_var *this,jry_bl_var *that,jry_bl_uint8 copytype)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_var_init_as(this,that->f.f.type);
	if(jry_bl_var_get_type_high(this)!=0)
		jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].copy(this->data.p,that->data.p,copytype);
	else
		if(this->f.f.type>=8)
			jry_bl_var_functions[this->f.f.type-8].copy(this->data.p,that->data.p,copytype);
		else
			jry_bl_memory_copy(&this->data,&that->data,sizeof that->data);
	this->f.ff=that->f.ff;	
	
}
char jry_bl_var_space_ship(jry_bl_var *this,jry_bl_var *that)
{
	if(this->f.f.type==that->f.f.type)
	{
		if(jry_bl_var_get_type_high(this)!=0)
			jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].space_ship(this->data.p,that->data.p);
		else
			if(this->f.f.type>=8)
				jry_bl_var_functions[this->f.f.type-8].space_ship(this->data.p,that->data.p);
			else		
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_LONG_LONG			:return ((this->data.ll<that->data.ll)?(-1):((this->data.ll>that->data.ll)));break;
					case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG	:return ((this->data.ll<that->data.ull)?(-1):((this->data.ll>that->data.ull)));break;
					case JRY_BL_VAR_TYPE_DOUBLE				:return ((this->data.d<that->data.d)?(-1):((this->data.d>that->data.d)));break;
					case JRY_BL_VAR_TYPE_CHAR				:return ((this->data.c<that->data.c)?(-1):((this->data.c>that->data.c)));break;
				}
	}
	else
		return (this->f.f.type>that->f.f.type)?1:-1;
}
#if JRY_BL_STRING_ENABLE==1
void jry_bl_var_to_json(jry_bl_var *this,jry_bl_string *result)
{
	if(this==NULL||result==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	long double tmp;
	if(jry_bl_var_get_type_high(this)!=0)
		jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].to_json(this->data.p,result);
	else
		if(this->f.f.type>=8)
			jry_bl_var_functions[this->f.f.type-8].to_json(this->data.p,result);
		else
			switch(this->f.f.type)
			{
				case JRY_BL_VAR_TYPE_NULL				:					;jry_bl_string_add_char_pointer			(result,"null")			;break;
				case JRY_BL_VAR_TYPE_LONG_LONG			:tmp=this->data.ll	;jry_bl_string_add_int64			(result,tmp)			;break;
				case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG	:tmp=this->data.ull	;jry_bl_string_add_uint64	(result,tmp)			;break;
				case JRY_BL_VAR_TYPE_DOUBLE				:tmp=this->data.d	;jry_bl_string_add_double				(result,tmp)			;break;
				case JRY_BL_VAR_TYPE_TRUE				:					;jry_bl_string_add_uint64	(result,1)				;break;
				case JRY_BL_VAR_TYPE_FALSE				:					;jry_bl_string_add_uint64	(result,0)				;break;
				case JRY_BL_VAR_TYPE_CHAR				:tmp=this->data.c	;jry_bl_string_add_char					(result,tmp)			;break;
			}
}
jry_bl_string_size_type jry_bl_var_from_json_start(jry_bl_var *this,jry_bl_string *in,jry_bl_string_size_type i)
{
	if(this==NULL||in==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_size_type n=jry_bl_string_get_length(in);
	while(i<n)
	{
		register unsigned char c=jry_bl_string_get1(in,i);
		if(c=='"'&&(i==0||jry_bl_string_get1(in,i-1)!='\\'))
		{
			jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);
			return jry_bl_string_from_json_start(this->data.p,in,i);
		}
		else if(c=='{')
		{
			return i;
		}
		else if(c=='[')
		{
			jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_LINK_LIST);
			return jry_bl_link_list_from_json_start(this->data.p,in,i);
		}
		else if(c=='-')
		{
			jry_bl_string_size_type i1=i,i2=i;
			jry_bl_int64	t1=jry_bl_string_get_int64_start(in,&i1);
			double			t2=jry_bl_string_get_double_start(in,&i2);
			if(i1==i2)
				return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_LONG_LONG),this->data.ll=t1,i1;
			else if(i1<i2)
				return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_DOUBLE),this->data.d=t2,i2;
		}
		else if(c>='0'&&c<='9')
		{
			jry_bl_string_size_type i1=i,i2=i;
			jry_bl_uint64	t1=jry_bl_string_get_uint64_start(in,&i1);
			double			t2=jry_bl_string_get_double_start(in,&i2);
			if(i1==i2)
				return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG),this->data.ull=t1,i1;
			else if(i1<i2)
				return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_DOUBLE),this->data.d=t2,i2;
		}
		++i;
	}
	return i;
}
#endif
#if JRY_BL_USE_STDIO==1
void jry_bl_var_view_ex(jry_bl_var *this,FILE * file,char*str,int a,int tabs)
{
	if(this==NULL||file==NULL||str==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(jry_bl_var_get_type_high(this)!=0)
		jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].view_ex(this->data.p,file,str,a,tabs);
	else
		if(this->f.f.type>=8)
			jry_bl_var_functions[this->f.f.type-8].view_ex(this->data.p,file,str,a,tabs);
		else
		{
			char ss[20];
			for(register int i=0;i<tabs;++i,putc('\t',file));
			switch(this->f.f.type)
			{		
				case JRY_BL_VAR_TYPE_NULL				:fprintf(file,"null              %d",a)												;break;
				case JRY_BL_VAR_TYPE_LONG_LONG			:fprintf(file,"int64             %d:%lld",a,this->data.ll)							;break;
				case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG	:fprintf(file,"uint64            %d:%lld",a,this->data.ull)							;break;
				case JRY_BL_VAR_TYPE_DOUBLE				:fprintf(file,"double            %d:%lf",a,this->data.d)							;break;
				case JRY_BL_VAR_TYPE_TRUE				:fprintf(file,"true              %d",a)												;break;
				case JRY_BL_VAR_TYPE_FALSE				:fprintf(file,"false             %d",a)												;break;
				case JRY_BL_VAR_TYPE_CHAR				:fprintf(file,"char              %d:%c",a,this->data.c)								;break;
				case JRY_BL_VAR_TYPE_POINTER			:fprintf(file,"pointer           %d:0X%llX",a,((jry_bl_pointer_int)this->data.p))	;break;
			}
			fputc('\n',file);		
		}
}
#endif
#if JRY_BL_USE_STDARG==1
inline void	jry_bl_var_inits(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_var_init(va_arg(valist,jry_bl_var*));
	va_end(valist);	
}
inline void	jry_bl_var_frees(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_var_free(va_arg(valist,jry_bl_var*));
	va_end(valist);	
}
#if JRY_BL_USE_STDIO==1
inline void	jry_bl_var_views(FILE * file,int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_var_view_ex(va_arg(valist,jry_bl_var*),file,"views",i,jry_bl_view_default_tabs_num);
	va_end(valist);	
}
#endif
#endif
void jry_bl_var_turn(jry_bl_var *this,jry_bl_var_type_type type)
{
	if(this->f.f.type==type)
		return;
	long double tmp;
	switch(type)
	{
			case JRY_BL_VAR_TYPE_NULL:
				jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
				break;
			case JRY_BL_VAR_TYPE_LONG_LONG:
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG	:this->f.f.type=JRY_BL_VAR_TYPE_LONG_LONG;this->data.ll=this->data.ull	;break;
					case JRY_BL_VAR_TYPE_DOUBLE				:this->f.f.type=JRY_BL_VAR_TYPE_LONG_LONG;this->data.ll=this->data.d	;break;
					case JRY_BL_VAR_TYPE_TRUE				:this->f.f.type=JRY_BL_VAR_TYPE_LONG_LONG;this->data.ll=1				;break;
					case JRY_BL_VAR_TYPE_FALSE				:this->f.f.type=JRY_BL_VAR_TYPE_LONG_LONG;this->data.ll=0				;break;
					case JRY_BL_VAR_TYPE_CHAR				:this->f.f.type=JRY_BL_VAR_TYPE_LONG_LONG;this->data.ll=this->data.c	;break;
#if JRY_BL_STRING_ENABLE==1	
					case JRY_BL_VAR_TYPE_STRING				:tmp=jry_bl_string_get_int64(this->data.p);jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_LONG_LONG);this->data.ll=tmp;break;
#endif
					default:
						jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
				}
				break;
			case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG:
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_LONG_LONG			:this->f.f.type=JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG;this->data.ull=this->data.ll	;break;
					case JRY_BL_VAR_TYPE_DOUBLE				:this->f.f.type=JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG;this->data.ull=this->data.d	;break;
					case JRY_BL_VAR_TYPE_TRUE				:this->f.f.type=JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG;this->data.ull=1				;break;
					case JRY_BL_VAR_TYPE_FALSE				:this->f.f.type=JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG;this->data.ull=0				;break;
					case JRY_BL_VAR_TYPE_CHAR				:this->f.f.type=JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG;this->data.ull=this->data.c	;break;
#if JRY_BL_STRING_ENABLE==1	
					case JRY_BL_VAR_TYPE_STRING				:tmp=jry_bl_string_get_uint64(this->data.p);jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG);this->data.ull=tmp;break;
#endif
					default:
						jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
				}
				break;
			case JRY_BL_VAR_TYPE_DOUBLE:
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_LONG_LONG			:this->f.f.type=JRY_BL_VAR_TYPE_DOUBLE;this->data.d=this->data.ll	;break;
					case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG	:this->f.f.type=JRY_BL_VAR_TYPE_DOUBLE;this->data.d=this->data.ull	;break;
					case JRY_BL_VAR_TYPE_TRUE				:this->f.f.type=JRY_BL_VAR_TYPE_DOUBLE;this->data.d=1				;break;
					case JRY_BL_VAR_TYPE_FALSE				:this->f.f.type=JRY_BL_VAR_TYPE_DOUBLE;this->data.d=0				;break;
					case JRY_BL_VAR_TYPE_CHAR				:this->f.f.type=JRY_BL_VAR_TYPE_DOUBLE;this->data.d=this->data.c	;break;
#if JRY_BL_STRING_ENABLE==1	
					case JRY_BL_VAR_TYPE_STRING				:tmp=jry_bl_string_get_double(this->data.p);jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_DOUBLE);this->data.d=tmp;break;
#endif
					default:
						jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
				}
				break;
			case JRY_BL_VAR_TYPE_TRUE:
			case JRY_BL_VAR_TYPE_FALSE:
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_LONG_LONG			:this->f.f.type=(this->data.ll==0?JRY_BL_VAR_TYPE_FALSE:JRY_BL_VAR_TYPE_TRUE)	;break;
					case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG	:this->f.f.type=(this->data.ull==0?JRY_BL_VAR_TYPE_FALSE:JRY_BL_VAR_TYPE_TRUE)	;break;
					case JRY_BL_VAR_TYPE_TRUE				:;break;
					case JRY_BL_VAR_TYPE_FALSE				:;break;
					case JRY_BL_VAR_TYPE_CHAR				:this->f.f.type=(this->data.c==0?JRY_BL_VAR_TYPE_FALSE:JRY_BL_VAR_TYPE_TRUE)	;break;
#if JRY_BL_STRING_ENABLE==1	
					case JRY_BL_VAR_TYPE_STRING				:tmp=jry_bl_string_get_int64(this->data.p);jry_bl_var_init_as(this,(tmp==0?JRY_BL_VAR_TYPE_FALSE:JRY_BL_VAR_TYPE_TRUE));break;
#endif
					default:
						jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
				}
				break;
			case JRY_BL_VAR_TYPE_CHAR:
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_LONG_LONG			:this->f.f.type=JRY_BL_VAR_TYPE_CHAR;this->data.c=this->data.ll	;break;
					case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG	:this->f.f.type=JRY_BL_VAR_TYPE_CHAR;this->data.c=this->data.ull;break;
					case JRY_BL_VAR_TYPE_DOUBLE				:this->f.f.type=JRY_BL_VAR_TYPE_CHAR;this->data.c=this->data.d	;break;
					case JRY_BL_VAR_TYPE_TRUE				:this->f.f.type=JRY_BL_VAR_TYPE_CHAR;this->data.c=1				;break;
					case JRY_BL_VAR_TYPE_FALSE				:this->f.f.type=JRY_BL_VAR_TYPE_CHAR;this->data.c=0				;break;
#if JRY_BL_STRING_ENABLE==1	
					case JRY_BL_VAR_TYPE_STRING				:tmp=jry_bl_string_get1(((jry_bl_string*)this->data.p),0);jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_CHAR);this->data.c=tmp;break;
#endif
					default:
						jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
				}
				break;
			case JRY_BL_VAR_TYPE_POINTER:
				jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
				break;
#if JRY_BL_STRING_ENABLE==1				
			case JRY_BL_VAR_TYPE_STRING:
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_LONG_LONG			:tmp=this->data.ll	;jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_int64			(this->data.p,tmp);break;
					case JRY_BL_VAR_TYPE_UNSIGNED_LONG_LONG	:tmp=this->data.ull	;jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_uint64	(this->data.p,tmp);break;
					case JRY_BL_VAR_TYPE_DOUBLE				:tmp=this->data.d	;jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_double				(this->data.p,tmp);break;
					case JRY_BL_VAR_TYPE_TRUE				:					;jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_int64			(this->data.p,1)	;break;
					case JRY_BL_VAR_TYPE_FALSE				:					;jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_int64			(this->data.p,0)	;break;
					case JRY_BL_VAR_TYPE_CHAR				:tmp=this->data.c	;jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_char				(this->data.p,tmp);break;
					default:
						jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
				}				
				break;
#endif
			default:
				jry_bl_exception(JRY_BL_ERROR_TYPE_ERROR);
	}
	
}
#endif