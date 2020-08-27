/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JWL_SOCKET_CONFIG_H
#define __JWL_SOCKET_CONFIG_H
#include "jwl_config.h"
#if JWL_SOCKET_ENABLE==1
#define	JWL_SOCKET_STREAM_BUF_LENGTH		128
#define	JWL_SOCKET_TRANSFER_MAX_TIME 		1			//单位是秒

#define jwl_socket_payload					jbl_uint8
#define jwl_socket_payload_NULL	0	
#define jwl_socket_payload_free(x)			(x=jwl_socket_payload_NULL)
#define jwl_socket_payload_copy(x)			(x)
#define jwl_socket_payload_view(x,y)		jbl_stream_push_uint(x,y)




#endif
#endif
