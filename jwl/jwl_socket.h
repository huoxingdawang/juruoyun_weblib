/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JWL_SOCKET_H
#define __JWL_SOCKET_H
#include "jwl_socket_config.h"
#if JWL_SOCKET_ENABLE==1
#include "../jbl/jbl_include.h"
#include "jwl_ying.h"
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <winsock2.h>
#endif
typedef struct __jwl_socket
{
	jbl_gc gc;
#ifdef __linux__
	int handle;
#else
	SOCKET handle;
#endif
}jwl_socket;
void			jwl_socket_start		();													//启动socket
jwl_socket *	jwl_socket_new			();													//新建一个socket
jwl_socket *	jwl_socket_init			(jwl_socket *this);									//初始化一个socket
jwl_socket *	jwl_socket_free			(jwl_socket *this);									//释放一个socket
jwl_socket *	jwl_socket_bind			(jwl_socket *this,jbl_uint64 ip,jbl_uint32 port);	//启动一个socket监听
jwl_socket *	jwl_socket_connect		(jwl_socket *this,jbl_uint64 ip,jbl_uint32 port);	//发起一个socket连接
jwl_socket *	jwl_socket_close		(jwl_socket *this);									//关闭一个socket请求
jwl_socket *	jwl_socket_accept		(jwl_socket *this,jbl_uint64 *ip,jbl_uint32 *port);	//接受一个socket请求
jwl_socket *	jwl_socket_send			(jwl_socket *this,jbl_string *data);				//发送
jbl_string *	jwl_socket_receive		(jwl_socket *this,jbl_string *data);				//接收
jwl_socket *	jwl_socket_send_safe	(jwl_socket *this,jbl_string *data);				//安全发送
jbl_string *	jwl_socket_receive_safe	(jwl_socket *this,jbl_string *data);				//安全接收



#if JBL_STREAM_ENABLE==1
extern			const jbl_stream_operater	jwl_stream_socket_operators;
#define			jwl_stream_socket_new(x)	jbl_stream_new(&jwl_stream_socket_operators,x,JWL_SOCKET_STREAM_BUF_LENGTH,NULL,0,NULL)	//新建base64加密流

#endif








#else
	#define jwl_socket_start()
#endif
#endif