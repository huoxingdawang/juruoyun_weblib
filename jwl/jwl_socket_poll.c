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
#include "jwl_socket_poll.h"
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
#elif __APPLE__
	this->event_len=0;
	if(-1==(this->handle=kqueue()))jbl_exception("POLL FAILED");
#elif __linux__
	this->event_len=0;
	if(-1==(this->handle=epoll_create1(0)))jbl_exception("POLL FAILED");
#endif
	return this;
}
inline jwl_socket_poll *jwl_socket_poll_copy(jwl_socket_poll * this)
{
	jbl_gc_plus(this);
	return this;
}
void __jwl_socket_poll_data_free(jwl_socket_poll_data *this)
{
	this->socket=jwl_socket_free(this->socket);
	this->sockets=jbl_stream_free(this->sockets);
	this->ins=jbl_stream_free(this->ins);
	this->outs=jbl_stream_free(this->outs);
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
			for(jwl_socket_poll_data *i=this->data,*j=NULL;i;__jwl_socket_poll_data_free(i),j=i->nxt,jbl_free(i),i=j);
#if defined(__APPLE__) || defined(__linux__)
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
jwl_socket_poll *jwl_socket_poll_add(jwl_socket_poll *this,jwl_socket* socket)
{
	if(!this||!socket)jbl_exception("NULL POINTER");	
	jwl_socket_poll * thi=jbl_refer_pull(this);

	jwl_socket_poll_data *data=jbl_malloc(sizeof(jwl_socket_poll_data));
	data->socket=jwl_socket_copy(socket);
#if JBL_STREAM_ENABLE==1
	data->sockets=NULL;
	data->ins=NULL;
	data->outs=NULL;
#endif
	data->nxt=thi->data;
	thi->data=data;

	++thi->len;
	socket=jbl_refer_pull(socket);	
#ifdef __linux__
	struct epoll_event ev;
	ev.data.fd=socket->handle;
	ev.data.ptr=data;
    ev.events=EPOLLIN|EPOLLET;
    epoll_ctl(this->handle,EPOLL_CTL_ADD,socket->handle,&ev);	
#elif __APPLE__
	struct kevent ev;
	EV_SET(&ev,socket->handle,EVFILT_READ,EV_ADD|EV_ENABLE,0,0,NULL);
	ev.udata=data;
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
	__jwl_socket_poll_data_free(found);
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
		if(jwl_socket_closed(i->socket))
		{
			jwl_socket_poll_data *j=i->nxt;
#ifdef __linux__
			epoll_ctl(this->handle,EPOLL_CTL_DEL,((jwl_socket*)jbl_refer_pull(i->socket))->handle,NULL);	
#elif __APPLE__
			struct kevent ev;
			EV_SET(&ev,((jwl_socket*)jbl_refer_pull(i->socket))->handle,EVFILT_READ,EV_DELETE|EV_DISABLE,0,0,NULL);
			kevent(this->handle,&ev,1,NULL,0,NULL);
#elif _WIN32
			if(thi->j==i)thi->j=pre;
#endif
			__jwl_socket_poll_data_free(i);
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
jwl_socket_poll_data * jwl_socket_poll_get(jwl_socket_poll *this)
{
	if(!this)jbl_exception("NULL POINTER");	
	jwl_socket_poll * thi=jbl_refer_pull(this);
#ifdef _WIN32
	for(jwl_socket_poll_data *jj=(thi->j)?thi->j->nxt:NULL;thi->j;thi->j=jj,jj=(thi->j)?thi->j->nxt:NULL)
		if(FD_ISSET(((jwl_socket*)jbl_refer_pull(thi->j->socket))->handle,&thi->fd))
			{jwl_socket_poll_data *jjj=thi->j;thi->j=jj,jj=(thi->j)?thi->j->nxt:NULL;return jjj;}
#else
	while(thi->event_len)
	{
#ifdef __linux__
		jwl_socket_poll_data *data=(jwl_socket_poll_data*)thi->events[--thi->event_len].data.ptr;
#elif __APPLE__
		jwl_socket_poll_data *data=(jwl_socket_poll_data*)thi->events[--thi->event_len].udata;
#endif
		jwl_socket *sock=jbl_refer_pull(data->socket);
		if(-1==sock->handle)this=jwl_socket_poll_remove(this,data->socket); 
		else				return data;
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
