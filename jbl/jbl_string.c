/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_string.h"
#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            扩展包含                                                      */
/*******************************************************************************************/
#include "jbl_string_cc.h"
/*******************************************************************************************/
/*                            依赖jbl_gc jbl_malloc jbl_ying jbl_exception                */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_gc.h"
#include "jbl_malloc.h"
#include "jbl_ying.h"
/*******************************************************************************************/
/*                            联动jbl_stream jbl_var jbl_ll jbl_ht                         */
/*******************************************************************************************/
#include "jbl_stream.h"
#include "jbl_var.h"
#include "jbl_ll.h"
#include "jbl_ht.h"
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1
jbl_ht *__jbl_string_cache=NULL;
#endif
/*******************************************************************************************/
/*                            以下函数处理传统字符串                                     */
/*******************************************************************************************/
jbl_string_size_type jbl_strlen(const char *a){if(!a)return 0;register jbl_string_size_type b=0;while(a[b++]);return b-1;}
jbl_string_hash_type jbl_strhash(const char *s,jbl_string_size_type len)
{
	if(!s)return 0;
	register jbl_string_hash_type h=0;
	if(len)	for(register jbl_string_size_type i=0;i<len;h=(h<<5)+h+((unsigned char*)s)[i],++i);
	else	for(;*s;h=(h<<5)+h+*((unsigned char*)s),++s);
	return h;
}
/*******************************************************************************************/
/*                            以下函数完成字符串组件启动和停止                           */
/*******************************************************************************************/
inline void jbl_string_start()
{
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1
	__jbl_string_cache=jbl_ht_new();
	jbl_ht_set_sys(__jbl_string_cache);
#endif
	jbl_string_cc_start();
}
inline void jbl_string_stop()
{
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1
#if JBL_STRING_CACHE_DEBUG==1
	jbl_ht_view(__jbl_string_cache);
#endif
	__jbl_string_cache=jbl_ht_free(__jbl_string_cache);
#endif
	jbl_string_cc_stop();
}
/*******************************************************************************************/
/*                            以下函实现字符串基本操作                                   */
/*******************************************************************************************/
inline jbl_string * jbl_string_new()
{
	return jbl_string_init(jbl_malloc((sizeof(jbl_string))));
}
jbl_string * jbl_string_init(jbl_string *this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_gc_init(this);
	jbl_gc_plus(this);//增加引用计数
	this->len=0;
	this->h=0;
	this->size=0;
	this->s=NULL;
	return this;	
}
jbl_string* jbl_string_free(jbl_string *this)
{
//	if(!this)jbl_exception("NULL POINTER");	
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		((jbl_gc_is_ref(this)||jbl_gc_is_pvar(this))?jbl_string_free((jbl_string*)(((jbl_reference*)this)->ptr)):(this->size?jbl_free(this->s):0));
#if JBL_VAR_ENABLE==1
		if(jbl_gc_is_var(this))
			jbl_free((char*)this-sizeof(jbl_var));
		else
#endif
			jbl_free(this);
	}
	return NULL;
}
inline jbl_string *jbl_string_copy(jbl_string *that)
{
	if(that==NULL)return NULL;
	jbl_gc_plus(that);
	return that;
}
jbl_string *jbl_string_extend_to(jbl_string *this,jbl_string_size_type size,jbl_uint8 add,jbl_string **pthi)
{
	if(!this)this=jbl_string_new();		
	jbl_reference *ref=NULL;jbl_string *thi=jbl_refer_pull_keep_father(this,&ref);
#if JBL_VAR_ENABLE==1
	size+=(jbl_gc_is_pvar(thi)?(((jbl_string*)((jbl_reference*)thi)->ptr)->len):(thi->len))*(add&1);
#else
	size+=thi->len*(add&1);
#endif
	//[0,JBL_STRING_MIN_LENGTH]=>JBL_STRING_MIN_LENGTH,[JBL_STRING_MIN_LENGTH,4K]=>2倍增,(4k,+oo)=>4k对齐
	size=(size<=JBL_STRING_MIN_LENGTH?JBL_STRING_MIN_LENGTH:size<=0xFFF?(1LL<<(jbl_highbit(size-1)+1)):(((size&(0XFFF))!=0)+(size>>12))<<12);
#if JBL_VAR_ENABLE==1
	if(jbl_gc_is_pvar(thi))
	{
		jbl_string *tmp=jbl_Vstring(jbl_Vstring_new());
		jbl_string *ptr=((jbl_reference*)thi)->ptr;
		tmp->size=size;
		tmp->len=ptr->len;
		tmp->h=ptr->h;
		if(jbl_gc_refcnt(thi)==1&&jbl_gc_refcnt(ptr)==1&&size<=thi->size)
			tmp->s=ptr->s,ptr->s=NULL,ptr->size=0;
		else
			tmp->s=(unsigned char *)jbl_malloc(tmp->size),jbl_memory_copy(tmp->s,ptr->s,ptr->len);
		jbl_string_free(thi);
		thi=tmp;
	}
	else
#endif
	if(jbl_gc_refcnt(thi)==1)
	{
		if(thi->size<thi->len)//如果this->size<thi->len,则该字符串是常量,必须扩容
		{
			unsigned char *s=thi->s;
			thi->s=(unsigned char *)jbl_malloc(thi->size=size);
			jbl_memory_copy(thi->s,s,thi->len);
		}
		else if(size>thi->size)
			thi->size=size,thi->s=(thi->s?(unsigned char *)jbl_realloc(thi->s,thi->size):(unsigned char *)jbl_malloc(thi->size));		
	}
	else
	{
		jbl_string *tmp;
#if JBL_VAR_ENABLE==1
		if(jbl_gc_is_var(thi))
			tmp=jbl_Vstring(jbl_Vstring_new());
		else
#endif		
			tmp=jbl_string_new();
		tmp->size=size;
		tmp->len=thi->len;
		tmp->h=thi->h;
		tmp->s=(unsigned char *)jbl_malloc(tmp->size);
		jbl_memory_copy(tmp->s,thi->s,thi->len);
		jbl_string_free(thi);
		thi=tmp;
	}
	if(ref)		ref->ptr=thi;
	else		this=thi;
	if(pthi)	*pthi=thi;
	return this;
}
jbl_string *jbl_string_clear(jbl_string *this)
{
	if(!this)return NULL;
	jbl_string *thi;this=jbl_string_extend_to(this,0,0,&thi);jbl_string_hash_clear(thi);	
	thi->len=0;
	return this;
}
/*******************************************************************************************/
/*                            以下函数实现字符串的缓冲操作                               */
/*******************************************************************************************/
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1
jbl_string *jbl_string_cache_get(const unsigned char *in)
{
	if(!in)return NULL;
	jbl_ht_data *data=jbl_ht_get_ht_data_chars(__jbl_string_cache,in);
	if(data)
	{
		++data->u;
		return jbl_string_copy(data->k);
	}
#if JBL_VAR_ENABLE==1 && JBL_STRING_CACHE_NEW_VAR==1 && JBL_STRING_CACHE_UNVAR_ONLY==0
	jbl_string *str=jbl_string_add_const((jbl_string*)jbl_Vstring_new(),in);
#else
	jbl_string *str=jbl_string_add_const(NULL,in);
#endif
	if(str->len<=JBL_STRING_CACHE_MAX_LENGTH)
		__jbl_string_cache=jbl_ht_insert_force(__jbl_string_cache,jbl_string_hash(str),str,(jbl_var *)1);
	return str;
}
jbl_string *jbl_string_cache_replace(jbl_string *str)
{
	if(!str)return NULL;
	if(jbl_gc_is_ref(str))return str;
#if JBL_STRING_CACHE_UNVAR_ONLY==1
	if(jbl_gc_is_var(str))return str;
#endif
	if(str->len>JBL_STRING_CACHE_MAX_LENGTH)return str;
	jbl_ht_data *data=jbl_ht_get_ht_data(__jbl_string_cache,str);
	if(data)
	{
#if JBL_STRING_CACHE_UNVAR_ONLY==0
		if(jbl_gc_is_var(str)&&!jbl_gc_is_var(data->k))//str是var,cache不是,替换cache.
		{
			jbl_string_free(data->k);
			data->u=1;
			data->k=jbl_string_copy(str);
			return str;
		}
		//str不是var,cache不是
		//str不是var,cache是
		//str是var,cache是均可直接返回
#endif
		jbl_string_free(str);
		++data->u;
		return jbl_string_copy(data->k);
	}
	if(str->len<=JBL_STRING_CACHE_MAX_LENGTH)
		__jbl_string_cache=jbl_ht_insert_force(__jbl_string_cache,jbl_string_hash(str),str,(jbl_var *)1);
	return str;
}
#endif
/*******************************************************************************************/
/*                            以下函数实现字符串的增添类操作                             */
/*******************************************************************************************/
jbl_string *jbl_string_add_const_length(jbl_string *this,const unsigned char *in,jbl_string_size_type len)
{
	if(!in)return this;
	if(this&&(jbl_gc_is_ref(this)||this->s))return jbl_string_add_chars_length(this,in,len);
	if(!this)this=jbl_string_new();
	this->s=(unsigned char*)in;
	this->size=0;
	this->len=len;
	return this;
}
jbl_string *jbl_string_add_chars_length(jbl_string *this,const unsigned char *in,jbl_string_size_type len)
{
	if(!in)return this;
	jbl_string *thi;this=jbl_string_extend_to(this,len,1,&thi);jbl_string_hash_clear(thi);
	jbl_memory_copy(thi->s+thi->len,in,len);
	thi->len=(thi->len+len);
	return this;
}
jbl_string *jbl_string_add_string(jbl_string *this,const jbl_string *in)
{
	if(!in)return this;
	in=jbl_refer_pull(in);
	jbl_string *thi;this=jbl_string_extend_to(this,in->len,1,&thi);jbl_string_hash_clear(thi);
	jbl_memory_copy(thi->s+thi->len,in->s,in->len);
	thi->len=(thi->len+in->len);
	return this;
}
inline jbl_string *jbl_string_add_char(jbl_string *this,const char c)
{
	jbl_string *thi;this=jbl_string_extend_to(this,1,1,&thi);jbl_string_hash_clear(thi);
	thi->s[thi->len]=c,++thi->len;
	return this;
}
inline jbl_string * jbl_string_add_int64_length(jbl_string *this,jbl_int64 in,jbl_uint8 len,char c)
{
	jbl_string *thi;this=jbl_string_extend_to(this,22,1,&thi);
	if(in<0)
		jbl_string_add_char_force(thi,'-'),in=-in;
	return jbl_string_add_uint64_length(this,in,len,c);
}
jbl_string * jbl_string_add_uint64_length(jbl_string *this,jbl_uint64 in,jbl_uint8 len,char c)
{
	jbl_string *thi;this=jbl_string_extend_to(this,jbl_max(len,21),1,&thi);jbl_string_hash_clear(thi);
	if(!in)
	{
		for(jbl_uint8 i=1;i<len;jbl_string_add_char_force(thi,c),++i);
		return jbl_string_add_char_force(thi,'0'),thi;
	}
	int cnt=20;
	unsigned char b[21];
	b[cnt--]=0;
	while(in)b[cnt--]=in%10+'0',in/=10;
	for(jbl_uint8 i=19-cnt;i<len;jbl_string_add_char_force(thi,c),++i);
	return jbl_string_add_chars_length(this,b+cnt+1,19-cnt);
}
jbl_string * jbl_string_add_double_length(jbl_string *this,double in,unsigned char len)
{
	this=jbl_string_extend(this,jbl_max(len,40));
	this=jbl_string_add_int64(this,in);
	this=jbl_string_add_char(this,'.');
	if(in<0)in=-in;
	in-=(jbl_uint64)in;
	
	jbl_uint64 ji=1;
	for(register unsigned char i=0,n=len+2;i<n;++i,ji=(ji<<3)+(ji<<1));
	jbl_uint64 t=(in*ji+0.5)/10;
	if(t==0)
		return jbl_string_add_char(this,'0');

	unsigned char b[21];
	for(register unsigned char i=0;i<21;b[i]='0',++i);
	register unsigned char cnt=20;
	b[cnt--]=0;
	while(t)b[cnt--]=t%10+'0',t/=10;
	return jbl_string_add_chars_length(this,b+20-len-1,len);	
}
jbl_string * jbl_string_add_hex(jbl_string *this,jbl_uint64 in)
{
	unsigned char n=1;
	while((in>>(n<<2)))++n;
	jbl_string *thi;this=jbl_string_extend_to(this,n+1,1,&thi);jbl_string_hash_clear(thi);
	const char hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	for(;n--;thi->s[thi->len]=(hex[(in>>(n<<2))&15]),++thi->len);
	return this;
}
inline jbl_string * jbl_string_add_hex_8bits(jbl_string *this,jbl_uint8 in)
{
	const char hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	jbl_string *thi;this=jbl_string_extend_to(this,2,1,&thi);jbl_string_hash_clear(thi);
	thi->s[thi->len]=(hex[(in>>4)&15])	,++thi->len;
	thi->s[thi->len]=(hex[in&15])		,++thi->len;	
	return this;
}
//TODO START 实现jbl_file后移除这两个函数
#include <stdio.h>
jbl_string* jbl_string_add_file(jbl_string *this,FILE * file)
{
	if(!file)jbl_exception("NULL POINTER");
#if __SIZEOF_POINTER__ == 8 && !defined __linux__
	_fseeki64(file,0L,SEEK_END);
	jbl_string_size_type size=_ftelli64(file);	
	_fseeki64(file,0L,SEEK_SET);
#else
	fseek(file,0L,SEEK_END);
	jbl_string_size_type size=ftell(file);	
	fseek(file,0L,SEEK_SET);
#endif	
	jbl_string *thi;this=jbl_string_extend_to(this,size,1,&thi);jbl_string_hash_clear(thi);
	thi->len+=fread(thi->s+thi->len,1,size,file);
	return this;
}
inline void jbl_string_print(const jbl_string *this,FILE * file)
{
	if(!this)return;
	if(!file)jbl_exception("NULL POINTER");
	this=jbl_refer_pull(this);
	fwrite(this->s,1,this->len,file);
}
//TODO END
/*******************************************************************************************/
/*                            以下函数实现字符串的获取类操作                             */
/*******************************************************************************************/
inline unsigned char jbl_string_get(const jbl_string *this,jbl_string_size_type i)
{
	if(!this)return 0;
	this=jbl_refer_pull(this);		
	return (i<this->len)?this->s[i]:0;
}
jbl_int64 jbl_string_get_int64_start(const jbl_string *this,jbl_string_size_type *start)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);		
	jbl_string_size_type i=start?(*start):0; 	
	if(i>=this->len)return 0;
	unsigned char c,f;jbl_uint64 x=0;
	for(f=0;((c=this->s[i])<'0'||c>'9')&&i<this->len;f=c=='-',++i);
	for(x=c-'0',++i;(c=this->s[i])>='0'&&c<='9'&&i<this->len;x=(x<<3)+(x<<1)+c-'0',++i);
	start?(*start=i):0;
	return f?-x:x;	
}
jbl_uint64 jbl_string_get_uint64_start(const jbl_string *this,jbl_string_size_type *start)
{
	if(!this)jbl_exception("NULL POINTER");	
	const jbl_string *thi=jbl_refer_pull(this);		
	jbl_string_size_type i=start?(*start):0; 	
	if(i>=thi->len)
		return 0;
	unsigned char c;jbl_uint64 x=0;
	for(;((c=thi->s[i])<'0'||c>'9')&&i<thi->len;++i);
	for(x=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&i<thi->len;x=(x<<3)+(x<<1)+c-'0',++i);
	start?(*start=i):0;
	return x;	
}
double jbl_string_get_double_start(const jbl_string *this,jbl_string_size_type *start)
{
	if(!this)jbl_exception("NULL POINTER");	
	const jbl_string *thi=jbl_refer_pull(this);		
	jbl_string_size_type i=start?(*start):0; 	
	if(i>=thi->len)
		return 0;
	unsigned char c,f;jbl_uint64 x=0;
	for(f=0;((c=thi->s[i])<'0'||c>'9')&&i<thi->len;f=c=='-',++i);
	for(x=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&i<thi->len;x=(x<<3)+(x<<1)+c-'0',++i);
	start?(*start=i):0;
	if(thi->s[i]!='.'||i==thi->len)
		return (f?-(double)x:(double)x);
	jbl_uint64 ji=10,y;++i;
	for(c=thi->s[i],y=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&i<thi->len;y=(y<<3)+(y<<1)+c-'0',ji=(ji<<3)+(ji<<1),++i);
	start?(*start=i):0;
	return f?(-(((double)y/ji)+x)):(((double)y/ji)+x);
}
jbl_uint64 jbl_string_get_hex_start(const jbl_string *this,jbl_string_size_type *start)
{
	if(!this)jbl_exception("NULL POINTER");	
	const jbl_string *thi=jbl_refer_pull(this);			
	jbl_string_size_type i=start?(*start):0; 	
	if(i>=thi->len)
		return 0;
	unsigned char c;jbl_uint64 x=0;
	for(;((c=thi->s[i])<'0'||c>'9')&&(c<'a'||c>'f')&&(c<'A'||c>'F')&&i<thi->len;++i);
	for(x=((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):(c-'0'))),++i;(((c=thi->s[i])>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))&&i<thi->len;x=(x<<4)+((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):(c-'0'))),++i);
	start?(*start=i):0;
	return x;
}
jbl_uint64 jbl_string_get_hex_start_len(const jbl_string *this,jbl_string_size_type *start,jbl_string_size_type len)
{
	if(!this)jbl_exception("NULL POINTER");	
	const jbl_string *thi=jbl_refer_pull(this);			
	jbl_string_size_type i=start?(*start):0; 	
	if(i>=thi->len)
		return 0;
	unsigned char c;jbl_uint64 x=0;
	for(;((c=thi->s[i])<'0'||c>'9')&&(c<'a'||c>'f')&&(c<'A'||c>'F')&&i<thi->len;++i);
	for(x=((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):(c-'0'))),++i;(--len)&&(((c=thi->s[i])>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))&&i<thi->len;x=(x<<4)+((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):(c-'0'))),++i);
	start?(*start=i):0;
	return x;
}
/*******************************************************************************************/
/*                            以下函数实现字符串的比较类操作                             */
/*******************************************************************************************/
char jbl_string_space_ship(const jbl_string *this,const jbl_string *that)
{
	if(this==that){return 0;}if(!this){return -1;}if(that==NULL){return 1;}
	const jbl_string *thi=jbl_refer_pull(this);	
	const jbl_string *tha=jbl_refer_pull(that);	
	if(thi==tha){return 0;}if(thi==NULL){return -1;}if(tha==NULL){return 1;}
	if(thi->len!=tha->len)
		return (thi->len<tha->len)?-1:1;
	for(jbl_string_size_type i=0;i<thi->len;++i)
		if(thi->s[i]<tha->s[i])
			return -1;
		else if(thi->s[i]>tha->s[i])
			return 1;
	return 0;
}
char jbl_string_space_ship_chars(const jbl_string *this,const char *that)
{
	if(this==(const jbl_string *)that){return 0;}if(!this){return -1;}if(!that){return 1;}
	const jbl_string *thi=jbl_refer_pull(this);
	register jbl_string_size_type i=0;
	for(;i<thi->len;++i)
	{
		if(thi->s[i]<that[i])
			return -1;
		else if(!that[i]||thi->s[i]>that[i])
			return 1;
	}
	if(that[i])
		return -1;
	return 0;
}
char jbl_string_if_equal(const jbl_string *thi,const jbl_string *that_)
{	
	if(thi==that_){return 1;}if(thi==NULL||that_==NULL){return 0;}
	const jbl_string *this=jbl_refer_pull(thi);	
	const jbl_string *that=jbl_refer_pull(that_);	
	if(this==that){return 1;}if(this==NULL||that==NULL){return 0;}
	if(this->h!=0&&this->h!=that->h)return 0;
	if(this->len!=that->len)return 0;
	for(jbl_string_size_type i=0;i<this->len;i++)
		if(this->s[i]!=that->s[i])
			return 0;
	return 1;
}
/*******************************************************************************************/
/*                            以下函数实现字符串的查找类操作                             */
/*******************************************************************************************/
jbl_string_size_type jbl_string_find_char_start(const jbl_string *this,unsigned char in,jbl_string_size_type start)
{
	if(!this)jbl_exception("NULL POINTER");	
	const jbl_string *thi=jbl_refer_pull(this);			
	for(;start<thi->len&&thi->s[start]!=in;++start);
	return (start);
}
/*******************************************************************************************/
/*                            以下函数实现字符串的hash操作                               */
/*******************************************************************************************/
jbl_string_hash_type jbl_string_hash(jbl_string *this)
{
	this=jbl_refer_pull(this);
	if(!this)return 0;
	if(this->h==0)
		for(jbl_string_size_type i=0;i<this->len;++i)
			this->h=(this->h<<5)+this->h+this->s[i];
	return this->h;
}
/*******************************************************************************************/
/*                            以下函数实现字符串的修改操作                               */
/*******************************************************************************************/
jbl_string * jbl_string_to_upper_case(jbl_string *this)
{
	jbl_string *thi;this=jbl_string_extend_to(this,0,1,&thi);jbl_string_hash_clear(thi);
	for(jbl_string_size_type i=0;i<thi->len;++i)
		if(thi->s[i]>='a'&&thi->s[i]<='z')
			thi->s[i]=thi->s[i]-'a'+'A';
	return  this;
}
jbl_string * jbl_string_to_lower_case(jbl_string *this)
{
	jbl_string *thi;this=jbl_string_extend_to(this,0,1,&thi);jbl_string_hash_clear(thi);
	for(jbl_string_size_type i=0;i<thi->len;++i)
		if(thi->s[i]>='A'&&thi->s[i]<='Z')
			thi->s[i]=thi->s[i]-'A'+'a';
	return  this;
}
#if JBL_JSON_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的JSON操作                               */
/*******************************************************************************************/
jbl_string* jbl_string_json_encode(const jbl_string* this,jbl_string *out,char format,jbl_int32 tabs)
{
	out=jbl_string_json_put_format(this=jbl_refer_pull(this),out,format,&tabs);if(!this)return out;
	out=jbl_string_add_char(out,'"');
	for(jbl_string_size_type i=0;i<this->len;++i)
	{
		if(this->s[i]>31&&this->s[i]!='\"' &&this->s[i]!='\\')
			out=jbl_string_add_char(out,this->s[i]);
		else
		{
			out=jbl_string_add_char(out,'\\');
			jbl_uint8 token;
			switch(token=this->s[i])
			{
				case '\\':out=jbl_string_add_char(out,'\\');	break;
				case '\"':out=jbl_string_add_char(out,'\"');	break;
				case '\b':out=jbl_string_add_char(out,'b');	break;
				case '\f':out=jbl_string_add_char(out,'f');	break;
				case '\n':out=jbl_string_add_char(out,'n');	break;
				case '\r':out=jbl_string_add_char(out,'r');	break;
				case '\t':out=jbl_string_add_char(out,'t');	break;
				default  :out=jbl_string_add_chars_length(out,(unsigned char*)"u00",3),out=jbl_string_add_hex_8bits(out,token);break;
			}
		}				
	}
	out=jbl_string_add_char(out,'"');
	return out;
}
jbl_string* jbl_string_json_put_format(const void* this,jbl_string *out,char format,jbl_int32 *tabs)
{
	if(!out)out=jbl_string_new();
	if(format&&*tabs>=0)for(jbl_int16 i=0;i<*tabs;out=jbl_string_add_char(out,'\t'),++i);else *tabs=-*tabs;	
	++*tabs;
	if(!this)out=jbl_string_add_chars_length(out,(unsigned char *)"null",4);
	return out;
}
jbl_string* jbl_string_json_decode(jbl_string *this,const jbl_string* in,jbl_string_size_type *start)
{
	in=jbl_refer_pull(in);
	if(!in)jbl_exception("NULL POINTER");
	jbl_string_size_type i=start?(*start):0,n=in->len;
	for(;i<n&&in->s[i]<=32;++i);
//	pchar(in->s[i]);pn();
	if(in->s[i]!='"')
		goto fail;
	for(++i;i<n;)
	{
		switch(in->s[i])
		{
			case '"':
				{++i;goto success;}
				break;
			case '\\':
				switch(in->s[i+1])
				{
					case '\\':
						i+=2;this=jbl_string_add_char(this,'\\');
						break;
					case '"':
						i+=2;this=jbl_string_add_char(this,'"');
						break;
					case 'b':
						i+=2;this=jbl_string_add_char(this,'\b');
						break;
					case 'f':
						i+=2;this=jbl_string_add_char(this,'\f');
						break;
					case 'n':
						i+=2;this=jbl_string_add_char(this,'\n');
						break;
					case 'r':
						i+=2;this=jbl_string_add_char(this,'\r');
						break;
					case 't':
						i+=2;this=jbl_string_add_char(this,'\t');
						break;
					case 'u':
						i+=2;this=jbl_string_add_utf8_from_unicode(this,jbl_string_get_hex_start_len(in,&i,4));
						break;
					default:
						i+=2;this=jbl_string_add_char(this,'\\');
						break;
				}
				break;
			default :
				this=jbl_string_add_char(this,in->s[i]),++i;
		}
	}
	goto fail;
success:;
	if(!this)this=jbl_string_new();
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1 && JBL_STRING_USE_CACHE_WHEN_JSON_DECODE==1
	this=jbl_string_cache_replace(this);
#endif
	start?(*start=i):0;
	return this;
fail:;
	return NULL;
}
#if JBL_STREAM_ENABLE==1
void jbl_string_json_put(const jbl_string* this,jbl_stream *out,char format,jbl_int32 tabs)
{
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,&tabs))return;
	jbl_stream_push_char(out,'"');
	for(jbl_string_size_type i=0;i<this->len;++i)
	{
		if(this->s[i]>31&&this->s[i]!='\"' &&this->s[i]!='\\')
			jbl_stream_push_char(out,this->s[i]);
		else
		{
			jbl_stream_push_char(out,'\\');
			jbl_uint8 token;
			switch(token=this->s[i])
			{
				case '\\':jbl_stream_push_char(out,'\\');	break;
				case '\"':jbl_stream_push_char(out,'\"');	break;
				case '\b':jbl_stream_push_char(out,'b');	break;
				case '\f':jbl_stream_push_char(out,'f');	break;
				case '\n':jbl_stream_push_char(out,'n');	break;
				case '\r':jbl_stream_push_char(out,'r');	break;
				case '\t':jbl_stream_push_char(out,'t');	break;
				default  :jbl_stream_push_chars(out,UC"u00"),jbl_stream_push_hex_8bits(out,token);break;
			}
		}		
	}
	jbl_stream_push_char(out,'"');
}
#endif
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的浏览操作                               */
/*******************************************************************************************/
void jbl_string_view_put(const jbl_string* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs)
{
	if(jbl_stream_view_put_format(this=jbl_refer_pull(this),out,"jbl_string    ",format,str,&tabs))return;
	jbl_stream_push_chars(out,UC"\tsize:");
	jbl_stream_push_uint(out,this->size);
	jbl_stream_push_chars(out,UC"\tlen:");
	jbl_stream_push_uint(out,this->len);
	jbl_stream_push_chars(out,UC"\ts:");
	for(jbl_string_size_type i=0;i<this->len;jbl_stream_push_char(out,this->s[i]),++i);
}
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的STREAM操作                             */
/*******************************************************************************************/
jbl_stream * jbl_string_stream_new(jbl_string *str)
{
	jbl_string *st;str=jbl_string_extend_to(str,128,1,&st);
	return jbl_stream_new(&jbl_stream_string_operators,str,st->size-st->len,st->s+st->len,1);
}
#if JBL_VAR_ENABLE==1
jbl_var * jbl_string_Vstream_new(jbl_string *str)
{
	jbl_string *st;str=jbl_string_extend_to(str,128,1,&st);
	return jbl_Vstream_new(&jbl_stream_string_operators,str,st->size-st->len,st->s+st->len,1);
}
#endif
void __jbl_string_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jbl_string *str;this->data=jbl_string_extend_to(((jbl_string*)this->data),128,1,&str);jbl_string_hash_clear(str);
	str->len+=this->en;
	this->buf=str->s+str->len;
	this->size=str->size-str->len;
	this->en=0;
	if(nxt)
	{
		while(this->tmp[0].u<str->len)
		{
			jbl_uint16 len=jbl_min((str->len-this->tmp[0].u),(nxt->size-nxt->en));	
			jbl_memory_copy(nxt->buf+nxt->en,str->s+this->tmp[0].u,len);
			this->tmp[0].u+=len;
			nxt->en+=len;
			jbl_stream_do(nxt,0);
		}
		jbl_stream_do(nxt,flags);
	}
}
jbl_string *jbl_string_copy_for_stream(jbl_string *that)
{
	if(!that)jbl_exception("NULL POINTER");
	return jbl_string_copy(jbl_refer_pull(that));
}
void jbl_string_update_stream_buf(jbl_stream* this)
{
	this=jbl_refer_pull(this);
	jbl_string *st;this->data=jbl_string_extend_to((jbl_string*)this->data,128,1,&st);
	this->buf=st->s+st->len;
}
jbl_stream_operators_new(jbl_stream_string_operators,__jbl_string_stream_operater,jbl_string_free,jbl_string_update_stream_buf);
void jbl_stream_push_string(jbl_stream *out,const jbl_string* this)
{
	if(!out)jbl_exception("NULL POINTER");
	if(!this)return;
	out=jbl_refer_pull(out);
	this=jbl_refer_pull(this);
	for(jbl_string_size_type i=0;i<this->len;++i)
		jbl_stream_push_char(out,this->s[i]);
}
jbl_string *jbl_string_read(jbl_string *this,const unsigned char *c)
{
	jbl_stream_push_chars(jbl_stream_stdout,c);
	jbl_stream_do(jbl_stream_stdout,jbl_stream_force);
	if(!this)this=jbl_string_new();
	jbl_stream *tmp=jbl_string_stream_new(jbl_refer(&this));
	jbl_stream_connect(jbl_stream_stdin_link,tmp);
	jbl_stream_do(jbl_stream_stdin,jbl_stream_force);
	jbl_string * that=(jbl_string*)jbl_derefer(this);
	this=jbl_string_free(this);
	tmp=jbl_stream_free(tmp);
	return that;
}
#endif
#if JBL_VAR_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的var操作                                */
/*******************************************************************************************/
jbl_var_operators_new(jbl_string_operators,jbl_string_free,jbl_string_copy,jbl_string_space_ship,jbl_string_json_encode,jbl_string_view_put,jbl_string_json_put);

jbl_var * jbl_Vstring_new()
{
	jbl_var *this=(jbl_var*)((char*)(jbl_malloc((sizeof(jbl_string))+(sizeof(jbl_var)))+(sizeof(jbl_var))));
	jbl_string_init((jbl_string*)this);
	jbl_gc_set_var((jbl_string*)this);
	jbl_var_set_operators(this,&jbl_string_operators);
	return this;
}
inline jbl_string * jbl_Vstring(jbl_var * this){if(this&&!Vis_jbl_string(this))jbl_exception("VAR TYPE ERROR");return((jbl_string*)this);}
jbl_var * jbl_string_get_number_start(const jbl_string *this,jbl_string_size_type *start)
{
	if(!this||!start)jbl_exception("NULL POINTER");	
	const jbl_string *thi=jbl_refer_pull(this);		
	jbl_string_size_type i=*start;
	if(i>=thi->len)
		return NULL;
	unsigned char c,f;jbl_uint64 x=0;
	for(f=0;((c=thi->s[i])<'0'||c>'9')&&i<thi->len;f=c=='-',++i);
	for(x=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&i<thi->len;x=(x<<3)+(x<<1)+c-'0',++i);
	*start=i;
	if(thi->s[i]=='e')
		goto e;
	if(thi->s[i]!='.'||i==thi->len)
		return (f?jbl_Vint_set(NULL,-x):jbl_Vuint_set(NULL,x));
	jbl_uint64 ji=10,y;++i;
	for(c=thi->s[i],y=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&i<thi->len;y=(y<<3)+(y<<1)+c-'0',ji=(ji<<3)+(ji<<1),++i);
	*start=i;
	if(thi->s[i]=='e')
		goto e;
	return jbl_Vdouble_set(NULL,f?(-(((double)y/ji)+x)):(((double)y/ji)+x));
e:;
	jbl_uint64 e=jbl_string_get_uint64_start(this,start),ji2=1;
	while(e--)ji2=(ji2<<3)+(ji2<<1);
	return jbl_Vdouble_set(NULL,(f?(-(((double)y/ji)+x)):(((double)y/ji)+x))*ji2);
}
#endif
#if JBL_LL_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的切割操作                               */
/*******************************************************************************************/
jbl_ll * jbl_string_cut_start(jbl_string *this,jbl_ll *list,char cut,jbl_string_size_type start)
{
	if(!this)return list;
	this=jbl_refer_pull(this);
	for(jbl_var *v=NULL;start<this->len;((jbl_string_get_length(jbl_Vstring(v))!=0)?list=jbl_ll_add(list,v):0),v=jbl_var_free(v),++start)
		for(v=jbl_Vstring_new();start<this->len&&this->s[start]!=cut;jbl_string_add_char(jbl_Vstring(v),this->s[start]),++start);
	return (list);	
}
#endif
#endif