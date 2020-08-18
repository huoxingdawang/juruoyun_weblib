/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_ying.h"
#if JBL_YING_ENABLE==1
#include "jbl_include.h"
#include <signal.h>
void jbl_signal_callback(int sign)
{
	jbl_exit(0);
}
void jbl_start()
{
	signal(SIGINT,jbl_signal_callback);
	signal(SIGTERM,jbl_signal_callback);	
	jbl_exception_add_exit_function(jbl_stop);
	jbl_log_start();
	jbl_malloc_start();
	jbl_string_start();
	jbl_stream_start();
	jbl_rand_start();
}
void jbl_stop()
{
	jbl_string_stop();
	jbl_log_stop();
	jbl_stream_stop();
	jbl_malloc_stop();
}
inline jbl_int64 jbl_ceil(long double i)
{
	return (((jbl_int64)i)==i)?((jbl_int64)i):(((jbl_int64)i)+1);
}
inline jbl_uint64 jbl_min2pow(jbl_uint64 i)
{
	jbl_uint64 j=1;
	for(;j<i;j<<=1);
	return j;
}
inline jbl_uint32 jbl_getuint32_raw(jbl_uint8*s)
{
	return ((((jbl_uint32)s[0])&0x000000FF)|(((jbl_uint32)s[1]<<8)&0x0000FF00)|(((jbl_uint32)s[2]<<16)&0x00FF0000)|(((jbl_uint32)s[3]<<24)&0xFF000000));	
}
inline jbl_uint64 jbl_getuint64_raw(jbl_uint8*s)
{
	return ((((jbl_uint64)s[0])&0x00000000000000FF)|(((jbl_uint64)s[1]<<8)&0x000000000000FF00)|(((jbl_uint64)s[2]<<16)&0x0000000000FF0000)|(((jbl_uint64)s[3]<<24)&0x00000000FF000000)|(((jbl_uint64)s[4]<<32)&0x000000FF00000000)|(((jbl_uint64)s[5]<<40)&0x0000FF0000000000)|(((jbl_uint64)s[6]<<48)&0x00FF0000)|(((jbl_uint64)s[7]<<56)&0xFF00000000000000));	
}
inline jbl_uint32 jbl_getuint64(jbl_uint8 *s)
{
	jbl_uint8 c,i=0;
	jbl_uint32 x;
	for(x=0;((c=s[i])<'0'||c>'9')&&s[i];++i);
	for(x=c-'0',++i;(c=s[i])>='0'&&c<='9'&&s[i];x=(x<<3)+(x<<1)+c-'0',++i);
	return x;
}
/*
jbl_uint8 jbl_get_uint64_length(jbl_uint64 tmp)
{
	jbl_uint8 len=0;
	do{++len;}while((tmp=(((jbl_uint64)tmp)/10)));	
	return len;
}
jbl_uint8 jbl_get_int64_length(jbl_int64 tmp)
{
	jbl_uint8 len=0;
	if(tmp<0)
		tmp=-tmp,++len;
	do{++len;}while((tmp=(((jbl_uint64)tmp)/10)));	
	return len;
}
jbl_uint8 jbl_get_double_length(double tmp)
{
	jbl_uint8 len=1;
	len+=jbl_get_int64_length(tmp);
	if(tmp<0)
		tmp=-tmp;
	tmp-=(jbl_uint64)tmp;
	jbl_uint64 ji=10;
	for(double t=tmp*ji;t-(jbl_uint64)t<(-JBL_DOUBLE_PRECISION)||t-(jbl_uint64)t>(JBL_DOUBLE_PRECISION);ji=(ji<<3)+(ji<<1),t=tmp*ji);
	len+=jbl_get_uint64_length(tmp*ji);
	return len;
}
*/
#endif
