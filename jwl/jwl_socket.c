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
#elif defined(__APPLE__) || defined(__linux__)
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif
//undefined reference to `__imp_WSAStartup'
void jwl_socket_start()
{
#ifdef _WIN32
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
		jbl_exception("DLL LOADING FAILED");
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
jwl_socket * jwl_socket_free(jwl_socket *this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		(jbl_gc_is_ref(this))?jwl_socket_free((jwl_socket *)jbl_refer_pull(this)):jwl_socket_close(this);
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
		this=jwl_socket_close(this),jbl_log("CONNECT FAILED");
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
		jbl_log("ACCEPT FAILED");
		return NULL;
	}
	jwl_socket *client=jwl_socket_new();
	client->handle=handle;
	client->port=ntohs(claddr.sin_port);
	client->ip=claddr.sin_addr.s_addr;	
	return client;
}
inline jwl_socket * jwl_socket_send(jwl_socket *this,jbl_string *data)
{
	if(!this)jbl_exception("NULL POINTER");
	if(jwl_socket_is_host(this))jbl_exception("SEND FROM HOST SOCKET");	
	if(data)
		if(send(this->handle,(char*)jbl_string_get_chars(data),jbl_string_get_length(data),0)==-1)
			jbl_log("SEND FAILED"),jbl_logstr(jbl_string_copy(data));
	return this;
}
jbl_string * jwl_socket_receive(jwl_socket *this,jbl_string *data)
{
	if(!this)jbl_exception("NULL POINTER");	
	if(jwl_socket_is_host(this))jbl_exception("RECEIVE FROM HOST SOCKET");		
    int ret=0;
	do
	{
		data=jbl_string_extend(data,JWL_SOCKET_RECEIVE_BUF_LENGTH);
		ret=recv(this->handle,(char*)jbl_string_get_chars(data)+jbl_string_get_length(data),JWL_SOCKET_RECEIVE_BUF_LENGTH,0);
		jbl_string_set_length(data,jbl_string_get_length(data)+ret);
	}while(ret==JWL_SOCKET_RECEIVE_BUF_LENGTH);
	return data;
}
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
		jbl_log("SEND FAILED"),jbl_logstr(jbl_string_copy(data));
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
	jwl_socket* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jwl_socket",line,varname,func,file))return this;
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
void jwl_socket_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
/*
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jwl_socket *sock=((jwl_socket*)this->data);
	sock=jbl_refer_pull(sock);
	if(this->en)
		if(send(sock->handle,(char*)this->buf,this->en,0)==-1)
			jbl_exception("SEND FAILED");	
	this->en=0;
	if(nxt!=NULL)
	{
		ret=recv(this->handle,(char*)jbl_string_get_chars(data)+jbl_string_get_length(data),JWL_SOCKET_RECEIVE_BUF_LENGTH,0);
		
	}
		while(this->tmpv<str->len)
		{
			jbl_uint16 len=jbl_min((str->len-this->tmpv),(nxt->size-nxt->en));	
			jbl_memory_copy(nxt->buf+nxt->en,str->s+this->tmpv,len);
			this->tmpv+=len;
			nxt->en+=len;
			jbl_stream_do(nxt,0);
		}
		jbl_stream_do(nxt,flags);
*/
}
jbl_stream_operators_new(jwl_stream_socket_operators,jwl_socket_stream_operater,jwl_socket_free,NULL);

/*
void jbl_string_update_stream_buf(jbl_stream* this)
{
	this=jbl_refer_pull(this);	
	this->data=jbl_string_extend_to((jbl_string*)this->data,0);
	jbl_string *str_=jbl_refer_pull(((jbl_string*)this->data));	
	this->buf=str_->s+str_->len;
}
void jbl_string_view_put(jbl_string* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs)
{
	if(jbl_stream_view_put_format(this=jbl_refer_pull(this),out,"jbl_string    ",format,str,&tabs))return;
	jbl_stream_push_chars(out,UC":size:");
	jbl_stream_push_uint(out,this->size);
	jbl_stream_push_chars(out,UC"\tlen:");
	jbl_stream_push_uint(out,this->len);
	jbl_stream_push_chars(out,UC"\ts:");
	for(jbl_string_size_type i=0;i<this->len;jbl_stream_push_char(out,this->s[i]),++i);
}
void jbl_stream_push_string(jbl_stream *out,jbl_string* this)
{
	if(out==NULL)jbl_exception("NULL POINTER");
	out=jbl_refer_pull(out);
	this=jbl_refer_pull(this);
	for(jbl_string_size_type i=0;i<this->len;++i)
		jbl_stream_push_char_force(out,this->s[i]);
}
#if JBL_JSON_ENABLE==1
void jbl_string_json_put(jbl_string* this,jbl_stream *out,char format,jbl_int32 tabs)
{
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,&tabs))return;
	jbl_stream_push_char(out,'"');
	for(jbl_string_size_type i=0;i<this->len;++i)
	{
		if(this->s[i]>31&&this->s[i]!='\"' &&this->s[i]!='\\')
			jbl_stream_push_char(out,this->s[i]);
		else
		{
			jbl_stream_push_char(out,'\\');
			jbl_uint8 token;
			switch(token=this->s[i])
			{
				case '\\':jbl_stream_push_char(out,'\\');	break;
				case '\"':jbl_stream_push_char(out,'\"');	break;
				case '\b':jbl_stream_push_char(out,'b');	break;
				case '\f':jbl_stream_push_char(out,'f');	break;
				case '\n':jbl_stream_push_char(out,'n');	break;
				case '\r':jbl_stream_push_char(out,'r');	break;
				case '\t':jbl_stream_push_char(out,'t');	break;
				default  :jbl_stream_push_chars(out,UC"u00"),jbl_stream_push_hex_8bits(out,token);break;
			}
		}		
	}
	jbl_stream_push_char(out,'"');
}
#endif
*/
#endif





















#endif
