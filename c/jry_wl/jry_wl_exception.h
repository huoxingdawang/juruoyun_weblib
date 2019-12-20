/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_WL_EXCEPTION_H
#define __JRY_WL_EXCEPTION_H
#include "jry_wl_exception_config.h"
#if JRY_WL_EXCEPTION_ENABLE==1
#include "jry_wl_ying.h"
#define JRY_WL_ERROR_NO_ERROR				2000000
#define JRY_WL_ERROR_SOCKET_INIT_FAILED		2000001
#define JRY_WL_ERROR_SOCKET_CONNECT_FAILED	2000002
#define JRY_WL_ERROR_SOCKET_BIND_FAILED		2000003
#define JRY_WL_ERROR_SOCKET_LISTEN_FAILED	2000004
#define JRY_WL_ERROR_SOCKET_DLL_UNLOAD		2000005
#define JRY_WL_ERROR_SOCKET_ACCEPT_FAILED	2000006





#include <stdio.h>
#include <stdlib.h>
#define jry_wl_exception(x) __jry_wl_exception(__FILE__,__LINE__,x)
void __jry_wl_exception(const char * file,int line,int x);
#endif
#endif
