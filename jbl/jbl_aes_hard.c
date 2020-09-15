/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
	  http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_aes.h"
#if JBL_AES_128_ENABLE==1
#if JBL_AES_128_AESNI_ENABLE ==1
#include <wmmintrin.h>
#define __jbl_aes_128_key_exp(k,rcon) __jbl_aes_128_key_expansion(k,_mm_aeskeygenassist_si128(k,rcon))

static __m128i __jbl_aes_128_key_expansion(__m128i key, __m128i keygened)
{
	keygened=_mm_shuffle_epi32(keygened,_MM_SHUFFLE(3,3,3,3));
	key=_mm_xor_si128(key,_mm_slli_si128(key,4));
	key=_mm_xor_si128(key,_mm_slli_si128(key,4));
	key=_mm_xor_si128(key,_mm_slli_si128(key,4));
	return _mm_xor_si128(key,keygened);
}
jbl_aes_128_key* jbl_aes_128_key_set(jbl_aes_128_key *this,unsigned char* key)
{
	if(!this)this=jbl_aes_128_key_new();
	jbl_reference *ref=NULL;jbl_aes_128_key *thi=jbl_refer_pull_keep_father(this,&ref);
	if(jbl_gc_refcnt(thi)!=1)
	{
		jbl_aes_128_key_free(thi);
		jbl_aes_128_key_free(this),thi=jbl_aes_128_key_new();
	}
	thi->key[0] =_mm_loadu_si128((const __m128i*) key);
	thi->key[1] =__jbl_aes_128_key_exp(thi->key[0],0x01);
	thi->key[2] =__jbl_aes_128_key_exp(thi->key[1],0x02);
	thi->key[3] =__jbl_aes_128_key_exp(thi->key[2],0x04);
	thi->key[4] =__jbl_aes_128_key_exp(thi->key[3],0x08);
	thi->key[5] =__jbl_aes_128_key_exp(thi->key[4],0x10);
	thi->key[6] =__jbl_aes_128_key_exp(thi->key[5],0x20);
	thi->key[7] =__jbl_aes_128_key_exp(thi->key[6],0x40);
	thi->key[8] =__jbl_aes_128_key_exp(thi->key[7],0x80);
	thi->key[9] =__jbl_aes_128_key_exp(thi->key[8],0x1B);
	thi->key[10]=__jbl_aes_128_key_exp(thi->key[9],0x36);
	thi->key[11]=_mm_aesimc_si128(thi->key[9]);
	thi->key[12]=_mm_aesimc_si128(thi->key[8]);
	thi->key[13]=_mm_aesimc_si128(thi->key[7]);
	thi->key[14]=_mm_aesimc_si128(thi->key[6]);
	thi->key[15]=_mm_aesimc_si128(thi->key[5]);
	thi->key[16]=_mm_aesimc_si128(thi->key[4]);
	thi->key[17]=_mm_aesimc_si128(thi->key[3]);
	thi->key[18]=_mm_aesimc_si128(thi->key[2]);
	thi->key[19]=_mm_aesimc_si128(thi->key[1]);		
		
	if(ref)ref->ptr=thi;else this=thi;
	return this;
}
void __jbl_aes_128_encode_16(__jbl_aes_128_ex_key w,jbl_uint8 *a,jbl_uint8 *b)
{
	__m128i m=_mm_loadu_si128((__m128i*)a);
	m = _mm_xor_si128		(m, w[ 0]);
	m = _mm_aesenc_si128	(m, w[ 1]);
	m = _mm_aesenc_si128	(m, w[ 2]);
	m = _mm_aesenc_si128	(m, w[ 3]);
	m = _mm_aesenc_si128	(m, w[ 4]);
	m = _mm_aesenc_si128	(m, w[ 5]);
	m = _mm_aesenc_si128	(m, w[ 6]);
	m = _mm_aesenc_si128	(m, w[ 7]);
	m = _mm_aesenc_si128	(m, w[ 8]);
	m = _mm_aesenc_si128	(m, w[ 9]);
	m = _mm_aesenclast_si128(m, w[10]);
	_mm_storeu_si128((__m128i*)b,m);
}

void __jbl_aes_128_decode_16(__jbl_aes_128_ex_key w,jbl_uint8 *b,jbl_uint8 *a){
	__m128i m = _mm_loadu_si128((__m128i *) b);
	m = _mm_xor_si128		(m, w[10]);
	m = _mm_aesdec_si128	(m, w[11]);
	m = _mm_aesdec_si128	(m, w[12]);
	m = _mm_aesdec_si128	(m, w[13]);
	m = _mm_aesdec_si128	(m, w[14]);
	m = _mm_aesdec_si128	(m, w[15]);
	m = _mm_aesdec_si128	(m, w[16]);
	m = _mm_aesdec_si128	(m, w[17]);
	m = _mm_aesdec_si128	(m, w[18]);
	m = _mm_aesdec_si128	(m, w[19]);
	m = _mm_aesdeclast_si128(m, w[0]);
	_mm_storeu_si128((__m128i*)a,m);
}





#endif
#endif