/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jwl_socket.h"
#if JWL_SOCKET_ENABLE==1
#ifdef _WIN32
	#include <winsock2.h>
#elif __APPLE__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <signal.h>
	#include <sys/event.h>
	#include <sys/types.h>
#elif __linux__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <signal.h>
#endif
#include <errno.h>
jbl_var_operators_new(jwl_socket_operators,jwl_socket_free,jwl_socket_copy,NULL,NULL,jwl_socket_view_put,NULL);
void jwl_socket_start()
{
#ifdef _WIN32
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
		jbl_exception("DLL LOADING FAILED");
#elif defined(__APPLE__) || defined(__linux__)
	signal(SIGPIPE, SIG_IGN);
#endif
}
jwl_socket * jwl_socket_new()
{
	jwl_socket *this=jbl_malloc(sizeof(jwl_socket));
	jbl_gc_init(this);
	jbl_gc_plus(this);
	jbl_var_set_operators(this,&jwl_socket_operators);	
	this->handle=-1;
	this->ip=0;
	this->port=0;
	return this;
}
inline jwl_socket *jwl_socket_copy(jwl_socket * this)
{
	jbl_gc_plus(this);
	return this;
}
jwl_socket * jwl_socket_free(jwl_socket *this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this))
			jwl_socket_free((jwl_socket*)(((jbl_reference*)this)->ptr));
		else
			jwl_socket_close(this);
		jbl_free(this);
	}
	return NULL;
}
jwl_socket * jwl_socket_bind(jwl_socket *this,jbl_uint32 ip,jbl_uint16 port,jbl_uint8 mode)
{
//分离
	if(!this)this=jwl_socket_new();
	jwl_socket* thi=jbl_refer_pull(this);
	if(thi->handle!=-1)jbl_exception("SOCKET REUSE");
	if(mode==JWL_SOCKET_MODE_TCP)		thi->handle=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	else if(mode==JWL_SOCKET_MODE_UDP)	thi->handle=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	struct sockaddr_in skaddr;
	skaddr.sin_family=AF_INET;
	jbl_endian_to_big_uint16(&port,&skaddr.sin_port);
	skaddr.sin_addr.s_addr=ip;	
	if(bind(thi->handle,((struct sockaddr *)&skaddr),sizeof(skaddr))==-1)
		thi=jwl_socket_close(thi),jbl_exception("BIND FAILED");
	if(listen(thi->handle,1024)==-1)
		thi=jwl_socket_close(thi),jbl_exception("LISTEN FAILED");
	jwl_socket_set_host(thi);
	thi->port=port;
	thi->ip=ip;
	thi->mode=mode;
	return this;
}
jwl_socket * jwl_socket_connect(jwl_socket *this,jbl_uint32 ip,jbl_uint16 port,jbl_uint8 mode)
{
//分离
	if(!this)this=jwl_socket_new();
	jwl_socket* thi=jbl_refer_pull(this);
	if(thi->handle!=-1)jbl_exception("SOCKET REUSE");
	if(mode==JWL_SOCKET_MODE_TCP)		thi->handle=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	else if(mode==JWL_SOCKET_MODE_UDP)	thi->handle=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	thi->port=port;
	thi->ip=ip;
	thi->mode=mode;
	if(mode==JWL_SOCKET_MODE_TCP)
	{
		struct sockaddr_in skaddr;
		skaddr.sin_family=AF_INET;
		jbl_endian_to_big_uint16(&port,&skaddr.sin_port);
		skaddr.sin_addr.s_addr=ip;
		if(connect(thi->handle,(struct sockaddr*)&skaddr,sizeof(skaddr))==-1)
			thi=jwl_socket_close(thi),jbl_log(UC "CONNECT FAILED");
	}
#ifdef _WIN32
	int timeout = JWL_SOCKET_TRANSFER_MAX_TIME*1000;
	setsockopt(thi->handle,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	setsockopt(thi->handle,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
#elif defined(__APPLE__) || defined(__linux__)
	struct timeval timeout={JWL_SOCKET_TRANSFER_MAX_TIME,0};
	setsockopt(thi->handle,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	setsockopt(thi->handle,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
#endif

	return this;
}
inline jwl_socket * jwl_socket_close(jwl_socket *this)
{
	if(!this)return NULL;
	jwl_socket* thi=jbl_refer_pull(this);
#ifdef _WIN32
	if(thi)(thi->handle==-1?0:closesocket(thi->handle));
#elif defined(__APPLE__) || defined(__linux__)
	if(thi)(thi->handle==-1?0:close(thi->handle));
#endif
	jwl_socket_reset_host(thi);
	thi->port=0;
	thi->ip=0;
	thi->handle=-1;
	return this;
}
inline jwl_socket * jwl_socket_accept(jwl_socket *this)
{
	if(!this)return NULL;
	jwl_socket* thi=jbl_refer_pull(this);
	if(!jwl_socket_is_host(thi))jbl_exception("ACCEPT FROM UNHOST SOCKET");
	struct sockaddr_in claddr;
#ifdef _WIN32
	int length=sizeof(claddr);
	SOCKET handle;
#elif defined(__APPLE__) || defined(__linux__)
	unsigned int length=sizeof(claddr);
	int handle;
#endif	
	if((handle=accept(thi->handle,(struct sockaddr*)&claddr,&length))==-1)
	{
		jbl_log(UC "ACCEPT FAILED");
		return NULL;
	}
	jwl_socket *client=jwl_socket_new();
	client->handle=handle;	
	jbl_endian_from_big_uint16(&claddr.sin_port,&client->port);
	client->ip=claddr.sin_addr.s_addr;
	client->mode=thi->mode;
#ifdef _WIN32
	int timeout = JWL_SOCKET_TRANSFER_MAX_TIME;
	setsockopt(client->handle,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	setsockopt(client->handle,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
#elif defined(__APPLE__) || defined(__linux__)
	struct timeval timeout={JWL_SOCKET_TRANSFER_MAX_TIME/1000,JWL_SOCKET_TRANSFER_MAX_TIME%1000};
	setsockopt(client->handle,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	setsockopt(client->handle,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
#endif	
	return client;
}
jwl_socket * jwl_socket_send_chars(jwl_socket* this,jbl_uint8 *data,jbl_uint64 len)
{
	if(!this)return NULL;
	jwl_socket* thi=jbl_refer_pull(this);
	if(jwl_socket_is_host(thi))jbl_exception("SEND FROM HOST SOCKET");	
	if(thi->mode==JWL_SOCKET_MODE_TCP)
	{
		
		
	}
	else if(thi->mode==JWL_SOCKET_MODE_UDP)
	{
		struct sockaddr_in skaddr;
		skaddr.sin_family=AF_INET;
		jbl_endian_to_big_uint16(&thi->port,&skaddr.sin_port);
		skaddr.sin_addr.s_addr=thi->ip;
		for(jbl_uint8 i=0;thi->handle!=-1&&sendto(thi->handle,(char*)data,len,0,(struct sockaddr*)&skaddr,sizeof(skaddr))<0;++i,jbl_log(UC "Send failed\terrno:%d retrying %d",errno,i))
			if(errno==ECONNRESET||errno==EPIPE||i>=7)
			{
				jbl_log(UC "Send failed\terrno:%d",errno);
				jwl_socket_close(thi);
				break;
			}
	}
	else
		jbl_exception("SEND FROM UNKNOW MODE");	
	return this;
}
jbl_uint64 jwl_socket_receive_chars(jwl_socket* this,jbl_uint8 *data,jbl_uint64 len)
{
	if(!this)return 0;
	jwl_socket* thi=jbl_refer_pull(this);
	if(jwl_socket_is_host(thi))jbl_exception("RECEIVE FROM HOST SOCKET");	
	jbl_uint64 length=0;
	if(thi->mode==JWL_SOCKET_MODE_TCP)
	{
		
		
	}
	else if(thi->mode==JWL_SOCKET_MODE_UDP)
	{
		struct sockaddr_in skaddr;
		skaddr.sin_family=AF_INET;
		jbl_endian_to_big_uint16(&thi->port,&skaddr.sin_port);
		skaddr.sin_addr.s_addr=thi->ip;
#ifdef _WIN32
		jbl_int32 size=sizeof(skaddr);
#elif defined(__APPLE__) || defined(__linux__)
		jbl_uint32 size=sizeof(skaddr);
#endif	
		for(jbl_uint8 i=0;thi->handle!=-1&&(length=recvfrom(thi->handle,(char*)data,len,0,(struct sockaddr*)&skaddr,&size))<0;++i,jbl_log(UC "Receive failed\terrno:%d retrying %d",errno,i))
			if(errno==ECONNRESET||errno==EPIPE||i>=7)
			{
				jbl_log(UC "Receive failed\terrno:%d",errno);
				jwl_socket_close(thi);
				break;
			}
	}
	else
		jbl_exception("RECEIVE FROM UNKNOW MODE");	
	return length;
}
#ifdef jwl_socket_payload
jwl_socket * jwl_socket_set_payload(jwl_socket * this,jwl_socket_payload payload)
{
	if(!this)jbl_exception("NULL POINTER");	
	jwl_socket* thi=jbl_refer_pull(this);
	thi->payload=jwl_socket_payload_copy(payload);
	return this;
}
jwl_socket_payload jwl_socket_get_payload(jwl_socket * this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jwl_socket* thi=jbl_refer_pull(this);
	return jwl_socket_payload_copy(thi->payload);
}
#endif
#if JBL_STREAM_ENABLE==1
jwl_socket* jwl_socket_view_put(jwl_socket* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jwl_socket* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jwl_socket",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	if(thi->handle==-1)
		jbl_stream_push_chars(out,UC" disconnected");
	else
	{
		if(jwl_socket_is_host(thi))
			jbl_stream_push_chars(out,UC" listen  on ");
		else
			jbl_stream_push_chars(out,UC" connect to ");
		jbl_stream_push_chars(out,UC"ip:");
		jbl_string* tmp=jwl_get_string_ip(thi->ip,NULL);
		jbl_stream_push_string(jbl_stream_stdout,tmp);
		tmp=jbl_string_free(tmp);
		jbl_stream_push_chars(out,UC" port:");
		jbl_stream_push_uint(out,thi->port);
		jbl_stream_push_chars(out,UC" mode:");
		switch(thi->mode)
		{
			case JWL_SOCKET_MODE_TCP:jbl_stream_push_chars(out,UC"TCP");break;
			case JWL_SOCKET_MODE_UDP:jbl_stream_push_chars(out,UC"UDP");break;
			default:jbl_stream_push_chars(out,UC"unknow");break;
		}
#ifdef jwl_socket_payload
		jbl_stream_push_chars(out,UC" payload:");
		jwl_socket_payload_view(out,thi->payload);
#endif
	}
	jbl_stream_push_char(out,'\n');
	return this;
}
jbl_stream *jwl_socket_stream_new(jwl_socket* socket)
{
	if(!socket)jbl_exception("NULL POINTER");
	if(jwl_socket_is_host(socket))jbl_exception("NEW STREAM FROM HOST SOCKET");	
	if(((jwl_socket*)jbl_refer_pull(socket))->mode!=JWL_SOCKET_MODE_TCP)jbl_exception("NEW STREAM FROM NOT TCP SOCKET");	
	jbl_stream *this=jbl_stream_new(&jwl_stream_socket_operators,socket,JWL_SOCKET_STREAM_BUF_LENGTH,NULL,0);
	return this;
}
static void __jwl_socket_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jwl_socket *socket=jbl_refer_pull((jwl_socket*)this->data);
	if(this->en)
	{
		if((flags&jbl_stream_force)||(this->en>JWL_SOCKET_STREAM_BUF_LENGTH>>1))
		{
			for(jbl_uint8 i=0;socket->handle!=-1&&send(socket->handle,(char*)this->buf,this->en,0)==-1;++i,jbl_log(UC "Send failed\terrno:%d retrying %d",errno,i))
				if(errno==ECONNRESET||errno==EPIPE||i>=7)
				{
					jbl_log(UC "Send failed\terrno:%d",errno);
					this->stop=1;
					jwl_socket_close(socket);
					break;
				}
			this->en=0;
		}
	}
	else if(nxt)
	{
		while(socket->handle!=-1)
		{
			jbl_stream_do(nxt,0);if(nxt->stop)return;
			jbl_stream_buf_size_type len=nxt->size-nxt->en;
			int j;
			for(jbl_uint8 i=0;(socket->handle!=-1)&&(j=recv(socket->handle,(char*)nxt->buf+nxt->en,len,0))<=0;++i,jbl_log(UC "Receive failed\terrno:%d retrying %d",errno,i))
				if(errno==ECONNRESET||errno==EPIPE||(!j))
				{
					jbl_log(UC "Receive failed\terrno:%d",errno);
					jwl_socket_close(socket);
					jbl_stream_do(nxt,flags);					
					return;
				}
				else if(i>=7)
				{
					jbl_log(UC "Receive failed time out:%d",i);
					jbl_stream_do(nxt,0);
					return;
				}
				else if(errno==EAGAIN)
				{
					jbl_stream_do(nxt,0);
					return;
				}
			jbl_log(UC "Receive success,data length:%d",j);			
			nxt->en+=j;
		}
		jbl_stream_do(nxt,flags);	
	}
}
jbl_stream_operators_new(jwl_stream_socket_operators,__jwl_socket_stream_operater,jwl_socket_free,NULL);

#endif


#endif
