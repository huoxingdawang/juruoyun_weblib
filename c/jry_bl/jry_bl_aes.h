/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_AES_H
#define __JRY_BL_AES_H
#include "jry_bl_aes_config.h"
#if JRY_BL_AES_128_ENABLE==1
#include "jry_bl_exception.h"
#include "jry_bl_malloc.h"
#include "jry_bl_string.h"
#include "jry_bl_ying.h"
typedef unsigned char jry_bl_aes_128_extened_key[11][4][4];
void	jry_bl_aes_128_extend_key		(unsigned char* key,jry_bl_aes_128_extened_key w);
#if JRY_BL_AES_128_ECB_ENABLE==1
void	jry_bl_aes_128_ecb_encode		(jry_bl_aes_128_extened_key w,jry_bl_string *in,jry_bl_string *out);
void	jry_bl_aes_128_ecb_decode		(jry_bl_aes_128_extened_key w,jry_bl_string *in,jry_bl_string *out);
#endif
#if JRY_BL_AES_128_CBC_ENABLE==1
void	jry_bl_aes_128_cbc_encode		(jry_bl_aes_128_extened_key w,unsigned char *vi,jry_bl_string *in,jry_bl_string *out);
void	jry_bl_aes_128_cbc_decode		(jry_bl_aes_128_extened_key w,unsigned char *vi,jry_bl_string *in,jry_bl_string *out);
#endif
#endif
#endif