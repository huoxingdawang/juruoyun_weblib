/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_WL_YING_H
#define __JRY_WL_YING_H
#include "jry_wl_ying_config.h"
#if JRY_WL_YING_ENABLE==1
#include "../jry_bl/jry_bl_include.h"
#define jry_wl_start()	\
jry_wl_socket_start()
jry_bl_uint64	jry_wl_get_binary_ip(jry_bl_string *this);
void			jry_wl_get_string_ip(jry_bl_uint64 ip,jry_bl_string *this);

#endif
#endif