/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_AES_H
#define __JBL_AES_H
#include "jbl_aes_config.h"
#if JBL_AES_128_ENABLE==1
#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************************/
/*                            依赖jbl_malloc jbl_ying jbl_exception jbl_gc                  */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_malloc.h"
#include "jbl_gc.h"
#include "jbl_ying.h"
#include "jbl_pthread.h"
/*******************************************************************************************/
/*                            联动jbl_stream jbl_string jbl_var                            */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_stream.h"
#include "jbl_var.h"
/*******************************************************************************************/
/*                            结构体定义                                                  */
/*******************************************************************************************/
jbl_var_operators_extern(jbl_aes_128_key_operators);


#if JBL_AES_128_AESNI_ENABLE ==1
typedef long long __m128i __attribute__ ((__vector_size__ (16), __may_alias__));
typedef __m128i __jbl_aes_128_ex_key[20];		//aes128扩展完毕的key
#else
typedef jbl_uint8 __jbl_aes_128_ex_key[11][4][4];		//aes128扩展完毕的key
#endif
typedef struct __jbl_aes_128_key
{
	jbl_gc						gc;
	jbl_var_ops_define			;
	jbl_pthread_lock_define		;
	__jbl_aes_128_ex_key key;								//密钥
}jbl_aes_128_key;											//蒟蒻云基础库AES128密钥结构
/*******************************************************************************************/
/*                            以下函实现aes密钥基本操作                                  */
/*******************************************************************************************/
jbl_aes_128_key*	jbl_aes_128_key_new						();																				//新建一个aes128加密密钥
jbl_aes_128_key*	jbl_aes_128_key_set						(jbl_aes_128_key *this,unsigned char* key);										//生成一个aes128加密密钥
jbl_aes_128_key*	jbl_aes_128_key_copy					(jbl_aes_128_key *that);														//复制一个aes128加密密钥
jbl_aes_128_key*	jbl_aes_128_key_free					(jbl_aes_128_key *this);														//释放一个aes128加密密钥



#if JBL_AES_128_ECB_ENABLE==1
#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的aes128ecb加解密操作                   */
/*******************************************************************************************/
jbl_string *	jbl_aes_128_ecb_encode					(jbl_aes_128_key *w,jbl_string *this,jbl_string *result);						//进行AES128ECB加密
jbl_string *	jbl_aes_128_ecb_decode					(jbl_aes_128_key *w,jbl_string *this,jbl_string *result);						//进行AES128ECB解密
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现stream的aes128ecb加解密操作                    */
/*******************************************************************************************/
extern const jbl_stream_operater jbl_stream_aes_128_ecb_encode_operators;																	//AES128ECB加密流处理器
extern const jbl_stream_operater jbl_stream_aes_128_ecb_decode_operators;																	//AES128ECB解密流处理器
#define			jbl_stream_aes_128_ecb_encode_new(w)	jbl_stream_new(&jbl_stream_aes_128_ecb_encode_operators,jbl_aes_128_key_copy(w),JBL_STREAM_EXCEED_LENGTH+32,NULL,0)	//新建AES128ECB加密流
#define			jbl_stream_aes_128_ecb_decode_new(w)	jbl_stream_new(&jbl_stream_aes_128_ecb_decode_operators,jbl_aes_128_key_copy(w),JBL_STREAM_EXCEED_LENGTH+32,NULL,0)	//新建AES128ECB解密流

#endif
#endif
#if JBL_AES_128_CBC_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的aes128cbc加解密操作                   */
/*******************************************************************************************/
jbl_string *	jbl_aes_128_cbc_encode					(jbl_aes_128_key *w,unsigned char *vi,jbl_string *this,jbl_string *result);	//进行AES128CBC加密
jbl_string *	jbl_aes_128_cbc_decode					(jbl_aes_128_key *w,unsigned char *vi,jbl_string *this,jbl_string *result);	//进行AES128CBC解密
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现stream的aes128cbc加解密操作                    */
/*******************************************************************************************/
extern const jbl_stream_operater jbl_stream_aes_128_cbc_encode_operators;	//AES128CBC加密流处理器
extern const jbl_stream_operater jbl_stream_aes_128_cbc_decode_operators;	//AES128CBC解密流处理器
jbl_stream * jbl_stream_aes_128_cbc_encode_new(jbl_aes_128_key *w,const unsigned char * v);
jbl_stream * jbl_stream_aes_128_cbc_decode_new(jbl_aes_128_key *w,const unsigned char * v);

#endif

#endif
#endif
#ifdef __cplusplus
}
#endif
#endif