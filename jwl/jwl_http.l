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
	this->charset	=0;	
	this->content_type	=NULL;
	this->etag			=NULL;
	this->v				=NULL;
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
	tmp->v				=jbl_ht_copy(this->v);
	
	jwl_http_resh_free(this);
	if(ref)		ref->ptr=tmp;
	else		this=tmp;
	if(pthi)	*pthi=tmp;	
	return this;
}
jbl_string* jwl_http_resh_encode(jbl_string *buf,jwl_http_resh *head,jbl_string_size_type size)
{
#define rn() buf=jbl_string_add_char(jbl_string_add_char(buf,'\r'),'\n')
	if(!head)return buf;
	head=jbl_refer_pull(head);
	buf=jbl_string_extend(buf,JWL_HTTP_MAX_HEADER_LENGTH);
	buf=jbl_string_add_chars(buf,UC"HTTP/1.1 ");
	buf=jbl_string_add_uint64(buf,head->status);
	switch(head->status)
	{
		case 200:
			buf=jbl_string_add_chars(buf,UC" OK");
			break;
		case 304:
			buf=jbl_string_add_chars(buf,UC" Not Modified");
			break;
	}
	rn();
	buf=jbl_string_add_chars(buf,UC"Server: "JWL_HTTP_RESPONSE_SERVER_NAME)															,rn();
	buf=jbl_string_add_chars(buf,UC"Content-Length: ")						,buf=jbl_string_add_uint64(buf,size)					,rn();
	buf=jbl_string_add_chars(buf,UC"Accept-Ranges: bytes")																			,rn();
	if(head->cache==JWL_HTTP_CACHE_MAX_AGE)
	{
		buf=jbl_string_add_chars(buf,UC"Cache-Control: max-age=")	,buf=jbl_string_add_uint64(buf,head->cache_max_age)			,rn();
		jbl_time * t=jbl_time_add_second(jbl_time_now(NULL),head->cache_max_age);
//		buf=jbl_string_add_chars(buf,UC"Date: ")	,buf=jbl_time_to_string_format(t,buf,UC JBL_TIME_FORMAT_RFC1123)				,rn();
//		buf=jbl_string_add_chars(buf,UC"Expires: ")	,buf=jbl_time_to_string_format(t,buf,UC JBL_TIME_FORMAT_RFC1123)				,rn();
		t=jbl_time_free(t);
	}
	else
	{
		buf=jbl_string_add_chars(buf,UC"Pragma: no-cache")																			,rn();
		buf=jbl_string_add_chars(buf,UC"Cache-Control: no-cache")																	,rn();
	}
	if(head->content_type)
	{
		buf=jbl_string_add_chars(buf,UC"Content-Type: ")					,buf=jbl_string_add_string(buf,head->content_type);
		char *charset[]={"","UTF-8"};
		if(head->charset)
			buf=jbl_string_add_chars(buf,UC"; charset=")					,buf=jbl_string_add_chars(buf,UC charset[head->charset]);
		rn();
	}
	if(head->etag)
		buf=jbl_string_add_chars(buf,UC"ETag: ")							,buf=jbl_string_add_string(buf,head->etag)				,rn();
	if(head->v)
		jbl_ht_foreach(head->v,i)
		{
			buf=jbl_string_add_string(buf,jbl_htk(i));
			buf=jbl_string_add_chars(buf,UC": ");
//			buf=jbl_string_add_var(&buf,jbl_htv(i));
			rn();
		}
	
	
	
//	buf=jbl_string_add_chars(buf,UC"Connection: keep-alive");rn();
//	buf=jbl_string_add_chars(buf,UC"Keep-Alive: timeout=4");rn();
//	buf=jbl_string_add_chars(buf,UC"Proxy-Connection: keep-alive");rn();
	
	rn();
#undef rn
	return buf;
}
jwl_socket* jwl_http_send_res(jwl_socket *sock,jwl_http_resh *head,jbl_string *data)
{
	jbl_string *buf=jwl_http_resh_encode(NULL,head,jbl_string_get_length(data));
	sock=jwl_socket_send(sock,buf);
	buf=jbl_string_free(buf);
	sock=jwl_socket_send(sock,data);
	return sock;
}

jwl_http_resh * jwl_http_resh_set_status			(jwl_http_resh * this,jbl_uint16 status)			{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->status=status;return this;}
jwl_http_resh * jwl_http_resh_set_cache				(jwl_http_resh * this,jbl_uint16 cache)				{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->cache=cache  ;return this;}
jwl_http_resh * jwl_http_resh_set_charset			(jwl_http_resh * this,jbl_uint16 charset)			{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->charset=charset  ;return this;}
jwl_http_resh * jwl_http_resh_set_cache_max_age		(jwl_http_resh * this,jbl_uint32 cache_max_age)		{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);thi->cache_max_age=cache_max_age;return this;}
jwl_http_resh * jwl_http_resh_set_content_type		(jwl_http_resh * this,jbl_string * content_type)	{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);jbl_string_free(thi->content_type);thi->content_type=(content_type);return this;}
jwl_http_resh * jwl_http_resh_set_etag				(jwl_http_resh * this,jbl_string * etag)			{jwl_http_resh *thi;this=jwl_http_resh_extend(this,&thi);jbl_string_free(thi->etag);thi->etag=(etag);return this;}
//jwl_http_resh * jwl_http_resh_set					(jwl_http_resh * this,jbl_string * k, jbl_var *v);
#if JBL_STREAM_ENABLE==1
void jwl_http_resh_view_put(const jwl_http_resh* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs)
{
	if(jbl_stream_view_put_format(this=jbl_refer_pull(this),out,"jwl_http_resh ",format,str,&tabs))return;
	if(format){jbl_stream_push_char(out,'\n');for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"status       :"),jbl_stream_push_uint(out,this->status)				,jbl_stream_push_char(out,'\n');
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"cache        :"),jbl_stream_push_uint(out,this->cache)					,jbl_stream_push_char(out,'\t');
	if(this->cache==JWL_HTTP_CACHE_MAX_AGE)
		jbl_stream_push_chars(out,UC"(max-age,"),jbl_stream_push_uint(out,this->cache_max_age)			,jbl_stream_push_chars(out,UC")\n");
	else
		jbl_stream_push_chars(out,UC"(no-cache)\n");
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"content type :"),jbl_stream_push_string(out,this->content_type)		,jbl_stream_push_char(out,'\n');
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	char *charset[]={"","UTF-8"};
	jbl_stream_push_chars(out,UC"charset      :"),jbl_stream_push_chars(out,UC charset[this->charset])	,jbl_stream_push_char(out,'\n');
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}//格式化的\t和\n
	jbl_stream_push_chars(out,UC"etag         :"),jbl_stream_push_string(out,this->etag);
	if(this->v)jbl_ht_foreach(this->v,i)
	{
		if(format){jbl_stream_push_char(out,'\n');for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}//格式化的\t和\n
		jbl_stream_push_char(out,'"'),jbl_stream_push_string(out,i->k),jbl_stream_push_char(out,'"');
		jbl_stream_push_char(out,':');
		jbl_var_view_put(i->v,out,(format?(-1):0),NULL,-tabs);
	}	
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
			case JWL_HTTP_KEY_END				:goto finish;																															;break;
			case JWL_HTTP_KEY_UA				:for(i=addr-buf->s;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n';reqh->ua		=jbl_string_add_char(reqh->ua		,buf->s[i]),++i)	;--reqh->ua->len		;break;
			case JWL_HTTP_KEY_HOST				:for(i=addr-buf->s;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n';reqh->host	=jbl_string_add_char(reqh->host		,buf->s[i]),++i)	;--reqh->host->len		;break;
			case JWL_HTTP_KEY_REFERER			:for(i=addr-buf->s;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n';reqh->referer	=jbl_string_add_char(reqh->referer	,buf->s[i]),++i)	;--reqh->referer->len	;break;
			case JWL_HTTP_KEY_ETAG				:for(i=addr-buf->s;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n';reqh->etag	=jbl_string_add_char(reqh->etag		,buf->s[i]),++i)	;--reqh->etag->len		;break;
			
			case JWL_HTTP_KEY_COOKIE			:break;
			case JWL_HTTP_KEY_ACCEPT			:break;
			case JWL_HTTP_KEY_ACCEPT_ENCODING	:break;
			case JWL_HTTP_KEY_ACCEPT_LANGUAGE	:break;
			case JWL_HTTP_KEY_CACHE_CONTROL		:break;
			
			
			
			default:break;
		}
/*	
		for(;i<buf->len&&buf->s[i-1]!='\r'&&buf->s[i]!='\n';reqh->ua=jbl_string_add_char(reqh->ua,buf->s[i]),++i);
		
		for(j=(addr-s),k=0;j<n&&s[j-1]!='\r'&&s[j]!='\n';tmp[k]=s[j],++j,++k);k-=1;tmp[k]=0;i=j;




		{
			case JWL_HTTP_KEY_HOST			:	printf("host            matched:   %s\n",tmp);break;
			case JWL_HTTP_KEY_COOKIE			:	printf("cookie          matched:   %s\n",tmp);break;
			case JWL_HTTP_KEY_ACCEPT			:	printf("accept          matched:   %s\n",tmp);break;
			case JWL_HTTP_KEY_ACCEPT_ENCODING:	printf("accept encoding matched:   %s\n",tmp);break;
			case JWL_HTTP_KEY_ACCEPT_LANGUAGE:	printf("accept language matched:   %s\n",tmp);break;
			case JWL_HTTP_KEY_CACHE_CONTROL	:	printf("cache control   matched:   %s\n",tmp);break;
		}
*/
		
	}
finish:;
	if(start)*start=i;
	
	
	return reqh;
}








#if JBL_STREAM_ENABLE==1
void jwl_http_reqh_view_put(const jwl_http_reqh* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs)
{
	if(jbl_stream_view_put_format(this=jbl_refer_pull(this),out,"jwl_http_reqh ",format,str,&tabs))return;
	if(format){jbl_stream_push_char(out,'\n');for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	char * method[]={"unknow","get","post"};
	char * protocol[]={"unknow","HTTP"};
	jbl_stream_push_chars(out,UC"method       :"),jbl_stream_push_chars(out,UC method[this->method])	,jbl_stream_push_char(out,'\n');	
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"url          :"),jbl_stream_push_string(out,this->url)					,jbl_stream_push_char(out,'\n');	
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"protocol     :"),jbl_stream_push_chars(out,UC protocol[this->protocol]),jbl_stream_push_char(out,'\n');	
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"user agent   :"),jbl_stream_push_string(out,this->ua)					,jbl_stream_push_char(out,'\n');	
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"host         :"),jbl_stream_push_string(out,this->host)				,jbl_stream_push_char(out,'\n');	
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"referer      :"),jbl_stream_push_string(out,this->referer)				,jbl_stream_push_char(out,'\n');	
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"etag         :"),jbl_stream_push_string(out,this->etag)				,jbl_stream_push_char(out,'\n');	
/*
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"cache        :"),jbl_stream_push_uint(out,this->cache),jbl_stream_push_char(out,'\t');
	if(this->cache==JWL_HTTP_CACHE_MAX_AGE)
		jbl_stream_push_chars(out,UC"(max-age,"),jbl_stream_push_uint(out,this->cache_max_age),jbl_stream_push_chars(out,UC")\n");
	else
		jbl_stream_push_chars(out,UC"(no-cache)\n");
	jbl_stream_push_chars(out,UC"content type :"),jbl_stream_push_string(out,this->content_type),jbl_stream_push_char(out,'\n');
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}
	jbl_stream_push_chars(out,UC"charset      :");
	switch(this->charset)
	{
		case JWL_HTTP_CHARSET_UTF8:jbl_stream_push_chars(out,UC"UTF-8");break;
	}	
	jbl_stream_push_char(out,'\n');
	if(format){for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}//格式化的\t和\n
	jbl_stream_push_chars(out,UC"etag         :"),jbl_stream_push_string(out,this->etag);
	if(this->v)jbl_ht_foreach(this->v,i)
	{
		if(format){jbl_stream_push_char(out,'\n');for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);}//格式化的\t和\n
		jbl_stream_push_char(out,'"'),jbl_stream_push_string(out,i->k),jbl_stream_push_char(out,'"');
		jbl_stream_push_char(out,':');
		jbl_var_view_put(i->v,out,(format?(-1):0),NULL,-tabs);
	}
*/	
}
#endif

/*
#define sg(x,y)	jbl_string_get2(x,y)
#define _(x)	this->status=0;	\
				this->cache=0;\
				this->cache_max_age=0;\
				jbl_string_##x(&this->content_type);\
				jbl_string_##x(&this->etag);\
				jbl_hash_table_##x(&this->v);
inline void jwl_http_response_head_init(jwl_http_response_head *this){_(init);}
inline void jwl_http_response_head_free(jwl_http_response_head *this){_(free);}
inline void jwl_http_response_head_clear(jwl_http_response_head *this){_(clear);}
#undef _
#define _(x)	this->method=0;	\
				this->protocol=0;\
				this->connection=0;\
				this->cache=0;\
				this->cache_max_age=0;\
				jbl_string_##x(&this->url);\
				jbl_string_##x(&this->host);\
				jbl_string_##x(&this->ua);\
				jbl_string_##x(&this->referer);\
				jbl_string_##x(&this->etag);\
				jbl_hash_table_##x(&this->cookie);\
				jbl_hash_table_##x(&this->v);
inline void jwl_http_request_head_init(jwl_http_request_head *this){_(init);}
inline void jwl_http_request_head_free(jwl_http_request_head *this){_(free);}
inline void jwl_http_request_head_clear(jwl_http_request_head *this){_(clear);}
#undef _
void jwl_http_send_response_head(jbl_socket_handle *client,jwl_http_response_head *head,jbl_string *data)
{
#define rn() jbl_string_add_char1(&buf,'\r'),jbl_string_add_char1(&buf,'\n')
	if(client==NULL||head==NULL||data==NULL)jbl_exception(JBL_ERROR_NULL_POINTER);	
	jbl_string buf;jbl_string_init(&buf);jbl_string_extend_to(&buf,JWL_HTTP_MAX_HEADER_LENGTH);
	jbl_string_add_chars(&buf,"HTTP/1.1 ");
	jbl_string_add_uint64(&buf,head->status);
	switch(head->status)
	{
		case 200:
			jbl_string_add_chars(&buf," OK");
			break;
		case 304:
			jbl_string_add_chars(&buf," Not Modified");
			break;
	}
	rn();
	jbl_string_add_chars(&buf,"Server:"JWL_HTTP_RESPONSE_SERVER_NAME"\r\n");
	jbl_string_add_chars(&buf,"Accept-Ranges: bytes\r\n");
	jbl_string_add_chars(&buf,"Content-Length: ");jbl_string_add_uint64(&buf,jbl_string_get_length(data));rn();
	jbl_string_add_chars(&buf,"Connection: close\r\n");
	jbl_string_add_chars(&buf,"Expires: Sat, 26 Jul 1997 05:00:00 GMT\r\n");
	jbl_string_add_chars(&buf,"Content-Type: ");jbl_string_add_string(&buf,&head->content_type);rn();
	jbl_string_add_chars(&buf,"ETag: ");jbl_string_add_string(&buf,&head->etag);rn();
	jbl_string_add_chars(&buf,"Cache-Control: public, ");if(head->cache==JWL_HTTP_CACHE_MAX_AGE)jbl_string_add_chars(&buf,"max-age="),jbl_string_add_uint64(&buf,head->cache_max_age);else jbl_string_add_chars(&buf,"no-cache");rn();
	jbl_hash_table_foreach(&head->v,i)
	{
		jbl_string_add_string(&buf,jbl_hash_table_get_key(i));
		jbl_string_add_char1(&buf,':');jbl_string_add_char1(&buf,' ');
		jbl_string_add_var(&buf,jbl_hash_table_get_var(i));
		rn();
	}	
	rn();
	jwl_socket_send(client,&buf);
	jbl_string_free(&buf);
#undef rn
}
void jwl_http_send_response(jbl_socket_handle *client,jwl_http_response_head *head,jbl_string *data)
{
	if(client==NULL||head==NULL||data==NULL)jbl_exception(JBL_ERROR_NULL_POINTER);	
	jwl_http_send_response_head(client,head,data);
	jwl_socket_send(client,data);
}

jbl_uint8 jwl_http_get_request_head(jbl_socket_handle *client,jwl_http_request_head *head,jbl_string *remain_data)
{
	if(client==NULL||head==NULL||remain_data==NULL)jbl_exception(JBL_ERROR_NULL_POINTER);	
	jbl_string tmp1,tmp2,tmp3,tmp4,tmp5;jbl_string_init(&tmp1);jbl_string_init(&tmp2);jbl_string_init(&tmp3);jbl_string_init(&tmp4);jbl_string_init(&tmp5);
	jwl_socket_receive_length(client,&tmp1,JWL_HTTP_MAX_HEADER_LENGTH);
	jbl_string_size_type i=0,n=jbl_string_get_length(&tmp1),j,ii;
	jbl_uint8 flag=(n==JWL_HTTP_MAX_HEADER_LENGTH);
	static const jbl_string cn=jbl_string_const("Connection"			,10);
	static const jbl_string ua=jbl_string_const("User-Agent"			,10);
	static const jbl_string ho=jbl_string_const("Host"				,4);
	static const jbl_string re=jbl_string_const("Referer"				,7);
	static const jbl_string pr=jbl_string_const("Pragma"				,6);
	static const jbl_string cc=jbl_string_const("Cache-Control"		,13);
	static const jbl_string co=jbl_string_const("Cookie"				,6);
	static const jbl_string et=jbl_string_const("ETag"				,4);
	static const jbl_string nm=jbl_string_const("If-None-Match"		,13);
	static const jbl_string or=jbl_string_const("Origin"				,6);
	
	static const jbl_string ac=jbl_string_const("Accept"				,6);
	static const jbl_string ae=jbl_string_const("Accept-Encoding"		,15);
	static const jbl_string al=jbl_string_const("Accept-Language"		,15);
	static const jbl_string uir=jbl_string_const("Upgrade-Insecure-Requests",25);
method_start:
	if(((i+2)<n)&&sg(&tmp1,i+0)=='G'&&sg(&tmp1,i+1)=='E'&&sg(&tmp1,i+2)=='T')
		head->method=JWL_HTTP_METHOD_GET,i+=3;
	else if(((i+3)<n)&&sg(&tmp1,i+0)=='P'&&sg(&tmp1,i+1)=='O'&&sg(&tmp1,i+2)=='S'&&sg(&tmp1,i+1)=='T')
		head->method=JWL_HTTP_METHOD_POST,i+=4;
	else
		goto fail;
	++i;
url_start:
	j=jbl_string_find_char_start(&tmp1,' ',i);
	jbl_string_equal_chars_length(&head->url,jbl_string_get_chars(&tmp1)+i,j-i);
	i=j+1;
protocol_start:
	if(((i+8)<n)&&	sg(&tmp1,i+0)=='H'&&sg(&tmp1,i+1)=='T'&&sg(&tmp1,i+2)=='T'&&sg(&tmp1,i+3)=='P'&&sg(&tmp1,i+4)=='/'&&
					sg(&tmp1,i+5)=='1'&&sg(&tmp1,i+6)=='.'&&sg(&tmp1,i+7)=='1')
		head->protocol=JWL_HTTP_PROTOCOL_HTTP,i+=8;
	else
		goto fail;
	i+=2;
value_start:
	while((i+4)<n&&(!(sg(&tmp1,i)=='\r'&&sg(&tmp1,i+1)=='\n'&&sg(&tmp1,i+2)=='\r'&&sg(&tmp1,i+3)=='\n')))
	{
		j=jbl_string_find_char_start(&tmp1,':',i);
		jbl_string_equal_chars_length(&tmp2,jbl_string_get_chars(&tmp1)+i,j-i);
		i=j+1;
		while(i<n&&(sg(&tmp1,i)==' '))++i;
		j=jbl_string_find_char_start(&tmp1,'\r',i);
		jbl_string_equal_chars_length(&tmp3,jbl_string_get_chars(&tmp1)+i,j-i);
		i=j+2;
		if(jbl_string_if_equal(&tmp2,&cn))
		{
			ii=jbl_string_get_length(&tmp3);
			if((9<ii)&&	sg(&tmp3,0)=='k'&&sg(&tmp3,1)=='e'&&sg(&tmp3,2)=='e'&&sg(&tmp3,3)=='p'&&sg(&tmp3,4)=='-'&&
						sg(&tmp3,5)=='a'&&sg(&tmp3,6)=='l'&&sg(&tmp3,7)=='i'&&sg(&tmp3,8)=='v'&&sg(&tmp3,9)=='e')
				head->connection=JWL_HTTP_CONNECTION_KEEP_ALIVE;
			else if((4<ii)&&sg(&tmp3,0)=='c'&&sg(&tmp3,1)=='l'&&sg(&tmp3,2)=='o'&&sg(&tmp3,3)=='s'&&sg(&tmp3,4)=='e')
				head->connection=JWL_HTTP_CONNECTION_CLOSE;
			else if((6<ii)&&sg(&tmp3,0)=='U'&&sg(&tmp3,1)=='p'&&sg(&tmp3,2)=='g'&&sg(&tmp3,3)=='r'&&sg(&tmp3,4)=='a'&&
							sg(&tmp3,5)=='d'&&sg(&tmp3,6)=='e')
				head->connection=JWL_HTTP_CONNECTION_UPGRADE;				
			else 
				goto fail;
		}
		else if(jbl_string_if_equal(&tmp2,&ua))
			jbl_string_equal_move(&head->ua,&tmp3);
		else if(jbl_string_if_equal(&tmp2,&ho))
			jbl_string_equal_move(&head->host,&tmp3);
		else if(jbl_string_if_equal(&tmp2,&re)||jbl_string_if_equal(&tmp2,&or))
			jbl_string_equal_move(&head->referer,&tmp3);
		else if(jbl_string_if_equal(&tmp2,&et)||jbl_string_if_equal(&tmp2,&nm))
			jbl_string_equal_move(&head->etag,&tmp3);		
		else if(jbl_string_if_equal(&tmp2,&pr)||jbl_string_if_equal(&tmp2,&cc))
		{
			ii=jbl_string_get_length(&tmp3);
			if((7<ii)&&	sg(&tmp3,0)=='n'&&sg(&tmp3,1)=='o'&&sg(&tmp3,2)=='-'&&sg(&tmp3,3)=='c'&&sg(&tmp3,4)=='a'&&
						sg(&tmp3,5)=='c'&&sg(&tmp3,6)=='h'&&sg(&tmp3,7)=='e')
				head->cache=JWL_HTTP_CACHE_NO;
			else if ((7<ii)&&	sg(&tmp3,0)=='m'&&sg(&tmp3,1)=='a'&&sg(&tmp3,2)=='x'&&sg(&tmp3,3)=='-'&&sg(&tmp3,4)=='a'&&
								sg(&tmp3,5)=='g'&&sg(&tmp3,6)=='e'&&sg(&tmp3,7)=='=')
				head->cache=JWL_HTTP_CACHE_MAX_AGE,head->cache_max_age=jbl_string_get_uint64(&tmp3);
			else 
				goto fail;			
		}	
		else if(jbl_string_if_equal(&tmp2,&co))
		{
			ii=jbl_string_get_length(&tmp3);
			jbl_var v;jbl_var_init(&v);
			for(register jbl_string_size_type i1=0,j1;i1<ii;i1++)
			{
				while(i1<ii&&(sg(&tmp3,i1)==' '))++i1;
				j1=jbl_string_find_char_start(&tmp3,'=',i1);
				jbl_string_equal_chars_length(&tmp4,jbl_string_get_chars(&tmp3)+i1,j1-i1);
				i1=j1+1;
				j1=jbl_string_find_char_start(&tmp3,';',i1);
				jbl_string_equal_chars_length(&tmp5,jbl_string_get_chars(&tmp3)+i1,j1-i1);
				i1=j1+1;
				jbl_var_equal_string(&v,&tmp5,move);
				jbl_hash_table_insert(&head->cookie,&tmp4,&v,move,move);				
			}
			jbl_var_free(&v);
		}
		else if(jbl_string_if_equal(&tmp2,&ac)||jbl_string_if_equal(&tmp2,&ae)||jbl_string_if_equal(&tmp2,&al)||jbl_string_if_equal(&tmp2,&uir)){}
		else
		{
			jbl_var v;jbl_var_init(&v);
			jbl_var_equal_string(&v,&tmp3,move);
			jbl_hash_table_insert(&head->v,&tmp2,&v,move,move);
			jbl_var_free(&v);
		}
	}
	i+=4;
	
	
	
	
	
	
	
	
	
	goto success;
fail:
	jwl_http_request_head_clear(head);
	i=0;
success:
	if(i<n)
		jbl_string_add_chars_length(remain_data,jbl_string_get_chars(&tmp1)+i,n-i);
	jbl_string_free(&tmp1);jbl_string_free(&tmp2);jbl_string_free(&tmp3);jbl_string_free(&tmp4);jbl_string_free(&tmp5);
	return flag;
}

jbl_uint8 jwl_http_send_request_head(jbl_socket_handle *client,jwl_http_request_head *head)
{
	if(client==NULL||head==NULL)jbl_exception(JBL_ERROR_NULL_POINTER);
#define rn() jbl_string_add_char1(&buf,'\r'),jbl_string_add_char1(&buf,'\n')
	jbl_string buf;jbl_string_init(&buf);jbl_string_extend_to(&buf,JWL_HTTP_MAX_HEADER_LENGTH);
	if(head->method==JWL_HTTP_METHOD_POST)				jbl_string_add_chars(&buf,"POST ");
	else if(head->method==JWL_HTTP_METHOD_GET)			jbl_string_add_chars(&buf,"GET ");
	else goto fail;
	jbl_string_add_string(&buf,&head->url);
	jbl_string_add_char1(&buf,' ');
	if(head->protocol==JWL_HTTP_PROTOCOL_HTTP)			jbl_string_add_chars(&buf,"HTTP/1.1");
	else goto fail;	
	rn();
	jbl_string_add_chars(&buf,"Connection: ");
	if(head->method==JWL_HTTP_CONNECTION_CLOSE)			jbl_string_add_chars(&buf,"close");
	else if(head->method==JWL_HTTP_CONNECTION_KEEP_ALIVE)jbl_string_add_chars(&buf,"keep-alive");
	else goto fail;	
	rn();
	jbl_string_add_chars(&buf,"Cache-Control: ");
	if(head->cache==JWL_HTTP_CACHE_NO)					jbl_string_add_chars(&buf,"no-cache");
	else if(head->cache==JWL_HTTP_CACHE_MAX_AGE)			jbl_string_add_chars(&buf,"max-age="),jbl_string_add_uint64(&buf,head->cache_max_age);
	else goto fail;
	rn();
	if(jbl_string_get_length(&head->host)>0)			jbl_string_add_chars(&buf,"Host: ")		,jbl_string_add_string(&buf,&head->host)			,rn();
	if(jbl_string_get_length(&head->ua)>0)	jbl_string_add_chars(&buf,"User-Agent: ")	,jbl_string_add_string(&buf,&head->ua)	,rn();
//	jbl_string_add_chars(&buf,"Cookie: ");jbl_string_add_string(&buf,&head->cookie);rn();
	rn();
success:
	jwl_socket_send(client,&buf);	
fail:
		
	jbl_string_free(&buf);
#undef rn
}

void jwl_request_head_view_ex(jwl_http_request_head *this,FILE * file,char*str,int a,int tabs)
{
	if(this==NULL||file==NULL||str==NULL)jbl_exception(JBL_ERROR_NULL_POINTER);
	if(tabs>=0)
		for(register int i=0;i<tabs;++i,putc('\t',file));
	else
		tabs=-tabs;
	if(a>=0)
		fprintf(file,"jbl_request_head    %s %d:",str,a);
	else
		fprintf(file,"jbl_request_head     \t:");	
	++tabs;
	putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));	fprintf(file,"method:");	
	if(this->method==JWL_HTTP_METHOD_GET)			fprintf(file,"get");
	else if(this->method==JWL_HTTP_METHOD_POST)		fprintf(file,"post");
	else												fprintf(file,"unknow");
	putc('\n',file);

	for(register int i=0;i<tabs;++i,putc('\t',file));	fprintf(file,"protocol:");
	if(this->protocol==JWL_HTTP_PROTOCOL_HTTP)		fprintf(file,"http");
	else if(this->protocol==JWL_HTTP_PROTOCOL_HTTPS)	fprintf(file,"https");
	else												fprintf(file,"unknow");
	putc('\n',file);
	
	for(register int i=0;i<tabs;++i,putc('\t',file));		fprintf(file,"connection:");
	if(this->connection==JWL_HTTP_CONNECTION_KEEP_ALIVE)	fprintf(file,"keep-alive");
	if(this->connection==JWL_HTTP_CONNECTION_UPGRADE)	fprintf(file,"upgrade");
	else													fprintf(file,"unknow");
	putc('\n',file);
	
	for(register int i=0;i<tabs;++i,putc('\t',file));		fprintf(file,"cache_control:");
	if(this->cache==JWL_HTTP_CACHE_NO)					fprintf(file,"no");
	else if(this->cache==JWL_HTTP_CACHE_MAX_AGE)			fprintf(file,"max_age:%d",this->cache_max_age);
	else													fprintf(file,"unknow");
	putc('\n',file);	
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"url:")		;jbl_string_print(&this->url,file)			;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"host:")		;jbl_string_print(&this->host,file)			;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"ua:");jbl_string_print(&this->ua,file)	;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"referer:")	;jbl_string_print(&this->referer,file)		;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"etag:")		;jbl_string_print(&this->etag,file)		;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"cookie:\n");	
	jbl_hash_table_foreach(&this->cookie,i)
	{
		for(register int i=0;i<tabs+1;++i,putc('\t',file));	
		jbl_string_print(jbl_hash_table_get_key(i),file);
		putc(':',file);
		jbl_var_view_ex(jbl_hash_table_get_var(i),file,"",-1,-(tabs+2));
	}	
	jbl_hash_table_foreach(&this->v,i)
	{
		for(register int i=0;i<tabs;++i,putc('\t',file));	
		jbl_string_print(jbl_hash_table_get_key(i),file);
		putc(':',file);
		jbl_var_view_ex(jbl_hash_table_get_var(i),file,"",-1,-(tabs+1));
	}
	

	
}
*/
#endif
