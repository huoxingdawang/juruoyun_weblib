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
#include "jbl_ht.h"
#include "jbl_file.h"
#include "jbl_malloc.h"
#include "jbl_ll.h"
#include "jbl_string.h"

JBL_INLINE jbl_var * jbl_var_set_operators(jbl_var * this,const jbl_var_operators *ops)
{
	if(!jbl_gc_is_var((jbl_var_data*)this))
		jbl_exception("VAR TYPE ERROR");
	((jbl_var*)(((char*)this)-(sizeof(jbl_var))))->ops=ops;
	return this;
}
JBL_INLINE const jbl_var_operators * jbl_var_get_operators(jbl_var * this)
{
	if(!this)return NULL;
	if(!jbl_gc_is_var((jbl_var_data*)this))
		jbl_exception("VAR TYPE ERROR");
	return ((jbl_var*)(((char*)this)-(sizeof(jbl_var))))->ops;
}


JBL_INLINE jbl_var * jbl_var_free(jbl_var * this){if(!this)return NULL;void*(*free)(void*)=jbl_var_get_operators(this)->free;if(free)return free(this);return NULL;}
JBL_INLINE jbl_var * jbl_var_copy(jbl_var * this){if(!this)return NULL;void*(*copy)(void*)=jbl_var_get_operators(this)->copy;if(copy)return copy(this);return NULL;}
JBL_INLINE char      jbl_var_space_ship(jbl_var * this,jbl_var * that){if(this==that){return 0;}if(this==NULL){return -1;}if(that==NULL){return 1;}char(*space_ship)(const void*,const void*)=jbl_var_get_operators(this)->space_ship;if(space_ship)return space_ship(this,that);return -1;}
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

#if JBL_STRING_ENABLE==1
#if JBL_JSON_ENABLE==1
jbl_string * jbl_var_json_encode(jbl_var * this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(!this)
	{
		if(out==NULL)out=jbl_string_new();
		out=jbl_refer_pull(out);
		if(format&&tabs>=0)for(jbl_int16 i=0;i<tabs;out=jbl_string_add_char(out,'\t'),++i);	
		return jbl_string_add_chars_length(out,(unsigned char *)"null",4);
	}
	jbl_string*(*json_encode)(const void*,jbl_string *,jbl_uint8,jbl_uint32)=jbl_var_get_operators(this)->json_encode;
	if(json_encode)return json_encode(this,out,format,tabs);
	return out;
}
jbl_var* jbl_var_json_decode(jbl_var *this,jbl_string* in,jbl_string_size_type *start)
{
	in=jbl_refer_pull(in);
	jbl_string_size_type i=start?(*start):0,n=in->len;
	for(;i<n&&in->s[i]<=32;++i);
	jbl_var *v1=NULL,*v2=NULL;
	switch(in->s[i])
	{
#if JBL_STRING_ENABLE==1
		case '"':
			start?(*start=i):0;start=start?start:&i;
			v2=(jbl_var *)jbl_string_json_decode(jbl_Vstring(v1=jbl_Vstring_new()),in,start);
			if(!v2)v1=v2=jbl_var_free(v1);
			return v2;
			break;
#endif
#if JBL_LL_ENABLE==1
		case '[':
			start?(*start=i):0;start=start?start:&i;
			v2=(jbl_var *)jbl_ll_json_decode(jbl_Vll(v1=jbl_Vll_new()),in,start);
			if(!v2)v1=v2=jbl_var_free(v1);
			return v2;
			break;
#endif
#if JBL_HT_ENABLE==1
		case '{':
			start?(*start=i):0;start=start?start:&i;
			v2=(jbl_var *)jbl_ht_json_decode(jbl_Vht(v1=jbl_Vht_new()),in,start);
			if(!v2)v1=v2=jbl_var_free(v1);
			return v2;
			break;
#endif
		case 'n':
			if((i+3)<n&&in->s[i+1]=='u'&&in->s[i+2]=='l'&&in->s[i+3]=='l')
			{
				i+=4;
				start?(*start=i):0;
				return jbl_Vnull_new();
			}
			else
				return NULL;
			break;
		case 't':
			if((i+3)<n&&in->s[i+1]=='r'&&in->s[i+2]=='u'&&in->s[i+3]=='e')
			{
				i+=4;
				start?(*start=i):0;
				return jbl_Vtrue_new();
			}
			else
				return NULL;
			break;
		case 'f':
			if((i+4)<n&&in->s[i+1]=='a'&&in->s[i+2]=='l'&&in->s[i+3]=='s'&&in->s[i+4]=='e')
			{
				i+=5;
				start?(*start=i):0;
				return jbl_Vfalse_new();
			}
			else
				return NULL;
			break;			
		case '0':
			if((i+1)<n&&(in->s[i+1]=='x'||in->s[i+1]=='X'))
			{
				i+=2;
				start?(*start=i):0;start=start?start:&i;
				return jbl_Vuint_set(NULL,jbl_string_get_hex_start(in,start));
			}
		case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':case '8':case '9':case '-':
			{
				start?(*start=i):0;start=start?start:&i;
				return jbl_string_get_number_start(in,start);				
			}
			break;
	}
	return NULL;
	
}
#endif
#endif
#if JBL_STREAM_ENABLE==1
jbl_var* jbl_var_view_put(jbl_var* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	if(!this)goto print;
	void *	(*view_put)(void*,jbl_stream *,jbl_uint8,jbl_uint32,jbl_uint32,unsigned char*,unsigned char *,unsigned char *)=jbl_var_get_operators(this)->view_put;
	if(view_put)view_put(this,out,format,tabs,line,varname,func,file);
	else goto print;
	return this;
print:;
	jbl_stream_view_put_format(this,out,format,tabs,UC"unprintable var",line,varname,func,file);jbl_stream_push_char(out,'\n');
	return this;
}
#if JBL_JSON_ENABLE==1
void jbl_var_json_put(jbl_var * this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(out==NULL)jbl_exception("NULL POINTER");
	if(!this){out=jbl_refer_pull(out);if(format&&tabs>=0)for(jbl_int16 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);jbl_stream_push_chars(out,UC"null");return;}
	void(*json_put)(const void*,jbl_stream *,jbl_uint8,jbl_uint32)=jbl_var_get_operators(this)->json_put;
	if(json_put)json_put(this,out,format,tabs);
}
#endif
#endif


jbl_var_scanner_key jbl_var_scanner(unsigned char * YYCURSOR,unsigned char **addr)
{
//	unsigned char * YYMARKER;
	#define YYCTYPE		unsigned char
	/*!re2c
re2c:yyfill:enable = 0;
		[^]		{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_UNDEFINED;}	
		[\x00]	{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_END;}	
		"%d"	{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_INT;}	
		"%f"	{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_DOUBLE;}
		"%s"	{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_CHARS;}
		"%c"	{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_CHAR;}
		"%X"	{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_HEX;}
		"%v"	{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_VAR;}
		"%j"	{if(addr)*addr=YYCURSOR;return JBL_VAR_SCANNER_KEY_JSON;}
     */
	#undef YYCTYPE
	#undef YYFILL	
}



#endif