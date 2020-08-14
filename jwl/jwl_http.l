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
jwl_http_head *jwl_http_head_new()
{
	return jwl_http_head_init(jbl_malloc(sizeof(jwl_http_head)));
}
jwl_http_head *jwl_http_head_init(jwl_http_head * this)
{
	if(!this)jbl_exception("NULL POINTER");		
	jbl_gc_init(this);
	jbl_gc_plus(this);
	jwl_http_head_set_response(this);
	this->status		=0;
	this->charset		=0;	

	this->method		=0;
	this->protocol		=0;
	this->connection	=0;
	
	this->cache			=0;
	this->cache_max_age	=0;	
	this->range.start	=0;	
	this->range.end		=0;	
	this->etag			=NULL;

	this->cond			=0;	
//jbl_string
	this->content_type	=NULL;
	this->filename		=NULL;

	this->url			=NULL;
	this->host			=NULL;
	this->ua			=NULL;
	this->referer		=NULL;
//jbl_ht
	this->cookie		=NULL;
	this->v				=NULL;	
	return this;
}
jwl_http_head *jwl_http_head_free(jwl_http_head * this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this))
			jwl_http_head_free((jwl_http_head*)(((jbl_reference*)this)->ptr));
		else
		{
			this->etag			=jbl_string_free(this->etag);
			
			this->content_type	=jbl_string_free(this->content_type);
			this->filename		=jbl_string_free(this->filename);
			
			this->url			=jbl_string_free(this->url);
			this->host			=jbl_string_free(this->host);
			this->ua			=jbl_string_free(this->ua);
			this->referer		=jbl_string_free(this->referer);
			
			this->v				=jbl_ht_free	(this->v);
			this->cookie		=jbl_ht_free	(this->v);
		}
		if(jbl_gc_is_var(this))//因为该功能依赖jbl_ht,jbl_ht强制依赖var所以不用宏判断
			jbl_free((char*)this-sizeof(jbl_var));
		else
			jbl_free(this);
	}
	return NULL;	
	
}
inline jwl_http_head *jwl_http_head_copy(jwl_http_head * this)
{
	jbl_gc_plus(this);
	return this;
}
inline jwl_http_head *jwl_http_head_extend(jwl_http_head * this,jwl_http_head **pthi)
{
	if(!this){this=jwl_http_head_new();if(pthi)*pthi=this;return this;}
	jbl_reference *ref=NULL;jwl_http_head *thi=jbl_refer_pull_keep_father(this,&ref);
	if(jbl_gc_is_pvar(thi))
	{
		jwl_http_head *ttt=((jbl_reference*)thi)->ptr;
		if(jbl_gc_refcnt(thi)==1)
			((jbl_reference*)thi)->ptr=NULL,
		jwl_http_head_free(thi);
		thi=ttt;
	}	
	if((jbl_gc_refcnt(thi)<=1)){if(pthi)*pthi=thi;return this;}

	jwl_http_head * tmp	=jwl_http_head_new();
	
	tmp->status			=this->status;
	tmp->charset		=this->charset;
	
	tmp->method			=this->method;
	tmp->protocol		=this->protocol;
	tmp->connection		=this->connection;
	
	tmp->cache			=this->cache;
	tmp->cache_max_age	=this->cache_max_age;
	tmp->range.start	=this->range.start;
	tmp->range.end		=this->range.end;
	tmp->etag			=jbl_string_copy(this->etag);
	
	
	tmp->content_type	=jbl_string_copy(this->content_type);
	tmp->filename		=jbl_string_copy(this->filename);
	
	
	tmp->url			=jbl_string_copy(this->url);
	tmp->host			=jbl_string_copy(this->host);
	tmp->ua				=jbl_string_copy(this->ua);
	tmp->referer		=jbl_string_copy(this->referer);
	
	tmp->cookie			=jbl_ht_copy(this->cookie);
	tmp->v				=jbl_ht_copy(this->v);
	
	jwl_http_head_free(this);
	if(ref)		ref->ptr=tmp;
	else		this=tmp;
	if(pthi)	*pthi=tmp;	
	return this;
}
jwl_http_head * jwl_http_head_set_status			(jwl_http_head * this,jbl_uint16 status)				{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->status		=status			;return this;}
jwl_http_head * jwl_http_head_set_connection		(jwl_http_head * this,jbl_uint16 connection)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->connection	=connection		;return this;}
jwl_http_head * jwl_http_head_set_cache				(jwl_http_head * this,jbl_uint16 cache)					{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->cache			=cache  		;return this;}
jwl_http_head * jwl_http_head_set_charset			(jwl_http_head * this,jbl_uint16 charset)				{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->charset		=charset  		;return this;}
jwl_http_head * jwl_http_head_set_cache_max_age		(jwl_http_head * this,jbl_uint32 cache_max_age)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->cache_max_age	=cache_max_age	;return this;}
jwl_http_head * jwl_http_head_set_content_type		(jwl_http_head * this,jbl_string * content_type)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->content_type)	;thi->content_type	=jbl_string_copy(content_type)	;return this;}
jwl_http_head * jwl_http_head_set_etag				(jwl_http_head * this,jbl_string * etag)				{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->etag)			;thi->etag			=jbl_string_copy(etag)			;return this;}
jwl_http_head *	jwl_http_head_set_range				(jwl_http_head * this,jbl_uint64 start,jbl_uint64 end)	{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->range.start=start;thi->range.end=end;return this;}
jwl_http_head * jwl_http_head_set_filename			(jwl_http_head * this,jbl_string * filename)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->filename)		;thi->filename		=jbl_string_copy(filename)		;return this;}
//jwl_http_head * jwl_http_head_set					(jwl_http_head * this,jbl_string * k, jbl_var *v);



void jwl_http_head_encode(jbl_stream *stream,jwl_http_head *head,jbl_string_size_type size)
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
	if(head->connection)
	{
		char * connection[]={"","keep-alive","close","upgrade"};
		jbl_stream_push_chars(stream,UC"Connection: ")						,jbl_stream_push_chars(stream,UC connection[head->connection])	,rn();
//		jbl_stream_push_chars(stream,UC"Keep-Alive: timeout=5, max=1000"),rn();
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
	
	
	
//	jbl_stream_push_chars(stream,UC"Keep-Alive: timeout=4");rn();
//	jbl_stream_push_chars(stream,UC"Proxy-Connection: keep-alive");rn();
	
	rn();
#undef rn
}
/*!types:re2c */

jwl_http_head* jwl_http_head_decode(jbl_string *buf,jbl_string_size_type *start)
{	
	if(!buf)return NULL;
	buf=jbl_refer_pull(buf);
	if(!buf->len)return NULL;
	jwl_http_head * head=jwl_http_head_new();
	jwl_http_head_set_request(head);
	unsigned char *		YYMARKER;
	unsigned char *		YYCURSOR=buf->s+(start?(*start):0);
	#define YYCTYPE		unsigned char
/*!re2c
	re2c:yyfill:enable					=0;
	re2c:condenumprefix					=jwl_http_scanner_condition_;
	re2c:define:YYCONDTYPE				= jwl_http_scanner_condition;
	re2c:define:YYSETCONDITION			= "head->cond = ";
	re2c:define:YYSETCONDITION@cond		= #;
	re2c:define:YYGETCONDITION			= head->cond;
	re2c:define:YYGETCONDITION:naked	= 1;
	<> :=> method
	<method> 		"POST"								{head->method=JWL_HTTP_METHOD_POST														;goto yyc_method;}
	<method> 		"GET"								{head->method=JWL_HTTP_METHOD_GET														;goto yyc_method;}
	<method>		" " 				=> url			{head->url				=jbl_string_free    (head->url)									;goto yyc_url;}
	
	<url>			[]									{head->url				=jbl_string_add_char(head->url,*YYCURSOR)						;++YYCURSOR;goto yyc_url;}
	<url>			" " 				=> protocol		{																						;goto yyc_protocol;}
	
	<protocol>		"HTTP/1.1"							{head->protocol			=JWL_HTTP_PROTOCOL_HTTP_1_1										;goto yyc_protocol;}
	<protocol>		"\r\n"				=> head_body	{																						;goto yyc_head_body;}
	
	<head_body>		"Host: "			=> head_host	{head->host				=jbl_string_free    (head->host)								;goto yyc_head_host;}
	<head_host>		[]									{head->host				=jbl_string_add_char(head->host,*YYCURSOR)						;++YYCURSOR;goto yyc_head_host;}
	<head_host>		"\r\n"				=> head_body	{																						;goto yyc_head_body;}

	<head_body>		"User-Agent: "		=> head_ua		{head->ua				=jbl_string_free    (head->ua)									;goto yyc_head_ua;}
	<head_ua>		[]									{head->ua				=jbl_string_add_char(head->ua,*YYCURSOR)						;++YYCURSOR;goto yyc_head_ua;}
	<head_ua>		"\r\n"				=> head_body	{																						;goto yyc_head_body;}
	
	<head_body>		"Referer: "			=> head_refer	{head->referer			=jbl_string_free    (head->referer)								;goto yyc_head_refer;}
	<head_refer>	[]									{head->referer			=jbl_string_add_char(head->referer,*YYCURSOR)					;++YYCURSOR;goto yyc_head_refer;}
	<head_refer>	"\r\n"				=> head_body	{																						;goto yyc_head_body;}

	<head_body>		"Etag: "			=> head_etag	{head->etag				=jbl_string_free    (head->etag)								;goto yyc_head_etag;}
	<head_body>		"If-None-Match: "	=> head_etag	{head->etag				=jbl_string_free    (head->etag)								;goto yyc_head_etag;}
	<head_etag>		[]									{head->etag				=jbl_string_add_char(head->etag,*YYCURSOR)						;++YYCURSOR;goto yyc_head_etag;}
	<head_etag>		"\r\n"				=> head_body	{																						;goto yyc_head_body;}
	
	<head_body>		"Connection: "		=> head_conn	{																						;goto yyc_head_conn;}
	<head_conn>		"keep-alive"						{head->connection		=JWL_HTTP_CONNECTION_KEEP_ALIVE									;goto yyc_head_conn;}
	<head_conn>		"close"								{head->connection		=JWL_HTTP_CONNECTION_CLOSE										;goto yyc_head_conn;}
	<head_conn>		"\r\n"				=> head_body	{																						;goto yyc_head_body;}
	
	<head_body>		"Range: bytes="		=> head_range_s	{head->range.start		=0																;goto yyc_head_range_s;}
	<head_range_s>	"-"					=> head_range_e	{head->range.end		=0																;goto yyc_head_range_e;}
	<head_range_s>	[0-9]								{head->range.start		=(head->range.start<<3)+(head->range.start<<1)+*(YYCURSOR-1)-'0';goto yyc_head_range_s;}
	<head_range_s>	"\r\n"				=> head_body	{																						;goto yyc_head_body;}
	<head_range_e>	[0-9]								{head->range.end		=(head->range.end<<3)+(head->range.end<<1)+*(YYCURSOR-1)-'0'	;goto yyc_head_range_e;}
	<head_range_e>	"\r\n"				=> head_body	{head->range.end		=(head->range.end)?(head->range.end+1):0						;goto yyc_head_body;}
	
	<head_body>		"Accept: "			=> head_accept	{																						;goto yyc_head_accept;}
	<head_accept>	[]									{																						;++YYCURSOR;goto yyc_head_accept;}
	<head_accept>	"\r\n"				=> head_body	{																						;goto yyc_head_body;}

	<head_body>		"Accept-Encoding: "	=> head_acen	{																						;goto yyc_head_acen;}
	<head_acen>		[]									{																						;++YYCURSOR;goto yyc_head_acen;}
	<head_acen>		"\r\n"				=> head_body	{																						;goto yyc_head_body;}

	<head_body>		"Accept-Language: "	=> head_aclan	{																						;goto yyc_head_aclan;}
	<head_aclan>	[]									{																						;++YYCURSOR;goto yyc_head_aclan;}
	<head_aclan>	"\r\n"				=> head_body	{																						;goto yyc_head_body;}

	<head_body>		"Cookie: "			=> head_cookie	{																						;goto yyc_head_cookie;}
	<head_cookie>	[]									{																						;++YYCURSOR;goto yyc_head_cookie;}
	<head_cookie>	"\r\n"				=> head_body	{																						;goto yyc_head_body;}

	<head_body>		"Cache-Control: "	=> head_checcon	{																						;goto yyc_head_checcon;}
	<head_checcon>	[]									{																						;++YYCURSOR;goto yyc_head_checcon;}
	<head_checcon>	"\r\n"				=> head_body	{																						;goto yyc_head_body;}




	
	<head_body>		[]									{																						;++YYCURSOR;goto yyc_head_body;}
	<head_body>		"\r\n\r\n"							{if(start)*start=YYCURSOR-buf->s;return head;}
	<head_body>		[\x00]								{if(start)*start=YYCURSOR-buf->s;return head;}
		
 */
	#undef YYCTYPE	


	return head;
}


#if JBL_STREAM_ENABLE==1
jwl_http_head* jwl_http_head_view_put(jwl_http_head* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jwl_http_head* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jwl_http_head",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_char(out,'\n');
	++tabs;
	char * charset		[]={"unknow","UTF-8"};
	char * method		[]={"unknow","get","post"};
	char * protocol		[]={"unknow","HTTP/1.1"};
	char * connection	[]={"unknow","keep alive","close","upgrade"};
	char * cache		[]={"unknow","no","max age,"};
	char * type			[]={"response","request"};
	if(1)
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"type         :");jbl_stream_push_chars(out,UC type			[jwl_http_head_is_request(this)?1:0])	;jbl_stream_push_char(out,'\n');
	if(jwl_http_head_is_response(this))
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"status       :");jbl_stream_push_uint(out,thi->status)								;jbl_stream_push_char(out,'\n');
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"charset      :");jbl_stream_push_chars(out,UC charset		[thi->charset])			;jbl_stream_push_char(out,'\n');
	}
	if(jwl_http_head_is_request(this))
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"method       :");jbl_stream_push_chars(out,UC method		[thi->method])			;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"protocol     :");jbl_stream_push_chars(out,UC protocol		[thi->protocol])		;jbl_stream_push_char(out,'\n');	
	}
	if(1)
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"connection   :");jbl_stream_push_chars(out,UC connection	[thi->connection])		;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"cache        :");jbl_stream_push_chars(out,UC cache		[thi->cache])			;if(thi->cache==JWL_HTTP_CACHE_MAX_AGE){jbl_stream_push_uint(out,thi->cache_max_age);}	;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"range        :");jbl_stream_push_uint(out,thi->range.start)						;jbl_stream_push_char(out,'-' );	jbl_stream_push_uint(out,thi->range.end)			;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"etag         :");jbl_stream_push_string(out,thi->etag)								;jbl_stream_push_char(out,'\n');
	}
	if(jwl_http_head_is_response(this))
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"content type :");jbl_stream_push_string(out,thi->content_type)						;jbl_stream_push_char(out,'\n');
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"filename     :");jbl_stream_push_string(out,thi->filename)							;jbl_stream_push_char(out,'\n');
	}
	if(jwl_http_head_is_request(this))
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"url          :");jbl_stream_push_string(out,thi->url)								;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"host         :");jbl_stream_push_string(out,thi->host)								;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"user agent   :");jbl_stream_push_string(out,thi->ua)								;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"referer      :");jbl_stream_push_string(out,thi->referer)							;jbl_stream_push_char(out,'\n');	
	}	
	if(thi->v)
		jbl_ht_foreach(thi->v,i)
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
