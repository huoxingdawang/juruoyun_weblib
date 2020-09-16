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
jbl_var_operators_new(jwl_http_head_operators,jwl_http_head_free,jwl_http_head_copy,NULL,NULL,jwl_http_head_view_put,NULL);
jwl_http_head *jwl_http_head_new()
{
	jwl_http_head * this=jbl_malloc(sizeof(jwl_http_head));
	jbl_gc_init(this);
	jbl_gc_plus(this);
	jbl_var_set_operators(this,&jwl_http_head_operators);	
	this->status		=0;
	this->charset		=0;	
	this->content_type	=0;
	this->method		=0;
	this->protocol		=0;
	this->len			=0;
	this->total_len		=0;
	this->tren			=0;
	this->connection	=0;
	this->cache			=0;
	this->upgrade		=0;
	this->cache_max_age	=0;	
	this->range.start	=0;	
	this->range.end		=0;	
	this->etag			=NULL;
	this->filename		=NULL;
	this->url			=NULL;
	this->host			=NULL;
	this->ua			=NULL;
	this->referer		=NULL;
	this->cookie		=NULL;
	this->v				=NULL;	
	this->parameter		=NULL;	

	this->yy.cond		=0;	
	this->yy.state		=0;	
	this->yy.ch			=0;	
	this->yy.marker		=0;	
	this->yy.k			=NULL;	
	this->yy.v			=NULL;	
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
			this->cookie		=jbl_ht_free	(this->cookie);
			this->parameter		=jbl_ht_free	(this->parameter);

			this->yy.k			=jbl_string_free(this->yy.k);
			this->yy.v			=jbl_var_free(this->yy.v);
		}
		jbl_free(this);
	}
	return NULL;	
	
}
jwl_http_head *jwl_http_head_clear(jwl_http_head * this)
{
	jwl_http_head *thi;
	this=jwl_http_head_extend(this,&thi);
	thi->status			=0;
	thi->charset		=0;	
	thi->content_type	=0;
	thi->method			=0;
	thi->protocol		=0;
	thi->len			=0;
	thi->total_len		=0;
	thi->tren			=0;
	thi->connection		=0;
	thi->cache			=0;
	thi->upgrade		=0;
	thi->cache_max_age	=0;	
	thi->range.start	=0;	
	thi->range.end		=0;	
	thi->yy.cond		=0;	
	thi->yy.state		=0;	
	thi->yy.ch			=0;	
	thi->yy.marker		=0;	
	thi->etag			=jbl_string_free(thi->etag);
	thi->filename		=jbl_string_free(thi->filename);
	thi->url			=jbl_string_free(thi->url);
	thi->host			=jbl_string_free(thi->host);
	thi->ua				=jbl_string_free(thi->ua);
	thi->referer		=jbl_string_free(thi->referer);
	thi->v				=jbl_ht_free	(thi->v);
	thi->cookie			=jbl_ht_free	(thi->cookie);
	thi->parameter		=jbl_ht_free	(thi->parameter);

	thi->yy.k			=jbl_string_free(thi->yy.k);
	thi->yy.v			=jbl_var_free(thi->yy.v);
	return this;
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
	if((jbl_gc_refcnt(thi)<=1)){if(pthi)*pthi=thi;return this;}

	jwl_http_head * tmp	=jwl_http_head_new();
	
	tmp->status			=this->status;
	tmp->charset		=this->charset;
	tmp->content_type	=this->content_type;
	tmp->method			=this->method;
	tmp->protocol		=this->protocol;
	tmp->connection		=this->connection;
	tmp->len			=this->len;
	tmp->total_len		=this->total_len;
	tmp->tren			=this->tren;
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
	tmp->parameter		=jbl_ht_copy(this->parameter);
	
	tmp->yy.cond		=this->yy.cond;
	tmp->yy.state		=this->yy.state;
	tmp->yy.ch			=this->yy.ch;
	tmp->yy.marker		=this->yy.marker;
	tmp->yy.k			=jbl_string_copy(this->yy.k);
	tmp->yy.v			=jbl_var_copy(this->yy.v);
	
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
jwl_http_head *		jwl_http_head_set_tren			(jwl_http_head * this,jwl_http_tren			tren)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->tren			=tren  			;return this;}
jwl_http_head *		jwl_http_head_set_length		(jwl_http_head * this,jbl_uint64			len)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->len			=len			;return this;}
jwl_http_head *		jwl_http_head_set_total_length	(jwl_http_head * this,jbl_uint64			total_len)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->total_len		=total_len		;return this;}
jwl_http_head *		jwl_http_head_set_cache_max_age	(jwl_http_head * this,jbl_uint32			cache_max_age)	{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->cache_max_age	=cache_max_age	;return this;}
jwl_http_head *		jwl_http_head_set_range			(jwl_http_head * this,jwl_http_head_range	range)			{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);thi->range			=range			;return this;}
jwl_http_head *		jwl_http_head_set_etag			(jwl_http_head * this,jbl_string *	etag)					{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->etag)			;thi->etag			=jbl_string_copy(etag)							;return this;}
jwl_http_head *		jwl_http_head_set_filename		(jwl_http_head * this,jbl_string * filename)				{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->filename)		;thi->filename		=jbl_string_copy(filename)						;return this;}
jwl_http_head *		jwl_http_head_set_url			(jwl_http_head * this,jbl_string * url)						{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->url)			;thi->url			=jbl_string_copy(url)							;return this;}
jwl_http_head *		jwl_http_head_set_host			(jwl_http_head * this,jbl_string * host)					{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->host)			;thi->host			=jbl_string_copy(host)							;return this;}
jwl_http_head *		jwl_http_head_set_ua			(jwl_http_head * this,jbl_string * ua)						{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->ua)			;thi->ua			=jbl_string_copy(ua)							;return this;}
jwl_http_head *		jwl_http_head_set_referer		(jwl_http_head * this,jbl_string * referer)					{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);jbl_string_free(thi->referer)		;thi->referer		=jbl_string_copy(referer)						;return this;}
jwl_http_head *		jwl_http_head_set				(jwl_http_head * this,unsigned char * key,void* var)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi)									;thi->v				=jbl_ht_insert_chars(thi->v			,key,var)	;return this;}
jwl_http_head *		jwl_http_head_set_parameter		(jwl_http_head * this,unsigned char * key,void* var)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);									thi->parameter		=jbl_ht_insert_chars(thi->parameter	,key,var)	;return this;}
jwl_http_head *		jwl_http_head_set_cookie		(jwl_http_head * this,unsigned char * key,void* var)		{jwl_http_head *thi;this=jwl_http_head_extend(this,&thi);									thi->cookie			=jbl_ht_insert_chars(thi->cookie	,key,var)	;return this;}

jbl_uint32			jwl_http_head_get_status		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->status;}
jwl_http_charset	jwl_http_head_get_charget		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->charset;}
jbl_file_ct			jwl_http_head_get_content_type	(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->content_type;}
jwl_http_method		jwl_http_head_get_method		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->method;}
jwl_http_protocol	jwl_http_head_get_protocol		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->protocol;}
jwl_http_connection	jwl_http_head_get_connection	(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->connection;}
jwl_http_cache		jwl_http_head_get_cache			(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->cache;}
jwl_http_upgrade	jwl_http_head_get_upgrade		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->upgrade;}
jwl_http_tren		jwl_http_head_get_tren			(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->tren;}
jbl_uint64			jwl_http_head_get_length		(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->len;}
jbl_uint64			jwl_http_head_get_total_length	(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->total_len;}
jbl_uint32			jwl_http_head_get_cache_max_age	(jwl_http_head * this){if(!this)return 0;this=jbl_refer_pull(this);return this->cache_max_age;}
jwl_http_head_range	jwl_http_head_get_range			(jwl_http_head * this){if(!this)return (jwl_http_head_range){0,0};this=jbl_refer_pull(this);return this->range;}
jbl_string *		jwl_http_head_get_etag			(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->etag);}
jbl_string *		jwl_http_head_get_filename		(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->filename);}
jbl_string *		jwl_http_head_get_url			(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->url);}
jbl_string *		jwl_http_head_get_host			(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->host);}
jbl_string *		jwl_http_head_get_ua			(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->ua);}
jbl_string *		jwl_http_head_get_referer		(jwl_http_head * this){if(!this)return NULL;this=jbl_refer_pull(this);return jbl_string_copy(this->referer);}
void       *		jwl_http_head_get				(jwl_http_head * this,unsigned char * key){if(!this)return NULL;this=jbl_refer_pull(this);if(!this->v)			return NULL;return jbl_ht_get_chars(this->v,key);}
void       *		jwl_http_head_get_parameter		(jwl_http_head * this,unsigned char * key){if(!this)return NULL;this=jbl_refer_pull(this);if(!this->parameter)	return NULL;return jbl_ht_get_chars(this->parameter,key);}
void       *		jwl_http_head_get_cookie		(jwl_http_head * this,unsigned char * key){if(!this)return NULL;this=jbl_refer_pull(this);if(!this->cookie)		return NULL;return jbl_ht_get_chars(this->cookie,key);}


void jwl_http_head_encode(jwl_http_head *head,jbl_stream *stream)
{
#define rn() jbl_stream_push_char(stream,'\r'),jbl_stream_push_char(stream,'\n')
	if(!head)return;
	head=jbl_refer_pull(head);
	if(jwl_http_head_is_response(head))
	{
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
	}
	else if(jwl_http_head_is_request(head)&&head->status==JWL_HTTP_STATUS_PROXY)
	{
		jbl_stream_push_chars(stream,UC"CONNECT ");
		jbl_stream_push_string(stream,head->host);
		jbl_stream_push_chars(stream,UC" HTTP/1.1\r\n\r\n");
		jbl_stream_do(stream,jbl_stream_force);
		return;
	}
	else
	{
		char * method		[]={"","GET ","POST "};
		char * protocol		[]={""," HTTP/1.1",""};
		jbl_stream_push_chars	(stream,UC method[head->method]);
		if(jbl_string_get_length(head->url))jbl_stream_push_string	(stream,head->url);
		else jbl_stream_push_char(stream,'/');
		if(head->parameter)
		{
			jbl_stream_push_char(stream,'?');
			jbl_uint8 f=0;
			jbl_ht_foreach(head->parameter,i)
			{
				if(f)jbl_stream_push_char(stream,'&');
				f=1;
				jbl_stream_push_string(stream,i->k);
				jbl_stream_push_char(stream,'=');
				if(jbl_var_is(i->v,jbl_string_operators))jbl_stream_push_string(stream,i->v);
				else					jbl_var_json_put(i->v,stream,0,0);
			}		
		}		
		jbl_stream_push_chars	(stream,UC protocol[head->protocol]);
	}
	rn();
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
		jbl_stream_push_chars(stream,UC"Upgrade: ");
		char * upgrade[]={"","websocket"};
		jbl_stream_push_chars(stream,UC upgrade[head->upgrade]);
		rn();
	}
	if(head->etag&&jwl_http_head_is_response(head))
		jbl_stream_push_chars(stream,UC"ETag: ")							,jbl_stream_push_string(stream,head->etag)				,rn();
	if(head->etag&&jwl_http_head_is_request(head))
		jbl_stream_push_chars(stream,UC"If-None-Match: ")					,jbl_stream_push_string(stream,head->etag)				,rn();
	if(head->content_type)
	{
		jbl_stream_push_chars(stream,UC"Content-Type: ")					,jbl_stream_push_chars(stream,jbl_file_get_ct_chars_by_ctid(head->content_type));
		char *charset[]={"","UTF-8"};
		if(head->charset)
			jbl_stream_push_chars(stream,UC"; charset=")					,jbl_stream_push_chars(stream,UC charset[head->charset]);
		rn();
	}
	if(!(head->tren&JWL_HTTP_TREN_CHUNKED))
	{
		jbl_stream_push_chars(stream,UC"Content-Length: ")						,jbl_stream_push_uint(stream,head->len)					,rn();
		if(head->status==206&&jwl_http_head_is_response(head))
			jbl_stream_push_chars(stream,UC"Content-Range: bytes ")				,jbl_stream_push_uint(stream,head->range.start)			,jbl_stream_push_char(stream,'-')	,jbl_stream_push_uint(stream,head->range.end?(head->range.end-1):0)	,jbl_stream_push_char(stream,'/')	,jbl_stream_push_uint(stream,head->total_len)	,rn();
	}
	if(jwl_http_head_is_request(head)&&(head->range.start||head->range.end))
		jbl_stream_push_chars(stream,UC"Range: bytes=")							,jbl_stream_push_uint(stream,head->range.start)			,jbl_stream_push_char(stream,'-')	,jbl_stream_push_uint(stream,head->range.end?(head->range.end-1):0),rn();		
	
	if(head->tren)
	{
		char * tren			[]={"","chunked","compress","deflate","gzip","identity"};
		jbl_uint8 f=0;
		if(jwl_http_head_is_response(head))		jbl_stream_push_chars(stream,UC"Transfer-Encoding: ");
		else									jbl_stream_push_chars(stream,UC"Accept-Encoding: ");
		if(head->tren&JWL_HTTP_TREN_CHUNKED		){jbl_stream_push_chars(stream,UC tren[1]);f=1;}
		if(head->tren&JWL_HTTP_TREN_COMPRESS	){if(f)jbl_stream_push_chars(stream,UC", ");jbl_stream_push_chars(stream,UC tren[2]);f=1;}
		if(head->tren&JWL_HTTP_TREN_DEFLATE		){if(f)jbl_stream_push_chars(stream,UC", ");jbl_stream_push_chars(stream,UC tren[3]);f=1;}
		if(head->tren&JWL_HTTP_TREN_GZIP		){if(f)jbl_stream_push_chars(stream,UC", ");jbl_stream_push_chars(stream,UC tren[4]);f=1;}
		if(head->tren&JWL_HTTP_TREN_IDENTITY	){if(f)jbl_stream_push_chars(stream,UC", ");jbl_stream_push_chars(stream,UC tren[5]);f=1;}
		rn();
	}	
	
	if(head->host&&jwl_http_head_is_request(head))
		jbl_stream_push_chars(stream,UC"Host: ")							,jbl_stream_push_string(stream,head->host)				,rn();	
	if(head->ua&&jwl_http_head_is_request(head))
		jbl_stream_push_chars(stream,UC"User-Agent: ")						,jbl_stream_push_string(stream,head->ua)				,rn();	
	
	
	if(jwl_http_head_is_response(head))
	{
		jbl_stream_push_chars(stream,UC"Server: "JWL_HTTP_RESPONSE_SERVER_NAME)														,rn();
		jbl_stream_push_chars(stream,UC"Accept-Ranges: bytes")																		,rn();
		if(head->filename)
		{
			jbl_stream_push_chars(stream,UC"Content-Disposition: attachment; filename=\"");
			jbl_stream_push_string(stream,head->filename);
			jbl_stream_push_char(stream,'"');
			rn();
		}
	}
	if(head->v)
		jbl_ht_foreach(head->v,i)
		{
			jbl_stream_push_string(stream,jbl_htk(i));
			jbl_stream_push_chars(stream,UC": ");
			if(jbl_var_is(i->v,jbl_string_operators))jbl_stream_push_string(stream,i->v);
			else					jbl_var_json_put(i->v,stream,0,0);
			rn();
		}
	rn();
#undef rn
}
/*!types:re2c */
jwl_http_head* __jwl_http_head_decode(jwl_http_head * head,unsigned char* in,unsigned char*YYLIMIT,jbl_string_size_type *start)
{
	jwl_http_head * hea;head=jwl_http_head_extend(head,&hea);
	jbl_string_size_type _=0;if(!start)start=&_;
#define upd *start=YYCURSOR-in	
	unsigned char	*	YYCURSOR=in+*start;
///*!re2c
	re2c:condenumprefix						=jwl_http_scanner_condition_;
	re2c:define:YYCONDTYPE					=jwl_http_scanner_condition;
	re2c:define:YYSETCONDITION				="hea->yy.cond = ";
	re2c:define:YYGETCONDITION				=hea->yy.cond ;
	re2c:define:YYGETCONDITION:naked		=1;
	re2c:define:YYSETSTATE					="hea->yy.state = ";
	re2c:define:YYGETSTATE					=hea->yy.state ;
	re2c:define:YYGETSTATE:naked			=1;
	re2c:define:YYMARKER					=hea->yy.marker;
	re2c:yyfill:enable						=1;
	re2c:define:YYFILL:naked				=1;
	re2c:define:YYFILL						="{upd;return head;}";
	re2c:variable:yych						=hea->yy.ch;
	<> :=> start
	<start> 		"POST"										{upd;hea->method		=JWL_HTTP_METHOD_POST			;hea=jwl_http_head_set_request(hea)			;goto yyc_start		;}
	<start> 		"GET"										{upd;hea->method		=JWL_HTTP_METHOD_GET			;hea=jwl_http_head_set_request(hea)			;goto yyc_start		;}
	<start> 		"HTTP/1.1"					=> starts		{upd;hea->protocol		=JWL_HTTP_PROTOCOL_HTTP_1_1		;hea=jwl_http_head_set_response(hea)		;goto yyc_starts	;}
	<start>			[]											{   ;																				;++YYCURSOR;upd	;goto yyc_starts	;}
	<start>			" " 						=> url			{upd;hea->url			=jbl_string_free    (hea->url)												;goto yyc_url		;}
	<starts>		" " 						=> status		{upd;hea->status		=0																			;goto yyc_status	;}
	
	<status>		[0-9]										{upd;hea->status		=((hea->status<<3)+(hea->status<<1)+*(YYCURSOR-1)-'0')						;goto yyc_status	;}
	<status>		[]		 									{   ;																				;++YYCURSOR;upd	;goto yyc_status	;}
	<status>		"\r\n" 						=> head_body	{upd;																								;goto yyc_head_body	;}

	<url>			[]											{   ;hea->url			=jbl_string_add_char(hea->url,*YYCURSOR)					;++YYCURSOR;upd	;goto yyc_url		;}
	<url>			" " 						=> protocol		{upd;																								;goto yyc_protocol	;}
	<url>			"?" 						=> parameter	{upd;hea->yy.k			=jbl_string_free(hea->yy.k)													;goto yyc_parameter	;}
	<parameter>		[]											{   ;hea->yy.k			=jbl_string_add_char(hea->yy.k,*YYCURSOR)					;++YYCURSOR;upd	;goto yyc_parameter	;}
	<parameter>		" " 						=> protocol		{upd;hea->yy.k			=jbl_string_free(hea->yy.k);hea->yy.v=jbl_var_free(hea->yy.v)				;goto yyc_protocol	;}
	<parameter>		"&" 						=> parameter	{upd;hea->yy.k			=jbl_string_free(hea->yy.k);hea->yy.v=jbl_var_free(hea->yy.v)				;goto yyc_parameter	;}
	<parameter>		"="							=> parameterv	{upd;hea->yy.v			=jbl_var_free(hea->yy.v);hea->yy.v=jbl_string_new()						;goto yyc_parameterv;}
	<parameterv>	[]											{   ;hea->yy.v			=jbl_string_add_char(hea->yy.v,*YYCURSOR);++YYCURSOR;upd;goto yyc_parameterv;}
	<parameterv>	"&"							=> head_body	{upd;hea->parameter		=jbl_ht_insert(hea->parameter,hea->yy.k,hea->yy.v);hea->yy.k=jbl_string_free(hea->yy.k);hea->yy.v=jbl_var_free(hea->yy.v);			;goto yyc_parameter	;}
	<parameterv>	" " 						=> protocol		{upd;hea->parameter		=jbl_ht_insert(hea->parameter,hea->yy.k,hea->yy.v);hea->yy.k=jbl_string_free(hea->yy.k);hea->yy.v=jbl_var_free(hea->yy.v);			;goto yyc_protocol	;}
	
	
	<protocol>		"HTTP/1.1"									{upd;hea->protocol		=JWL_HTTP_PROTOCOL_HTTP_1_1													;goto yyc_protocol	;}
	<protocol>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}
	
	<head_body>		"Host:"" "*					=> head_host	{upd;hea->host			=jbl_string_free(hea->host)													;goto yyc_head_host	;}
	<head_host>		[]											{   ;hea->host			=jbl_string_add_char(hea->host,*YYCURSOR)					;++YYCURSOR;upd	;goto yyc_head_host	;}
	<head_host>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}
	
	<head_body>		"User-Agent:"" "*			=> head_ua		{upd;hea->ua			=jbl_string_free(hea->ua)													;goto yyc_head_ua	;}
	<head_ua>		[]											{   ;hea->ua			=jbl_string_add_char(hea->ua,*YYCURSOR)						;++YYCURSOR;upd	;goto yyc_head_ua	;}
	<head_ua>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}
	
	<head_body>		"Referer:"" "*				=> head_refer	{upd;hea->referer		=jbl_string_free(hea->referer)												;goto yyc_head_refer;}
	<head_refer>	[]											{   ;hea->referer		=jbl_string_add_char(hea->referer,*YYCURSOR)				;++YYCURSOR;upd	;goto yyc_head_refer;}
	<head_refer>	"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}

	
	<head_body>		"Etag:"" "*					=> head_etag	{upd;hea->etag			=jbl_string_free(hea->etag)													;goto yyc_head_etag	;}
	<head_body>		"If-None-Match:"" "*		=> head_etag	{upd;hea->etag			=jbl_string_free(hea->etag)													;goto yyc_head_etag	;}
	<head_etag>		[]											{   ;hea->etag			=jbl_string_add_char(hea->etag,*YYCURSOR)					;++YYCURSOR;upd	;goto yyc_head_etag	;}
	<head_etag>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}
	
	<head_body>		"Connection:"" "*			=> head_conn	{upd;																								;goto yyc_head_conn	;}
	<head_conn>		"keep-alive"								{upd;hea->connection	|=JWL_HTTP_CONNECTION_KEEP_ALIVE											;goto yyc_head_conn	;}
	<head_conn>		"close"										{upd;hea->connection	|=JWL_HTTP_CONNECTION_CLOSE													;goto yyc_head_conn	;}
	<head_conn>		"Upgrade"									{upd;hea->connection	|=JWL_HTTP_CONNECTION_UPGRADE												;goto yyc_head_conn	;}
	<head_conn>		", "										{upd;																								;goto yyc_head_conn	;}
	<head_conn>		[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_conn	;}
	<head_conn>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}
	
	<head_body>		"Accept-Encoding:"" "*		=> head_tren	{upd;																								;goto yyc_head_tren	;}
	<head_body>		"Transfer-Encoding:"" "*	=> head_tren	{upd;																								;goto yyc_head_tren	;}
	<head_tren>		"chunked"									{upd;hea->tren			|=JWL_HTTP_TREN_CHUNKED														;goto yyc_head_tren	;}
	<head_tren>		"compress"									{upd;hea->tren			|=JWL_HTTP_TREN_COMPRESS													;goto yyc_head_tren	;}
	<head_tren>		"deflate"									{upd;hea->tren			|=JWL_HTTP_TREN_DEFLATE														;goto yyc_head_tren	;}
	<head_tren>		"gzip"										{upd;hea->tren			|=JWL_HTTP_TREN_GZIP														;goto yyc_head_tren	;}
	<head_tren>		"identity"									{upd;hea->tren			|=JWL_HTTP_TREN_IDENTITY													;goto yyc_head_tren	;}
	<head_tren>		", "										{upd;																								;goto yyc_head_tren	;}
	<head_tren>		[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_tren	;}
	<head_tren>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}

	<head_body>		"Upgrade:"" "*				=> head_upgrade	{upd;																								;goto yyc_head_upgrade;}
	<head_upgrade>	"websocket"									{upd;hea->upgrade		=JWL_HTTP_UPGRADE_WEBSOCKET													;goto yyc_head_upgrade;}
	<head_upgrade>	[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_upgrade;}
	<head_upgrade>	"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}
	

	<head_body>		"Range:"" "*"bytes="		=> head_range_s	{upd;hea->range.start	=0																			;goto yyc_head_range_s;}
	<head_body>		"Content-Range:"" "*"bytes"	=> head_range_s	{upd;hea->range.start	=0																			;goto yyc_head_range_s;}
	<head_range_s>	"-"							=> head_range_e	{upd;hea->range.end		=0																			;goto yyc_head_range_e;}
	<head_range_e>	"/"							=> head_range_e	{upd;hea->total_len		=0																			;goto yyc_head_range_t;}
	<head_range_s>	[0-9]										{upd;hea->range.start	=(hea->range.start<<3)+(hea->range.start<<1)+*(YYCURSOR-1)-'0'				;goto yyc_head_range_s;}
	<head_range_e>	[0-9]										{upd;hea->range.end		=(hea->range.end<<3)+(hea->range.end<<1)+*(YYCURSOR-1)-'0'					;goto yyc_head_range_e;}
	<head_range_t>	[0-9]										{upd;hea->total_len		=(hea->total_len<<3)+(hea->total_len<<1)+*(YYCURSOR-1)-'0'					;goto yyc_head_range_t;}
	<head_range_s>	[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_range_s;}
	<head_range_e>	[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_range_e;}
	<head_range_t>	[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_range_t;}
	<head_range_s>	"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}
	<head_range_e>	"\r\n"						=> head_body	{upd;hea->range.end		=(hea->range.end)?(hea->range.end+1):0										;goto yyc_head_body	;}
	<head_range_t>	"\r\n"						=> head_body	{upd;hea->range.end		=(hea->range.end)?(hea->range.end+1):0										;goto yyc_head_body	;}

	<head_body>		"Cache-Control:"" "*		=> head_cache	{upd;																								;goto yyc_head_cache;}
	<head_body>		"Pragma: "					=> head_cache	{upd;																								;goto yyc_head_cache;}
	<head_cache>	"no-cache"									{upd;hea->cache			=JWL_HTTP_CACHE_NO															;goto yyc_head_cache;}
	<head_cache>	[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_cache;}
	<head_cache>	"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}


	<head_body>		"Content-Type:"" "*			=> head_ct		{upd;hea->yy.k			=jbl_string_free(hea->yy.k)													;goto yyc_head_ct	;}
	<head_ct>		[]											{   ;hea->yy.k			=jbl_string_add_char(hea->yy.k,*YYCURSOR)					;++YYCURSOR;upd	;goto yyc_head_ct	;}
	<head_ct>		";"" "*"charset="			=> head_cs		{upd;hea->content_type	=jbl_file_get_ctid_by_ct(hea->yy.k);hea->yy.k=jbl_string_free(hea->yy.k)	;goto yyc_head_cs	;}
	<head_ct>		"\r\n"						=> head_body	{upd;hea->content_type	=jbl_file_get_ctid_by_ct(hea->yy.k);hea->yy.k=jbl_string_free(hea->yy.k)	;goto yyc_head_body	;}
	<head_cs>		"UTF-8"										{upd;hea->charset		=JWL_HTTP_CHARSET_UTF8														;goto yyc_head_cs	;}
	<head_cs>		[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_cs	;}
	<head_cs>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}

	<head_body>		"Content-Length:"" "*		=> head_cl		{upd;hea->len			=0																			;goto yyc_head_cl	;}
	<head_cl>		[0-9]										{upd;hea->len			=(hea->len<<3)+(hea->len<<1)+*(YYCURSOR-1)-'0'								;goto yyc_head_cl	;}
	<head_cl>		[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_cl	;}
	<head_cl>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}


	<head_body>		"Cookie:"" "*				=> head_cookie	{upd;hea->yy.k			=jbl_string_free(hea->yy.k)													;goto yyc_head_cookie;}
	<head_cookie>	[]											{   ;hea->yy.k			=jbl_string_add_char(hea->yy.k,*YYCURSOR)					;++YYCURSOR;upd	;goto yyc_head_cookie;}
	<head_cookie>	"="							=> head_cookiev	{upd;hea->yy.v			=jbl_var_free(hea->yy.v);hea->yy.v=jbl_string_new()						;goto yyc_head_cookiev;}
	<head_cookie>	"\r\n"						=> head_body	{upd;hea->yy.k			=jbl_string_free(hea->yy.k)													;goto yyc_head_body	;}
	<head_cookiev>	[]											{   ;hea->yy.v			=jbl_string_add_char(hea->yy.v,*YYCURSOR);++YYCURSOR;upd;goto yyc_head_cookiev;}
	<head_cookiev>	"; "						=> head_cookie	{upd;hea->cookie		=jbl_ht_insert(hea->cookie,hea->yy.k,hea->yy.v);hea->yy.k=jbl_string_free(hea->yy.k);hea->yy.v=jbl_var_free(hea->yy.v)		;goto yyc_head_cookie	;}
	<head_cookiev>	"\r\n"						=> head_body	{upd;hea->cookie		=jbl_ht_insert(hea->cookie,hea->yy.k,hea->yy.v);hea->yy.k=jbl_string_free(hea->yy.k);hea->yy.v=jbl_var_free(hea->yy.v)		;goto yyc_head_body		;}


	<head_body>		[]							=> special_key	{   ;hea->yy.k			=jbl_string_add_char(jbl_string_free(hea->yy.k),*YYCURSOR)	;++YYCURSOR;upd	;goto yyc_special_key;}
	<special_key>	[]											{   ;hea->yy.k			=jbl_string_add_char(hea->yy.k,*YYCURSOR)					;++YYCURSOR;upd	;goto yyc_special_key;}
	<special_key>	"\r\n"						=> head_body	{upd;hea->yy.k			=jbl_string_free(hea->yy.k);hea->yy.v=jbl_var_free(hea->yy.v)				;goto yyc_head_body	;}
	<special_key>	":"" "*						=> special_var	{upd;hea->yy.v			=jbl_var_free(hea->yy.v);hea->yy.v=jbl_string_new()							;goto yyc_special_var;}
	<special_var>	[]											{   ;hea->yy.v			=jbl_string_add_char(hea->yy.v,*YYCURSOR);++YYCURSOR;upd;goto yyc_special_var;}
	<special_var>	"\r\n"						=> head_body	{upd;hea->v				=jbl_ht_insert(hea->v,hea->yy.k,hea->yy.v);hea->yy.k=jbl_string_free(hea->yy.k);hea->yy.v=jbl_var_free(hea->yy.v)			;goto yyc_head_body		;}

	<head_body>		"Server:"" "*				=> head_ign		{upd;																								;goto yyc_head_ign	;}
	<head_body>		"Accept:"" "*				=> head_ign		{upd;																								;goto yyc_head_ign	;}
	<head_body>		"Accept-Language:"" "*		=> head_ign		{upd;																								;goto yyc_head_ign	;}
	<head_ign>		[]											{   ;																				;++YYCURSOR;upd	;goto yyc_head_ign	;}
	<head_ign>		"\r\n"						=> head_body	{upd;																								;goto yyc_head_body	;}
	
	<head_body>		"\r\n"										{upd;hea->yy.cond=-1;return head;}
	<*>				[\x00]										{return head;}
		
 */
	#undef YYCTYPE	


	return head;
}
jwl_http_head* jwl_http_head_decode(jbl_string *buf,jbl_string_size_type *start)
{
	if(!buf)return NULL;
	buf=jbl_refer_pull(buf);
	if(!buf->len)return NULL;
	jbl_string_set_tail(buf);
	return __jwl_http_head_decode(jwl_http_head_new(),buf->s,buf->s+buf->len,start);
}

#if JBL_STREAM_ENABLE==1
jbl_stream *jwl_http_encode_stream_new(jwl_http_head* head)
{
	if(!head)jbl_exception("NULL POINTER");
	return jbl_stream_new(&jwl_http_encode_stream_operaters,head,JWL_HTTP_STREAM_BUF_LENGTH,NULL,2);
}
jbl_stream *jwl_http_decode_stream_new(jwl_http_head* head)
{
	if(!head)jbl_exception("NULL POINTER");
	return jbl_stream_new(&jwl_http_decode_stream_operaters,head,JWL_HTTP_STREAM_BUF_LENGTH+1,NULL,2);
}
static void __jwl_http_encode_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jwl_http_head *head=jbl_refer_pull((jwl_http_head*)this->data);
	if(nxt&&((flags&jbl_stream_force)||(this->en>JWL_HTTP_STREAM_BUF_LENGTH>>1))&&(!this->stop))
	{
		if(!this->extra[0].c8[0])
			jwl_http_head_encode(head,nxt),this->extra[0].c8[0]=1;
		jbl_stream_buf_size_type i=0;
		for(;i<this->en;)
		{
			jbl_stream_buf_size_type len=jbl_min(this->en-i,nxt->size-nxt->en-(head->tren&JWL_HTTP_TREN_CHUNKED?12:0));
			if(head->tren&JWL_HTTP_TREN_CHUNKED)jbl_stream_push_hex(nxt,len),jbl_stream_push_char(nxt,'\r'),jbl_stream_push_char(nxt,'\n');
			jbl_memory_copy(nxt->buf+nxt->en,this->buf+i,len);
			i+=len;
			nxt->en+=len;
			this->extra[1].u+=len;
			if(head->tren&JWL_HTTP_TREN_CHUNKED)jbl_stream_push_char(nxt,'\r'),jbl_stream_push_char(nxt,'\n');
			if((!(head->tren&JWL_HTTP_TREN_CHUNKED))&&(this->extra[1].u==head->len))goto pushf;
			jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))goto backup;
		}
		if((!(head->tren&JWL_HTTP_TREN_CHUNKED))&&(this->extra[1].u==head->len))goto pushf;
		if((head->tren&JWL_HTTP_TREN_CHUNKED)&&(flags&jbl_stream_force))
		{
			jbl_stream_push_chars(nxt,UC"0\r\n\r\n");
pushf:;			
			jbl_stream_do(nxt,0);
			this->stop=1;
			this->extra[0].c8[0]=0,this->extra[1].u=0;
			goto backup;
		}
backup:;
		jbl_memory_copy(this->buf,this->buf+i,this->en-=i);
	}
}

static void __jwl_http_decode_stream_usf(jbl_stream* this){this->size=JWL_HTTP_STREAM_BUF_LENGTH;}
static void __jwl_http_decode_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jbl_string_size_type i=0;
	jwl_http_head *head=NULL;
	if((!this->extra[0].c8[0])&&(!this->stop))//头没搞完
	{
		this->buf[this->en]=0;
		this->data=__jwl_http_head_decode((jwl_http_head*)this->data,this->buf,this->buf+this->size,&i);
		head=jbl_refer_pull((jwl_http_head*)this->data);
		if(head->yy.cond!=(jbl_uint8)-1)
			goto backup;
		this->extra[0].c8[0]=1;
		this->extra[0].c8[1]=0;
		if(head->tren&JWL_HTTP_TREN_CHUNKED)head->len=0;
		else if(head->len==0)this->stop=1,this->extra[0].c8[0]=0,this->en=0;
	}
	else
		head=jbl_refer_pull((jwl_http_head*)this->data);
	if(nxt&&(!this->stop))
	{
start:;
		if((i<this->en)&&(!this->extra[1].u))//未指定长度
		{
			if(head->tren&JWL_HTTP_TREN_CHUNKED)//读chunk的头
			{
				if((i+2>=this->en))goto backup;
				if(this->extra[0].c8[1]){if(this->buf[i]!='\r'){jbl_exception("ERROR");}++i;}
				if(this->extra[0].c8[1]){if(this->buf[i]!='\n'){jbl_exception("ERROR");}++i;}
				if((i+3>=this->en))goto backup;
				jbl_string *s1=jbl_string_add_const_length(NULL,this->buf,this->en);
				this->extra[1].u=jbl_string_get_hex_start(s1,&i);
				s1=jbl_string_free(s1);
//				puint(this->extra[1].u);pn();pf();
				if(this->buf[i]!='\r'){jbl_exception("ERROR");}++i;
				if(this->buf[i]!='\n'){jbl_exception("ERROR");}++i;
				head->len+=this->extra[1].u;
				if(!this->extra[1].u)//最后一包
				{
					this->stop=1;
					jbl_stream_do(nxt,jbl_stream_force);
					this->extra[0].c8[0]=0;
					goto backup;
				}
			}
			else
				this->extra[1].u=head->len;
		}
		for(;i<this->en;)
		{
			jbl_stream_buf_size_type len=jbl_min(this->en-i,nxt->size-nxt->en);
			if((head->tren&JWL_HTTP_TREN_CHUNKED))jbl_min_update(len,this->extra[1].u);
			jbl_memory_copy(nxt->buf+nxt->en,this->buf+i,len);
			i+=len;
			nxt->en+=len;
			this->extra[1].u-=len;
			jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))goto backup;
			if((!this->extra[1].u))//收完了一包
			{
				if((head->tren&JWL_HTTP_TREN_CHUNKED))
				{
					this->extra[0].c8[1]=1;
					jbl_memory_copy(this->buf,this->buf+i,this->en-=i);
					i=0;
					goto start;
				}
				else
				{
					jbl_stream_do(nxt,jbl_stream_force);
					this->stop=1;
					this->extra[0].c8[0]=0;
					goto backup;
				}
			}
		}
backup:;
		jbl_memory_copy(this->buf,this->buf+i,this->en-=i);
	}
	else this->stop=1;
}
jbl_stream_operators_new(jwl_http_decode_stream_operaters,__jwl_http_decode_stream_operater,jwl_http_head_free,__jwl_http_decode_stream_usf);
jbl_stream_operators_new(jwl_http_encode_stream_operaters,__jwl_http_encode_stream_operater,jwl_http_head_free,NULL);



jwl_http_head* jwl_http_head_view_put(jwl_http_head* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jwl_http_head* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jwl_http_head",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_char(out,'\n');
	++tabs;
	char * type			[]={"response","request"};
	char * charset		[]={"","UTF-8"};
	char * method		[]={"","get","post"};
	char * protocol		[]={"","HTTP/1.1"};
	char * connection	[]={"","keep alive","close","upgrade"};
	char * cache		[]={"","no","max age,"};
	char * upgrade		[]={"","websocket"};
	char * tren			[]={"","chunked","compress","deflate","gzip","identity"};
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"type         :");jbl_stream_push_chars(out,UC type			[jwl_http_head_is_request(this)?1:0])	;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"method       :");jbl_stream_push_chars(out,UC method		[thi->method])						;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"status       :");jbl_stream_push_uint(out,thi->status)											;jbl_stream_push_char(out,'\n');
	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"length       :");jbl_stream_push_uint(out,thi->len)											;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"total_length :");jbl_stream_push_uint(out,thi->total_len)										;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"protocol     :");jbl_stream_push_chars(out,UC protocol		[thi->protocol])					;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"charset      :");jbl_stream_push_chars(out,UC charset		[thi->charset])						;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"upgrade      :");jbl_stream_push_chars(out,UC upgrade		[thi->upgrade])						;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"cache        :");jbl_stream_push_chars(out,UC cache		[thi->cache])						;if(thi->cache==JWL_HTTP_CACHE_MAX_AGE){jbl_stream_push_uint(out,thi->cache_max_age);}	;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"range        :");jbl_stream_push_uint(out,thi->range.start)									;jbl_stream_push_char(out,'-' );	jbl_stream_push_uint(out,thi->range.end)			;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"etag         :");jbl_stream_push_string(out,thi->etag)											;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"host         :");jbl_stream_push_string(out,thi->host)											;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"content type :");jbl_stream_push_chars(out,jbl_file_get_ct_chars_by_ctid(thi->content_type))	;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"filename     :");jbl_stream_push_string(out,thi->filename)										;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"user agent   :");jbl_stream_push_string(out,thi->ua)											;jbl_stream_push_char(out,'\n');	
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"referer      :");jbl_stream_push_string(out,thi->referer)										;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"url          :");jbl_stream_push_string(out,thi->url)											;jbl_stream_push_char(out,'\n');
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
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"tren         :");
	if(!thi->tren)
		jbl_stream_push_chars(out,UC connection	[0]);
	else 
	{
		if(thi->tren&JWL_HTTP_TREN_CHUNKED	)jbl_stream_push_chars(out,UC tren[1]),jbl_stream_push_char(out,',');
		if(thi->tren&JWL_HTTP_TREN_COMPRESS	)jbl_stream_push_chars(out,UC tren[2]),jbl_stream_push_char(out,',');
		if(thi->tren&JWL_HTTP_TREN_DEFLATE	)jbl_stream_push_chars(out,UC tren[3]),jbl_stream_push_char(out,',');
		if(thi->tren&JWL_HTTP_TREN_GZIP		)jbl_stream_push_chars(out,UC tren[4]),jbl_stream_push_char(out,',');
		if(thi->tren&JWL_HTTP_TREN_IDENTITY	)jbl_stream_push_chars(out,UC tren[5]),jbl_stream_push_char(out,',');
	}
	jbl_stream_push_char(out,'\n');	
			

	if(thi->parameter)
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"parameter    :");																				;jbl_stream_push_char(out,'\n');
		++tabs;
		jbl_ht_foreach(thi->parameter,i)
		{
			for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
			jbl_stream_push_string(out,i->k);
			for(jbl_int32 j=jbl_string_get_length(i->k);j<13;jbl_stream_push_char(out,' '),++j);//格式化的\t
			jbl_stream_push_char(out,':');
			if(jbl_var_is(i->v,jbl_string_operators))jbl_stream_push_string(out,i->v),jbl_stream_push_char(out,'\n');	
			else					jbl_var_view_put(i->v,out,0,tabs,0,NULL,NULL,NULL);
		}		
		--tabs;
	}
	if(thi->cookie)
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);	jbl_stream_push_chars(out,UC"cookie       :");																				;jbl_stream_push_char(out,'\n');
		++tabs;
		jbl_ht_foreach(thi->cookie,i)
		{
			for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
			jbl_stream_push_string(out,i->k);
			for(jbl_int32 j=jbl_string_get_length(i->k);j<13;jbl_stream_push_char(out,' '),++j);//格式化的\t
			jbl_stream_push_char(out,':');
			if(jbl_var_is(i->v,jbl_string_operators))jbl_stream_push_string(out,i->v),jbl_stream_push_char(out,'\n');	
			else					jbl_var_view_put(i->v,out,0,tabs,0,NULL,NULL,NULL);
		}		
		--tabs;
	}
	if(thi->v)
		jbl_ht_foreach(thi->v,i)
		{
			for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
			jbl_stream_push_string(out,i->k);
			for(jbl_int32 j=jbl_string_get_length(i->k);j<13;jbl_stream_push_char(out,' '),++j);//格式化的\t
			jbl_stream_push_char(out,':');
			if(jbl_var_is(i->v,jbl_string_operators))jbl_stream_push_string(out,i->v),jbl_stream_push_char(out,'\n');	
			else					jbl_var_view_put(i->v,out,0,tabs,0,NULL,NULL,NULL);
		}
	return this;
}
#endif




#endif
