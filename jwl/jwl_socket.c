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
	#include <sys/epoll.h>
#endif
#include <errno.h>
//undefined reference to `__imp_WSAStartup'
void jwl_socket_start()
{
#ifdef _WIN32
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
		jbl_exception("DLL LOADING FAILED");
#elif __linux__
	signal(SIGPIPE, SIG_IGN);
#elif __APPLE__
	signal(SIGPIPE, SIG_IGN);
#endif
}
inline jwl_socket * jwl_socket_new()
{
	return jwl_socket_init(jbl_malloc(sizeof(jwl_socket)));	
}
jwl_socket * jwl_socket_init(jwl_socket *this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_gc_init(this);
	jbl_gc_plus(this);
	this->handle=-1;
	this->ip=0;
	this->port=0;
#ifdef jwl_socket_payload
	this->payload=jwl_socket_payload_NULL;
#endif
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
		{
			jwl_socket_close(this);
#ifdef jwl_socket_payload
			jwl_socket_payload_free(this->payload);
#endif
		}
#if JBL_VAR_ENABLE==1
		if(jbl_gc_is_var(this))
			jbl_free((char*)this-sizeof(jbl_var));
		else
#endif
			jbl_free(this);
	}
	return NULL;
}
jwl_socket * jwl_socket_bind(jwl_socket *this,jbl_uint32 ip,jbl_uint16 port)
{
//分离
	if(!this)this=jwl_socket_new();
	jwl_socket* thi=jbl_refer_pull(this);
	if(thi->handle!=-1)jbl_exception("SOCKET REUSE");
	thi->handle=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
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
	return this;
}
jwl_socket * jwl_socket_connect(jwl_socket *this,jbl_uint32 ip,jbl_uint16 port)
{
//分离
	if(!this)this=jwl_socket_new();
	jwl_socket* thi=jbl_refer_pull(this);
	if(thi->handle!=-1)jbl_exception("SOCKET REUSE");
	thi->handle=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	struct sockaddr_in skaddr;
	skaddr.sin_family=AF_INET;
	jbl_endian_to_big_uint16(&port,&skaddr.sin_port);
	skaddr.sin_addr.s_addr=ip;
	if(connect(thi->handle,(struct sockaddr*)&skaddr,sizeof(skaddr))==-1)
		thi=jwl_socket_close(thi),jbl_log(UC "CONNECT FAILED");
	thi->port=port;
	thi->ip=ip;
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
#ifdef _WIN32
	int timeout = JWL_SOCKET_TRANSFER_MAX_TIME*1000;
	setsockopt(client->handle,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	setsockopt(client->handle,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
#elif defined(__APPLE__) || defined(__linux__)
	struct timeval timeout={JWL_SOCKET_TRANSFER_MAX_TIME,0};
	setsockopt(client->handle,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	setsockopt(client->handle,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
#endif	
	return client;
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
	jbl_stream *this=jbl_stream_new(&jwl_stream_socket_operators,socket,JWL_SOCKET_STREAM_BUF_LENGTH,NULL,0);
	return this;
}
#if JBL_VAR_ENABLE==1
jbl_var *jwl_socket_Vstream_new(jwl_socket* socket)
{
	if(!socket)jbl_exception("NULL POINTER");
	if(jwl_socket_is_host(socket))jbl_exception("NEW STREAM FROM HOST SOCKET");	
	jbl_stream *this=(jbl_stream *)jbl_Vstream_new(&jwl_stream_socket_operators,socket,JWL_SOCKET_STREAM_BUF_LENGTH,NULL,0);
	return (jbl_var *)this;
}
#endif
static void __jwl_socket_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jwl_socket *socket=jbl_refer_pull((jwl_socket*)this->data);
	if(this->en&&((flags&jbl_stream_force)||(this->en>JWL_SOCKET_STREAM_BUF_LENGTH>>1)))
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


//连接池
inline jwl_socket_poll * jwl_socket_poll_new()
{
	return jwl_socket_poll_init(jbl_malloc(sizeof(jwl_socket_poll)+
#ifdef __linux__	
	(sizeof(struct epoll_event))
#elif __APPLE__
	(sizeof(struct kevent))
#elif _WIN32
	0
#endif	
	*128));	
}
jwl_socket_poll * jwl_socket_poll_init(jwl_socket_poll *this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_gc_init(this);
	jbl_gc_plus(this);
	this->len=0;
	this->data=NULL;
	
#ifdef _WIN32
	FD_ZERO(&this->fd);
#else
	this->event_len=0;
#ifdef __linux__
	if(-1==(this->handle=epoll_create1(0)))jbl_exception("POLL FAILED");
#elif __APPLE__
	if(-1==(this->handle=kqueue()))jbl_exception("POLL FAILED");
#endif
#endif
	return this;
}
inline jwl_socket_poll *jwl_socket_poll_copy(jwl_socket_poll * this)
{
	jbl_gc_plus(this);
	return this;
}
jwl_socket_poll * jwl_socket_poll_free(jwl_socket_poll *this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this))
			jwl_socket_poll_free((jwl_socket_poll*)(((jbl_reference*)this)->ptr));
		else
		{
			for(jwl_socket_poll_data *i=this->data,*j=NULL;i;jwl_socket_free(i->socket),j=i->nxt,jbl_free(i),i=j);
#ifdef __linux__
			close(this->handle);
#elif __APPLE__
			close(this->handle);
#endif
		}
#if JBL_VAR_ENABLE==1
		if(jbl_gc_is_var(this))
			jbl_free((char*)this-sizeof(jbl_var));
		else
#endif
			jbl_free(this);
	}
	return NULL;
}
jwl_socket_poll * jwl_socket_poll_add(jwl_socket_poll *this,jwl_socket* socket)
{
	if(!this||!socket)jbl_exception("NULL POINTER");	
	jwl_socket_poll * thi=jbl_refer_pull(this);

	jwl_socket_poll_data *data=jbl_malloc(sizeof(jwl_socket_poll_data));
	data->socket=jwl_socket_copy(socket);
	data->nxt=thi->data;
	thi->data=data;

	++thi->len;
	socket=jbl_refer_pull(socket);	
#ifdef __linux__
	struct epoll_event ev;
	ev.data.fd=socket->handle;
	ev.data.ptr=socket;
    ev.events=EPOLLIN|EPOLLET;
    epoll_ctl(this->handle,EPOLL_CTL_ADD,socket->handle,&ev);	
#elif __APPLE__
	struct kevent ev;
	EV_SET(&ev,socket->handle,EVFILT_READ,EV_ADD|EV_ENABLE,0,0,NULL);
	ev.udata=socket;
	kevent(this->handle,&ev,1,NULL,0,NULL);
#elif _WIN32
#endif
	return this;
}
jwl_socket_poll * jwl_socket_poll_remove(jwl_socket_poll *this,jwl_socket* socket)
{
	if(!this||!socket)jbl_exception("NULL POINTER");	
//jwl_socket_view(socket);
	jwl_socket_poll * thi=jbl_refer_pull(this);
	socket=jbl_refer_pull(socket);	

	jwl_socket_poll_data * found=NULL,*pre=NULL;
	for(jwl_socket_poll_data *i=thi->data;i;pre=i,i=i->nxt)
		if(jwl_socket_if_equal(i->socket,socket))
		{
			found=i;
			break;
		}
	if(!found)
		jbl_exception("SOCKET NOT IN POLL");
#ifdef __linux__
	epoll_ctl(this->handle,EPOLL_CTL_DEL,socket->handle,NULL);	
#elif __APPLE__
	struct kevent ev;
	EV_SET(&ev,socket->handle,EVFILT_READ,EV_DELETE|EV_DISABLE,0,0,NULL);
	kevent(this->handle,&ev,1,NULL,0,NULL);
#elif _WIN32
#endif
	jwl_socket_free(found->socket);
	if(pre)	pre->nxt=found->nxt;
	else	thi->data=found->nxt;
	jbl_free(found);
	--thi->len;
	return this;
}
jwl_socket_poll * jwl_socket_poll_remove_closed(jwl_socket_poll *this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jwl_socket_poll * thi=jbl_refer_pull(this);
	for(jwl_socket_poll_data *i=thi->data,*pre=NULL;i;)
		if(-1==i->socket->handle)
		{
			jwl_socket_poll_data *j=i->nxt;
#ifdef __linux__
			epoll_ctl(this->handle,EPOLL_CTL_DEL,((jwl_socket*)jbl_refer_pull(i->socket))->handle,NULL);	
#elif __APPLE__
			struct kevent ev;
			EV_SET(&ev,((jwl_socket*)jbl_refer_pull(i->socket))->handle,EVFILT_READ,EV_DELETE|EV_DISABLE,0,0,NULL);
			kevent(this->handle,&ev,1,NULL,0,NULL);
#elif _WIN32
#endif
			jwl_socket_free(i->socket);
			if(pre)	pre->nxt=i->nxt;
			else	thi->data=i->nxt;
			jbl_free(i);
			i=j;
			--thi->len;
		}
		else
			pre=i,i=i->nxt;
	return this;
}
jwl_socket_poll * jwl_socket_poll_wait(jwl_socket_poll *this)
{
	if(!this)jbl_exception("NULL POINTER");	
#ifdef _WIN32
	this=jwl_socket_poll_remove_closed(this);
#endif
	jwl_socket_poll * thi=jbl_refer_pull(this);
	if(!thi->len)return this;
#ifdef __linux__
	if(-1==(thi->event_len=epoll_wait(thi->handle,thi->events,128,-1)))			jbl_exception("POLL FAILED");
#elif __APPLE__
	if(-1==(thi->event_len=kevent(thi->handle,NULL,0,thi->events,128,NULL)))	jbl_exception("POLL FAILED");
#elif _WIN32
	thi=jbl_refer_pull(this);
	FD_ZERO(&thi->fd);
	for(jwl_socket_poll_data *j=thi->data;j;j=j->nxt)
		FD_SET(((jwl_socket*)jbl_refer_pull(j->socket))->handle,&thi->fd);
	if(select(0,&thi->fd,NULL,NULL,NULL)==SOCKET_ERROR)							jbl_exception("POLL FAILED");
	thi->j=thi->data;
#endif
	return this;
}
jwl_socket * jwl_socket_poll_get(jwl_socket_poll *this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jwl_socket_poll * thi=jbl_refer_pull(this);
#ifdef _WIN32
	for(jwl_socket_poll_data *jj=(thi->j)?thi->j->nxt:NULL;thi->j;thi->j=jj,jj=(thi->j)?thi->j->nxt:NULL)
		if(FD_ISSET(((jwl_socket*)jbl_refer_pull(thi->j->socket))->handle,&thi->fd))
			{jwl_socket_poll_data *jjj=thi->j;thi->j=jj,jj=(thi->j)?thi->j->nxt:NULL;return jwl_socket_copy(jjj->socket);}
#else
	while(thi->event_len)
	{
#ifdef __linux__
		jwl_socket *socket=(jwl_socket*)thi->events[--thi->event_len].data.ptr;
#elif __APPLE__
		jwl_socket *socket=(jwl_socket*)thi->events[--thi->event_len].udata;
#endif
		jwl_socket *sock=jbl_refer_pull(socket);
		if(-1==sock->handle)this=jwl_socket_poll_remove(this,socket); 
		else				return jwl_socket_copy(socket);
	}
#endif
	return NULL;
}
#if JBL_STREAM_ENABLE==1
jwl_socket_poll* jwl_socket_poll_view_put(jwl_socket_poll* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jwl_socket_poll* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jwl_socket_poll",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_chars(out,UC"\tlen:");jbl_stream_push_uint(out,thi->len);
	jbl_stream_push_char(out,'\n');

	jbl_ll_size_type j=0;
	++tabs;
	for(jwl_socket_poll_data *i=thi->data;i;i=i->nxt)
	{
		for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);
		jbl_stream_push_uint(out,j++);
		jbl_stream_push_char(out,':');
		jwl_socket_view_put(i->socket,out,0,tabs,0,NULL,NULL,NULL);
	}
	jbl_stream_push_char(out,'\n');

	return this;
}
#endif



#endif
