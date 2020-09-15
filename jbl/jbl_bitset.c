/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_bitset.h"
#if JBL_BITSET_ENABLE==1
jbl_uint8 jbl_highbit(jbl_uint64 a)
{
	jbl_uint32 b=-1;
	if(a>=0X80000000)	b+=32	,a>>=32;
	if(a>=0X8000)		b+=16	,a>>=16;
	if(a>=0X80)			b+=8	,a>>=8;
	if(a>=0X8)			b+=4	,a>>=4;
	if(a>=0X4)			b+=2	,a>>=2;
	if(a>=0X2)			b+=1	,a>>=1;
	if(a>=0X1)			b+=1;
//	while(a)++b,a>>=1;
	return b;
}
jbl_uint8 jbl_highbit0(jbl_uint64 a)
{
	a=~a;
	jbl_uint32 b=-1;
	if(a>=0X80000000)	b+=32	,a>>=32;
	if(a>=0X8000)		b+=16	,a>>=16;
	if(a>=0X80)			b+=8	,a>>=8;
	if(a>=0X8)			b+=4	,a>>=4;
	if(a>=0X4)			b+=2	,a>>=2;
	if(a>=0X2)			b+=1	,a>>=1;
	if(a>=0X1)			b+=1;
	return b;
}
jbl_uint8 jbl_highbit32(jbl_uint32 a)
{
	jbl_uint32 b=-1;
	if(a>=0X8000)		b+=16	,a>>=16;
	if(a>=0X80)			b+=8	,a>>=8;
	if(a>=0X8)			b+=4	,a>>=4;
	if(a>=0X4)			b+=2	,a>>=2;
	if(a>=0X2)			b+=1	,a>>=1;
	if(a>=0X1)			b+=1;
	return b;
}
jbl_uint8 jbl_highbit320(jbl_uint32 a)
{
	a=~a;
	jbl_uint32 b=-1;
	if(a>=0X8000)		b+=16	,a>>=16;
	if(a>=0X80)			b+=8	,a>>=8;
	if(a>=0X8)			b+=4	,a>>=4;
	if(a>=0X4)			b+=2	,a>>=2;
	if(a>=0X2)			b+=1	,a>>=1;
	if(a>=0X1)			b+=1;
	return b;
}
#define	slm(a,b)	(((b)>=64)?0:((a)<<(b)))
#if jbl_bitset_bits==32
#define hb(x)	jbl_highbit32(x)
#define hb0(x)	jbl_highbit320(x)
#else 
#define hb(x)	jbl_highbit(x)
#define hb0(x)	jbl_highbit0(x)
#endif
JBL_INLINE void jbl_bitset_init(jbl_bitset_type *bitset,jbl_uint32 len)
{
	while(len--)
		bitset[len]=0;
}
void jbl_bitset_set(jbl_bitset_type *bitset,jbl_uint32 i,jbl_uint32 cnt)
{
	jbl_uint32 start_page=(i>>jbl_bitset_2bits);
	jbl_uint32 end_page=((cnt+i-1)>>jbl_bitset_2bits);
	cnt=cnt+i-1-(end_page<<jbl_bitset_2bits);
	i-=(start_page<<jbl_bitset_2bits);
	if(start_page==end_page)
		{bitset[start_page]|=slm(slm(1LL,(cnt+1-i))-1,(jbl_bitset_bits-cnt-1));return;}
	bitset[start_page]|=(slm(1LL,(jbl_bitset_bits-i))-1);
	++start_page;
	while(start_page<end_page)bitset[start_page]=-1,++start_page;
	bitset[end_page]|=slm((slm(1LL,(cnt+1))-1),(jbl_bitset_bits-cnt-1));	
}
void jbl_bitset_reset(jbl_bitset_type *bitset,jbl_uint32 i,jbl_uint32 cnt)
{
	jbl_uint32 start_page=(i>>jbl_bitset_2bits);
	jbl_uint32 end_page=((cnt+i-1)>>jbl_bitset_2bits);
	cnt=cnt+i-1-(end_page<<jbl_bitset_2bits);
	i-=(start_page<<jbl_bitset_2bits);
	if(start_page==end_page)
		{bitset[start_page]&=~slm(slm(1LL,(cnt+1-i))-1,(jbl_bitset_bits-cnt-1));return;}
	bitset[start_page]&=~(slm(1LL,(jbl_bitset_bits-i))-1);
	++start_page;
	while(start_page<end_page)bitset[start_page]=0,++start_page;
	bitset[end_page]&=~slm((slm(1LL,(cnt+1))-1),(jbl_bitset_bits-cnt-1));	
}
jbl_uint32 jbl_bitset_find0(jbl_bitset_type *bitset,jbl_uint32 i,jbl_uint32 len)
{
	jbl_uint32 p=(i>>jbl_bitset_2bits);
	i-=(p<<jbl_bitset_2bits);	
	jbl_bitset_type tmp;
	tmp=bitset[p];
	tmp|=slm((slm(1LL,(i))-1),(jbl_bitset_bits-i));
	if(tmp!=-1)
		return ((jbl_uint32)p<<jbl_bitset_2bits)+jbl_bitset_bits-hb0(tmp)-1;
	++p;
	while(bitset[p]==-1&&p<=len)++p;
	if(p>=(len))
		return (len<<jbl_bitset_2bits);
	return ((jbl_uint32)p<<jbl_bitset_2bits)+jbl_bitset_bits-hb0(bitset[p])-1;
}
jbl_uint32 jbl_bitset_find1(jbl_bitset_type *bitset,jbl_uint32 i,jbl_uint32 len)
{
	jbl_uint32 p=(i>>jbl_bitset_2bits);
	i-=(p<<jbl_bitset_2bits);	
	jbl_bitset_type tmp;
	tmp=bitset[p];
	tmp&=(slm(1LL,(jbl_bitset_bits-i))-1);
	if(tmp!=0)
		return ((jbl_uint32)p<<jbl_bitset_2bits)+jbl_bitset_bits-hb(tmp)-1;
	++p;
	while(bitset[p]==0&&p<=len)++p;	
	if(p>=(len))
		return (len<<jbl_bitset_2bits);
	return ((jbl_uint32)p<<jbl_bitset_2bits)+jbl_bitset_bits-hb(bitset[p])-1;
}
#undef slm
#undef hb
#undef hb0
#endif
