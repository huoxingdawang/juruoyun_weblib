/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JWL_WEBSOCKET_H
#define __JWL_WEBSOCKET_H
#include "jwl_websocket_config.h"
#if JWL_WEBSOCKET_ENABLE==1
#include "jwl_ying.h"
#include "jwl_http.h"
typedef enum
{
	JWL_WEBSOCKET_STATUS_EXTRA	=0X0,
	JWL_WEBSOCKET_STATUS_TEXT	=0X1,
	JWL_WEBSOCKET_STATUS_BIN	=0X2,
	JWL_WEBSOCKET_STATUS_CLOSE	=0X8,
	JWL_WEBSOCKET_STATUS_PING	=0X9,
	JWL_WEBSOCKET_STATUS_PONG	=0X10,
}jwl_websocket_status;
jbl_string *	jwl_websocket_get_sec			(jwl_http_head* head,jbl_string *res);
jwl_http_head *	jwl_websocket_set_response_head	(jwl_http_head* response,jwl_http_head* request);
jbl_string *	jwl_websocket_get_head			(jbl_uint64 len,jbl_uint8 is_last,jwl_websocket_status opcode,jbl_string* head);
jbl_string *	jwl_websocket_decode			(jbl_string* head);


#if JBL_STREAM_ENABLE==1
extern			const jbl_stream_operater			jwl_stream_websocket_operators;
jbl_stream *jwl_websocket_stream_new();
jbl_var *jwl_websocket_Vstream_new();

#define jwl_websocket_stream_finished(x) (((jbl_stream*)jbl_refer_pull(x))->extra[0].u>=((jbl_stream*)jbl_refer_pull(x))->extra[1].u)
#define jwl_websocket_stream_get_status(x) (((jbl_stream*)jbl_refer_pull(x))->extra[2].c8[5])

#endif


#endif
#endif
