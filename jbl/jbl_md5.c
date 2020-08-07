/* Copyright (c) [2020] juruoyun developer team
	 Juruoyun basic lib is licensed under the Mulan PSL v1.
	 You can use this software according to the terms and conditions of the Mulan PSL v1.
	 You may obtain a copy of Mulan PSL v1 at:
			http://license.coscl.org.cn/MulanPSL
	 THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
	 IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
	 PURPOSE.
	 See the Mulan PSL v1 for more details.*/
#include "jbl_md5.h"
#if JBL_MD5_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_ying jbl_exception jbl_string                        */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_ying.h"
#include "jbl_string.h"
/*******************************************************************************************/
/*                            以下函实现md5                                               */
/*******************************************************************************************/
#define F(x,y,z) (((x)&(y))|((~x)&(z)))
#define G(x,y,z) (((x)&(z))|((y)&(~z)))
#define H(x,y,z) ((x)^(y)^(z))
#define I(x,y,z) ((y)^((x)|(~z)))
#define RL(num,n) ((((jbl_uint64)(num))<<(n))|(((jbl_uint64)(num))>>(32-(n))))
#define FF(a,b,c,d,x,s,ac) {(a)+=F((b),(c),(d))+(x)+ac;(a)=RL((a),(s));(a)+=(b);}
#define GG(a,b,c,d,x,s,ac) {(a)+=G((b),(c),(d))+(x)+ac;(a)=RL((a),(s));(a)+=(b);}
#define HH(a,b,c,d,x,s,ac) {(a)+=H((b),(c),(d))+(x)+ac;(a)=RL((a),(s));(a)+=(b);}
#define II(a,b,c,d,x,s,ac) {(a)+=I((b),(c),(d))+(x)+ac;(a)=RL((a),(s));(a)+=(b);}
void __jbl_md5_transform(jbl_uint32 state[4],const jbl_uint8 block[64])
{
	jbl_uint32 a=state[0],b=state[1],c=state[2],d=state[3];
	jbl_uint32 x[16];
	for(jbl_uint8 i=0,j=0;j<64;++i,j+=4)
		x[i]=((jbl_uint32)block[j])|(((jbl_uint32)block[j+1])<<8)|(((jbl_uint32)block[j+2])<<16)|(((jbl_uint32)block[j+3])<<24);
	FF(a,b,c,d,x[ 0], 7,0xd76aa478);FF(d,a,b,c,x[ 1],12,0xe8c7b756);FF(c,d,a,b,x[ 2],17,0x242070db);FF(b,c,d,a,x[ 3],22,0xc1bdceee);
	FF(a,b,c,d,x[ 4], 7,0xf57c0faf);FF(d,a,b,c,x[ 5],12,0x4787c62a);FF(c,d,a,b,x[ 6],17,0xa8304613);FF(b,c,d,a,x[ 7],22,0xfd469501);
	FF(a,b,c,d,x[ 8], 7,0x698098d8);FF(d,a,b,c,x[ 9],12,0x8b44f7af);FF(c,d,a,b,x[10],17,0xffff5bb1);FF(b,c,d,a,x[11],22,0x895cd7be);
	FF(a,b,c,d,x[12], 7,0x6b901122);FF(d,a,b,c,x[13],12,0xfd987193);FF(c,d,a,b,x[14],17,0xa679438e);FF(b,c,d,a,x[15],22,0x49b40821);
	GG(a,b,c,d,x[ 1], 5,0xf61e2562);GG(d,a,b,c,x[ 6], 9,0xc040b340);GG(c,d,a,b,x[11],14,0x265e5a51);GG(b,c,d,a,x[ 0],20,0xe9b6c7aa);
	GG(a,b,c,d,x[ 5], 5,0xd62f105d);GG(d,a,b,c,x[10], 9,0x02441453);GG(c,d,a,b,x[15],14,0xd8a1e681);GG(b,c,d,a,x[ 4],20,0xe7d3fbc8);
	GG(a,b,c,d,x[ 9], 5,0x21e1cde6);GG(d,a,b,c,x[14], 9,0xc33707d6);GG(c,d,a,b,x[ 3],14,0xf4d50d87);GG(b,c,d,a,x[ 8],20,0x455a14ed);
	GG(a,b,c,d,x[13], 5,0xa9e3e905);GG(d,a,b,c,x[ 2], 9,0xfcefa3f8);GG(c,d,a,b,x[ 7],14,0x676f02d9);GG(b,c,d,a,x[12],20,0x8d2a4c8a);
	HH(a,b,c,d,x[ 5], 4,0xfffa3942);HH(d,a,b,c,x[ 8],11,0x8771f681);HH(c,d,a,b,x[11],16,0x6d9d6122);HH(b,c,d,a,x[14],23,0xfde5380c);
	HH(a,b,c,d,x[ 1], 4,0xa4beea44);HH(d,a,b,c,x[ 4],11,0x4bdecfa9);HH(c,d,a,b,x[ 7],16,0xf6bb4b60);HH(b,c,d,a,x[10],23,0xbebfbc70);
	HH(a,b,c,d,x[13], 4,0x289b7ec6);HH(d,a,b,c,x[ 0],11,0xeaa127fa);HH(c,d,a,b,x[ 3],16,0xd4ef3085);HH(b,c,d,a,x[ 6],23,0x04881d05);
	HH(a,b,c,d,x[ 9], 4,0xd9d4d039);HH(d,a,b,c,x[12],11,0xe6db99e5);HH(c,d,a,b,x[15],16,0x1fa27cf8);HH(b,c,d,a,x[ 2],23,0xc4ac5665);
	II(a,b,c,d,x[ 0], 6,0xf4292244);II(d,a,b,c,x[ 7],10,0x432aff97);II(c,d,a,b,x[14],15,0xab9423a7);II(b,c,d,a,x[ 5],21,0xfc93a039);
	II(a,b,c,d,x[12], 6,0x655b59c3);II(d,a,b,c,x[ 3],10,0x8f0ccc92);II(c,d,a,b,x[10],15,0xffeff47d);II(b,c,d,a,x[ 1],21,0x85845dd1);
	II(a,b,c,d,x[ 8], 6,0x6fa87e4f);II(d,a,b,c,x[15],10,0xfe2ce6e0);II(c,d,a,b,x[ 6],15,0xa3014314);II(b,c,d,a,x[13],21,0x4e0811a1);
	II(a,b,c,d,x[ 4], 6,0xf7537e82);II(d,a,b,c,x[11],10,0xbd3af235);II(c,d,a,b,x[ 2],15,0x2ad7d2bb);II(b,c,d,a,x[ 9],21,0xeb86d391);
	state[0]+=a;state[1]+=b;state[2]+=c;state[3]+=d;
}
void __jbl_md5_encode(const jbl_uint32* input,jbl_uint8* output,jbl_string_size_type length)
{
	for(jbl_string_size_type i=0,j=0;j<length;++i,j+=4)
		output[j]=(jbl_uint8)(input[i]&0xff),output[j+1]=(jbl_uint8)((input[i]>>8)&0xff),output[j+2]=(jbl_uint8)((input[i]>>16)&0xff),output[j+3]=(jbl_uint8)((input[i]>>24)&0xff);
}
void __jbl_md5_init(const jbl_uint8* input,jbl_string_size_type len,jbl_uint32 state[4],jbl_uint32 count[2],jbl_uint8 buffer[64])
{
	jbl_uint32 i,index,partlen;
	index=(jbl_uint32)((count[0]>>3)&0x3f);
	if((count[0]+=((jbl_uint32)len<<3))<((jbl_uint32)len<<3))
		++count[1];
	count[1]+=((jbl_uint32)len>>29);
	partlen=64-index;
	if (len>=partlen)
	{
		jbl_memory_copy(&buffer[index],input,partlen);
		__jbl_md5_transform(state,buffer);
		for(i=partlen;i+63<len;i+=64)
			__jbl_md5_transform(state,&input[i]);
		index = 0;
	}
	else
		i = 0;
	jbl_memory_copy(&buffer[index],&input[i],len-i);
}
jbl_string* jbl_md5(jbl_string* this,jbl_string* out)
{
	if(this==NULL)jbl_exception("NULL POINTER");
	jbl_uint32 state[4]={0x67452301,0xefcdab89,0x98badcfe,0x10325476},count[2]={0,0};
	jbl_uint8  padding[64]={0x80};
	jbl_uint8  buffer[64],digest[16];
	jbl_string *		thi=jbl_refer_pull(this);
	jbl_string_size_type	len=jbl_string_get_length_force(thi);
	__jbl_md5_init(jbl_string_get_chars_force(thi),len,state,count,buffer);
	jbl_uint8 bits[8];
	jbl_uint32  index,padlen;
	__jbl_md5_encode(count,bits,8);
	index=(jbl_uint32)((count[0]>>3)&0x3f);
	padlen=(index<56)?(56-index):(120-index);
	__jbl_md5_init(padding,padlen,state,count,buffer);
	__jbl_md5_init(bits,8,state,count,buffer);
	__jbl_md5_encode(state,digest,16);
	out=jbl_string_extend(out,32);
	for (jbl_string_size_type i=0;i<16;++i)
		out=jbl_string_add_hex_8bits(out,digest[i]);
	return out;
}
#endif