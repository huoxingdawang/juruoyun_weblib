/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_wl_ying.h"
#if JRY_WL_YING_ENABLE==1
jry_bl_uint64 jry_wl_get_binary_ip(jry_bl_string *this)
{
	jry_bl_string_size_type i=0;
	return (jry_bl_string_get_uint64_start(this,&i))+(jry_bl_string_get_uint64_start(this,&i)<<8)+(jry_bl_string_get_uint64_start(this,&i)<<16)+(jry_bl_string_get_uint64_start(this,&i)<<24);
}
void jry_wl_get_string_ip(jry_bl_uint64 ip,jry_bl_string *this)
{
	jry_bl_string_extend_to(this,jry_bl_string_get_length(this)+15);
	jry_bl_string_add_uint64(this,(ip&0XFF));ip>>=8;jry_bl_string_add_char(this,'.');
	jry_bl_string_add_uint64(this,(ip&0XFF));ip>>=8;jry_bl_string_add_char(this,'.');
	jry_bl_string_add_uint64(this,(ip&0XFF));ip>>=8;jry_bl_string_add_char(this,'.');
	jry_bl_string_add_uint64(this,(ip&0XFF));
}
#endif
