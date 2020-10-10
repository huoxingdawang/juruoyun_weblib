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
#include "jbl_log.h"
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1
jbl_ht *__jbl_string_cache=NULL;
#endif
jbl_var_operators_new(jbl_string_operators,jbl_string_free,jbl_string_copy,jbl_string_space_ship,jbl_string_json_encode,jbl_string_view_put,jbl_string_json_put);
/*******************************************************************************************/
/*                            以下函数处理传统字符串                                     */
/*******************************************************************************************/
jbl_string_size_type jbl_strlen(const unsigned char *a){if(!a)return 0;register jbl_string_size_type b=0;while(a[b])++b;return b;}
jbl_string_hash_type jbl_strhash(const unsigned char *s,jbl_string_size_type len)
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
JBL_INLINE void jbl_string_start()
{
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1
	__jbl_string_cache=jbl_ht_new();
	jbl_ht_set_sys(__jbl_string_cache);
#endif
	jbl_string_cc_start();
}
JBL_INLINE void jbl_string_stop()
{
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1
#if JBL_STRING_CACHE_DEBUG==1
	jbl_log(UC"%v",__jbl_string_cache);
#endif
	__jbl_string_cache=jbl_ht_free(__jbl_string_cache);
#endif
	jbl_string_cc_stop();
}
/*******************************************************************************************/
/*                            以下函实现字符串基本操作                                   */
/*******************************************************************************************/
jbl_string * jbl_string_new()
{
	jbl_string * this=jbl_malloc(sizeof(jbl_string));
	jbl_gc_init(this);
	jbl_gc_plus(this);//增加引用计数
	jbl_var_set_operators(this,&jbl_string_operators);
	jbl_pthread_lock_init(this);
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
		if (jbl_gc_is_ref(this))
			jbl_string_free((jbl_string*)(((jbl_reference*)this)->ptr));
		else if (this->size)
			jbl_free(this->s);
		jbl_free(this);
	}
	return NULL;
}
JBL_INLINE jbl_string *jbl_string_copy(jbl_string *that)
{
	if(that==NULL)return NULL;
	jbl_gc_plus(that);
	return that;
}
jbl_string *jbl_string_extend_to(jbl_string *this,jbl_string_size_type size,jbl_uint8 add,jbl_string **pthi)
{
	if(!this)this=jbl_string_new();		
	jbl_reference *ref=NULL;jbl_string *thi=jbl_refer_pull_keep_father(this,&ref);
	size+=thi->len*(add&1);
	//[0,JBL_STRING_MIN_LENGTH]=>JBL_STRING_MIN_LENGTH,[JBL_STRING_MIN_LENGTH,4K]=>2倍增,(4k,+oo)=>4k对齐
	size=(size<=JBL_STRING_MIN_LENGTH?JBL_STRING_MIN_LENGTH:size<=0xFFF?(1LL<<(jbl_highbit(size-1)+1)):(((size&(0XFFF))!=0)+(size>>12))<<12);
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
//		++data->u;
		return jbl_string_copy(data->k);
	}
	jbl_string *str=jbl_string_add_const(NULL,in);
	if(str->len<=JBL_STRING_CACHE_MAX_LENGTH)
		__jbl_string_cache=jbl_ht_insert_force(__jbl_string_cache,jbl_string_hash(str),str,(void*)1);
	return str;
}
jbl_string *jbl_string_cache_replace(jbl_string *str)
{
	if(!str)return NULL;
	if(jbl_gc_is_ref(str))return str;
	if(str->len>JBL_STRING_CACHE_MAX_LENGTH)return str;
	jbl_ht_data *data=jbl_ht_get_ht_data(__jbl_string_cache,str);
	if(data)
	{
		jbl_string_free(str);
//		++data->u;
		return jbl_string_copy(data->k);
	}
	if(str->len<=JBL_STRING_CACHE_MAX_LENGTH)
		__jbl_string_cache=jbl_ht_insert_force(__jbl_string_cache,jbl_string_hash(str),str,(void *)1);
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
jbl_string *jbl_string_add_string(jbl_string *this,jbl_string *in)
{
	if(!in)return this;
	in=jbl_refer_pull(in);
	if(!this)return jbl_string_copy(in);
	jbl_string *thi;this=jbl_string_extend_to(this,in->len,1,&thi);jbl_string_hash_clear(thi);
	jbl_memory_copy(thi->s+thi->len,in->s,in->len);
	thi->len=(thi->len+in->len);
	return this;
}
JBL_INLINE jbl_string *jbl_string_add_char(jbl_string *this,const char c)
{
	jbl_string *thi;this=jbl_string_extend_to(this,1,1,&thi);jbl_string_hash_clear(thi);
	thi->s[thi->len]=c,++thi->len;
	return this;
}
JBL_INLINE jbl_string * jbl_string_add_int_length(jbl_string *this,jbl_int64 in,jbl_uint8 len,char c)
{
	jbl_string *thi;this=jbl_string_extend_to(this,22,1,&thi);
	if(in<0)
		jbl_string_add_char_force(thi,'-'),in=-in;
	return jbl_string_add_uint_length(this,in,len,c);
}
jbl_string * jbl_string_add_uint_length(jbl_string *this,jbl_uint64 in,jbl_uint8 len,char c)
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
	this=jbl_string_add_int(this,in);
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
JBL_INLINE jbl_string * jbl_string_add_hex_8bits(jbl_string *this,jbl_uint8 in)
{
	const char hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	jbl_string *thi;this=jbl_string_extend_to(this,2,1,&thi);jbl_string_hash_clear(thi);
	thi->s[thi->len]=(hex[(in>>4)&15])	,++thi->len;
	thi->s[thi->len]=(hex[in&15])		,++thi->len;	
	return this;
}
JBL_INLINE jbl_string* jbl_string_set_tail(jbl_string *this)
{
	if(!this)return NULL;
	jbl_string* thi=jbl_refer_pull(this);
	if(thi->s&&thi->size&&thi->len<thi->size)
		thi->s[thi->len]=0;
	return this;
}
/*******************************************************************************************/
/*                            以下函数实现字符串的获取类操作                             */
/*******************************************************************************************/
JBL_INLINE unsigned char jbl_string_get(jbl_string *this,jbl_string_size_type i)
{
	if(!this)return 0;
	this=jbl_refer_pull(this);		
	return (i<this->len)?this->s[i]:0;
}
jbl_int64 jbl_string_get_int_start(jbl_string *this,jbl_string_size_type *start)
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
jbl_uint64 jbl_string_get_uint_start(jbl_string *this,jbl_string_size_type *start)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);		
	jbl_string_size_type i=start?(*start):0; 	
	if(i>=thi->len)
		return 0;
	unsigned char c;jbl_uint64 x=0;
	for(;((c=thi->s[i])<'0'||c>'9')&&i<thi->len;++i);
	for(x=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&i<thi->len;x=(x<<3)+(x<<1)+c-'0',++i);
	start?(*start=i):0;
	return x;	
}
jbl_uint64 jbl_string_get_uint_start_end(jbl_string *this,jbl_string_size_type *start,unsigned char end)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);		
	jbl_string_size_type i=start?(*start):0; 	
	if(i>=thi->len)
		return 0;
	unsigned char c;jbl_uint64 x=0;
	for(;((c=thi->s[i])<'0'||c>'9')&&i<thi->len&&c!=end;++i);
	if(c==end)return 0;
	for(x=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&c!=end&&i<thi->len;x=(x<<3)+(x<<1)+c-'0',++i);
	start?(*start=i):0;
	return x;	
}
double jbl_string_get_double_start(jbl_string *this,jbl_string_size_type *start)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);		
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
jbl_uint64 jbl_string_get_hex_start(jbl_string *this,jbl_string_size_type *start)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);			
	jbl_string_size_type i=start?(*start):0; 	
	if(i>=thi->len)
		return 0;
	unsigned char c;jbl_uint64 x=0;
	for(;((c=thi->s[i])<'0'||c>'9')&&(c<'a'||c>'f')&&(c<'A'||c>'F')&&i<thi->len;++i);
	for(x=((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):(c-'0'))),++i;(((c=thi->s[i])>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))&&i<thi->len;x=(x<<4)+((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):(c-'0'))),++i);
	start?(*start=i):0;
	return x;
}
jbl_uint64 jbl_string_get_hex_start_len(jbl_string *this,jbl_string_size_type *start,jbl_string_size_type len)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);			
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
char jbl_string_space_ship(jbl_string *this,jbl_string *that)
{
	if(this==that){return 0;}if(!this){return -1;}if(that==NULL){return 1;}
	jbl_string *thi=jbl_refer_pull(this);	
	jbl_string *tha=jbl_refer_pull(that);	
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
char jbl_string_space_ship_chars(jbl_string *this,const char *that)
{
	if(this==(jbl_string *)that){return 0;}if(!this){return -1;}if(!that){return 1;}
	jbl_string *thi=jbl_refer_pull(this);
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
char jbl_string_if_equal(jbl_string *thi,jbl_string *that_)
{	
	if(thi==that_){return 1;}if(thi==NULL||that_==NULL){return 0;}
	jbl_string *this=jbl_refer_pull(thi);	
	jbl_string *that=jbl_refer_pull(that_);	
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
jbl_string_size_type jbl_string_find_char_start(jbl_string *this,unsigned char in,jbl_string_size_type start)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);			
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
jbl_string* jbl_string_json_encode(jbl_string* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	out=jbl_string_json_put_format(this=jbl_refer_pull(this),out,format,tabs);if(!this)return out;
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
	if(format&2){out=jbl_string_add_char(out,',');}if((format&1)||(format&4)){out=jbl_string_add_char(out,'\n');}
	return out;
}
jbl_string* jbl_string_json_put_format(const void* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(!out)out=jbl_string_new();
	if(format&1)for(jbl_uint32 i=0;i<tabs;out=jbl_string_add_char(out,'\t'),++i);
	if(!this)out=jbl_string_add_chars_length(out,UC"null",4);
	return out;
}
#if JBL_STREAM_ENABLE==1
void jbl_string_json_put(jbl_string* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,tabs))return;
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
	if(format&2){jbl_stream_push_char(out,',');}if((format&1)||(format&4)){jbl_stream_push_char(out,'\n');}
}
#endif
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的浏览操作                               */
/*******************************************************************************************/
jbl_string* jbl_string_view_put(jbl_string* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jbl_string *thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jbl_string",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_chars(out,UC" size:");
	jbl_stream_push_uint(out,thi->size);
	jbl_stream_push_chars(out,UC"\tlen:");
	jbl_stream_push_uint(out,thi->len);
	jbl_stream_push_chars(out,UC"\ts:");	
	for(jbl_string_size_type i=0;i<thi->len;++i)
		jbl_stream_push_char(out,thi->s[i]);
	jbl_stream_push_char(out,'\n');
	return this;
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
void __jbl_string_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jbl_string *str;this->data=jbl_string_extend_to(((jbl_string*)this->data),128+this->en,1,&str);jbl_string_hash_clear(str);
	str->len+=this->en;
	this->buf=str->s+str->len;
	this->size=str->size-str->len;
	this->en=0;
	if(nxt)
	{
		while(this->extra[0].u<str->len)
		{
			jbl_stream_buf_size_type len=jbl_min((str->len-this->extra[0].u),(nxt->size-nxt->en));	
			jbl_memory_copy(nxt->buf+nxt->en,str->s+this->extra[0].u,len);
			this->extra[0].u+=len;
			nxt->en+=len;
			{jbl_stream_do(nxt,0);if(nxt->stop)return;}
		}
		jbl_stream_do(nxt,flags);
	}
}
void jbl_string_update_stream_buf(jbl_stream* this)
{
	this=jbl_refer_pull(this);
	jbl_string *st;this->data=jbl_string_extend_to((jbl_string*)this->data,128,1,&st);
	this->buf=st->s+st->len;
}
jbl_stream_operators_new(jbl_stream_string_operators,__jbl_string_stream_operater,jbl_string_free,jbl_string_update_stream_buf);
jbl_string*  jbl_stream_push_string_start_end(jbl_stream *out,jbl_string* this,jbl_string_size_type i,jbl_string_size_type end)
{
	if(!out)jbl_exception("NULL POINTER");
	if(!this)return this;
	out=jbl_refer_pull(out);
	jbl_string *thi=jbl_refer_pull(this);
	for(jbl_min_update(end,thi->len);i<end;)
	{
		jbl_stream_buf_size_type len=jbl_min((end-i),(out->size-out->en));	
		jbl_memory_copy(out->buf+out->en,thi->s+i,len);
		i+=len;
		out->en+=len;
		{jbl_stream_do(out,0);if(out->stop)return this;}
	}
	return this;
}
jbl_string *jbl_string_read(jbl_string *this,const unsigned char *c)
{
	jbl_stream_push_chars(jbl_stream_stdout,c);
	jbl_stream_do(jbl_stream_stdout,jbl_stream_force);
	if(!this)this=jbl_string_new();
	this=jbl_string_extend(this,0);
	jbl_stream *tmp=jbl_string_stream_new(this);
	jbl_stream_connect(jbl_stream_stdin_link,tmp);
	jbl_stream_do(jbl_stream_stdin,jbl_stream_force);
	tmp->data=NULL;
	tmp=jbl_stream_free(tmp);
	return this;
}
#endif
#if JBL_VAR_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的var操作                                */
/*******************************************************************************************/
jbl_var_data * jbl_string_get_number_start(jbl_string *this,jbl_string_size_type *start)
{
	if(!this||!start)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);		
	jbl_string_size_type i=*start;
	unsigned char c,f;jbl_uint64 x=0;
	jbl_uint64 ji=10,y=0;
	if(i>=thi->len)
		return NULL;
	for(f=0;((c=thi->s[i])<'0'||c>'9')&&i<thi->len;f=c=='-',++i);
	for(x=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&i<thi->len;x=(x<<3)+(x<<1)+c-'0',++i);
	*start=i;
	if(thi->s[i]=='e')
		goto e;
	if(thi->s[i]!='.'||i==thi->len)
		return (f?jbl_Vint_set(NULL,-x):jbl_Vuint_set(NULL,x));
	++i;
	for(c=thi->s[i],y=c-'0',++i;(c=thi->s[i])>='0'&&c<='9'&&i<thi->len;y=(y<<3)+(y<<1)+c-'0',ji=(ji<<3)+(ji<<1),++i);
	*start=i;
	if(thi->s[i]=='e')
		goto e;
	return jbl_Vdouble_set(NULL,f?(-(((double)y/ji)+x)):(((double)y/ji)+x));
e:;
	jbl_uint64 e=jbl_string_get_uint_start(this,start),ji2=1;
	while(e--)ji2=(ji2<<3)+(ji2<<1);
	return jbl_Vdouble_set(NULL,(f?(-(((double)y/ji)+x)):(((double)y/ji)+x))*ji2);
}
#endif
/*******************************************************************************************/
/*                            以下函数实现字符串的切割操作                               */
/*******************************************************************************************/
#if JBL_LL_ENABLE==1
jbl_string *jbl_string_delete(jbl_string *this,jbl_string_size_type start,jbl_string_size_type end)
{
	if(start>=end)return this;
	jbl_string *thi;this=jbl_string_extend_to(this,0,1,&thi);jbl_string_hash_clear(thi);
	jbl_min_update(end,thi->len);
	for(jbl_string_size_type j=end,i=start;j<thi->len;thi->s[i]=thi->s[j],++j,++i);
	thi->len-=(end-start);
	return this;
}
jbl_ll * jbl_string_cut_start(jbl_string *this,jbl_ll *list,char cut,jbl_string_size_type start)
{
	if(!this)return list;
	this=jbl_refer_pull(this);
	for(jbl_string *v=NULL;start<this->len;((jbl_string_get_length(v)!=0)?list=jbl_ll_add(list,v):0),v=jbl_var_free(v),++start)
		for(v=jbl_string_new();start<this->len&&this->s[start]!=cut;jbl_string_add_char(v,this->s[start]),++start);
	return (list);	
}
#endif
#endif