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
typedef struct __jry_wl_http_response_head
{
	jry_bl_uint16 status;
	jry_bl_string content_type;
	
	
}jry_wl_http_response_head;
#define	jry_wl_http_response_head_const(status,content_type)	{status,content_type}

void jry_wl_http_send_respose_head(jry_bl_socket_handle *client,jry_wl_http_response_head *head,jry_bl_string *data);
void jry_wl_http_send_respose(jry_bl_socket_handle *client,jry_wl_http_response_head *head,jry_bl_string *data);



#endif
#endif