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
jbl_string *	jwl_websocket_get_head			(jbl_uint64 len,jbl_uint8 is_last,jbl_uint8 need_mask,jwl_websocket_status opcode,jbl_string* head);
jbl_string *	jwl_websocket_decode			(jbl_string* head);


#if JBL_STREAM_ENABLE==1

extern			const jbl_stream_operater			jwl_stream_websocket_decode_operators;
extern			const jbl_stream_operater			jwl_stream_websocket_encode_operators;
#define 		jwl_websocket_encode_stream_new()	jbl_stream_new(&jwl_stream_websocket_encode_operators,NULL,JWL_WEBSOCKET_STREAM_BUF_LENGTH,NULL,1);
#define 		jwl_websocket_decode_stream_new()	jbl_stream_new(&jwl_stream_websocket_decode_operators,NULL,JWL_WEBSOCKET_STREAM_BUF_LENGTH,NULL,3);
#if JBL_VAR_ENABLE==1
#define			jwl_websocket_encode_Vstream_new()	jbl_Vstream_new(&jwl_stream_websocket_encode_operators,NULL,JWL_WEBSOCKET_STREAM_BUF_LENGTH,NULL,1);
#define			jwl_websocket_decode_Vstream_new()	jbl_Vstream_new(&jwl_stream_websocket_decode_operators,NULL,JWL_WEBSOCKET_STREAM_BUF_LENGTH,NULL,3);
#endif


#define			jwl_websocket_decode_stream_finished(x)		(((jbl_stream*)jbl_refer_pull(x))->extra[0].u>=((jbl_stream*)jbl_refer_pull(x))->extra[1].u)
#define			jwl_websocket_decode_stream_get_status(x)	(((jbl_stream*)jbl_refer_pull(x))->extra[2].c8[5]&0X0F)
#define			jwl_websocket_encode_stream_set_opcode(x,y)	(((jbl_stream*)jbl_refer_pull(x))->extra[0].c8[5]=(y))
#define			jwl_websocket_encode_stream_set_mask(x,y)	(((jbl_stream*)jbl_refer_pull(x))->extra[0].c8[6]=(y))

#endif


#endif
#endif
