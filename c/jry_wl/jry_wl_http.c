/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_wl_http.h"
#if JRY_WL_HTTP_ENABLE==1
void jry_wl_http_send_respose_head(jry_bl_socket_handle *client,jry_wl_http_response_head *head,jry_bl_string *data)
{
	jry_bl_string buf;jry_bl_string_init(&buf);jry_bl_string_extend_to(&buf,1024);
	jry_bl_string_add_char_pointer(&buf,"HTTP/1.1 ");
	jry_bl_string_add_uint64(&buf,head->status);
	switch(head->status)
	{
		case 200:
			jry_bl_string_add_char_pointer(&buf,"OK");
			break;
	}
	jry_bl_string_add_char_pointer(&buf,"\r\n");
	jry_bl_string_add_char_pointer(&buf,"Server:"JRY_WL_HTTP_RESPONSE_SERVER_NAME"\r\n");
	jry_bl_string_add_char_pointer(&buf,"Accept-Ranges: bytes\r\n");
	jry_bl_string_add_char_pointer(&buf,"Content-Length: ");jry_bl_string_add_uint64(&buf,jry_bl_string_get_length(data));jry_bl_string_add_char_pointer(&buf,"\r\n");
	jry_bl_string_add_char_pointer(&buf,"Connection: close\r\n");
	jry_bl_string_add_char_pointer(&buf,"Content-Type: ");jry_bl_string_add_string(&buf,&head->content_type);jry_bl_string_add_char_pointer(&buf,"\r\n");
	jry_bl_string_add_char_pointer(&buf,"\r\n");
	jry_wl_socket_send(client,&buf);	
}
void jry_wl_http_send_respose(jry_bl_socket_handle *client,jry_wl_http_response_head *head,jry_bl_string *data)
{
	jry_wl_http_send_respose_head(client,head,data);
	jry_wl_socket_send(client,data);
}

#endif
