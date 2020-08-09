/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jwl_http.h"
#if JWL_HTTP_ENABLE==1
jwl_http_resh *jwl_http_resh_new()
{
	return jwl_http_resh_init(jbl_malloc(sizeof(jwl_http_resh)));
}
jwl_http_resh *jwl_http_resh_init(jwl_http_resh * this)
{
	if(!this)jbl_exception("NULL POINTER");		
	jbl_gc_init(this);
	jbl_gc_plus(this);
	this->status		=0;
	this->cache			=0;
	this->cache_max_age	=0;	
	this->charset		=0;	
	this->content_type	=NULL;
	this->etag			=NULL;
	this->filename		=NULL;
	this->v				=NULL;
	this->range.start	=0;
	this->range.end		=0;
	return this;
}
jwl_http_resh *jwl_http_resh_free(jwl_http_resh * this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this))
			jwl_http_resh_free((jwl_http_resh *)jbl_refer_pull(this));
		else
		{
			this->content_type	=jbl_string_free(this->content_type);
			this->etag			=jbl_string_free(this->etag);
			this->filename		=jbl_string_free(this->filename);
			this->v				=jbl_ht_free	(this->v);
		}
		if(jbl_gc_is_var(this))//因为该功能依赖jbl_ht,jbl_ht强制依赖var所以不用宏判断
			jbl_free((char*)this-sizeof(jbl_var));
		else
			jbl_free(this);
	}
	return NULL;	
	
}
inline jwl_http_resh *jwl_http_resh_copy(jwl_http_resh * this)
{
	jbl_gc_plus(this);
	return this;
}
inline jwl_http_resh *jwl_http_resh_extend(jwl_http_resh * this,jwl_http_resh **pthi)
{
	if(!this){this=jwl_http_resh_new();if(pthi)*pthi=this;return this;}
	jbl_reference *ref=NULL;jwl_http_resh *thi=jbl_refer_pull_keep_father(this,&ref);
	if(jbl_gc_is_pvar(thi))
	{
		jwl_http_resh *ttt=((jbl_reference*)thi)->ptr;
		if(jbl_gc_refcnt(thi)==1)
			((jbl_reference*)thi)->ptr=NULL,pl();
		jwl_http_resh_free(thi);
		thi=ttt;
	}	
	if((jbl_gc_refcnt(thi)<=1)){if(pthi)*pthi=thi;return this;}

	jwl_http_resh * tmp=jwl_http_resh_new();
	tmp->status			=this->status;
	tmp->cache			=this->cache;
	tmp->charset		=this->charset;
	tmp->cache_max_age	=this->cache_max_age;
	tmp->content_type	=jbl_string_copy(this->content_type);
	tmp->etag			=jbl_string_copy(this->etag);
	tmp->filename		=jbl_string_copy(this->filename);
	tmp->v				=jbl_ht_copy(this->v);
	tmp->range.start	=this->range.start;
	tmp->range.end		=this->range.end;
	
	jwl_http_resh_free(this);
	if(ref)		ref->ptr=tmp;
	else		this=tmp;
	if(pthi)	*pthi=tmp;	
	return this;
}
void jwl_http_resh_encode(jbl_stream *stream,jwl_http_resh *head,jbl_string_size_type size)
{
#define rn() jbl_stream_push_char(stream,'\r'),jbl_stream_push_char(stream,'\n')
	if(!head)return;
	head=jbl_refer_pull(head);
	jbl_stream_push_chars(stream,UC"HTTP/1.1 ");
	jbl_stream_push_uint(stream,head->status);
	switch(head->status)
	{
		case 200:	jbl_stream_push_chars(stream,UC" OK")				;break;
		case 206:	jbl_stream_push_chars(stream,UC" Partial Content")	;break;
		case 304:	jbl_stream_push_chars(stream,UC" Not Modified")		;break;
		case 404:	jbl_stream_push_chars(stream,UC" Not Found")		;break;
		case 416:	jbl_stream_push_chars(stream,UC" Range Not Satisfiable")		;break;
	}
	rn();
	jbl_stream_push_chars(stream,UC"Server: "JWL_HTTP_RESPONSE_SERVER_NAME)															,rn();
	if(head->status==206)
	{
		jbl_stream_push_chars(stream,UC"Content-Length: ")					,jbl_stream_push_uint(stream,(head->range.end?head->range.end:size)-head->range.start)	,rn();
		//注意，range是闭区间，而jwl,jbl要求左闭右开区间
		jbl_stream_push_chars(stream,UC"Content-Range: bytes ")				,jbl_stream_push_uint(stream,head->range.start)			,jbl_stream_push_char(stream,'-')	,jbl_stream_push_uint(stream,head->range.end?(head->range.end-1):0)	,jbl_stream_push_char(stream,'/')	,jbl_stream_push_uint(stream,size)	,rn();
	}
	else
		jbl_stream_push_chars(stream,UC"Content-Length: ")					,jbl_stream_push_uint(stream,size)						,rn();
	jbl_stream_push_chars(stream,UC"Accept-Ranges: bytes")																			,rn();
	if(head->cache==JWL_HTTP_CACHE_MAX_AGE)
	{
		jbl_stream_push_chars(stream,UC"Cache-Control: max-age=")	,jbl_stream_push_uint(stream,head->cache_max_age)				,rn();
		jbl_time * t=jbl_time_add_second(jbl_time_now(NULL),head->cache_max_age);
//		jbl_stream_push_chars(stream,UC"Date: ")	,jbl_time_to_string_format(t,stream,UC JBL_TIME_FORMAT_RFC1123)					,rn();
//		jbl_stream_push_chars(stream,UC"Expires: ")	,jbl_time_to_string_format(t,stream,UC JBL_TIME_FORMAT_RFC1123)					,rn();
		t=jbl_time_free(t);
	}
	else
	{
		jbl_stream_push_chars(stream,UC"Pragma: no-cache")																			,rn();
		jbl_stream_push_chars(stream,UC"Cache-Control: no-cache")																	,rn();
	}
	if(head->content_type)
	{
		jbl_stream_push_chars(stream,UC"Content-Type: ")					,jbl_stream_push_string(stream,head->content_type);
		char *charset[]={"","UTF-8"};
		if(head->charset)
			jbl_stream_push_chars(stream,UC"; charset=")					,jbl_stream_push_chars(stream,UC charset[head->charset]);
		rn();
	}
	if(head->etag)
		jbl_stream_push_chars(stream,UC"ETag: ")							,jbl_stream_push_string(stream,head->etag)				,rn();
	if(head->filename)
	{
		jbl_stream_push_chars(stream,UC"Content-Disposition: attachment; filename=\"");
		jbl_stream_push_string(stream,head->filename);
		jbl_stream_push_char(stream,'"');
		rn();
	}
	if(head->v)
		jbl_ht_foreach(head->v,i)
		{
			jbl_stream_push_string(stream,jbl_htk(i));
			jbl_stream_push_chars(stream,UC": ");
//			jbl_string_add_var(&stream,jbl_htv(i));
			rn();
		}
	
	
	
//	jbl_stream_push_chars(stream,UC"Connection: keep-alive");rn();
//	jbl_stream_push_chars(stream,UC"Keep-Alive: timeout=4");rn();
//	jbl_stream_push_chars(stream,UC"Proxy-Connection: keep-alive");rn();
	
	rn();
#undef rn
}

jwl_http_resh * jwl_http_resh_set_status			(jwl_http_resh * this,jbl_uint16 status)				{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->status=status;return this;}
jwl_http_resh * jwl_http_resh_set_cache				(jwl_http_resh * this,jbl_uint16 cache)					{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->cache=cache  ;return this;}
jwl_http_resh * jwl_http_resh_set_charset			(jwl_http_resh * this,jbl_uint16 charset)				{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->charset=charset  ;return this;}
jwl_http_resh * jwl_http_resh_set_cache_max_age		(jwl_http_resh * this,jbl_uint32 cache_max_age)			{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->cache_max_age=cache_max_age;return this;}
jwl_http_resh * jwl_http_resh_set_content_type		(jwl_http_resh * this,jbl_string * content_type)		{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);jbl_string_free(thi->content_type);thi->content_type=(content_type);return this;}
jwl_http_resh * jwl_http_resh_set_etag				(jwl_http_resh * this,jbl_string * etag)				{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);jbl_string_free(thi->etag);thi->etag=(etag);return this;}
jwl_http_resh *	jwl_http_resh_set_range				(jwl_http_resh * this,jbl_uint64 start,jbl_uint64 end)	{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->range.start=start;thi->range.end=end;return this;}
jwl_http_resh * jwl_http_resh_set_filename			(jwl_http_resh * this,jbl_string * filename)			{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);jbl_string_free(thi->filename);thi->filename=(filename);return this;}
//jwl_http_resh * jwl_http_resh_set					(jwl_http_resh * this,jbl_string * k, jbl_var *v);
#if JBL_STREAM_ENABLE==1
jwl_http_resh* jwl_http_resh_view_put(jwl_http_resh* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jwl_http_resh* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jwl_http_resh",line,varname,func,file))return this;
	jbl_stream_push_char(out,'\n');
	++tabs;
	char *charset[]={"","UTF-8"};
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"status       :");jbl_stream_push_uint(out,thi->status)					;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"filename     :");jbl_stream_push_string(out,thi->filename)				;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"cache        :");jbl_stream_push_uint(out,thi->cache)					;jbl_stream_push_char(out,'\t');
	if(thi->cache==JWL_HTTP_CACHE_MAX_AGE)
		jbl_stream_push_chars(out,UC"(max-age,"),jbl_stream_push_uint(out,thi->cache_max_age)			,jbl_stream_push_chars(out,UC")\n");
	else
		jbl_stream_push_chars(out,UC"(no-cache)\n");
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"content type :");jbl_stream_push_string(out,thi->content_type)			;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"charset      :");jbl_stream_push_chars(out,UC charset[thi->charset])	;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"etag         :");jbl_stream_push_string(out,thi->etag)					;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"range        :");jbl_stream_push_uint(out,thi->range.start)			;jbl_stream_push_char(out,'-' );	jbl_stream_push_uint(out,thi->range.end)			;jbl_stream_push_char(out,'\n');	
	if(thi->v)jbl_ht_foreach(thi->v,i)
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
		jbl_stream_push_char(out,'"'),jbl_stream_push_string(out,i->k),jbl_stream_push_char(out,'"');
		jbl_stream_push_char(out,':');
		jbl_var_view_put(i->v,out,0,tabs,0,NULL,NULL,NULL);
	}
	return this;
}
#endif




// GET / HTTP/1.1
// Host: test0.juruoyun.top:1217
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
// Accept-Encoding: gzip, deflate
// Accept-Language: zh-CN,zh;q=0.9
// Cache-Control: max-age=0
// Cookie: id=1; code=7FRWuMuuKnotkD2S4qqydnMwQrEJV4HykR0bmmjpfx41ChsWdw4540709f5c6c74b70576c38cf7f7608d55b974d6625187f24fe780700e42bd57
// Upgrade-Insecure-Requests: 1


// HTTP/1.1 200 OK
// Content-Length: 10054
// Cache-Control: no-store, no-cache, must-revalidate
// Connection: keep-alive
// Content-Encoding: gzip
// Content-Type: text/html; charset=UTF-8
// Date: Thu, 11 Jun 2020 15:10:18 GMT
// Expires: Thu, 19 Nov 1981 08:52:00 GMT
// Keep-Alive: timeout=4
// Pragma: no-cache
// Proxy-Connection: keep-alive
// Server: Apache/2.4.29 (Ubuntu)
// Vary: Accept-Encoding

jwl_http_reqh *jwl_http_reqh_new()
{
	return jwl_http_reqh_init(jbl_malloc(sizeof(jwl_http_reqh)));
}
jwl_http_reqh *jwl_http_reqh_init(jwl_http_reqh * this)
{
	if(!this)jbl_exception("NULL POINTER");		
	jbl_gc_init(this);
	jbl_gc_plus(this);
	this->method		=0;
	this->protocol		=0;
	this->connection	=0;
	this->cache			=0;
	this->cache_max_age	=0;	
	this->range.start	=0;	
	this->range.end		=0;	
//jbl_string	
	this->url		=NULL;
	this->host		=NULL;
	this->ua		=NULL;
	this->referer	=NULL;
	this->etag		=NULL;
//jbl_ht
	this->cookie	=NULL;
	this->v			=NULL;
	return this;
}
jwl_http_reqh *jwl_http_reqh_free(jwl_http_reqh * this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this))
			jwl_http_reqh_free((jwl_http_reqh *)jbl_refer_pull(this));
		else
		{
			this->url		=jbl_string_free(this->url);
			this->host		=jbl_string_free(this->host);
			this->ua		=jbl_string_free(this->ua);
			this->referer	=jbl_string_free(this->referer);
			this->etag		=jbl_string_free(this->etag);
			
			this->v			=jbl_ht_free	(this->v);
			this->cookie	=jbl_ht_free	(this->v);
		}
		if(jbl_gc_is_var(this))//因为该功能依赖jbl_ht,jbl_ht强制依赖var所以不用宏判断
			jbl_free((char*)this-sizeof(jbl_var));
		else
			jbl_free(this);
	}
	return NULL;	
	
}
inline jwl_http_reqh *jwl_http_reqh_extend(jwl_http_reqh * this,jwl_http_reqh **pthi)
{
	if(!this){this=jwl_http_reqh_new();if(pthi)*pthi=this;return this;}
	jbl_reference *ref=NULL;jwl_http_reqh *thi=jbl_refer_pull_keep_father(this,&ref);
	if(jbl_gc_is_pvar(thi))
	{
		jwl_http_reqh *ttt=((jbl_reference*)thi)->ptr;
		if(jbl_gc_refcnt(thi)==1)
			((jbl_reference*)thi)->ptr=NULL,pl();
		jwl_http_reqh_free(thi);
		thi=ttt;
	}	
	if((jbl_gc_refcnt(thi)<=1)){if(pthi)*pthi=thi;return this;}

	jwl_http_reqh * tmp=jwl_http_reqh_new();
	tmp->method			=this->method;
	tmp->protocol		=this->protocol;
	tmp->connection		=this->connection;
	tmp->cache			=this->cache;
	tmp->cache_max_age	=this->cache_max_age;
	
	tmp->url		=jbl_string_copy(this->url);
	tmp->host		=jbl_string_copy(this->host);
	tmp->ua			=jbl_string_copy(this->ua);
	tmp->referer	=jbl_string_copy(this->referer);
	tmp->etag		=jbl_string_copy(this->etag);
	
	tmp->cookie		=jbl_ht_copy(this->cookie);
	tmp->v			=jbl_ht_copy(this->v);
	
	tmp->range.start	=this->range.start;
	tmp->range.end		=this->range.end;	
	
	jwl_http_reqh_free(this);
	if(ref)		ref->ptr=tmp;
	else		this=tmp;
	if(pthi)	*pthi=tmp;	
	return this;
}
jwl_http_key __jwl_http_rqhd_scanner(unsigned char * YYCURSOR,unsigned char * YYLIMIT,unsigned char **addr)
{
	unsigned char *		YYMARKER;
	#define YYCTYPE		unsigned char
	#define YYFILL(n)	return JWL_HTTP_KEY_END;
    /*!re2c
		[^]								{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_UNDEFINED;}	
		"\r\n\r\n"						{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_END;}
		"Host: "						{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_HOST;}
		"User-Agent: "					{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_UA;}
		"Accept: "						{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_ACCEPT;}
		"Accept-Encoding: "				{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_ACCEPT_ENCODING;}
		"Accept-Language: "				{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_ACCEPT_LANGUAGE;}
		"Cache-Control: "				{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_CACHE_CONTROL;}
		"Cookie: "						{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_CACHE_CONTROL;}
		"Referer: "						{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_REFERER;}
		"Etag: "						{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_ETAG;}
		"If-None-Match: "				{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_ETAG;}
		"Range: bytes="					{if(addr)*addr=YYCURSOR;return JWL_HTTP_KEY_RANGE_BYTES;}
     */
	#undef YYCTYPE
	#undef YYFILL
}
jwl_http_method __jwl_http_method_scanner(unsigned char * YYCURSOR,unsigned char * YYLIMIT,unsigned char **addr)
{
	unsigned char *		YYMARKER;
	#define YYCTYPE		unsigned char
	#define YYFILL(n)	return 0;
    /*!re2c
		[^]		{if(addr)*addr=YYCURSOR;return JWL_HTTP_METHOD_UNKNOW;}	
		"POST"	{if(addr)*addr=YYCURSOR;return JWL_HTTP_METHOD_POST;}
		"GET"	{if(addr)*addr=YYCURSOR;return JWL_HTTP_METHOD_GET;}
     */
	#undef YYCTYPE
	#undef YYFILL
}
jwl_http_protocol __jwl_http_protocol_scanner(unsigned char * YYCURSOR,unsigned char * YYLIMIT,unsigned char **addr)
{
	unsigned char *		YYMARKER;
	#define YYCTYPE		unsigned char
	#define YYFILL(n)	return 0;
    /*!re2c
		[^]			{if(addr)*addr=YYCURSOR;return JWL_HTTP_PROTOCOL_UNKNOW;}	
		"HTTP/1.1"	{if(addr)*addr=YYCURSOR;return JWL_HTTP_PROTOCOL_HTTP;}
     */
	#undef YYCTYPE
	#undef YYFILL
}

jwl_http_reqh* jwl_http_reqh_decode(jbl_string *buf,jbl_string_size_type *start)
{	
	if(!buf)return NULL;
	buf=jbl_refer_pull(buf);
	jbl_string_size_type i=0;if(start)i=*start;
	unsigned char *addr;
	jwl_http_reqh * reqh=jwl_http_reqh_new();
	
	reqh->method=__jwl_http_method_scanner(buf->s+i,buf->s+buf->len,&addr);i=addr-buf->s;
	
	for(++i;buf->s[i]!=' ';reqh->url=jbl_string_add_char(reqh->url,buf->s[i]),++i);++i;
	
	reqh->protocol=__jwl_http_protocol_scanner(buf->s+i,buf->s+buf->len,&addr);i=addr-buf->s;

	for(;i<buf->len;++i)
	{
		jwl_http_key key=__jwl_http_rqhd_scanner(buf->s+i,buf->s+buf->len,&addr);
		switch(key)
		{
			case JWL_HTTP_KEY_END				:goto finish;																																							;break;
			case JWL_HTTP_KEY_UA				:for(i=addr-buf->s;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n'	;reqh->ua		=jbl_string_add_char(reqh->ua		,buf->s[i]),++i)	;--reqh->ua->len			;break;
			case JWL_HTTP_KEY_HOST				:for(i=addr-buf->s;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n'	;reqh->host		=jbl_string_add_char(reqh->host		,buf->s[i]),++i)	;--reqh->host->len			;break;
			case JWL_HTTP_KEY_REFERER			:for(i=addr-buf->s;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n'	;reqh->referer	=jbl_string_add_char(reqh->referer	,buf->s[i]),++i)	;--reqh->referer->len		;break;
			case JWL_HTTP_KEY_ETAG				:for(i=addr-buf->s;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n'	;reqh->etag		=jbl_string_add_char(reqh->etag		,buf->s[i]),++i)	;--reqh->etag->len			;break;
			case JWL_HTTP_KEY_RANGE_BYTES		:reqh->range.start=jbl_string_get_uint_start_end(buf,&i,'-')		;reqh->range.end=jbl_string_get_uint_start_end(buf,&i,'\r');reqh->range.end?(reqh->range.end+=1):0	;break;//注意，range是闭区间，而jwl,jbl要求左闭右开区间
			
			case JWL_HTTP_KEY_COOKIE			:break;
			case JWL_HTTP_KEY_ACCEPT			:break;
			case JWL_HTTP_KEY_ACCEPT_ENCODING	:break;
			case JWL_HTTP_KEY_ACCEPT_LANGUAGE	:break;
			case JWL_HTTP_KEY_CACHE_CONTROL		:break;
			
			
			
			default:break;
		}
		
	}
finish:;
	if(start)*start=i;
	
	
	return reqh;
}








#if JBL_STREAM_ENABLE==1
jwl_http_reqh* jwl_http_reqh_view_put(jwl_http_reqh* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jwl_http_reqh* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jwl_http_reqh",line,varname,func,file))return this;
	char * method[]={"unknow","get","post"};
	char * protocol[]={"unknow","HTTP"};
	++tabs;
	jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"method       :");jbl_stream_push_chars(out,UC method[thi->method])		;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"url          :");jbl_stream_push_string(out,thi->url)					;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"protocol     :");jbl_stream_push_chars(out,UC protocol[thi->protocol])	;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"user agent   :");jbl_stream_push_string(out,thi->ua)					;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"host         :");jbl_stream_push_string(out,thi->host)					;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"referer      :");jbl_stream_push_string(out,thi->referer)				;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"etag         :");jbl_stream_push_string(out,thi->etag)					;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"range        :");jbl_stream_push_uint(out,thi->range.start)			;jbl_stream_push_char(out,'-' );	jbl_stream_push_uint(out,thi->range.end)			;jbl_stream_push_char(out,'\n');	
	if(thi->v)jbl_ht_foreach(thi->v,i)
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
		jbl_stream_push_char(out,'"'),jbl_stream_push_string(out,i->k),jbl_stream_push_char(out,'"');
		jbl_stream_push_char(out,':');
		jbl_var_view_put(i->v,out,0,tabs,0,NULL,NULL,NULL);
	}
	return this;
/*
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"cache        :"),jbl_stream_push_uint(out,this->cache),jbl_stream_push_char(out,'\t');
	if(this->cache==JWL_HTTP_CACHE_MAX_AGE)
		jbl_stream_push_chars(out,UC"(max-age,"),jbl_stream_push_uint(out,this->cache_max_age),jbl_stream_push_chars(out,UC")\n");
	else
		jbl_stream_push_chars(out,UC"(no-cache)\n");
	jbl_stream_push_chars(out,UC"content type :"),jbl_stream_push_string(out,this->content_type),jbl_stream_push_char(out,'\n');
*/	
}
#endif
#endif
