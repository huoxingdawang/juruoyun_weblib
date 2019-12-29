/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_bl_string.h"
#if JRY_BL_STRING_ENABLE==1
inline jry_bl_string_size_type	jry_bl_strlen			(char *a)											{jry_bl_string_size_type b=0;while(a[b++]);return b-1;}
inline void 					jry_bl_string_init		(jry_bl_string *this)								{if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);this->len=this->size=0;this->s=NULL;}
inline void 					jry_bl_string_free		(jry_bl_string *this)								{if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);if(this->size!=0&&this->s!=NULL)jry_bl_free(this->s);this->len=this->size=0;this->s=NULL;}
inline void						jry_bl_string_clear		(jry_bl_string *this)								{if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);this->len=0;if(this->size==0)this->s=NULL;}
inline void						jry_bl_string_parse		(jry_bl_string *this)								{if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);if(this->size==0)return;if(this->len==0)return jry_bl_string_free(this);jry_bl_string_size_type size=(jry_bl_ceil((long double)(this->len)/JRY_BL_STRING_BASIC_LENGTH))*JRY_BL_STRING_BASIC_LENGTH;if(size>=this->size)return ;this->s=(unsigned char *)jry_bl_realloc(this->s,size);this->size=size;}
inline void						jry_bl_string_extend_to	(jry_bl_string *this,jry_bl_string_size_type size)	{if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);if(this->size==0&&this->s!=NULL){this->size=(jry_bl_ceil((long double)(size)/JRY_BL_STRING_BASIC_LENGTH))*JRY_BL_STRING_BASIC_LENGTH;unsigned char * s=(unsigned char *)jry_bl_malloc(this->size);jry_bl_memory_copy(s,this->s,this->len);this->s=s;}else if(size>this->size){this->size=(jry_bl_ceil((long double)(size)/JRY_BL_STRING_BASIC_LENGTH))*JRY_BL_STRING_BASIC_LENGTH;this->s=(this->s==NULL?(unsigned char *)jry_bl_malloc(this->size):(unsigned char *)jry_bl_realloc(this->s,this->size));}}
jry_bl_uint64					jry_bl_string_hash		(jry_bl_string *this)								{jry_bl_uint64 h=0;for(jry_bl_string_size_type i=0;i<this->len;i++)h=(h<<5)+h+this->s[i];return h;}
inline unsigned char			jry_bl_string_get		(jry_bl_string *this,jry_bl_string_size_type i)		{if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);if(i<0)return 0;if(i<this->len)return this->s[i];return 0;}
inline unsigned char			jry_bl_string_set		(jry_bl_string *this,jry_bl_string_size_type i,unsigned char a){if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);if(i<0)return 0;jry_bl_string_extend_to(this,i);return this->s[i]=a;}
void jry_bl_string_add_string(jry_bl_string *this,jry_bl_string *in)
{
	if(this==NULL||in==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_extend(this,in->len);
	for(jry_bl_string_size_type i=0;i<in->len;i++)
		this->s[this->len+i]=in->s[i];
	this->len=(this->len+in->len);
}
void jry_bl_string_add_char_pointer_length(jry_bl_string *this,unsigned char *in,jry_bl_string_size_type len)
{
	if(this==NULL||in==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_extend(this,len);
	for(jry_bl_string_size_type i=0;i<len;i++)
		this->s[this->len+i]=in[i];
	this->len=(this->len+len);	
}
inline void jry_bl_string_add_char(jry_bl_string *this,unsigned char in)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_extend(this,1);
	this->s[this->len++]=in;
}
inline void jry_bl_string_add_int64(jry_bl_string *this,jry_bl_int64 in)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_extend(this,22);
	if(in<0)
		jry_bl_string_add_char(this,'-'),in=-in;
	jry_bl_string_add_uint64(this,in);
}
void jry_bl_string_add_uint64(jry_bl_string *this,jry_bl_uint64 in)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(in==0)
		return jry_bl_string_add_char(this,'0');
	register int cnt=20;
	unsigned char b[21];
	b[cnt--]=0;
	while(in)
		b[cnt--]=in%10+'0',in/=10;
	jry_bl_string_add_char_pointer(this,b+cnt+1);
}
void jry_bl_string_add_double_length(jry_bl_string *this,double in,unsigned char len)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(len>15)
		len=15;
	jry_bl_string_extend(this,40);
	register jry_bl_int64 inn=in;
	jry_bl_string_add_int64(this,inn);
	jry_bl_string_add_char(this,'.');
	if(in<0)
		inn=-inn,in=-in;
	in-=inn;
	register jry_bl_uint64 ji=10;
	register unsigned char i=0;
	while(i<len&&(in*ji!=(jry_bl_uint64)(in*ji)))
		ji=(ji<<3)+(ji<<1),i++;
	jry_bl_string_add_uint64(this,((jry_bl_uint64)((in*ji+0.5)/10)));
}
void jry_bl_string_add_unicode_as_utf8(jry_bl_string *this,unsigned long unicode)
{
	jry_bl_string_extend(this,6);
	if(unicode<=0x7F)
		jry_bl_string_add_char1(this,unicode&0x7F);
	else if(unicode>=0x80&&unicode<=0x7FF)
		jry_bl_string_add_char1(this,((unicode>>6)&0x1F)|0xC0),jry_bl_string_add_char1(this,(unicode&0x3F)|0x80);
	else if(unicode>=0x800&&unicode<=0xFFFF)
		jry_bl_string_add_char1(this,((unicode>>12)&0x0F)|0xE0),jry_bl_string_add_char1(this,((unicode>>6)&0x3F)|0x80),jry_bl_string_add_char1(this,(unicode&0x3F)|0x80);
	else if(unicode>=0x10000&&unicode<=0x10FFFF)
		jry_bl_string_add_char1(this,((unicode>>18)&0x7)|0xF0),jry_bl_string_add_char1(this,((unicode>>12)&0x3F)|0x80),jry_bl_string_add_char1(this,((unicode>>6)&0x3F)|0x80),jry_bl_string_add_char1(this,(unicode&0x3F)|0x80);
	else if(unicode>=0x200000&&unicode<=0x3FFFFFF)
		jry_bl_string_add_char1(this,((unicode>>24)&0x3)|0xF8),jry_bl_string_add_char1(this,((unicode>>18)&0x3F)|0x80),jry_bl_string_add_char1(this,((unicode>>12)&0x3F)|0x80),jry_bl_string_add_char1(this,((unicode>>6)&0x3F)|0x80),jry_bl_string_add_char1(this,(unicode&0x3F)|0x80);
	else if(unicode>=0x4000000&&unicode<=0x7FFFFFFF)
		jry_bl_string_add_char1(this,((unicode>>30)&0x1)|0xFC),jry_bl_string_add_char1(this,((unicode>>24)&0x3F)|0x80),jry_bl_string_add_char1(this,((unicode>>18)&0x3F)|0x80),jry_bl_string_add_char1(this,((unicode>>12)&0x3F)|0x80),jry_bl_string_add_char1(this,((unicode>>6)&0x3F)|0x80),jry_bl_string_add_char1(this,(unicode&0x3F)|0x80);
}

inline void jry_bl_string_copy(jry_bl_string *this,jry_bl_string *in,jry_bl_uint8 copytype)
{
	if(this==NULL||in==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(copytype==JRY_BL_COPY)
		jry_bl_string_clear(this),jry_bl_string_add_string(this,in);
	else
	{
		jry_bl_string_free(this);
		this->len=in->len;this->size=in->size;this->s=in->s;
		if(copytype==JRY_BL_COPY_LIGHT)
			this->size=0;
		else
			in->size=0;
	}
}
char jry_bl_string_space_ship(jry_bl_string *this,jry_bl_string *that)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this==that||this->s==that->s)
		return 0;
	if(this->len!=that->len)
		return (this->len<that->len)?-1:1;
	for(jry_bl_string_size_type i=0;i<this->len;i++)
		if(this->s[i]<that->s[i])
			return -1;
		else if(this->s[i]>that->s[i])
			return 1;
	return 0;
}
jry_bl_int64 jry_bl_string_get_int64_start(jry_bl_string *this,jry_bl_string_size_type *start)
{
	if(this==NULL||start==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_size_type i=*start; 	
	if(i>=this->len)
		return 0;
	register unsigned char c,f;register jry_bl_uint64 x=0;
	for(f=0;(c=this->s[i])<'0'||c>'9'&&i<this->len;f=c=='-',++i);
	for(x=c-'0',++i;(c=this->s[i])>='0'&&c<='9'&&i<this->len;x=(x<<3)+(x<<1)+c-'0',++i);
	*start=i;
	return f?-x:x;	
}
inline jry_bl_int64 jry_bl_string_get_int64_start_v(jry_bl_string *this,jry_bl_string_size_type start){return jry_bl_string_get_int64_start(this,&start);};
jry_bl_uint64 jry_bl_string_get_uint64_start(jry_bl_string *this,jry_bl_string_size_type *start)
{
	if(this==NULL||start==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_size_type i=*start; 	
	if(i>=this->len)
		return 0;
	register unsigned char c;register jry_bl_uint64 x=0;
	for(;(c=this->s[i])<'0'||c>'9'&&i<this->len;++i);
	for(x=c-'0',++i;(c=this->s[i])>='0'&&c<='9'&&i<this->len;x=(x<<3)+(x<<1)+c-'0',++i);
	*start=i;
	return x;	
}
inline jry_bl_uint64 jry_bl_string_get_uint64_start_v(jry_bl_string *this,jry_bl_string_size_type start){return jry_bl_string_get_uint64_start(this,&start);};
double jry_bl_string_get_double_start(jry_bl_string *this,jry_bl_string_size_type *start)
{
	if(this==NULL||start==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_size_type i=*start;
	if(i>=this->len)
		return 0;
	register unsigned char c,f;register jry_bl_uint64 x=0;
	for(f=0;(c=this->s[i])<'0'||c>'9'&&i<this->len;f=c=='-',++i);
	for(x=c-'0',++i;(c=this->s[i])>='0'&&c<='9'&&i<this->len;x=(x<<3)+(x<<1)+c-'0',++i);
	*start=i;
	if(this->s[i]!='.'||i==this->len)
		return (f?-x:x);
	register jry_bl_uint64 ji=10,y;++i;
	for(c=this->s[i],y=c-'0',++i;(c=this->s[i])>='0'&&c<='9'&&i<this->len;y=(y<<3)+(y<<1)+c-'0',ji=(ji<<3)+(ji<<1),++i);
	*start=i;
	return f?-(x+((long double)y/ji)):(x+((long double)y/ji));
}
inline double jry_bl_string_get_double_start_v(jry_bl_string *this,jry_bl_string_size_type start){return jry_bl_string_get_double_start(this,&start);};
jry_bl_uint64 jry_bl_string_get_hex_start(jry_bl_string *this,jry_bl_string_size_type *start)
{
	if(this==NULL||start==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_size_type i=*start; 	
	if(i>=this->len)
		return 0;
	register unsigned char c;register jry_bl_uint64 x=0;
	for(;((c=this->s[i])<'0'||c>'9')&&(c<'a'||c>'f')&&(c<'A'||c>'F')&&i<this->len;++i);
	for(x=((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):(c-'0'))),++i;(((c=this->s[i])>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))&&i<this->len;x=(x<<4)+((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):(c-'0'))),++i);
	*start=i;
	return x;
}
inline jry_bl_uint64 jry_bl_string_get_hex_start_v(jry_bl_string *this,jry_bl_string_size_type start){return jry_bl_string_get_hex_start(this,&start);};
void jry_bl_string_to_json_ex(jry_bl_string *this,jry_bl_string *result,jry_bl_uint8 type)
{
	if(this==NULL||result==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(type==1)
	{
		(*((jry_bl_string_size_type*)result))+=2+this->len;
		return;
	}
	if(type==0)
		jry_bl_string_extend(result,this->len+2);
	jry_bl_string_add_char(result,'"');
	for(jry_bl_string_size_type i=0;i<this->len;i++)
		(this->s[i]=='"'?jry_bl_string_add_char(result,'\\'):' '),jry_bl_string_add_char(result,this->s[i]);
	jry_bl_string_add_char(result,'"');	
}
jry_bl_string_size_type jry_bl_string_from_json_start(jry_bl_string *this,jry_bl_string *in,jry_bl_string_size_type start)
{
	if(this==NULL||in==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_string_size_type i=start,n=in->len,no=this->len,st,en;
	for(;(i<n)&&(!(in->s[i]=='"'&&(i==0||in->s[i-1]!='\\')));++i)if(in->s[i]!=' '&&in->s[i]!='\r'&&in->s[i]!='\t'&&in->s[i]!='\n')return start;++i;st=i;
	for(;i<n&&!(in->s[i]=='"'&&in->s[i-1]!='\\');++i);en=i;++i;
	if((i-1)>=n)
		return start;
	jry_bl_string_extend(this,en-st);
	for(i=st;i<en;i++)
	{
		if(((i+1)<en)&&in->s[i]=='\\'&&in->s[i+1]=='u')
			i+=2,jry_bl_string_add_unicode_as_utf8(this,jry_bl_string_get_hex_start(in,&i)),--i;
		else
			jry_bl_string_add_char1(this,in->s[i]);
	}
	return en+1;
}
jry_bl_string_size_type	jry_bl_string_find_char_start(jry_bl_string *this,unsigned char in,jry_bl_string_size_type start)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	for(;start<this->len&&this->s[start]!=in;++start);
	return (start);
}
#if JRY_BL_USE_STDIO==1
/*#include <time.h>*/
inline void jry_bl_string_print(jry_bl_string *this,FILE * file)
{
	if(this==NULL||file==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	register jry_bl_string_size_type i=0;
	/*clock_t __start=clock();*/
	i+=fwrite(this->s+i,1<<10,(this->len-i)>>10,file)<<10;
	for(;i<this->len;fputc(this->s[i++],file));
	/*fprintf(stderr,"\n\nUse Time:%fs\n",((double)(clock()-__start)/CLOCKS_PER_SEC));*/
}
void jry_bl_string_view_ex(jry_bl_string *this,FILE * file,char*str,int a,int tabs)
{
	if(this==NULL||file==NULL||str==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(tabs>=0)
		for(register int i=0;i<tabs;++i,putc('\t',file));
	else
		tabs=-tabs;
	if(a>=0)
		fprintf(file,"jry_bl_string    %s %d:",str,a);
	else
		fprintf(file,"jry_bl_string     \t:");
	fprintf(file,"size:%lld\tlen:%lld\ts:",(jry_bl_int64)this->size,(jry_bl_int64)this->len);
	jry_bl_string_print(this,file);
	fputc('\n',file);
}
void jry_bl_string_add_file(jry_bl_string *this,FILE * file)
{
	if(this==NULL||file==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	/*clock_t __start=clock();*/
	fseek(file,0L,SEEK_END);
	register unsigned char c;
	register jry_bl_string_size_type size=ftell(file),i=0;
	jry_bl_string_extend(this,size);
	fseek(file,0L,SEEK_SET);
	i+=fread(this->s+this->len+i,1<<10,(size-i)>>10,file)<<10;this->len+=i;
	while(i<size)
		++i,jry_bl_string_add_char(this,fgetc(file));
	/*fprintf(stderr,"\n\nUse Time:%fs\n",((double)(clock()-__start)/CLOCKS_PER_SEC));*/
}
void jry_bl_string_add_file_end_by(jry_bl_string *this,FILE * file,unsigned char end)
{
	if(this==NULL||file==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	fseek(file,0L,SEEK_END);
	register unsigned char c;
	register jry_bl_string_size_type size=ftell(file),i=0;
	jry_bl_string_extend(this,size);
	fseek(file,0L,SEEK_SET);
	while(i<size&&((c=fgetc(file))!=end))
		++i,jry_bl_string_add_char(this,c);
}
#endif
#if JRY_BL_USE_STDARG==1
inline void	jry_bl_string_inits(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_string_init(va_arg(valist,jry_bl_string*));
	va_end(valist);	
}
inline void	jry_bl_string_frees(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_string_free(va_arg(valist,jry_bl_string*));
	va_end(valist);	
}
inline void	jry_bl_string_clears(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_string_clear(va_arg(valist,jry_bl_string*));
	va_end(valist);	
}
#if JRY_BL_USE_STDIO==1
inline void	jry_bl_string_views(FILE * file,int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_string_view_ex(va_arg(valist,jry_bl_string*),file,"views",i,jry_bl_view_default_tabs_num);
	va_end(valist);	
}
#endif
#endif
#if JRY_BL_VAR_ENABLE==1
#include "jry_bl_var.h"	
inline void				jry_bl_var_equal_string				(jry_bl_var *this,jry_bl_string *that){jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_string(this->data.p,that);}
inline void				jry_bl_var_equal_string_light		(jry_bl_var *this,jry_bl_string *that){jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_string_light(this->data.p,that);}
inline void				jry_bl_var_equal_string_light_move	(jry_bl_var *this,jry_bl_string *that){jry_bl_var_init_as(this,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_string_light_move(this->data.p,that);}
inline void				jry_bl_var_equal_string_pointer		(jry_bl_var *this,jry_bl_string *that){jry_bl_var_free(this);jry_bl_var_flag_pointer(this)=true;this->f.f.type=JRY_BL_VAR_TYPE_STRING,this->data.p=that;}
inline void				jry_bl_string_equal_var				(jry_bl_string *this,jry_bl_var *that){jry_bl_var_turn(that,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_string(this,that->data.p);}
inline void				jry_bl_string_equal_var_light		(jry_bl_string *this,jry_bl_var *that){jry_bl_var_turn(that,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_string_light(this,that->data.p);}
inline void				jry_bl_string_equal_var_light_move	(jry_bl_string *this,jry_bl_var *that){jry_bl_var_turn(that,JRY_BL_VAR_TYPE_STRING);jry_bl_string_equal_string_light_move(this,that->data.p);}
#endif
#if JRY_BL_LINK_LIST_ENABLE==1
#include "jry_bl_link_list.h"
jry_bl_string_size_type jry_bl_string_cut_start(jry_bl_string *this,jry_bl_link_list *list,char cut,jry_bl_string_size_type start)
{
	if(this==NULL||list==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_var var;jry_bl_var_init(&var);jry_bl_var_init_as(&var,JRY_BL_VAR_TYPE_STRING);
	for(;start<this->len;++start,jry_bl_link_list_add_var_light_move(list,&var),jry_bl_string_clear(jry_bl_var_get_string(&var)))
		for(;start<this->len&&this->s[start]!=cut;jry_bl_string_add_char(jry_bl_var_get_string(&var),this->s[start]),++start);
	jry_bl_var_free(&var);
	return (start);	
}
#endif
#endif