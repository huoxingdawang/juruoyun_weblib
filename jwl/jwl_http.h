/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JWL_HTTP_H
#define __JWL_HTTP_H
#include "jwl_http_config.h"
#if JWL_HTTP_ENABLE==1
#include "jwl_ying.h"
#include "jwl_socket.h"
#include "jwl_http_content.h"
#define JWL_HTTP_CONNECTION_UNKNOW		0
#define JWL_HTTP_CONNECTION_KEEP_ALIVE	1
#define JWL_HTTP_CONNECTION_CLOSE		2
#define JWL_HTTP_CONNECTION_UPGRADE		3
#define JWL_HTTP_CACHE_UNKNOW			0
#define JWL_HTTP_CACHE_NO				1
#define JWL_HTTP_CACHE_MAX_AGE			2
#define JWL_HTTP_CHARSET_UNKNOW			0
#define JWL_HTTP_CHARSET_UTF8			1


typedef struct __jwl_http_resh//response head 响应头
{
	jbl_gc			gc;
	jbl_uint32 		status:9;
	jbl_uint32 		cache:2;
	jbl_uint32		charset:1;
	jbl_uint32 		cache_max_age;	
	
	
	jbl_string *	content_type;
	jbl_string *	etag;
	
	
	jbl_ht *		v;
}jwl_http_resh;
typedef struct __jwl_http_reqh//request head 请求头
{
	jbl_gc			gc;
	jbl_uint32		method:3;
	jbl_uint32		protocol:2;
	jbl_uint32		connection:2;
	jbl_uint32		cache:2;
	jbl_uint32		cache_max_age;
	
	
	
	jbl_string *	url;
	jbl_string *	host;
	jbl_string *	ua;
	jbl_string *	referer;
	jbl_string *	etag;
	
	
	jbl_ht *		cookie;
	jbl_ht *		v;
}jwl_http_reqh;
typedef enum
{
	JWL_HTTP_KEY_UNDEFINED,
	JWL_HTTP_KEY_END,
	JWL_HTTP_KEY_UA,
	JWL_HTTP_KEY_HOST,
	JWL_HTTP_KEY_REFERER,
	JWL_HTTP_KEY_ETAG,
	JWL_HTTP_KEY_COOKIE,
	JWL_HTTP_KEY_ACCEPT,
	JWL_HTTP_KEY_ACCEPT_ENCODING,
	JWL_HTTP_KEY_ACCEPT_LANGUAGE,
	JWL_HTTP_KEY_CACHE_CONTROL,
}jwl_http_key;
typedef enum
{
	JWL_HTTP_METHOD_UNKNOW,
	JWL_HTTP_METHOD_GET,
	JWL_HTTP_METHOD_POST,
}jwl_http_method;
typedef enum
{
	JWL_HTTP_PROTOCOL_UNKNOW,
	JWL_HTTP_PROTOCOL_HTTP,
	JWL_HTTP_PROTOCOL_HTTPS,
}jwl_http_protocol;
jwl_http_resh *	jwl_http_resh_new			();
jwl_http_resh *	jwl_http_resh_init			(jwl_http_resh * this);
jwl_http_resh *	jwl_http_resh_free			(jwl_http_resh * this);
jwl_http_resh *	jwl_http_resh_copy			(jwl_http_resh * this);
jwl_http_resh *	jwl_http_resh_extend		(jwl_http_resh * this,jwl_http_resh **pthi);


jbl_string *	jwl_http_resh_encode		(jbl_string *buf,jwl_http_resh *head,jbl_string_size_type size);
jwl_socket *	jwl_http_send_res			(jwl_socket *sock,jwl_http_resh *head,jbl_string *data);

jwl_http_resh *	jwl_http_resh_set_status			(jwl_http_resh * this,jbl_uint16 status);
jwl_http_resh *	jwl_http_resh_set_cache				(jwl_http_resh * this,jbl_uint16 cache);
jwl_http_resh *	jwl_http_resh_set_charset			(jwl_http_resh * this,jbl_uint16 charset);
jwl_http_resh *	jwl_http_resh_set_cache_max_age		(jwl_http_resh * this,jbl_uint32 cache_max_age);
jwl_http_resh *	jwl_http_resh_set_content_type		(jwl_http_resh * this,jbl_string * content_type);
jwl_http_resh *	jwl_http_resh_set_etag				(jwl_http_resh * this,jbl_string * etag);
//jwl_http_resh *	jwl_http_resh_set					(jwl_http_resh * this,jbl_string * k, jbl_var *v);
/*******************************************************************************************/
/*                            以下函实现响应头浏览操作                                   */
/*******************************************************************************************/
#if JBL_STREAM_ENABLE==1
jwl_http_resh*			jwl_http_resh_view_put				(jwl_http_resh* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个hash table
#define					jwl_http_resh_view(x)				jwl_http_resh_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个hash table
#endif



jwl_http_reqh *	jwl_http_reqh_new		();
jwl_http_reqh *	jwl_http_reqh_init		(jwl_http_reqh * this);
jwl_http_reqh *	jwl_http_reqh_free		(jwl_http_reqh * this);
jwl_http_reqh *	jwl_http_reqh_extend	(jwl_http_reqh * this,jwl_http_reqh **pthi);
jwl_http_reqh *	jwl_http_reqh_decode	(jbl_string *buf,jbl_string_size_type *start);

#if JBL_STREAM_ENABLE==1
jwl_http_reqh*			jwl_http_reqh_view_put				(jwl_http_reqh* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个hash table
#define					jwl_http_reqh_view(x)				jwl_http_reqh_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个hash table
#endif



/*

#include <stdio.h>

void 			jwl_http_response_head_init				(jwl_http_response_head *this);
void 			jwl_http_response_head_free				(jwl_http_response_head *this);
void 			jwl_http_response_head_clear				(jwl_http_response_head *this);
void 			jwl_http_request_head_init				(jwl_http_request_head *this);
void 			jwl_http_request_head_free				(jwl_http_request_head *this);
void 			jwl_http_request_head_clear				(jwl_http_request_head *this);

#define			jwl_http_response_head_const(s,ct)					{s,ct}
#define			jwl_http_response_head_set_status(this,s)			(this)->status=(s)	
#define			jwl_http_response_head_set_content_type(this,ct)		jbl_string_equal_chars_light(&(this)->content_type,(ct))
#define			jwl_http_response_head_set_cache(this,a)				(this)->cache=(a)
#define			jwl_http_response_head_set_max_age(this,a)			jwl_http_response_head_set_cache(this,JWL_HTTP_CACHE_MAX_AGE),(this)->cache_max_age=(a)
#define			jwl_http_response_head_set_etag(this,et,cpt)			jbl_string_copy(&(this)->etag,(et),cpt)
#define			jwl_http_response_head_set_etag_str(this,et)			jbl_string_equal_chars_light(&(this)->etag,(et))
#define			jwl_http_response_head_set(this,a,b,c,d)				jbl_hash_table_insert((&(this)->v),a,b,c,d)
#define			jwl_http_response_head_set_str(this,a,b,c,d,e)		jbl_hash_table_insert_str((&(this)->v),a,b,c,d,e)


*/
#endif
#endif