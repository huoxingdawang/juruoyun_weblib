/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jwl_ying.h"
#if JWL_YING_ENABLE==1
#include "jwl_include.h"
void jwl_start()
{
	jbl_exception_add_exit_function(jwl_stop);
	jwl_socket_start();
}
void jwl_stop()
{
	
}
jbl_uint64 jwl_get_binary_ip(jbl_string *this)
{
	jbl_string_size_type i=0;
	return (jbl_string_get_uint64_start(this,&i))+(jbl_string_get_uint64_start(this,&i)<<8)+(jbl_string_get_uint64_start(this,&i)<<16)+(jbl_string_get_uint64_start(this,&i)<<24);
}
jbl_string * jwl_get_string_ip(jbl_uint64 ip,jbl_string *this)
{
	this=jbl_string_extend(this,15);
	this=jbl_string_add_uint64(this,(ip&0XFF));this=jbl_string_add_char(this,'.');ip>>=8;
	this=jbl_string_add_uint64(this,(ip&0XFF));this=jbl_string_add_char(this,'.');ip>>=8;
	this=jbl_string_add_uint64(this,(ip&0XFF));this=jbl_string_add_char(this,'.');ip>>=8;
	this=jbl_string_add_uint64(this,(ip&0XFF));
	return this;
}
#endif
