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
	jbl_uint64 ip;
	jbl_uint32 port;
}jwl_socket;
void			jwl_socket_start		();													//启动socket
jwl_socket *	jwl_socket_new			();													//新建一个socket
jwl_socket *	jwl_socket_init			(jwl_socket *this);									//初始化一个socket
jwl_socket *	jwl_socket_free			(jwl_socket *this);									//释放一个socket
jwl_socket *	jwl_socket_bind			(jwl_socket *this,jbl_uint64 ip,jbl_uint32 port);	//启动一个socket监听
jwl_socket *	jwl_socket_connect		(jwl_socket *this,jbl_uint64 ip,jbl_uint32 port);	//发起一个socket连接
jwl_socket *	jwl_socket_close		(jwl_socket *this);									//关闭一个socket请求
jwl_socket *	jwl_socket_accept		(jwl_socket *this);									//接受一个socket请求
jwl_socket *	jwl_socket_send			(jwl_socket *this,jbl_string *data);				//发送
jbl_string *	jwl_socket_receive		(jwl_socket *this,jbl_string *data);				//接收
jwl_socket *	jwl_socket_send_safe	(jwl_socket *this,jbl_string *data);				//安全发送
jbl_string *	jwl_socket_receive_safe	(jwl_socket *this,jbl_string *data);				//安全接收

#define			jwl_socket_set_host(x)			(jbl_gc_set_user1((jwl_socket*)jbl_refer_pull(x)))		//设置host标记
#define			jwl_socket_reset_host(x)		(jbl_gc_reset_user1((jwl_socket*)jbl_refer_pull(x)))	//删除host标记
#define			jwl_socket_is_host(x)			(jbl_gc_is_user1((jwl_socket*)jbl_refer_pull(x)))		//获取host标记


#if JBL_STREAM_ENABLE==1
void			jwl_socket_view_put				(const jwl_socket* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs);	//从out浏览一个hash table
#define			jwl_socket_view(x)				jwl_socket_view_put(x,jbl_stream_stdout,__LINE__,#x " @ "__FILE__,JBL_VIEW_DEFAULT_TABS),jbl_stream_push_char(jbl_stream_stdout,'\n')//浏览一个hash table


extern			const jbl_stream_operater	jwl_stream_socket_operators;
#define			jwl_stream_socket_new(x)	jbl_stream_new(&jwl_stream_socket_operators,x,JWL_SOCKET_STREAM_BUF_LENGTH,NULL,0,NULL)	//新建base64加密流

#endif








#else
	#define jwl_socket_start()
#endif
#endif