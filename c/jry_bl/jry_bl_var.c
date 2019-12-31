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
#if JRY_BL_HASH_TABLE_ENABLE==1	
#include "jry_bl_hash_table.h"
#endif
#define jry_bl_var_get_type_low(x)	((x->f.f.type)&(1<<jry_bl_var_type_bit))
#define jry_bl_var_get_type_high(x)	((x->f.f.type)>>jry_bl_var_type_bit)
const jry_bl_var_functions_struct jry_bl_var_functions[4]=
{
	[JRY_BL_VAR_TYPE_VAR-9]			={(sizeof (jry_bl_var)),jry_bl_var_init,jry_bl_var_free,jry_bl_var_copy,jry_bl_var_space_ship,jry_bl_var_to_json_ex,jry_bl_var_view_ex},
#if JRY_BL_STRING_ENABLE==1
	[JRY_BL_VAR_TYPE_STRING-9]		={(sizeof (jry_bl_string)),jry_bl_string_init,jry_bl_string_free,jry_bl_string_copy,jry_bl_string_space_ship,jry_bl_string_to_json_ex,jry_bl_string_view_ex},
#endif
#if JRY_BL_LINK_LIST_ENABLE==1	
	[JRY_BL_VAR_TYPE_LINK_LIST-9]	={(sizeof (jry_bl_link_list)),jry_bl_link_list_init,jry_bl_link_list_free,jry_bl_link_list_copy,jry_bl_link_list_space_ship,jry_bl_link_list_to_json_ex,jry_bl_link_list_view_ex},
#endif
#if JRY_BL_HASH_TABLE_ENABLE==1
	[JRY_BL_VAR_TYPE_HASH_TABLE-9]	={(sizeof (jry_bl_hash_table)),jry_bl_hash_table_init,jry_bl_hash_table_free,jry_bl_hash_table_copy,jry_bl_hash_table_space_ship,jry_bl_hash_table_to_json_ex,jry_bl_hash_table_view_ex},
#endif
};
jry_bl_var_functions_struct jry_bl_var_tmp_functions[jry_bl_var_tmp_size];
jry_bl_var_functions_struct* jry_bl_var_fs[jry_bl_var_fs_size]={jry_bl_var_functions,jry_bl_var_tmp_functions};
inline void jry_bl_var_init(jry_bl_var *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	this->f.ff=(int)0;
	this->f.f.type=JRY_BL_VAR_TYPE_NULL;
}
inline void jry_bl_var_init_as(jry_bl_var *this,jry_bl_var_type_type type)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_var_free(this);
	this->f.f.type=type;
	if(jry_bl_var_get_type_high(this)!=0)
		this->data.p=jry_bl_malloc(jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].size),jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].init(this->data.p);
	else
		if(this->f.f.type>=9)
			this->data.p=jry_bl_malloc(jry_bl_var_functions[this->f.f.type-9].size),jry_bl_var_functions[this->f.f.type-9].init(this->data.p);
}
void jry_bl_var_free(jry_bl_var *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(jry_bl_var_get_type_high(this)!=0)
		jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].free(this->data.p);
	else
		if(this->f.f.type>=9)
			jry_bl_var_functions[this->f.f.type-9].free(this->data.p);
	if((!this->f.f.pointer)&&this->f.f.type>=9)
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
		if(this->f.f.type>=9)
			jry_bl_var_functions[this->f.f.type-9].copy(this->data.p,that->data.p,copytype);
		else
			jry_bl_memory_copy(&this->data,&that->data,sizeof that->data);
	this->f.ff=that->f.ff;	
	
}
char jry_bl_var_space_ship(const jry_bl_var *this,const jry_bl_var *that)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this==that)
		return 0;	
	if(this->f.f.type==that->f.f.type)
	{
		if(jry_bl_var_get_type_high(this)!=0)
			jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].space_ship(this->data.p,that->data.p);
		else
			if(this->f.f.type>=9)
				jry_bl_var_functions[this->f.f.type-9].space_ship(this->data.p,that->data.p);
			else		
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_INT64			:return ((this->data.ll<that->data.ll)?(-1):((this->data.ll>that->data.ll)));break;
					case JRY_BL_VAR_TYPE_UINT64	:return ((this->data.ll<that->data.ull)?(-1):((this->data.ll>that->data.ull)));break;
					case JRY_BL_VAR_TYPE_DOUBLE				:return ((this->data.d<that->data.d)?(-1):((this->data.d>that->data.d)));break;
					case JRY_BL_VAR_TYPE_CHAR				:return ((this->data.c<that->data.c)?(-1):((this->data.c>that->data.c)));break;
				}
	}
	else
		return (this->f.f.type>that->f.f.type)?1:-1;
}
#if JRY_BL_STRING_ENABLE==1
void jry_bl_var_to_json_ex(const jry_bl_var *this,jry_bl_string *result,jry_bl_uint8 type)
{
	if(this==NULL||result==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(jry_bl_var_get_type_high(this)!=0)
		jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].to_json(this->data.p,result,type);
	else
		if(this->f.f.type>=9)
			jry_bl_var_functions[this->f.f.type-9].to_json(this->data.p,result,type);
		else
		{
			long double tmp;
			if(type==1)
			{
				switch(this->f.f.type)
				{
					case JRY_BL_VAR_TYPE_NULL	:(*((jry_bl_string_size_type*)result))+=4 ;break;
					case JRY_BL_VAR_TYPE_TRUE	:(*((jry_bl_string_size_type*)result))+=1 ;break;
					case JRY_BL_VAR_TYPE_FALSE	:(*((jry_bl_string_size_type*)result))+=1 ;break;			
					case JRY_BL_VAR_TYPE_DOUBLE	:(*((jry_bl_string_size_type*)result))+=40;break;
					case JRY_BL_VAR_TYPE_CHAR	:(*((jry_bl_string_size_type*)result))+=3 ;break;
					case JRY_BL_VAR_TYPE_INT64	:
						tmp=this->data.ll;
						if(((jry_bl_int64)tmp)<0)
							tmp=(-((jry_bl_int64)tmp)),++(*((jry_bl_string_size_type*)result));
						do{++(*((jry_bl_string_size_type*)result));}while(tmp=(((jry_bl_int64)tmp)/10));
						break;
					case JRY_BL_VAR_TYPE_UINT64	:
						tmp=this->data.ll;
						do{++(*((jry_bl_string_size_type*)result));}while(tmp=(((jry_bl_uint64)tmp)/10));
						break;
				}
				return;
			}
			switch(this->f.f.type)
			{
				case JRY_BL_VAR_TYPE_NULL	:					;jry_bl_string_add_char_pointer	(result,"null")			;break;
				case JRY_BL_VAR_TYPE_INT64	:tmp=this->data.ll	;jry_bl_string_add_int64		(result,tmp)			;break;
				case JRY_BL_VAR_TYPE_UINT64	:tmp=this->data.ull	;jry_bl_string_add_uint64		(result,tmp)			;break;
				case JRY_BL_VAR_TYPE_DOUBLE	:tmp=this->data.d	;jry_bl_string_add_double		(result,tmp)			;break;
				case JRY_BL_VAR_TYPE_TRUE	:					;jry_bl_string_add_uint64		(result,1)				;break;
				case JRY_BL_VAR_TYPE_FALSE	:					;jry_bl_string_add_uint64		(result,0)				;break;
				case JRY_BL_VAR_TYPE_CHAR	:					;jry_bl_string_add_char1(result,'"');jry_bl_string_add_char1(result,this->data.c);jry_bl_string_add_char1(result,'"');break;
			}
		}
}
jry_bl_string_size_type jry_bl_var_from_json_start(jry_bl_var *this,const jry_bl_string *in,jry_bl_string_size_type start)
{
	if(this==NULL||in==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	for(jry_bl_string_size_type n=jry_bl_string_get_length(in),i=start,ii;i<n;++i)
	{
		register unsigned char c=jry_bl_string_get1(in,i);
		if(c=='"'&&(i==0||jry_bl_string_get1(in,i-1)!='\\'))
		{
			jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);
			return ((ii=jry_bl_string_from_json_start(this->data.p,in,i))==i)?(jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_NULL),start):ii;
		}
		else if(c=='{')
		{
			jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_HASH_TABLE);
			return ((ii=jry_bl_hash_table_from_json_start(this->data.p,in,i))==i)?(jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_NULL),start):ii;
		}
		else if(c=='[')
		{
			jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_LINK_LIST);
			return ((ii=jry_bl_link_list_from_json_start(this->data.p,in,i))==i)?(jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_NULL),start):ii;
		}
		else if(c=='n'&&(i+3)<n&&jry_bl_string_get1(in,i+1)=='u'&&jry_bl_string_get1(in,i+2)=='l'&&jry_bl_string_get1(in,i+3)=='l')
			return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_NULL),i+4;
		else if(c=='-')
		{
			jry_bl_string_size_type i1=i,i2=i;
			jry_bl_int64	t1=jry_bl_string_get_int64_start(in,&i1);
			double			t2=jry_bl_string_get_double_start(in,&i2);
			if(i1==i2)
				return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_INT64),this->data.ll=t1,i1;
			else if(i1<i2)
				return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_DOUBLE),this->data.d=t2,i2;
			return start;
		}
		else if(c>='0'&&c<='9')
		{
			jry_bl_string_size_type i1=i,i2=i;
			jry_bl_uint64	t1=jry_bl_string_get_uint64_start(in,&i1);
			double			t2=jry_bl_string_get_double_start(in,&i2);
			if(i1==i2)
				return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_UINT64),this->data.ull=t1,i1;
			else if(i1<i2)
				return jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_DOUBLE),this->data.d=t2,i2;
			return start;
		}
		else if(c!=' '&&c!='\r'&&c!='\t'&&c!='\n')
			return start;
	}
	return start;
}
#endif
#if JRY_BL_USE_STDIO==1
void jry_bl_var_view_ex(const jry_bl_var *this,FILE * file,char*str,int a,int tabs)
{
	if(this==NULL||file==NULL||str==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(jry_bl_var_get_type_high(this)!=0)
		jry_bl_var_fs[jry_bl_var_get_type_high(this)][jry_bl_var_get_type_low(this)].view_ex(this->data.p,file,str,a,tabs);
	else
		if(this->f.f.type>=9)
			jry_bl_var_functions[this->f.f.type-9].view_ex(this->data.p,file,str,a,tabs);
		else
		{
			char ss[20]={0};
			if(tabs>=0)
				for(register int i=0;i<tabs;++i,putc('\t',file));
			else
				tabs=-tabs;
			if(a>=0)
				sprintf(ss,"%s %d",str,a);
			switch(this->f.f.type)
			{		
				case JRY_BL_VAR_TYPE_UNUSE	:fprintf(file,"unuse             %s",ss)											;break;
				case JRY_BL_VAR_TYPE_NULL	:fprintf(file,"null              %s",ss)											;break;
				case JRY_BL_VAR_TYPE_INT64	:fprintf(file,"int64             %s:%lld",ss,this->data.ll)							;break;
				case JRY_BL_VAR_TYPE_UINT64	:fprintf(file,"uint64            %s:%lld",ss,this->data.ull)						;break;
				case JRY_BL_VAR_TYPE_DOUBLE	:fprintf(file,"double            %s:%lf",ss,this->data.d)							;break;
				case JRY_BL_VAR_TYPE_TRUE	:fprintf(file,"true              %s",ss)											;break;
				case JRY_BL_VAR_TYPE_FALSE	:fprintf(file,"false             %s",ss)											;break;
				case JRY_BL_VAR_TYPE_CHAR	:fprintf(file,"char              %s:%c",ss,this->data.c)							;break;
				case JRY_BL_VAR_TYPE_POINTER:fprintf(file,"pointer           %s:0X%llX",ss,((jry_bl_pointer_int)this->data.p))	;break;
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
#endif