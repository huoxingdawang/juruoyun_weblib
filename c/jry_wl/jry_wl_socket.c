/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_wl_socket.h"
#if JRY_WL_SOCKET_ENABLE==1
void jry_wl_socket_start()
{
#ifdef __linux__
#else	
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
		jry_wl_exception(JRY_WL_ERROR_SOCKET_DLL_UNLOAD);
#endif
}
void jry_wl_socket_init(jry_bl_socket_handle *this,jry_bl_uint64 ip,jry_bl_uint32 port,jry_bl_uint8 type)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	*this=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if((*this)==-1)
		jry_wl_exception(JRY_WL_ERROR_SOCKET_INIT_FAILED);		
	struct sockaddr_in skaddr;
	skaddr.sin_family=AF_INET;
	skaddr.sin_port=htons(port);
	skaddr.sin_addr.s_addr=ip;
	if(type==JRY_BL_SOCKET_MODE_SERVER)
	{
		if(bind(*this,((struct sockaddr *)&skaddr),sizeof(skaddr))==-1)
			jry_wl_socket_close(this),jry_wl_exception(JRY_WL_ERROR_SOCKET_BIND_FAILED);
		if(listen(*this,1024)==-1)
			jry_wl_socket_close(this),jry_wl_exception(JRY_WL_ERROR_SOCKET_LISTEN_FAILED);
	}
	else
	{
		if(connect(*this,(struct sockaddr*)&skaddr,sizeof(skaddr))==-1)
			jry_wl_socket_close(this),jry_wl_exception(JRY_WL_ERROR_SOCKET_CONNECT_FAILED);
	}
}
inline void	jry_wl_socket_accept(jry_bl_socket_handle *this,jry_bl_socket_handle *client)
{
	if(this==NULL||client==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	struct sockaddr_in claddr;int length=sizeof(claddr);	
	*client=accept(*this,(struct sockaddr*)&claddr,&length);
	if(*client==-1)
		jry_wl_exception(JRY_WL_ERROR_SOCKET_ACCEPT_FAILED);
}
inline void jry_wl_socket_send(jry_bl_socket_handle *this,jry_bl_string *data)
{
	if(this==NULL||data==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	send(*this,jry_bl_string_get_char_pointer(data),jry_bl_string_get_length(data),0);
}
void jry_wl_socket_receive(jry_bl_socket_handle *this,jry_bl_string *data)
{
	if(this==NULL||data==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
    char buf[256];
    int ret=0;
	do
	{
		ret=recv(*this,buf,256,0);
		jry_bl_string_add_char_pointer_length(data,buf,ret);
	}while(ret==256);
}
void jry_wl_socket_receive_length(jry_bl_socket_handle *this,jry_bl_string *data,jry_bl_string_size_type length)
{
	jry_bl_string_extend(data,length);
	jry_bl_string_get_length(data)+=recv(*this,jry_bl_string_get_char_pointer(data)+jry_bl_string_get_length(data),length,0);
}
void jry_wl_socket_send_safe(jry_bl_socket_handle *this,jry_bl_string *data)
{
	if(this==NULL||data==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	char buf[16];
	unsigned long long n=jry_bl_string_get_length(data);
	jry_bl_memory_copy(buf,&n,sizeof n);
	send(*this,buf,16,0);
	send(*this,jry_bl_string_get_char_pointer(data),jry_bl_string_get_length(data),0);
}
void jry_wl_socket_receive_safe(jry_bl_socket_handle *this,jry_bl_string *data)
{
	if(this==NULL||data==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
    char buf[256];int ret;
	unsigned long long n=0,i=0;
	recv(*this,buf,16,0);
	jry_bl_memory_copy(&n,buf,sizeof n);
	while(i<n)
		i+=(ret=recv(*this,buf,256,0)),jry_bl_string_add_char_pointer_length(data,buf,ret);
}
inline void jry_wl_socket_close(jry_bl_socket_handle *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
#ifdef __linux__	
	close(*this);
#else
	closesocket(*this);
#endif
	this=NULL;
}
#endif
