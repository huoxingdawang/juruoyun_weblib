/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_WL_SOCKET_H
#define __JRY_WL_SOCKET_H
#include "jry_wl_socket_config.h"
#if JRY_WL_SOCKET_ENABLE==1
#include "../jry_bl/jry_bl_include.h"
#include "jry_wl_ying.h"
#include "jry_wl_exception.h"
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
typedef int jry_bl_socket_handle;
#else
#include <winsock2.h>
typedef SOCKET jry_bl_socket_handle;
#endif
#define JRY_BL_SOCKET_MODE_SERVER 0
#define JRY_BL_SOCKET_MODE_CLIENT 1
void	jry_wl_socket_start();
void	jry_wl_socket_init			(jry_bl_socket_handle *this,jry_bl_uint64 ip,jry_bl_uint32 port,jry_bl_uint8 type);
void	jry_wl_socket_send			(jry_bl_socket_handle *this,jry_bl_string *data);
void	jry_wl_socket_receive		(jry_bl_socket_handle *this,jry_bl_string *data);
void	jry_wl_socket_receive_length(jry_bl_socket_handle *this,jry_bl_string *data,jry_bl_string_size_type length);
void	jry_wl_socket_accept		(jry_bl_socket_handle *this,jry_bl_socket_handle *client);
void	jry_wl_socket_send_safe		(jry_bl_socket_handle *this,jry_bl_string *data);
void	jry_wl_socket_receive_safe	(jry_bl_socket_handle *this,jry_bl_string *data);
void	jry_wl_socket_close			(jry_bl_socket_handle *this);

#endif
#endif