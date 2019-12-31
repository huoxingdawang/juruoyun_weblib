/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_WL_HTTP_H
#define __JRY_WL_HTTP_H
#include "jry_wl_http_config.h"
#if JRY_WL_HTTP_ENABLE==1
#include "../jry_bl/jry_bl_include.h"
#include "jry_wl_ying.h"
#include "jry_wl_exception.h"
#include "jry_wl_socket.h"
#include "jry_wl_http_content.h"
#define JRY_WL_HTTP_METHOD_UNKNOW	0
#define JRY_WL_HTTP_METHOD_GET		1
#define JRY_WL_HTTP_METHOD_POST		2
#define JRY_WL_HTTP_PROTOCOL_UNKNOW		0
#define JRY_WL_HTTP_PROTOCOL_HTTP		1
#define JRY_WL_HTTP_PROTOCOL_HTTPS		2
#define JRY_WL_HTTP_CONNECTION_UNKNOW		0
#define JRY_WL_HTTP_CONNECTION_KEEP_ALIVE	1
#define JRY_WL_HTTP_CACHE_UNKNOW	0
#define JRY_WL_HTTP_CACHE_NO		1
#define JRY_WL_HTTP_CACHE_MAX_AGE	2

#if JRY_BL_USE_STDIO==1
#include <stdio.h>
#endif

typedef struct __jry_wl_http_response_head
{
	jry_bl_uint16 status:9;
	jry_bl_uint32 cache:2;
	
	
	jry_bl_uint32 cache_max_age;	
	jry_bl_string content_type;
	jry_bl_string etag;
	
	
	jry_bl_hash_table v;
}jry_wl_http_response_head;
typedef struct __jry_wl_http_request_head
{
	jry_bl_uint32 method:3;
	jry_bl_uint32 protocol:2;
	jry_bl_uint32 connection:1;
	jry_bl_uint32 cache:2;
	
	
	jry_bl_uint32 cache_max_age;
	
	jry_bl_string url;
	jry_bl_string host;
	jry_bl_string user_agent;
	jry_bl_string referer;
	jry_bl_string etag;
	jry_bl_hash_table cookie;
		
	jry_bl_hash_table v;
}jry_wl_http_request_head;

void 			jry_wl_http_response_head_init				(jry_wl_http_response_head *this);
void 			jry_wl_http_response_head_free				(jry_wl_http_response_head *this);
void 			jry_wl_http_response_head_clear				(jry_wl_http_response_head *this);
void 			jry_wl_http_request_head_init				(jry_wl_http_request_head *this);
void 			jry_wl_http_request_head_free				(jry_wl_http_request_head *this);
void 			jry_wl_http_request_head_clear				(jry_wl_http_request_head *this);

#define			jry_wl_http_response_head_const(s,ct)					{s,ct}
#define			jry_wl_http_response_head_set_status(this,s)			(this)->status=(s)	
#define			jry_wl_http_response_head_set_content_type(this,ct)		jry_bl_string_equal_char_pointer_light(&(this)->content_type,(ct))
#define			jry_wl_http_response_head_set_cache(this,a)				(this)->cache=(a)
#define			jry_wl_http_response_head_set_max_age(this,a)			jry_wl_http_response_head_set_cache(this,JRY_WL_HTTP_CACHE_MAX_AGE),(this)->cache_max_age=(a)
#define			jry_wl_http_response_head_set_etag(this,et,cpt)			jry_bl_string_copy(&(this)->etag,(et),cpt)
#define			jry_wl_http_response_head_set_etag_str(this,et)			jry_bl_string_equal_char_pointer_light(&(this)->etag,(et))
#define			jry_wl_http_response_head_set(this,a,b,c,d)				jry_bl_hash_table_insert((&(this)->v),a,b,c,d)
#define			jry_wl_http_response_head_set_str(this,a,b,c,d,e)		jry_bl_hash_table_insert_str((&(this)->v),a,b,c,d,e)


void 			jry_wl_http_send_response_head				(jry_bl_socket_handle *client,jry_wl_http_response_head *head,jry_bl_string *data);
void 			jry_wl_http_send_response					(jry_bl_socket_handle *client,jry_wl_http_response_head *head,jry_bl_string *data);

jry_bl_uint8	jry_wl_http_get_request_head				(jry_bl_socket_handle *client,jry_wl_http_request_head *head,jry_bl_string *remain_data,jry_bl_uint8 enable_ex);

#define			jry_wl_http_request_head_get_method(x)		((x)->method)
#define			jry_wl_http_request_head_get_protocol(x)	((x)->protocol)
#define			jry_wl_http_request_head_get_connection(x)	((x)->connection)
#define			jry_wl_http_request_head_get_url(x)			(&((x)->url))
#define			jry_wl_http_request_head_get_etag(x)		(&((x)->etag))
#define			jry_wl_http_request_head_get_host(x)		(&((x)->host))
#define			jry_wl_http_request_head_get_user_agent(x)	(&((x)->user_agent))
#define			jry_wl_http_request_head_get_reffer(x)		(&((x)->referer))
#define			jry_wl_http_request_head_get(x,y,z,a)		jry_bl_hash_table_get(&(x)->v,y,z,a)

#if JRY_BL_USE_STDIO==1
#define			jry_wl_request_head_view(x,y) 				jry_wl_request_head_view_ex(x,y,#x " @ "__FILE__,__LINE__,jry_bl_view_default_tabs_num)
void 			jry_wl_request_head_view_ex					(jry_wl_http_request_head *this,FILE * file,char*str,int a,int tabs);
#endif

#endif
#endif