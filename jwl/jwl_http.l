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
	this->content_type	=0;
	
	this->method		=0;
	this->protocol		=0;
	
	this->connection	=0;
	this->cache			=0;
	this->upgrade		=0;
	this->cache_max_age	=0;	
	this->range.start	=0;	
	this->range.end		=0;	
	this->etag			=NULL;

//jbl_string
	this->filename		=NULL;

	this->url			=NULL;
	this->host			=NULL;
	this->ua			=NULL;
	this->referer		=NULL;
//jbl_ht
	this->cookie		=NULL;
	this->v				=NULL;	

	this->cond			=0;	
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
	tmp->content_type	=this->content_type;
	
	tmp->method			=this->method;
	tmp->protocol		=this->protocol;
	tmp->connection		=this->connection;
	
	tmp->cache			=this->cache;
	tmp->upgrade		=this->upgrade;
	tmp->cache_max_age	=this->cache_max_age;
	tmp->range.start	=this->range.start;
	tmp->range.end		=this->range.end;
	tmp->etag			=jbl_string_copy(this->etag);
	
	
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
jwl_http_head *		jwl_http_head_set_status		(jwl_http_head * this,jbl_uint32			status)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->status		=status			;return this;}
jwl_http_head *		jwl_http_head_set_charset		(jwl_http_head * this,jwl_http_charset		charset)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->charset		=charset  		;return this;}
jwl_http_head *		jwl_http_head_set_content_type	(jwl_http_head * this,jbl_file_ct			content_type)	{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->content_type	=content_type	;return this;}
jwl_http_head *		jwl_http_head_set_method		(jwl_http_head * this,jwl_http_method		method)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->method		=method  		;return this;}
jwl_http_head *		jwl_http_head_set_protocol		(jwl_http_head * this,jwl_http_protocol		protocol)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->protocol		=protocol  		;return this;}
jwl_http_head *		jwl_http_head_set_connection	(jwl_http_head * this,jwl_http_connection	connection)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->connection	=connection		;return this;}
jwl_http_head *		jwl_http_head_set_cache			(jwl_http_head * this,jwl_http_cache		cache)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->cache			=cache  		;return this;}
jwl_http_head *		jwl_http_head_set_upgrade		(jwl_http_head * this,jwl_http_upgrade		upgrade)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->upgrade		=upgrade  		;return this;}
jwl_http_head *		jwl_http_head_set_cache_max_age	(jwl_http_head * this,jbl_uint32			cache_max_age)	{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->cache_max_age	=cache_max_age	;return this;}
jwl_http_head *		jwl_http_head_set_range			(jwl_http_head * this,jwl_http_head_range	range)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->range			=range			;return this;}
jwl_http_head *		jwl_http_head_set_etag			(jwl_http_head * this,jbl_string *	etag)					{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->etag)			;thi->etag			=jbl_string_copy(etag)			;return this;}
jwl_http_head *		jwl_http_head_set_filename		(jwl_http_head * this,jbl_string * filename)				{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->filename)		;thi->filename		=jbl_string_copy(filename)		;return this;}
jwl_http_head *		jwl_http_head_set_url			(jwl_http_head * this,jbl_string * url)						{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->url)			;thi->url			=jbl_string_copy(url)			;return this;}
jwl_http_head *		jwl_http_head_set_host			(jwl_http_head * this,jbl_string * host)					{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->host)			;thi->host			=jbl_string_copy(host)			;return this;}
jwl_http_head *		jwl_http_head_set_ua			(jwl_http_head * this,jbl_string * ua)						{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->ua)			;thi->ua			=jbl_string_copy(ua)			;return this;}
jwl_http_head *		jwl_http_head_set_referer		(jwl_http_head * this,jbl_string * referer)					{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->referer)		;thi->referer		=jbl_string_copy(referer)		;return this;}
jwl_http_head *		jwl_http_head_set				(jwl_http_head * this,unsigned char * key,jbl_var* var)
{
	jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);
	thi->v=jbl_ht_insert_chars(thi->v,key,var);
	return this;
}

jbl_uint32			jwl_http_head_get_status		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->status;}
jwl_http_charset	jwl_http_head_get_charget		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->charset;}
jbl_file_ct			jwl_http_head_get_content_type	(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->content_type;}
jwl_http_method		jwl_http_head_get_method		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->method;}
jwl_http_protocol	jwl_http_head_get_protocol		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->protocol;}
jwl_http_connection	jwl_http_head_get_connection	(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->connection;}
jwl_http_cache		jwl_http_head_get_cache			(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->cache;}
jwl_http_upgrade	jwl_http_head_get_upgrade		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->upgrade;}
jbl_uint32			jwl_http_head_get_cache_max_age	(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->cache_max_age;}
jwl_http_head_range	jwl_http_head_get_range			(jwl_http_head * this){if(!this)return (jwl_http_head_range){0,0};this=jbl_refer_pull(this);return this->range;}
jbl_string *		jwl_http_head_get_etag			(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->etag);}
jbl_string *		jwl_http_head_get_filename		(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->filename);}
jbl_string *		jwl_http_head_get_url			(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->url);}
jbl_string *		jwl_http_head_get_host			(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->host);}
jbl_string *		jwl_http_head_get_ua			(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->ua);}
jbl_string *		jwl_http_head_get_referer		(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->referer);}
jbl_var    *		jwl_http_head_get				(jwl_http_head * this,unsigned char * key)
{
	if(!this)return NULL;
	this=jbl_refer_pull(this);
	if(!this->v)return NULL;
	return jbl_ht_get_chars(this->v,key);
}




void jwl_http_head_encode(jbl_stream *stream,jwl_http_head *head,jbl_string_size_type size)
{
#define rn() jbl_stream_push_char(stream,'\r'),jbl_stream_push_char(stream,'\n')
	if(!head)return;
	head=jbl_refer_pull(head);
	jbl_stream_push_chars(stream,UC"HTTP/1.1 ");
	jbl_stream_push_uint(stream,head->status);
	switch(head->status)
	{
		case 101:	jbl_stream_push_chars(stream,UC" Switching Protocols")	;break;
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
		jbl_stream_push_chars(stream,UC"Content-Type: ")					,jbl_stream_push_chars(stream,jbl_file_get_ct_chars_by_ctid(head->content_type));
		char *charset[]={"","UTF-8"};
		if(head->charset)
			jbl_stream_push_chars(stream,UC"; charset=")					,jbl_stream_push_chars(stream,UC charset[head->charset]);
		rn();
	}
	if(head->connection)
	{
		char * connection[]={"","keep-alive","close","Upgrade"};
		jbl_uint8 f=0;
		jbl_stream_push_chars(stream,UC"Connection: ");
		if(head->connection&JWL_HTTP_CONNECTION_KEEP_ALIVE	){jbl_stream_push_chars(stream,UC connection[1]);f=1;}
		if(head->connection&JWL_HTTP_CONNECTION_CLOSE		){if(f)jbl_stream_push_chars(stream,UC", ");jbl_stream_push_chars(stream,UC connection[2]);f=1;}
		if(head->connection&JWL_HTTP_CONNECTION_UPGRADE		){if(f)jbl_stream_push_chars(stream,UC", ");jbl_stream_push_chars(stream,UC connection[3]);f=1;}
		rn();
	}
	if(head->connection==JWL_HTTP_CONNECTION_UPGRADE&&head->upgrade)
	{
		char * upgrade[]={"","websocket"};
		jbl_stream_push_chars(stream,UC"Upgrade: ")							,jbl_stream_push_chars(stream,UC upgrade[head->upgrade])		,rn();
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
			if(Vis_jbl_string(i->v))jbl_stream_push_string(stream,jbl_Vstring(i->v));
			else					jbl_var_json_put(i->v,stream,0,0);
			rn();
		}
	
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
	unsigned char	*	YYMARKER;
	unsigned char	*	YYCURSOR=buf->s+(start?(*start):0);
	jbl_string		*	tmp_key	=NULL;
	jbl_var			*	tmp_var	=NULL;
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
	<head_conn>		"keep-alive"						{head->connection		|=JWL_HTTP_CONNECTION_KEEP_ALIVE								;goto yyc_head_conn;}
	<head_conn>		"close"								{head->connection		|=JWL_HTTP_CONNECTION_CLOSE										;goto yyc_head_conn;}
	<head_conn>		"Upgrade"							{head->connection		|=JWL_HTTP_CONNECTION_UPGRADE									;goto yyc_head_conn;}
	<head_conn>		", "								{																						;goto yyc_head_conn;}
	<head_conn>		"\r\n"				=> head_body	{																						;goto yyc_head_body;}

	<head_body>		"Upgrade: "			=> head_upgrade	{																						;goto yyc_head_upgrade;}
	<head_upgrade>	"websocket"							{head->upgrade			=JWL_HTTP_UPGRADE_WEBSOCKET										;goto yyc_head_upgrade;}
	<head_upgrade>	"\r\n"				=> head_body	{																						;goto yyc_head_body;}

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

	<head_body>		"Cache-Control: "	=> head_cache	{																						;goto yyc_head_cache;}
	<head_body>		"Pragma: "			=> head_cache	{																						;goto yyc_head_cache;}
	<head_conn>		"no-cache"							{head->cache			=JWL_HTTP_CACHE_NO												;goto yyc_head_cache;}
	<head_cache>	[]									{																						;++YYCURSOR;goto yyc_head_cache;}
	<head_cache>	"\r\n"				=> head_body	{																						;goto yyc_head_body;}


	<head_body>		[]					=> special_key	{tmp_key				=jbl_string_free    (tmp_key)	;tmp_key=jbl_string_add_char(tmp_key,*YYCURSOR)		;++YYCURSOR;		;goto yyc_special_key;}
	<special_key>	[]									{tmp_key				=jbl_string_add_char(tmp_key,*YYCURSOR)		;++YYCURSOR;		;goto yyc_special_key;}
	<special_key>	": "				=> special_var	{tmp_var				=jbl_var_free(tmp_var)			;tmp_var=jbl_Vstring_new();														;goto yyc_special_var;}
	<special_var>	[]									{tmp_var				=jbl_V(jbl_string_add_char(jbl_Vstring(tmp_var),*YYCURSOR))		;++YYCURSOR;goto yyc_special_var;}
	<special_var>	"\r\n"				=> head_body	{head->v				=jbl_ht_insert(head->v,tmp_key,tmp_var);tmp_key=jbl_string_free(tmp_key);tmp_var=jbl_var_free(tmp_var);			;goto yyc_head_body;}
	
	<head_body>		"\r\n"								{if(start)*start=YYCURSOR-buf->s;return head;}
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
	char * type			[]={"response","request"};
	char * charset		[]={"unknow","UTF-8"};
	char * method		[]={"unknow","get","post"};
	char * protocol		[]={"unknow","HTTP/1.1"};
	char * connection	[]={"unknow","keep alive","close","upgrade"};
	char * cache		[]={"unknow","no","max age,"};
	char * upgrade		[]={"unknow","websocket"};
	if(1)
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"type         :");jbl_stream_push_chars(out,UC type			[jwl_http_head_is_request(this)?1:0])	;jbl_stream_push_char(out,'\n');
	if(jwl_http_head_is_response(this))
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"status       :");jbl_stream_push_uint(out,thi->status)											;jbl_stream_push_char(out,'\n');
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"charset      :");jbl_stream_push_chars(out,UC charset		[thi->charset])						;jbl_stream_push_char(out,'\n');
	}
	if(jwl_http_head_is_request(this))
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"method       :");jbl_stream_push_chars(out,UC method		[thi->method])						;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"protocol     :");jbl_stream_push_chars(out,UC protocol		[thi->protocol])					;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"upgrade      :");jbl_stream_push_chars(out,UC upgrade		[thi->upgrade])						;jbl_stream_push_char(out,'\n');	
	}
	if(1)
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"connection   :");
		if(!thi->connection)
			jbl_stream_push_chars(out,UC connection	[0]);
		else 
		{
			if(thi->connection&JWL_HTTP_CONNECTION_KEEP_ALIVE	)jbl_stream_push_chars(out,UC connection[1]),jbl_stream_push_char(out,',');
			if(thi->connection&JWL_HTTP_CONNECTION_CLOSE		)jbl_stream_push_chars(out,UC connection[2]),jbl_stream_push_char(out,',');
			if(thi->connection&JWL_HTTP_CONNECTION_UPGRADE		)jbl_stream_push_chars(out,UC connection[3]),jbl_stream_push_char(out,',');
		}
		jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"cache        :");jbl_stream_push_chars(out,UC cache		[thi->cache])						;if(thi->cache==JWL_HTTP_CACHE_MAX_AGE){jbl_stream_push_uint(out,thi->cache_max_age);}	;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"range        :");jbl_stream_push_uint(out,thi->range.start)									;jbl_stream_push_char(out,'-' );	jbl_stream_push_uint(out,thi->range.end)			;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"etag         :");jbl_stream_push_string(out,thi->etag)											;jbl_stream_push_char(out,'\n');
	}
	if(jwl_http_head_is_response(this))
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"content type :");jbl_stream_push_chars(out,jbl_file_get_ct_chars_by_ctid(thi->content_type))	;jbl_stream_push_char(out,'\n');
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"filename     :");jbl_stream_push_string(out,thi->filename)										;jbl_stream_push_char(out,'\n');
	}
	if(jwl_http_head_is_request(this))
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"url          :");jbl_stream_push_string(out,thi->url)											;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"host         :");jbl_stream_push_string(out,thi->host)											;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"user agent   :");jbl_stream_push_string(out,thi->ua)											;jbl_stream_push_char(out,'\n');	
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"referer      :");jbl_stream_push_string(out,thi->referer)										;jbl_stream_push_char(out,'\n');
	}	
	if(thi->v)
		jbl_ht_foreach(thi->v,i)
		{
			for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
			jbl_stream_push_string(out,i->k);
			for(jbl_int32 j=jbl_string_get_length(i->k);j<13;jbl_stream_push_char(out,' '),++j);//格式化的\t
			jbl_stream_push_char(out,':');
			if(Vis_jbl_string(i->v))jbl_stream_push_string(out,jbl_Vstring(i->v)),jbl_stream_push_char(out,'\n');	
			else					jbl_var_view_put(i->v,out,0,tabs,0,NULL,NULL,NULL);
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
