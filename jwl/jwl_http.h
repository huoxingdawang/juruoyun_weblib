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
jbl_var_operators_extern(jwl_http_head_operators);
typedef struct __jwl_http_head_range 
{
		jbl_uint64 start;
		jbl_uint64 end;	
}jwl_http_head_range;
typedef struct __jwl_http_head//response head 响应头
{
	jbl_gc				gc;
#if JBL_VAR_ENABLE==1
	jbl_var_operators *		var_ops;
#endif
	jbl_uint32 			status:9;
	jbl_uint32			method:3;
	jbl_uint32			content_type:8;
	jbl_uint32			charset:1;
	jbl_uint32			protocol:2;
	jbl_uint32			connection:3;
	jbl_uint32 			upgrade:1;
	jbl_uint32 			cache:2;
	jbl_uint32 			tren:6;
	jbl_uint32 			cache_max_age;
	jbl_uint64 			len;
	jbl_uint64 			total_len;
	jwl_http_head_range	range;
	jbl_string *		url;
	jbl_string *		etag;
	jbl_string *		filename;	
	jbl_string *		host;
	jbl_string *		ua;
	jbl_string *		referer;
	
	
	jbl_ht *			cookie;
	jbl_ht *			v;
	jbl_ht *			parameter;
	
	struct
	{
		jbl_uint8		cond;
		jbl_uint8		state;		
		jbl_uint8		ch;
		unsigned char *	marker;
		jbl_string    *	k;
		void       *	v;
	}yy;
}jwl_http_head;
typedef enum
{
	JWL_HTTP_METHOD_UNKNOW,
	JWL_HTTP_METHOD_GET,
	JWL_HTTP_METHOD_POST,
}jwl_http_method;
typedef enum
{
	JWL_HTTP_STATUS_PROXY=1,
}jwl_http_status;
typedef enum
{
	JWL_HTTP_PROTOCOL_UNKNOW,
	JWL_HTTP_PROTOCOL_HTTP_1_1,
	JWL_HTTP_PROTOCOL_HTTPS,
}jwl_http_protocol;
typedef enum
{
	JWL_HTTP_CONNECTION_UNKNOW		=B0000_0000,
	JWL_HTTP_CONNECTION_KEEP_ALIVE	=B0000_0001,
	JWL_HTTP_CONNECTION_CLOSE		=B0000_0010,
	JWL_HTTP_CONNECTION_UPGRADE		=B0000_0100,
}jwl_http_connection;
typedef enum
{
	JWL_HTTP_CACHE_UNKNOW,
	JWL_HTTP_CACHE_NO,
	JWL_HTTP_CACHE_MAX_AGE,
}jwl_http_cache;
typedef enum
{
	JWL_HTTP_CHARSET_UNKNOW,
	JWL_HTTP_CHARSET_UTF8,
}jwl_http_charset;
typedef enum
{
	JWL_HTTP_UPGRADE_UNKNOW,
	JWL_HTTP_UPGRADE_WEBSOCKET,
}jwl_http_upgrade;
typedef enum
{
	JWL_HTTP_TREN_UNKNOW		=B0000_0000,
	JWL_HTTP_TREN_CHUNKED		=B0000_0010,
	JWL_HTTP_TREN_COMPRESS		=B0000_0100,
	JWL_HTTP_TREN_DEFLATE		=B0000_1000,
	JWL_HTTP_TREN_GZIP			=B0001_0000,
	JWL_HTTP_TREN_IDENTITY		=B0010_0000,
}jwl_http_tren;
jwl_http_head *	jwl_http_head_new			();
jwl_http_head *	jwl_http_head_free			(jwl_http_head * this);
jwl_http_head *	jwl_http_head_clear			(jwl_http_head * this);
jwl_http_head *	jwl_http_head_copy			(jwl_http_head * this);
jwl_http_head *	jwl_http_head_extend		(jwl_http_head * this,jwl_http_head **pthi);


void			jwl_http_head_encode		(jwl_http_head *head,jbl_stream *stream);
jwl_http_head *	jwl_http_head_decode		(jbl_string *buf,jbl_string_size_type *start);



jwl_http_head *		jwl_http_head_set_status			(jwl_http_head * this,jbl_uint32			status);
jwl_http_head *		jwl_http_head_set_charset			(jwl_http_head * this,jwl_http_charset		charset);
jwl_http_head *		jwl_http_head_set_content_type		(jwl_http_head * this,jbl_file_ct			content_type);
jwl_http_head *		jwl_http_head_set_method			(jwl_http_head * this,jwl_http_method		method);
jwl_http_head *		jwl_http_head_set_protocol			(jwl_http_head * this,jwl_http_protocol		protocol);
jwl_http_head *		jwl_http_head_set_connection		(jwl_http_head * this,jwl_http_connection	connection);
jwl_http_head *		jwl_http_head_set_cache				(jwl_http_head * this,jwl_http_cache		cache);
jwl_http_head *		jwl_http_head_set_upgrade			(jwl_http_head * this,jwl_http_upgrade		upgrade);
jwl_http_head *		jwl_http_head_set_tren				(jwl_http_head * this,jwl_http_tren			tren);
jwl_http_head *		jwl_http_head_set_length			(jwl_http_head * this,jbl_uint64			len);
jwl_http_head *		jwl_http_head_set_total_length		(jwl_http_head * this,jbl_uint64			len);
jwl_http_head *		jwl_http_head_set_cache_max_age		(jwl_http_head * this,jbl_uint32			cache_max_age);
jwl_http_head *		jwl_http_head_set_range				(jwl_http_head * this,jwl_http_head_range	range);
jwl_http_head *		jwl_http_head_set_etag				(jwl_http_head * this,jbl_string * etag);
jwl_http_head *		jwl_http_head_set_filename			(jwl_http_head * this,jbl_string * filename);
jwl_http_head *		jwl_http_head_set_url				(jwl_http_head * this,jbl_string * url);
jwl_http_head *		jwl_http_head_set_host				(jwl_http_head * this,jbl_string * host);
jwl_http_head *		jwl_http_head_set_ua				(jwl_http_head * this,jbl_string * ua);
jwl_http_head *		jwl_http_head_set_referer			(jwl_http_head * this,jbl_string * referer);
jwl_http_head *		jwl_http_head_set					(jwl_http_head * this,unsigned char * key,void* var);		
jwl_http_head *		jwl_http_head_set_parameter			(jwl_http_head * this,unsigned char * key,void* var);		
jwl_http_head *		jwl_http_head_set_cookie			(jwl_http_head * this,unsigned char * key,void* var);		

jbl_uint32			jwl_http_head_get_status			(jwl_http_head * this);
jwl_http_charset	jwl_http_head_get_charget			(jwl_http_head * this);
jbl_file_ct			jwl_http_head_get_content_type		(jwl_http_head * this);
jwl_http_method		jwl_http_head_get_method			(jwl_http_head * this);
jwl_http_protocol	jwl_http_head_get_protocol			(jwl_http_head * this);
jwl_http_connection	jwl_http_head_get_connection		(jwl_http_head * this);
jwl_http_cache		jwl_http_head_get_cache				(jwl_http_head * this);
jwl_http_upgrade	jwl_http_head_get_upgrade			(jwl_http_head * this);
jwl_http_tren		jwl_http_head_get_tren				(jwl_http_head * this);
jbl_uint64			jwl_http_head_get_length			(jwl_http_head * this);
jbl_uint64			jwl_http_head_get_total_length		(jwl_http_head * this);
jbl_uint32			jwl_http_head_get_cache_max_age		(jwl_http_head * this);
jwl_http_head_range	jwl_http_head_get_range				(jwl_http_head * this);
jbl_string *		jwl_http_head_get_etag				(jwl_http_head * this);
jbl_string *		jwl_http_head_get_filename			(jwl_http_head * this);
jbl_string *		jwl_http_head_get_url				(jwl_http_head * this);
jbl_string *		jwl_http_head_get_host				(jwl_http_head * this);
jbl_string *		jwl_http_head_get_ua				(jwl_http_head * this);
jbl_string *		jwl_http_head_get_referer			(jwl_http_head * this);
void    *		jwl_http_head_get					(jwl_http_head * this,unsigned char * key);		
void    *		jwl_http_head_get_parameter			(jwl_http_head * this,unsigned char * key);		
void    *		jwl_http_head_get_cookie			(jwl_http_head * this,unsigned char * key);		




#define			jwl_http_head_set_request(x)		(jbl_gc_set_user1((jwl_socket*)jbl_refer_pull(x)),x)	//设置request标记
#define			jwl_http_head_set_response(x)		(jbl_gc_reset_user1((jwl_socket*)jbl_refer_pull(x)),x)	//设置response标记
#define			jwl_http_head_is_request(x)			(jbl_gc_is_user1((jwl_socket*)jbl_refer_pull(x)))		//获取request标记
#define			jwl_http_head_is_response(x)		(!jbl_gc_is_user1((jwl_socket*)jbl_refer_pull(x)))		//获取response标记
/*******************************************************************************************/
/*                            以下函实现响应头浏览操作                                   */
/*******************************************************************************************/
#if JBL_STREAM_ENABLE==1
jwl_http_head*			jwl_http_head_view_put				(jwl_http_head* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个hash table
#define					jwl_http_head_view(x)				jwl_http_head_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个hash table
extern					const jbl_stream_operater			jwl_http_encode_stream_operaters;
extern					const jbl_stream_operater			jwl_http_decode_stream_operaters;
jbl_stream *			jwl_http_encode_stream_new			(jwl_http_head* head);
jbl_stream *			jwl_http_decode_stream_new			(jwl_http_head* head);
#define					jbl_stream_is_jwl_http_encode_stream(x)		(jbl_stream_get_ops(x)==&jwl_http_encode_stream_operaters)
#define					jbl_stream_is_jwl_http_decode_stream(x)		(jbl_stream_get_ops(x)==&jwl_http_decode_stream_operaters)
#endif

#endif
#endif