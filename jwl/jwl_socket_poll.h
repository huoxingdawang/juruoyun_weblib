/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JWL_SOCKET_POLL_H
#define __JWL_SOCKET_POLL_H
#include "jwl_socket_config.h"
#if JWL_SOCKET_ENABLE==1
#include "../jbl/jbl_include.h"
#include "jwl_ying.h"
#include "jwl_socket.h"
#ifdef _WIN32
	#include <winsock2.h>
#elif __linux__
	#include <sys/epoll.h>
#elif __APPLE__
#include <sys/event.h>
#include <sys/types.h>
#endif
jbl_var_operators_extern(jwl_socket_poll_operators);
typedef struct __jwl_socket_poll_data
{
	jwl_socket *socket;
#if JBL_STREAM_ENABLE==1
	jbl_stream *sockets;
	jbl_stream *outs;
	jbl_stream *ins;
#endif
	struct __jwl_socket_poll_data *nxt;
}jwl_socket_poll_data;
typedef struct __jwl_socket_poll
{
	jbl_gc gc;
#if JBL_VAR_ENABLE==1
	jbl_var_operators *		var_ops;
#endif
	jbl_uint16 len;
	jwl_socket_poll_data *data;
	
#ifdef _WIN32
	FD_SET fd;
	jwl_socket_poll_data *j;
#else
	int handle;
	int event_len;
#ifdef __linux__
	struct epoll_event	events[0];
#elif __APPLE__
	struct kevent 		events[0];	
#endif
#endif
}jwl_socket_poll;

jwl_socket_poll *		jwl_socket_poll_new				();														//新建一个socket
jwl_socket_poll *		jwl_socket_poll_free			(jwl_socket_poll *this);								//释放一个socket
jwl_socket_poll *		jwl_socket_poll_copy			(jwl_socket_poll * this);								//拷贝一个socket
#define					jwl_socket_poll_extend(this)	(this)
jwl_socket_poll *		jwl_socket_poll_add				(jwl_socket_poll *this,jwl_socket* socket);
jwl_socket_poll *		jwl_socket_poll_remove			(jwl_socket_poll *this,jwl_socket* socket);
jwl_socket_poll * 		jwl_socket_poll_remove_closed	(jwl_socket_poll *this);
jwl_socket_poll *		jwl_socket_poll_wait			(jwl_socket_poll *this);
jwl_socket_poll_data *	jwl_socket_poll_get				(jwl_socket_poll *this);
#define 				jwl_socket_poll_foreach(x,y)		for(jwl_socket_poll_data *(y)=((jwl_socket_poll *)jbl_refer_pull(x))->data;(y);(y)=(y)->nxt)	//枚举一个link list(不支持删除,会re)
#define 				jwl_socket_poll_foreach_del(x,y,z)	for(jwl_socket_poll_data *(y)=((jwl_socket_poll *)jbl_refer_pull(x))->data,*(z)=((y)==NULL?NULL:(y)->nxt);(y);(y)=(z),(z)=((y)==NULL?NULL:(y)->nxt))	//枚举一个link list(支持删除操作)
#if JBL_STREAM_ENABLE==1
jwl_socket_poll*		jwl_socket_poll_view_put		(jwl_socket_poll* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个socket
#define					jwl_socket_poll_view(x)			jwl_socket_poll_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个socket
#endif




#else
	#define jwl_socket_start()
#endif
#endif