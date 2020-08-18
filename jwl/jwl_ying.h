/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JWL_YING_H
#define __JWL_YING_H
#include "jwl_ying_config.h"
#if JWL_YING_ENABLE==1
#include "../jbl/jbl_include.h"
void			jwl_start();
void			jwl_stop();
jbl_uint32		jwl_get_binary_ip(jbl_string *this);
jbl_uint32		jwl_get_binary_ip_chars(jbl_uint8 *s);
jbl_string *	jwl_get_string_ip(jbl_uint64 ip,jbl_string *this);
#endif
#endif