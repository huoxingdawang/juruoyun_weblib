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
#if JBL_VAR_ENABLE==1
		if(jbl_gc_is_var(this))
			jbl_free((char*)this-sizeof(jbl_var));
		else
#endif
			jbl_free(this);
	}
	return NULL;
}
jwl_socket * jwl_socket_bind(jwl_socket *this,jbl_uint64 ip,jbl_uint32 port)
{
//分离
	if(!this)this=jwl_socket_new();
	if(this->handle!=-1)jbl_exception("SOCKET REUSE");
	this->handle=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	struct sockaddr_in skaddr;
	skaddr.sin_family=AF_INET;
	skaddr.sin_port=htons(port);
	skaddr.sin_addr.s_addr=ip;	
	if(bind(this->handle,((struct sockaddr *)&skaddr),sizeof(skaddr))==-1)
		this=jwl_socket_close(this),jbl_exception("BIND FAILED");
	if(listen(this->handle,1024)==-1)
		this=jwl_socket_close(this),jbl_exception("LISTEN FAILED");
	jwl_socket_set_host(this);
	this->port=port;
	this->ip=ip;
	return this;
}
jwl_socket * jwl_socket_connect(jwl_socket *this,jbl_uint64 ip,jbl_uint32 port)
{
//分离
	if(!this)this=jwl_socket_new();
	if(this->handle!=-1)jbl_exception("SOCKET REUSE");
	this->handle=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	struct sockaddr_in skaddr;
	skaddr.sin_family=AF_INET;
	skaddr.sin_port=htons(port);
	skaddr.sin_addr.s_addr=ip;
	if(connect(this->handle,(struct sockaddr*)&skaddr,sizeof(skaddr))==-1)
		this=jwl_socket_close(this),jbl_log(UC "CONNECT FAILED");
	this->port=port;
	this->ip=ip;
	return this;
}
inline jwl_socket * jwl_socket_close(jwl_socket *this)
{
#ifdef _WIN32
	if(this)(this->handle==-1?0:closesocket(this->handle));
#elif defined(__APPLE__) || defined(__linux__)
	if(this)(this->handle==-1?0:close(this->handle));
#endif
	jwl_socket_reset_host(this);
	this->port=0;
	this->ip=0;
	this->handle=-1;
	return this;
}
inline jwl_socket * jwl_socket_accept(jwl_socket *this)
{
	if(!this)return NULL;
	if(!jwl_socket_is_host(this))jbl_exception("ACCEPT FROM UNHOST SOCKET");
	struct sockaddr_in claddr;
#ifdef _WIN32
	int length=sizeof(claddr);
	SOCKET handle;
#elif defined(__APPLE__) || defined(__linux__)
	unsigned int length=sizeof(claddr);
	int handle;
#endif	
	if((handle=accept(this->handle,(struct sockaddr*)&claddr,&length))==-1)
	{
		jbl_log(UC "ACCEPT FAILED");
		return NULL;
	}
	jwl_socket *client=jwl_socket_new();
	client->handle=handle;
	client->port=ntohs(claddr.sin_port);
	client->ip=claddr.sin_addr.s_addr;	
	return client;
}
//TODO 考虑移除
jwl_socket * jwl_socket_send_safe(jwl_socket *this,jbl_string *data)
{
	if(!this)jbl_exception("NULL POINTER");
	if(jwl_socket_is_host(this))jbl_exception("SEND FROM HOST SOCKET");	
	if(!data)return this;
	char buf[16];
	unsigned long long n=jbl_string_get_length(data);
	jbl_memory_copy(buf,&n,sizeof n);
	if((send(this->handle,buf,16,0)==-1)||
	    send(this->handle,(char*)jbl_string_get_chars(data),jbl_string_get_length(data),0)==-1)
		jbl_log(UC "SEND FAILED");
	return this;
}
jbl_string * jwl_socket_receive_safe(jwl_socket *this,jbl_string *data)
{
	if(!this)jbl_exception("NULL POINTER");
	if(jwl_socket_is_host(this))jbl_exception("RECEIVE FROM HOST SOCKET");	
    char buf[16];int ret;
	unsigned long long n=0,i=0;
	recv(this->handle,buf,16,0);
	jbl_memory_copy(&n,buf,sizeof n);	
	data=jbl_string_extend(data,n);
	while(i<n)
	{
		ret=recv(this->handle,(char*)jbl_string_get_chars(data)+jbl_string_get_length(data),JWL_SOCKET_RECEIVE_BUF_LENGTH,0);
		jbl_string_set_length(data,jbl_string_get_length(data)+ret);
		i+=ret;
	}
	return data;
}
/*
void jwl_socket_receive_length(jbl_socket_handle *this,jbl_string *data,jbl_string_size_type length)
{
	jbl_string_extend(data,length);
	for(jbl_string_size_type i=jbl_string_get_length(data);((length-(jbl_string_get_length(data)-i)))!=0;)
	{	
		int j=recv(*this,jbl_string_get_chars(data)+jbl_string_get_length(data),((length-(jbl_string_get_length(data)-i))>4096?4096:(length%4096)),0);
		if(j==-1)
			jbl_string_get_length(data)=i,jwl_exception(JWL_ERROR_SOCKET_RECEIVE_FAILED);
		jbl_string_get_length(data)+=j;
		if(j!=4096)
			break;
	}
}
*/
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
	}
	jbl_stream_push_char(out,'\n');
	return this;
}
jbl_stream *jwl_socket_stream_new(jwl_socket* socket)
{
	if(!socket)jbl_exception("NULL POINTER");
	if(-1==((jwl_socket*)jbl_refer_pull(socket))->handle)jbl_exception("NEW STREAM FROM CLOSED SOCKET");	
	if(jwl_socket_is_host(socket))jbl_exception("NEW STREAM FROM HOST SOCKET");	
	jbl_stream *this=jbl_stream_new(&jwl_stream_socket_operators,socket,JWL_SOCKET_STREAM_BUF_LENGTH,NULL,2);
	this->tmp[0].u=0;	//第0为表示当前接受的长度
	this->tmp[1].u=-1;	//第1位表示当前要求总接收
	return this;
}
#if JBL_VAR_ENABLE==1
jbl_var *jwl_socket_Vstream_new(jwl_socket* socket)
{
	if(!socket)jbl_exception("NULL POINTER");
	if(jwl_socket_is_host(socket))jbl_exception("NEW STREAM FROM HOST SOCKET");	
	jbl_stream *this=(jbl_stream *)jbl_Vstream_new(&jwl_stream_socket_operators,socket,JWL_SOCKET_STREAM_BUF_LENGTH,NULL,2);
	this->tmp[0].u=0;	//第0为表示当前接受的长度
	this->tmp[1].u=-1;	//第1位表示当前要求总接收
	return (jbl_var *)this;
}
#endif
void jwl_socket_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jwl_socket *socket=((jwl_socket*)this->data);
	socket=jbl_refer_pull(socket);
	if(this->en)
	{
		for(jbl_uint8 i=0;i<1&&socket->handle!=-1&&send(socket->handle,(char*)this->buf,this->en,0)==-1;++i)
		{
			if((errno!=EINTR&&errno!=EWOULDBLOCK&&errno!=EAGAIN))
			{
				jbl_log(UC "Send failed\terrno:%d",errno);
				jwl_socket_close(socket);
				break;
			}
			jbl_log(UC "Send failed\terrno:%d retrying %d",errno,i);
		}
		this->en=0;
	}
	else if(nxt)
	{
		while(this->tmp[0].u<this->tmp[1].u&&socket->handle!=-1)
		{
			jbl_stream_buf_size_type len=jbl_min((this->tmp[1].u-this->tmp[0].u),(nxt->size-nxt->en));
			int j;
			for(jbl_uint8 i=0;i<10&&(j=recv(socket->handle,(char*)nxt->buf+nxt->en,len,0))==-1;++i,jbl_log(UC "Receive failed\terrno:%d retrying %d",errno,i))
				if((errno!=EINTR&&errno!=EWOULDBLOCK&&errno!=EAGAIN))
				{
					jbl_log(UC "Receive failed\terrno:%d",errno);
					socket->handle=-1;
					return;
				}				
			this->tmp[0].u+=j,nxt->en+=j;
			jbl_stream_do(nxt,0);
			if(j!=len)
				break;
		}
		jbl_stream_do(nxt,flags);	
	}
}
jbl_stream_operators_new(jwl_stream_socket_operators,jwl_socket_stream_operater,jwl_socket_free,NULL);

#endif


//连接池
inline jwl_socket_poll * jwl_socket_poll_new()
{
	return jwl_socket_poll_init(jbl_malloc(sizeof(jwl_socket_poll)+
#ifdef __linux__	
	(sizeof(struct epoll_event)
#elif __APPLE__
	(sizeof(struct kevent)
#endif	
	*128)));	
}
jwl_socket_poll * jwl_socket_poll_init(jwl_socket_poll *this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jbl_gc_init(this);
	jbl_gc_plus(this);
	this->data=NULL;
	
	this->len=0;
	this->event_len=0;
#ifdef __linux__
	if(-1==(this->handle=epoll_create1(0)))jbl_exception("POLL FAILED");
#elif __APPLE__
	if(-1==(this->handle=kqueue()))jbl_exception("POLL FAILED");
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
	epoll_ctl(this->handle,EPOLL_CTL_DEL,found->socket->handle,NULL);	
#elif __APPLE__
	struct kevent ev;
	EV_SET(&ev,found->socket->handle,EVFILT_READ,EV_DELETE|EV_DISABLE,0,0,NULL);
	kevent(this->handle,&ev,1,NULL,0,NULL);
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
			epoll_ctl(this->handle,EPOLL_CTL_DEL,i->socket->handle,NULL);	
#elif __APPLE__
			struct kevent ev;
			EV_SET(&ev,i->socket->handle,EVFILT_READ,EV_DELETE|EV_DISABLE,0,0,NULL);
			kevent(this->handle,&ev,1,NULL,0,NULL);
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
	jwl_socket_poll * thi=jbl_refer_pull(this);
	if(!thi->len)return this;
#ifdef __linux__
	if(-1==(thi->event_len=epoll_wait(thi->handle,thi->events,128,-1)))			jbl_exception("POLL FAILED");
#elif __APPLE__
	if(-1==(thi->event_len=kevent(thi->handle,NULL,0,thi->events,128,NULL)))	jbl_exception("POLL FAILED");
#endif
	return this;
}
jwl_socket * jwl_socket_poll_get(jwl_socket_poll *this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jwl_socket_poll * thi=jbl_refer_pull(this);
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
