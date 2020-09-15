/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
	  http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_rand.h"
#if JBL_RAND_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_ying jbl_exception                                   */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_ying.h"
/*******************************************************************************************/
/*                            联动jbl_string                                               */
/*******************************************************************************************/
#include "jbl_string.h"
#if JBL_RAND_USE_MERSENN==1
/*******************************************************************************************/
/*                            以下函数完成梅森旋转树伪随机                               */
/*******************************************************************************************/
jbl_uint16 __jbl_rand_index;
jbl_uint32 __jbl_rand_buf[624];
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
void jbl_rand_srand(jbl_uint32 seed)
{
	__jbl_rand_buf[__jbl_rand_index=0]=seed;
	for(jbl_uint16 i=1;i<624;++i)
		__jbl_rand_buf[i]=(1812433253*(__jbl_rand_buf[i-1]^(__jbl_rand_buf[i-1]>>30))+i)&0xffffffff;
}
void __jbl_rand_generate()
{
	for(jbl_uint16 i=0;i<624;i++)
	{
		jbl_uint32 y=(__jbl_rand_buf[i]&0x80000000)+(__jbl_rand_buf[(i+1)%624]&0x7fffffff);
		__jbl_rand_buf[i]=__jbl_rand_buf[(i+397)%624]^(y>>1);
		if(y&1)__jbl_rand_buf[i]^=2567483615;
	}
}
jbl_uint32 jbl_rand()
{
	if(__jbl_rand_index==0)
		__jbl_rand_generate();
	jbl_uint32 y=__jbl_rand_buf[__jbl_rand_index];
	y=y^(y>>11);
	y=y^((y<<7)&2636928640);
	y=y^((y<<15)&4022730752);
	y=y^(y>>18);
	__jbl_rand_index=(__jbl_rand_index+1)%624;
	return y;
}
#elif JBL_RAND_USE_STDLIB==1
/*******************************************************************************************/
/*                            以下函数完成对stdlib随机函数封装                           */
/*******************************************************************************************/
#include <stdlib.h>
JBL_INLINE void jbl_rand_srand(jbl_uint32 seed){srand(seed);}
JBL_INLINE jbl_uint32 jbl_rand(){return rand();}
#else
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
jbl_uint32 __jbl_rand_next;
/*******************************************************************************************/
/*                            以下函数完成我也不知道叫啥的伪随机                        */
/*******************************************************************************************/
JBL_INLINE void jbl_rand_srand(jbl_uint32 seed){__jbl_rand_next=seed;}
JBL_INLINE jbl_uint32 jbl_rand()
{
	__jbl_rand_next=__jbl_rand_next*1103515245+12345;
	return((jbl_uint32)(__jbl_rand_next/65536)%32768);
}
#endif
JBL_INLINE jbl_uint32 jbl_rand_between(jbl_uint32 a,jbl_uint32 b)
{
	return jbl_rand()%(b-a+1)+a;
}
#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现随机字符串                                     */
/*******************************************************************************************/
jbl_string * jbl_rand_string(jbl_string *this,jbl_string_size_type len,unsigned char* s)
{
	this=jbl_string_extend(this,len);
	jbl_string * thi=jbl_refer_pull(this);
	for(jbl_string_size_type i=0,n=jbl_strlen(s);i<len;jbl_string_add_char_force(thi,s[jbl_rand_between(0,n-1)]),++i);
	return this;
}
#endif 

#endif
