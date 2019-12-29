/* Copyright (c) [2019] juruoyun developer team
	 Juruoyun basic lib is licensed under the Mulan PSL v1.
	 You can use this software according to the terms and conditions of the Mulan PSL v1.
	 You may obtain a copy of Mulan PSL v1 at:
			http://license.coscl.org.cn/MulanPSL
	 THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
	 IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
	 PURPOSE.
	 See the Mulan PSL v1 for more details.*/
#include "jry_bl_hash_table.h"
#if JRY_BL_HASH_TABLE_ENABLE==1
#define st jry_bl_hash_table_size_type
#define dt jry_bl_hash_table_data
#define gh2(x,y) ((jry_bl_int64)(((jry_bl_uint64)(y))|(-(jry_bl_uint64)(x)->size)))
#define htvh(x,y) for(register st i=0;i<(x)->size;fprintf(y,"%lld ",(jry_bl_int64)((st*)(x)->data)[-i-1]),++i);putc('\n',y)
void jry_bl_hash_table_data_init(jry_bl_hash_table_data *this);
void jry_bl_hash_table_data_free(jry_bl_hash_table_data *this);
inline void jry_bl_hash_table_data_init(jry_bl_hash_table_data *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_bl_string_init(&this->k);
	jry_bl_var_init(&this->v);
	jry_bl_var_get_type(&this->v)=JRY_BL_VAR_TYPE_UNUSE;
	this->nxt=((jry_bl_int64)-1);
}
inline void jry_bl_hash_table_data_free(jry_bl_hash_table_data *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_bl_string_free(&this->k);
	jry_bl_var_free(&this->v);	
	jry_bl_var_get_type(&this->v)=JRY_BL_VAR_TYPE_UNUSE;
	this->nxt=((jry_bl_int64)-1);
}
inline void jry_bl_hash_table_init(jry_bl_hash_table *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	this->size=0;this->len=0;this->nxt=0;this->data=NULL;
}
void jry_bl_hash_table_free(jry_bl_hash_table *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	if(this->size==0)
	{
		this->len=0,this->nxt=0,this->data=NULL;
		return;
	}
	for(register st i=0;i<this->size;jry_bl_hash_table_data_free(&this->data[i]),++i);	
	jry_bl_free((char*)this->data-sizeof(st)*this->size),this->data=NULL;
	this->size=0,this->len=0,this->nxt=0;
}
void jry_bl_hash_table_extend_to(jry_bl_hash_table *this,st size)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	st old_size=this->size;
	if(size-old_size+this->len<=old_size&&(this->len<=(this->nxt>>1))&&this->data!=NULL)
		return jry_bl_hash_table_rehash(this);
	if(this->size==0)
		this->size=JRY_BL_HASH_TABLE_MIN_SIZE;
	while(this->size<size)
		this->size<<=1;
	if(this->size<=old_size)
		return;
	if(this->data==NULL)
	{
		this->data=(dt*)(((char*)jry_bl_malloc(this->size*(sizeof(dt)+sizeof(st))))+sizeof(st)*this->size);
		for(register st i=0;i<this->size;jry_bl_hash_table_data_init(&this->data[i]),((st*)this->data)[-i-1]=((jry_bl_int64)-1),++i);
	}
	else
	{
		dt *tmp=(dt*)(((char*)jry_bl_malloc(this->size*(sizeof(dt)+sizeof(st))))+sizeof(st)*this->size);
		register st old_nxt=this->nxt;
		this->nxt=0;
		for(register st i=0;i<this->size;jry_bl_hash_table_data_init(&tmp[i]),((st*)tmp)[-i-1]=((jry_bl_int64)-1),++i);
		for(register st j=0;j<old_nxt;++j)
			if(jry_bl_var_get_type(&this->data[j].v)!=JRY_BL_VAR_TYPE_UNUSE)
			{
				jry_bl_int64 h2=gh2(this,tmp[this->nxt].h=this->data[j].h);
				jry_bl_string_copy(&tmp[this->nxt].k,&this->data[j].k,JRY_BL_COPY_LIGHT_MOVE);
				jry_bl_var_copy(&tmp[this->nxt].v,&this->data[j].v,JRY_BL_COPY_LIGHT_MOVE);
				tmp[this->nxt].nxt=((st*)tmp)[h2];
				((st*)tmp)[h2]=this->nxt;
				++this->nxt;
			}
		if(old_size!=0)
		{
			for(register st i=0;i<old_size;jry_bl_hash_table_data_free(&this->data[i]),++i);	
			jry_bl_free((char*)this->data-sizeof(st)*old_size);
		}
		this->data=tmp;
	}	
}
void jry_bl_hash_table_insert(jry_bl_hash_table *this,jry_bl_string *k,jry_bl_var *v,jry_bl_uint8 kcpt,jry_bl_uint8 vcpt)
{
	if(this==NULL||k==NULL||v==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_uint64 h=jry_bl_string_hash(k);
	jry_bl_int64 h2=gh2(this,h);
	if(this->data!=NULL)
	{
		if(this->size==0)
			jry_bl_hash_table_extend(this,0),h2=gh2(this,h);
		for(register st i=((st*)this->data)[h2];i<this->nxt;i=this->data[i].nxt)
			if(h==this->data[i].h&&jry_bl_string_if_equal(&this->data[i].k,k))
				return jry_bl_var_copy(&this->data[i].v,v,vcpt);
	}
	jry_bl_hash_table_extend(this,1);
	h2=gh2(this,h);
	this->data[this->nxt].h=h;
	jry_bl_string_copy(&this->data[this->nxt].k,k,kcpt);
	jry_bl_var_copy(&this->data[this->nxt].v,v,vcpt);
	this->data[this->nxt].nxt=((st*)this->data)[h2];
	((st*)this->data)[h2]=this->nxt;
	++this->len;
	++this->nxt;
}
void jry_bl_hash_table_get(jry_bl_hash_table *this,jry_bl_string *k,jry_bl_var *v,jry_bl_uint8 vcpt)
{
	if(this==NULL||k==NULL||v==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this->size!=0)
	{
		jry_bl_uint64	h=jry_bl_string_hash(k);
		jry_bl_int64	h2=gh2(this,h);
		for(register st i=((st*)this->data)[h2];i<this->nxt;i=this->data[i].nxt)
			if(h==this->data[i].h&&jry_bl_string_if_equal(&this->data[i].k,k))
				return jry_bl_var_copy(v,&this->data[i].v,vcpt);
	}
	return jry_bl_var_init_as(v,JRY_BL_VAR_TYPE_UNUSE);
}
void jry_bl_hash_table_unset(jry_bl_hash_table *this,jry_bl_string *k)
{
	if(this==NULL||k==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this->size!=0)
	{
		jry_bl_uint64	h=jry_bl_string_hash(k);
		jry_bl_int64	h2=gh2(this,h);
		for(register st i=((st*)this->data)[h2],j=((jry_bl_int64)-1);i<this->nxt;j=i,i=this->data[i].nxt)
			if(h==this->data[i].h&&jry_bl_string_if_equal(&this->data[i].k,k))
			{
				if(j==((jry_bl_int64)-1))
					((st*)this->data)[h2]=this->data[i].nxt;
				else
					this->data[j].nxt=this->data[i].nxt;
				jry_bl_hash_table_data_free(&this->data[i]);
				--this->len;
				for(;this->nxt!=0&&jry_bl_var_get_type(&this->data[this->nxt-1].v)==JRY_BL_VAR_TYPE_UNUSE;--this->nxt);;
				return;
			}
	}
}
char jry_bl_hash_table_space_ship(jry_bl_hash_table *this,jry_bl_hash_table *that)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this==that)
		return 0;
	if(this->len!=that->len)
		return (this->len>that->len)?(1):(-1);
	jry_bl_var tv;jry_bl_var_init(&tv);
	jry_bl_hash_table_foreach(this,i)
	{
		jry_bl_hash_table_get(that,&i->k,&tv,JRY_BL_COPY_LIGHT);
		char tmp=jry_bl_var_space_ship(&i->v,&tv);
		if(!tmp)
			return jry_bl_var_free(&tv),tmp;
	}
	return jry_bl_var_free(&tv),0;
}
void jry_bl_hash_table_clear(jry_bl_hash_table *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this->len==0||this->data==NULL)
		return;
	for(register st i=0;i<this->size;jry_bl_hash_table_data_free(&this->data[i]),((st*)this->data)[-i-1]=((jry_bl_int64)-1),++i);
	this->len=0;
	this->nxt=0;
}
void jry_bl_hash_table_copy(jry_bl_hash_table *this,jry_bl_hash_table *that,jry_bl_uint8 copytype)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(copytype==JRY_BL_COPY)
	{
		jry_bl_hash_table_clear(this);
		jry_bl_hash_table_extend(this,that->len);
		jry_bl_hash_table_foreach(that,i)
			jry_bl_hash_table_insert(this,&i->k,&i->v,JRY_BL_COPY,JRY_BL_COPY);
	}
	else
	{		
		jry_bl_hash_table_free(this);
		this->len=that->len;
		this->nxt=that->nxt;
		this->data=that->data;
		this->size=that->size;
		if(copytype==JRY_BL_COPY_LIGHT)
			this->size=0;
		else
			that->size=0;
	}
}
void jry_bl_hash_table_merge(jry_bl_hash_table *this,jry_bl_hash_table *that,jry_bl_uint8 copytype)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this->len==0)
		return jry_bl_hash_table_copy(this,that,copytype);	
	jry_bl_hash_table_extend(this,that->len);
	jry_bl_hash_table_foreach(that,i)
		jry_bl_hash_table_insert(this,&i->k,&i->v,copytype,copytype);
}
void jry_bl_hash_table_to_json_ex(jry_bl_hash_table *this,jry_bl_string *out,jry_bl_uint8 type)
{
	if(this==NULL||out==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(type==1)
	{
		(*((jry_bl_string_size_type*)out))+=2;
		if(this->len==0)
			return;
		jry_bl_string_size_type tmp_len=0;
		jry_bl_hash_table_foreach(this,i)
			tmp_len+=4+jry_bl_string_get_length(&i->k),jry_bl_var_to_json_ex(&i->v,(jry_bl_string*)(&tmp_len),1);
		(*((jry_bl_string_size_type*)out))+=tmp_len-1;
		return;
	}
	if(type==0)
	{
		jry_bl_string_size_type tmp_len=0;
		jry_bl_hash_table_to_json_ex(this,(jry_bl_string*)(&tmp_len),1);
		jry_bl_string_extend(out,tmp_len);
	}
	jry_bl_string_add_char1(out,'{');
	jry_bl_hash_table_foreach(this,i)
		jry_bl_string_to_json_ex(&i->k,out,2),jry_bl_string_add_char1(out,':'),jry_bl_var_to_json_ex(&i->v,out,2),jry_bl_string_add_char1(out,',');
	if(this->len!=0)
		jry_bl_string_delete_1(out);
	jry_bl_string_add_char1(out,'}');
}
jry_bl_string_size_type jry_bl_hash_table_from_json_start(jry_bl_hash_table *this,jry_bl_string *in,jry_bl_string_size_type start)
{
	if(this==NULL||in==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	register jry_bl_string_size_type i=start,n=jry_bl_string_get_length(in),ii;
	jry_bl_string		k;jry_bl_string_init(&k);
	jry_bl_var 			v;jry_bl_var_init(&v);
	jry_bl_hash_table	t;jry_bl_hash_table_init(&t);	
	for(;(i<n)&&(!(in->s[i]=='{'));++i);++i;
	jry_bl_uint64 cnt=0,deep=0;jry_bl_uint8 str=1;
	for(register jry_bl_string_size_type j=i;(j<n)&&(!(in->s[j]=='}'&&deep==0));++j)
	{
		if((in->s[j]=='"'&&in->s[j-1]!='\\'))
			str=!str;
		else if(in->s[j]=='{'||in->s[j]=='[')
			deep+=str;
		else if(in->s[j]=='}'||in->s[j]==']')
			deep-=str;
		else if(in->s[j]==','&&deep==0)
			++cnt;
	}
	if(cnt!=0)++cnt;
	jry_bl_hash_table_extend(&t,cnt);
	while(i<n)
	{
begin:
		jry_bl_string_clear(&k);
		ii=jry_bl_string_from_json_start(&k,in,i);
		if(ii==i){for(;i<n;++i)if(in->s[i]=='}')goto success;else if(in->s[i]!=' '&&in->s[i]!='\r'&&in->s[i]!='\t'&&in->s[i]!='\n')goto fail;goto fail;}i=ii;
		for(;(i<n)&&(!(in->s[i]==':'));++i)if(in->s[i]!=' '&&in->s[i]!='\r'&&in->s[i]!='\t'&&in->s[i]!='\n')goto fail;if(i>=n)goto fail;++i;
		ii=jry_bl_var_from_json_start(&v,in,i);if(ii==i)goto fail;i=ii;
		jry_bl_hash_table_insert(&t,&k,&v,JRY_BL_COPY_LIGHT_MOVE,JRY_BL_COPY_LIGHT_MOVE);
		for(;i<n;++i)
			if(in->s[i]==',')
				{++i;goto begin;}
			else if(in->s[i]=='}')
				goto success;
			else if(in->s[i]!=' '&&in->s[i]!='\r'&&in->s[i]!='\t'&&in->s[i]!='\n')
				goto fail;
		goto fail;
	}
fail:	
	jry_bl_hash_table_free(&t);jry_bl_string_free(&k);jry_bl_var_free(&v);
	return start;
success:
	jry_bl_hash_table_merge(this,&t,JRY_BL_COPY_LIGHT_MOVE);
	jry_bl_hash_table_free(&t);jry_bl_string_free(&k);jry_bl_var_free(&v);
	return i+1;
}
void jry_bl_hash_table_rehash(jry_bl_hash_table *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this->data==NULL)
		return;
	register st i=0,j=0;
	for(;j<this->nxt&&jry_bl_var_get_type(&this->data[j].v)!=JRY_BL_VAR_TYPE_UNUSE;++j);	
	for(i=j+1;i<this->nxt;++i)
		if(jry_bl_var_get_type(&this->data[i].v)!=JRY_BL_VAR_TYPE_UNUSE&&j<i)
		{
			jry_bl_int64 h2=gh2(this,this->data[j].h=this->data[i].h);
			jry_bl_string_copy(&this->data[j].k,&this->data[i].k,JRY_BL_COPY_LIGHT_MOVE);
			jry_bl_var_copy(&this->data[j].v,&this->data[i].v,JRY_BL_COPY_LIGHT_MOVE);
			this->data[j].nxt=(((st*)this->data)[h2]>=i)?(this->data[i].nxt):(((st*)this->data)[h2]);
			((st*)this->data)[h2]=j;		
			++j;
			jry_bl_hash_table_data_free(&this->data[i]);
		}
	this->nxt=j;
}
#if JRY_BL_USE_STDIO==1
void jry_bl_hash_table_view_ex(jry_bl_hash_table *this,FILE * file,char*str,int a,int tabs)
{
	if(this==NULL||file==NULL||str==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(tabs>=0)
		for(register int i=0;i<tabs;++i,putc('\t',file));
	else
		tabs=-tabs;
	if(a>=0)
		fprintf(file,"jry_bl_hash_table %s %d\tlen:%lld\tnxt:%lld\tsize:%lld\n",str,a,(jry_bl_int64)this->len,(jry_bl_int64)this->nxt,(jry_bl_int64)this->size);
	else
		fprintf(file,"jry_bl_hash_table\tlen:%lld\tnxt:%lld\tsize:%lld\n",(jry_bl_int64)this->len,(jry_bl_int64)this->nxt,(jry_bl_int64)this->size);	
	jry_bl_hash_table_foreach(this,i)
	{
		for(register int i=0;i<tabs+1;++i,putc('\t',file));		
		putc('"',file);jry_bl_string_print(&i->k,file);putc('"',file);putc(':',file);
		jry_bl_var_view_ex(&i->v,file,"",-1,-(tabs+1));
	}
}
#endif
#if JRY_BL_USE_STDARG==1
void jry_bl_hash_table_inits(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_hash_table_init(va_arg(valist,jry_bl_hash_table*));
	va_end(valist);		
}
void jry_bl_hash_table_frees(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_hash_table_free(va_arg(valist,jry_bl_hash_table*));
	va_end(valist);		
}
void jry_bl_hash_table_clears(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_hash_table_clear(va_arg(valist,jry_bl_hash_table*));
	va_end(valist);		
}
#endif
#endif