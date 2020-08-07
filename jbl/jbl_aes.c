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
/*******************************************************************************************/
/*                            依赖jbl_malloc jbl_ying jbl_exception                        */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_malloc.h"
#include "jbl_ying.h"
/*******************************************************************************************/
/*                            联动jbl_stream jbl_string jbl_var                            */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_stream.h"
#include "jbl_var.h"
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
static const jbl_uint8 __jbl_aes_sbox[] =
{
	0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,  
	0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
	0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15, 
	0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75, 
	0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84, 
	0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
	0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,  
	0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2, 
	0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73, 
	0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb, 
	0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
	0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
	0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a, 
	0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
	0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf, 
	0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};
static const jbl_uint8 __jbl_aes_invsbox[] = 
{
	0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb, 
	0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
	0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e, 
	0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25, 
	0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92, 
	0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84, 
	0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06, 
	0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
	0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73, 
	0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
	0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
	0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4, 
	0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f, 
	0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef, 
	0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61, 
	0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d 
};
/*******************************************************************************************/
/*                            以下函实现密钥基本操作                                     */
/*******************************************************************************************/
void			__jbl_aes_128_encode_16		(__jbl_aes_128_ex_key *w,jbl_uint8* a,jbl_uint8* b);
void			__jbl_aes_128_decode_16		(__jbl_aes_128_ex_key *w,jbl_uint8* a,jbl_uint8 *b);
#if JBL_AES_128_FFMUL_TABLE_ENABLE ==1
#define ffmul(x,y)	__jbl_aes_128_ffmul_##x[y]
static const jbl_uint8	__jbl_aes_128_ffmul_0x02[] ={0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,27,25,31,29,19,17,23,21,11,9,15,13,3,1,7,5,59,57,63,61,51,49,55,53,43,41,47,45,35,33,39,37,91,89,95,93,83,81,87,85,75,73,79,77,67,65,71,69,123,121,127,125,115,113,119,117,107,105,111,109,99,97,103,101,155,153,159,157,147,145,151,149,139,137,143,141,131,129,135,133,187,185,191,189,179,177,183,181,171,169,175,173,163,161,167,165,219,217,223,221,211,209,215,213,203,201,207,205,195,193,199,197,251,249,255,253,243,241,247,245,235,233,239,237,227,225,231,229};
static const jbl_uint8	__jbl_aes_128_ffmul_0x03[] ={0,3,6,5,12,15,10,9,24,27,30,29,20,23,18,17,48,51,54,53,60,63,58,57,40,43,46,45,36,39,34,33,96,99,102,101,108,111,106,105,120,123,126,125,116,119,114,113,80,83,86,85,92,95,90,89,72,75,78,77,68,71,66,65,192,195,198,197,204,207,202,201,216,219,222,221,212,215,210,209,240,243,246,245,252,255,250,249,232,235,238,237,228,231,226,225,160,163,166,165,172,175,170,169,184,187,190,189,180,183,178,177,144,147,150,149,156,159,154,153,136,139,142,141,132,135,130,129,155,152,157,158,151,148,145,146,131,128,133,134,143,140,137,138,171,168,173,174,167,164,161,162,179,176,181,182,191,188,185,186,251,248,253,254,247,244,241,242,227,224,229,230,239,236,233,234,203,200,205,206,199,196,193,194,211,208,213,214,223,220,217,218,91,88,93,94,87,84,81,82,67,64,69,70,79,76,73,74,107,104,109,110,103,100,97,98,115,112,117,118,127,124,121,122,59,56,61,62,55,52,49,50,35,32,37,38,47,44,41,42,11,8,13,14,7,4,1,2,19,16,21,22,31,28,25,26};
static const jbl_uint8	__jbl_aes_128_ffmul_0x0e[] ={0,14,28,18,56,54,36,42,112,126,108,98,72,70,84,90,224,238,252,242,216,214,196,202,144,158,140,130,168,166,180,186,219,213,199,201,227,237,255,241,171,165,183,185,147,157,143,129,59,53,39,41,3,13,31,17,75,69,87,89,115,125,111,97,173,163,177,191,149,155,137,135,221,211,193,207,229,235,249,247,77,67,81,95,117,123,105,103,61,51,33,47,5,11,25,23,118,120,106,100,78,64,82,92,6,8,26,20,62,48,34,44,150,152,138,132,174,160,178,188,230,232,250,244,222,208,194,204,65,79,93,83,121,119,101,107,49,63,45,35,9,7,21,27,161,175,189,179,153,151,133,139,209,223,205,195,233,231,245,251,154,148,134,136,162,172,190,176,234,228,246,248,210,220,206,192,122,116,102,104,66,76,94,80,10,4,22,24,50,60,46,32,236,226,240,254,212,218,200,198,156,146,128,142,164,170,184,182,12,2,16,30,52,58,40,38,124,114,96,110,68,74,88,86,55,57,43,37,15,1,19,29,71,73,91,85,127,113,99,109,215,217,203,197,239,225,243,253,167,169,187,181,159,145,131,141};
static const jbl_uint8	__jbl_aes_128_ffmul_0x0b[] ={0,11,22,29,44,39,58,49,88,83,78,69,116,127,98,105,176,187,166,173,156,151,138,129,232,227,254,245,196,207,210,217,123,112,109,102,87,92,65,74,35,40,53,62,15,4,25,18,203,192,221,214,231,236,241,250,147,152,133,142,191,180,169,162,246,253,224,235,218,209,204,199,174,165,184,179,130,137,148,159,70,77,80,91,106,97,124,119,30,21,8,3,50,57,36,47,141,134,155,144,161,170,183,188,213,222,195,200,249,242,239,228,61,54,43,32,17,26,7,12,101,110,115,120,73,66,95,84,247,252,225,234,219,208,205,198,175,164,185,178,131,136,149,158,71,76,81,90,107,96,125,118,31,20,9,2,51,56,37,46,140,135,154,145,160,171,182,189,212,223,194,201,248,243,238,229,60,55,42,33,16,27,6,13,100,111,114,121,72,67,94,85,1,10,23,28,45,38,59,48,89,82,79,68,117,126,99,104,177,186,167,172,157,150,139,128,233,226,255,244,197,206,211,216,122,113,108,103,86,93,64,75,34,41,52,63,14,5,24,19,202,193,220,215,230,237,240,251,146,153,132,143,190,181,168,163};
static const jbl_uint8	__jbl_aes_128_ffmul_0x0d[] ={0,13,26,23,52,57,46,35,104,101,114,127,92,81,70,75,208,221,202,199,228,233,254,243,184,181,162,175,140,129,150,155,187,182,161,172,143,130,149,152,211,222,201,196,231,234,253,240,107,102,113,124,95,82,69,72,3,14,25,20,55,58,45,32,109,96,119,122,89,84,67,78,5,8,31,18,49,60,43,38,189,176,167,170,137,132,147,158,213,216,207,194,225,236,251,246,214,219,204,193,226,239,248,245,190,179,164,169,138,135,144,157,6,11,28,17,50,63,40,37,110,99,116,121,90,87,64,77,218,215,192,205,238,227,244,249,178,191,168,165,134,139,156,145,10,7,16,29,62,51,36,41,98,111,120,117,86,91,76,65,97,108,123,118,85,88,79,66,9,4,19,30,61,48,39,42,177,188,171,166,133,136,159,146,217,212,195,206,237,224,247,250,183,186,173,160,131,142,153,148,223,210,197,200,235,230,241,252,103,106,125,112,83,94,73,68,15,2,21,24,59,54,33,44,12,1,22,27,56,53,34,47,100,105,126,115,80,93,74,71,220,209,198,203,232,229,242,255,180,185,174,163,128,141,154,151};
static const jbl_uint8	__jbl_aes_128_ffmul_0x09[] ={0,9,18,27,36,45,54,63,72,65,90,83,108,101,126,119,144,153,130,139,180,189,166,175,216,209,202,195,252,245,238,231,59,50,41,32,31,22,13,4,115,122,97,104,87,94,69,76,171,162,185,176,143,134,157,148,227,234,241,248,199,206,213,220,118,127,100,109,82,91,64,73,62,55,44,37,26,19,8,1,230,239,244,253,194,203,208,217,174,167,188,181,138,131,152,145,77,68,95,86,105,96,123,114,5,12,23,30,33,40,51,58,221,212,207,198,249,240,235,226,149,156,135,142,177,184,163,170,236,229,254,247,200,193,218,211,164,173,182,191,128,137,146,155,124,117,110,103,88,81,74,67,52,61,38,47,16,25,2,11,215,222,197,204,243,250,225,232,159,150,141,132,187,178,169,160,71,78,85,92,99,106,113,120,15,6,29,20,43,34,57,48,154,147,136,129,190,183,172,165,210,219,192,201,246,255,228,237,10,3,24,17,46,39,60,53,66,75,80,89,102,111,116,125,161,168,179,186,133,140,151,158,233,224,251,242,205,196,223,214,49,56,35,42,21,28,7,14,121,112,107,98,93,84,79,70};
#else
#define ffmul(x,y)	__jbl_aes_128_ffmul(x,y)
inline jbl_uint8 __jbl_aes_128_ffmul(jbl_uint8 a,jbl_uint8 b)
{
	jbl_uint8 bw[4];
	jbl_uint8 res=0,aa=a;
	bw[0]=b;
	for(jbl_uint8 i=1;i<4;i++)
	{
		bw[i]=bw[i-1]<<1;
		if(bw[i-1]&0x80)
			bw[i]^=0x1b;
	}
	for(jbl_uint8 i=0;i<4;i++)
		if((a>>i)&0x01)
			res^=bw[i];
	return res;
}
#endif
inline jbl_aes_128_key* jbl_aes_128_key_new()
{
	return jbl_aes_128_key_init(jbl_malloc(sizeof(jbl_aes_128_key)));
}
inline jbl_aes_128_key* jbl_aes_128_key_init(jbl_aes_128_key *this)
{
	jbl_gc_init(this);
	jbl_gc_plus(this);	
	return this;
}
jbl_aes_128_key* jbl_aes_128_key_set(jbl_aes_128_key *this,unsigned char* key)
{
	if(!this)this=jbl_aes_128_key_new();
	jbl_reference *ref=NULL;jbl_aes_128_key *thi=jbl_refer_pull_keep_father(this,&ref);
	jbl_uint8 is_multiple=(jbl_gc_refcnt(thi)!=1);
#if JBL_VAR_ENABLE==1
	jbl_uint8 is_var=jbl_gc_is_var(thi);
	if(jbl_gc_is_pvar(thi))thi=((jbl_reference*)thi)->ptr,is_multiple|=(jbl_gc_refcnt(thi)!=1);
#endif
	if(is_multiple)
	{
		jbl_aes_128_key_free(thi);
#if JBL_VAR_ENABLE==1
		if(is_var)
			jbl_aes_128_key_free(this),thi=jbl_Vaes_128_key(jbl_Vaes_128_key_new());
		else
#endif
			jbl_aes_128_key_free(this),thi=jbl_aes_128_key_new();
	}
	jbl_uint8 rc[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
	for(jbl_uint8 r=0;r<4;r++)
		for(jbl_uint8 c=0;c<4;c++)
			thi->key[0][r][c]=key[r+c*4];
	for(jbl_uint8 i=1;i<=10;i++)
		for(jbl_uint8 j=0;j<4;j++)
		{
			jbl_uint8 t[4];
			for(jbl_uint8 r=0;r<4;r++)
				t[r]=j?thi->key[i][r][j-1]:thi->key[i-1][r][3];
			if(j==0)
			{
				jbl_uint8 temp=t[0];
				for(jbl_uint8 r=0;r<3;r++)
					t[r]=__jbl_aes_sbox[t[(r+1)%4]];
				t[3]=__jbl_aes_sbox[temp];
				t[0]^=rc[i-1];
			}
			for(jbl_uint8 r=0;r<4;r++)
				thi->key[i][r][j]=thi->key[i-1][r][j]^t[r];
		}
	if(ref)ref->ptr=thi;else this=thi;
	return this;
}
inline jbl_aes_128_key *jbl_aes_128_key_copy(jbl_aes_128_key *that)
{
	if(!that)return NULL;
	jbl_gc_plus(that);
	return that;
}
inline jbl_aes_128_key* jbl_aes_128_key_free(jbl_aes_128_key* this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		((jbl_gc_is_ref(this)||jbl_gc_is_pvar(this))?jbl_aes_128_key_free((jbl_aes_128_key*)(((jbl_reference*)this)->ptr)):0);
#if JBL_VAR_ENABLE==1
		if(jbl_gc_is_var(this))
			jbl_free((char*)this-sizeof(jbl_var));
		else
#endif
			jbl_free(this);		
	}
	return NULL;
}
#if JBL_VAR_ENABLE==1
jbl_var_operators_new(jbl_aes_128_key_operators,jbl_aes_128_key_free,jbl_aes_128_key_copy,NULL,NULL,NULL,NULL);
inline jbl_aes_128_key * jbl_Vaes_128_key(jbl_var * this){if(this&&!Vis_jbl_aes_128_key(this))jbl_exception("VAR TYPE ERROR");return((jbl_aes_128_key*)this);}
jbl_var * jbl_Vaes_128_key_new()
{
	jbl_var *this=(jbl_var*)((char*)(jbl_malloc((sizeof(jbl_aes_128_key))+(sizeof(jbl_var)))+(sizeof(jbl_var))));
	jbl_aes_128_key_init((jbl_aes_128_key*)this);
	jbl_gc_set_var((jbl_aes_128_key*)this);
	jbl_var_set_operators(this,&jbl_aes_128_key_operators);
	return this;
}
#endif

/*******************************************************************************************/
/*                            以下函实现aes 128 encode                                    */
/*******************************************************************************************/
inline void __jbl_aes_128_encode_16(__jbl_aes_128_ex_key *w,jbl_uint8* a,jbl_uint8* b)
{
	jbl_uint8 state[4][4],state2[4][4];
	state[0][0]=a[0]^(*w)[0][0][0],state[0][1]=a[4]^(*w)[0][0][1],state[0][2]=a[ 8]^(*w)[0][0][2],state[0][3]=a[12]^(*w)[0][0][3];
	state[1][0]=a[1]^(*w)[0][1][0],state[1][1]=a[5]^(*w)[0][1][1],state[1][2]=a[ 9]^(*w)[0][1][2],state[1][3]=a[13]^(*w)[0][1][3];
	state[2][0]=a[2]^(*w)[0][2][0],state[2][1]=a[6]^(*w)[0][2][1],state[2][2]=a[10]^(*w)[0][2][2],state[2][3]=a[14]^(*w)[0][2][3];
	state[3][0]=a[3]^(*w)[0][3][0],state[3][1]=a[7]^(*w)[0][3][1],state[3][2]=a[11]^(*w)[0][3][2],state[3][3]=a[15]^(*w)[0][3][3];
	for(jbl_uint8 i=1;i<=9;++i)
	{
		state2[0][0]=__jbl_aes_sbox[state[0][0]],state2[0][1]=__jbl_aes_sbox[state[0][1]],state2[0][2]=__jbl_aes_sbox[state[0][2]],state2[0][3]=__jbl_aes_sbox[state[0][3]];
		state2[1][0]=__jbl_aes_sbox[state[1][1]],state2[1][1]=__jbl_aes_sbox[state[1][2]],state2[1][2]=__jbl_aes_sbox[state[1][3]],state2[1][3]=__jbl_aes_sbox[state[1][0]];
		state2[2][0]=__jbl_aes_sbox[state[2][2]],state2[2][1]=__jbl_aes_sbox[state[2][3]],state2[2][2]=__jbl_aes_sbox[state[2][0]],state2[2][3]=__jbl_aes_sbox[state[2][1]];
		state2[3][0]=__jbl_aes_sbox[state[3][3]],state2[3][1]=__jbl_aes_sbox[state[3][0]],state2[3][2]=__jbl_aes_sbox[state[3][1]],state2[3][3]=__jbl_aes_sbox[state[3][2]];
		
		state[0][0]=ffmul(0x02,state2[0][0])^ffmul(0x03,state2[1][0])^(state2[2][0])^(state2[3][0])^(*w)[i][0][0];
		state[1][0]=ffmul(0x02,state2[1][0])^ffmul(0x03,state2[2][0])^(state2[3][0])^(state2[0][0])^(*w)[i][1][0];
		state[2][0]=ffmul(0x02,state2[2][0])^ffmul(0x03,state2[3][0])^(state2[0][0])^(state2[1][0])^(*w)[i][2][0];
		state[3][0]=ffmul(0x02,state2[3][0])^ffmul(0x03,state2[0][0])^(state2[1][0])^(state2[2][0])^(*w)[i][3][0];		
		
		state[0][1]=ffmul(0x02,state2[0][1])^ffmul(0x03,state2[1][1])^(state2[2][1])^(state2[3][1])^(*w)[i][0][1];
		state[1][1]=ffmul(0x02,state2[1][1])^ffmul(0x03,state2[2][1])^(state2[3][1])^(state2[0][1])^(*w)[i][1][1];
		state[2][1]=ffmul(0x02,state2[2][1])^ffmul(0x03,state2[3][1])^(state2[0][1])^(state2[1][1])^(*w)[i][2][1];
		state[3][1]=ffmul(0x02,state2[3][1])^ffmul(0x03,state2[0][1])^(state2[1][1])^(state2[2][1])^(*w)[i][3][1];
		
		state[0][2]=ffmul(0x02,state2[0][2])^ffmul(0x03,state2[1][2])^(state2[2][2])^(state2[3][2])^(*w)[i][0][2];
		state[1][2]=ffmul(0x02,state2[1][2])^ffmul(0x03,state2[2][2])^(state2[3][2])^(state2[0][2])^(*w)[i][1][2];
		state[2][2]=ffmul(0x02,state2[2][2])^ffmul(0x03,state2[3][2])^(state2[0][2])^(state2[1][2])^(*w)[i][2][2];
		state[3][2]=ffmul(0x02,state2[3][2])^ffmul(0x03,state2[0][2])^(state2[1][2])^(state2[2][2])^(*w)[i][3][2];
		
		state[0][3]=ffmul(0x02,state2[0][3])^ffmul(0x03,state2[1][3])^(state2[2][3])^(state2[3][3])^(*w)[i][0][3];
		state[1][3]=ffmul(0x02,state2[1][3])^ffmul(0x03,state2[2][3])^(state2[3][3])^(state2[0][3])^(*w)[i][1][3];
		state[2][3]=ffmul(0x02,state2[2][3])^ffmul(0x03,state2[3][3])^(state2[0][3])^(state2[1][3])^(*w)[i][2][3];
		state[3][3]=ffmul(0x02,state2[3][3])^ffmul(0x03,state2[0][3])^(state2[1][3])^(state2[2][3])^(*w)[i][3][3];
	}
	b[0]=__jbl_aes_sbox[state[0][0]]^(*w)[10][0][0],b[4]=__jbl_aes_sbox[state[0][1]]^(*w)[10][0][1],b[ 8]=__jbl_aes_sbox[state[0][2]]^(*w)[10][0][2],b[12]=__jbl_aes_sbox[state[0][3]]^(*w)[10][0][3];
	b[1]=__jbl_aes_sbox[state[1][1]]^(*w)[10][1][0],b[5]=__jbl_aes_sbox[state[1][2]]^(*w)[10][1][1],b[ 9]=__jbl_aes_sbox[state[1][3]]^(*w)[10][1][2],b[13]=__jbl_aes_sbox[state[1][0]]^(*w)[10][1][3];
	b[2]=__jbl_aes_sbox[state[2][2]]^(*w)[10][2][0],b[6]=__jbl_aes_sbox[state[2][3]]^(*w)[10][2][1],b[10]=__jbl_aes_sbox[state[2][0]]^(*w)[10][2][2],b[14]=__jbl_aes_sbox[state[2][1]]^(*w)[10][2][3];
	b[3]=__jbl_aes_sbox[state[3][3]]^(*w)[10][3][0],b[7]=__jbl_aes_sbox[state[3][0]]^(*w)[10][3][1],b[11]=__jbl_aes_sbox[state[3][1]]^(*w)[10][3][2],b[15]=__jbl_aes_sbox[state[3][2]]^(*w)[10][3][3];
}
/*******************************************************************************************/
/*                            以下函实现aes 128 decode                                    */
/*******************************************************************************************/
inline void __jbl_aes_128_decode_16(__jbl_aes_128_ex_key *w,jbl_uint8* a,jbl_uint8 *b)
{
	jbl_uint8 state[4][4],state2[4][4];
	state[0][0]=a[0]^(*w)[10][0][0],state[0][1]=a[4]^(*w)[10][0][1],state[0][2]=a[ 8]^(*w)[10][0][2],state[0][3]=a[12]^(*w)[10][0][3];
	state[1][0]=a[1]^(*w)[10][1][0],state[1][1]=a[5]^(*w)[10][1][1],state[1][2]=a[ 9]^(*w)[10][1][2],state[1][3]=a[13]^(*w)[10][1][3];
	state[2][0]=a[2]^(*w)[10][2][0],state[2][1]=a[6]^(*w)[10][2][1],state[2][2]=a[10]^(*w)[10][2][2],state[2][3]=a[14]^(*w)[10][2][3];
	state[3][0]=a[3]^(*w)[10][3][0],state[3][1]=a[7]^(*w)[10][3][1],state[3][2]=a[11]^(*w)[10][3][2],state[3][3]=a[15]^(*w)[10][3][3];
	for(jbl_uint8 i=9;i>0;--i)
	{
		state2[0][0]=__jbl_aes_invsbox[state[0][0]]^(*w)[i][0][0],state2[0][1]=__jbl_aes_invsbox[state[0][1]]^(*w)[i][0][1],state2[0][2]=__jbl_aes_invsbox[state[0][2]]^(*w)[i][0][2],state2[0][3]=__jbl_aes_invsbox[state[0][3]]^(*w)[i][0][3];
		state2[1][0]=__jbl_aes_invsbox[state[1][3]]^(*w)[i][1][0],state2[1][1]=__jbl_aes_invsbox[state[1][0]]^(*w)[i][1][1],state2[1][2]=__jbl_aes_invsbox[state[1][1]]^(*w)[i][1][2],state2[1][3]=__jbl_aes_invsbox[state[1][2]]^(*w)[i][1][3];
		state2[2][0]=__jbl_aes_invsbox[state[2][2]]^(*w)[i][2][0],state2[2][1]=__jbl_aes_invsbox[state[2][3]]^(*w)[i][2][1],state2[2][2]=__jbl_aes_invsbox[state[2][0]]^(*w)[i][2][2],state2[2][3]=__jbl_aes_invsbox[state[2][1]]^(*w)[i][2][3];
		state2[3][0]=__jbl_aes_invsbox[state[3][1]]^(*w)[i][3][0],state2[3][1]=__jbl_aes_invsbox[state[3][2]]^(*w)[i][3][1],state2[3][2]=__jbl_aes_invsbox[state[3][3]]^(*w)[i][3][2],state2[3][3]=__jbl_aes_invsbox[state[3][0]]^(*w)[i][3][3];
		
		state[0][0]=ffmul(0x0e,state2[0][0])^ffmul(0x0b,state2[1][0])^ffmul(0x0d,state2[2][0])^ffmul(0x09,state2[3][0]);
		state[1][0]=ffmul(0x0e,state2[1][0])^ffmul(0x0b,state2[2][0])^ffmul(0x0d,state2[3][0])^ffmul(0x09,state2[0][0]);
		state[2][0]=ffmul(0x0e,state2[2][0])^ffmul(0x0b,state2[3][0])^ffmul(0x0d,state2[0][0])^ffmul(0x09,state2[1][0]);
		state[3][0]=ffmul(0x0e,state2[3][0])^ffmul(0x0b,state2[0][0])^ffmul(0x0d,state2[1][0])^ffmul(0x09,state2[2][0]);

		state[0][1]=ffmul(0x0e,state2[0][1])^ffmul(0x0b,state2[1][1])^ffmul(0x0d,state2[2][1])^ffmul(0x09,state2[3][1]);
		state[1][1]=ffmul(0x0e,state2[1][1])^ffmul(0x0b,state2[2][1])^ffmul(0x0d,state2[3][1])^ffmul(0x09,state2[0][1]);
		state[2][1]=ffmul(0x0e,state2[2][1])^ffmul(0x0b,state2[3][1])^ffmul(0x0d,state2[0][1])^ffmul(0x09,state2[1][1]);
		state[3][1]=ffmul(0x0e,state2[3][1])^ffmul(0x0b,state2[0][1])^ffmul(0x0d,state2[1][1])^ffmul(0x09,state2[2][1]);

		state[0][2]=ffmul(0x0e,state2[0][2])^ffmul(0x0b,state2[1][2])^ffmul(0x0d,state2[2][2])^ffmul(0x09,state2[3][2]);
		state[1][2]=ffmul(0x0e,state2[1][2])^ffmul(0x0b,state2[2][2])^ffmul(0x0d,state2[3][2])^ffmul(0x09,state2[0][2]);
		state[2][2]=ffmul(0x0e,state2[2][2])^ffmul(0x0b,state2[3][2])^ffmul(0x0d,state2[0][2])^ffmul(0x09,state2[1][2]);
		state[3][2]=ffmul(0x0e,state2[3][2])^ffmul(0x0b,state2[0][2])^ffmul(0x0d,state2[1][2])^ffmul(0x09,state2[2][2]);

		state[0][3]=ffmul(0x0e,state2[0][3])^ffmul(0x0b,state2[1][3])^ffmul(0x0d,state2[2][3])^ffmul(0x09,state2[3][3]);
		state[1][3]=ffmul(0x0e,state2[1][3])^ffmul(0x0b,state2[2][3])^ffmul(0x0d,state2[3][3])^ffmul(0x09,state2[0][3]);
		state[2][3]=ffmul(0x0e,state2[2][3])^ffmul(0x0b,state2[3][3])^ffmul(0x0d,state2[0][3])^ffmul(0x09,state2[1][3]);
		state[3][3]=ffmul(0x0e,state2[3][3])^ffmul(0x0b,state2[0][3])^ffmul(0x0d,state2[1][3])^ffmul(0x09,state2[2][3]);	
	}
	b[0]=__jbl_aes_invsbox[state[0][0]]^(*w)[0][0][0],b[4]=__jbl_aes_invsbox[state[0][1]]^(*w)[0][0][1],b[ 8]=__jbl_aes_invsbox[state[0][2]]^(*w)[0][0][2],b[12]=__jbl_aes_invsbox[state[0][3]]^(*w)[0][0][3];
	b[1]=__jbl_aes_invsbox[state[1][3]]^(*w)[0][1][0],b[5]=__jbl_aes_invsbox[state[1][0]]^(*w)[0][1][1],b[ 9]=__jbl_aes_invsbox[state[1][1]]^(*w)[0][1][2],b[13]=__jbl_aes_invsbox[state[1][2]]^(*w)[0][1][3];
	b[2]=__jbl_aes_invsbox[state[2][2]]^(*w)[0][2][0],b[6]=__jbl_aes_invsbox[state[2][3]]^(*w)[0][2][1],b[10]=__jbl_aes_invsbox[state[2][0]]^(*w)[0][2][2],b[14]=__jbl_aes_invsbox[state[2][1]]^(*w)[0][2][3];
	b[3]=__jbl_aes_invsbox[state[3][1]]^(*w)[0][3][0],b[7]=__jbl_aes_invsbox[state[3][2]]^(*w)[0][3][1],b[11]=__jbl_aes_invsbox[state[3][3]]^(*w)[0][3][2],b[15]=__jbl_aes_invsbox[state[3][0]]^(*w)[0][3][3];
}
#if JBL_AES_128_ECB_ENABLE==1
#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的aes128ecb加解密操作                   */
/*******************************************************************************************/
jbl_string * jbl_aes_128_ecb_encode(jbl_aes_128_key *w,jbl_string *this,jbl_string *result)
{
	if(!w)jbl_exception("NULL POINTER");
	if(!this)return result;
	jbl_string *thi=jbl_refer_pull(this);														//this脱离引用
	__jbl_aes_128_ex_key *key=&(((jbl_aes_128_key*)jbl_refer_pull(w))->key);
	jbl_string_size_type i=0,len=jbl_string_get_length_force(thi);
	jbl_string *res;result=jbl_string_extend_to(result,len+16,1,&res);jbl_string_hash_clear(res);
	jbl_uint8 tmp[16],*sthi=jbl_string_get_chars_force(thi),*sres=jbl_string_get_chars_force(res);
	for(jbl_string_size_type n=(len>>4)<<4;i<n;i+=16)
		__jbl_aes_128_encode_16(key,sthi+i,sres+jbl_string_get_length_force(res)),jbl_string_set_length_force(res,jbl_string_get_length_force(res)+16);
	for(jbl_uint8 j=0,x=((len-i)==0?16:(16-(len&15)));j<16;tmp[j]=x,++j);
	for(jbl_uint8 j=0;i<len;tmp[j]=jbl_string_get_force(thi,i),++i,++j);
	__jbl_aes_128_encode_16(key,tmp,sres+jbl_string_get_length_force(res)),jbl_string_set_length_force(res,jbl_string_get_length_force(res)+16);
	return result;
}
jbl_string * jbl_aes_128_ecb_decode(jbl_aes_128_key *w,jbl_string *this,jbl_string *result)
{
	if(!w)jbl_exception("NULL POINTER");
	if(!this)return result;
	jbl_string *thi=jbl_refer_pull(this);		
	__jbl_aes_128_ex_key *key=&(((jbl_aes_128_key*)jbl_refer_pull(w))->key);	
	jbl_string_size_type len=jbl_string_get_length_force(thi);
	jbl_string *res;result=jbl_string_extend_to(result,len,1,&res);jbl_string_hash_clear(res);
	jbl_uint8 *sthi=jbl_string_get_chars_force(thi),*sres=jbl_string_get_chars_force(res);
	for(jbl_string_size_type i=0;i<len;i+=16)
		__jbl_aes_128_decode_16(key,sthi+i,sres+jbl_string_get_length_force(res)),jbl_string_set_length_force(res,jbl_string_get_length_force(res)+16);
	jbl_string_set_length_force(res,jbl_string_get_length_force(res)-sres[jbl_string_get_length_force(res)-1]);
	return result;
}
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现stream的aes128ecb加解密操作                    */
/*******************************************************************************************/
void __jbl_aes_128_ecb_seo(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream *nxt=jbl_refer_pull(this->nxt);			
	__jbl_aes_128_ex_key *key=&(((jbl_aes_128_key*)jbl_refer_pull(this->data))->key);
	if(nxt!=NULL)
	{
		jbl_uint16 i=0,len=(this->en>>4)<<4;
		if(len!=0)
			while(i<len)
			{
				if((nxt->en+16)>nxt->size)	
					jbl_stream_do(nxt,0);
				__jbl_aes_128_encode_16(key,this->buf+i,nxt->buf+nxt->en),nxt->en+=16,i+=16;
			}
		if((flags&jbl_stream_force))
		{
			if((nxt->en+16)>nxt->size)
				jbl_stream_do(nxt,0);
			jbl_uint8 s[16];
			for(jbl_uint8 j=0,x=(this->en==0?16:(16-(this->en&15)));j<16;s[j]=x,++j);
			for(jbl_uint8 j=0;i<this->en;s[j]=this->buf[i],++i,++j);
			__jbl_aes_128_encode_16(key,s,nxt->buf+nxt->en),nxt->en+=16,i=this->en;
			jbl_stream_do(nxt,flags);
		}
		jbl_memory_copy(this->buf,this->buf+i,this->en-i);
		this->en=this->en-i;
	}
}
void __jbl_aes_128_ecb_sdo(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");
	this=jbl_refer_pull(this);
	jbl_stream *nxt=jbl_refer_pull(this->nxt);			
	__jbl_aes_128_ex_key *key=&(((jbl_aes_128_key*)jbl_refer_pull(this->data))->key);
	if(nxt!=NULL)
	{
		jbl_uint16 i=0,len=(this->en>>4)<<4;
		if(len!=0)
			while(i<len)
			{
				if(nxt->en+16>nxt->size)	
					jbl_stream_do(nxt,0);
				__jbl_aes_128_decode_16(key,this->buf+i,nxt->buf+nxt->en),nxt->en+=16,i+=16;
			}
		if((flags&jbl_stream_force))
			nxt->en-=nxt->buf[nxt->en-1],jbl_stream_do(nxt,flags);
		jbl_memory_copy(this->buf,this->buf+i,this->en-i);
		this->en=this->en-i;
	}
}
jbl_stream_operators_new(jbl_stream_aes_128_ecb_encode_operators,__jbl_aes_128_ecb_seo,jbl_aes_128_key_free,NULL);
jbl_stream_operators_new(jbl_stream_aes_128_ecb_decode_operators,__jbl_aes_128_ecb_sdo,jbl_aes_128_key_free,NULL);
#endif
#endif
#if JBL_AES_128_CBC_ENABLE==1
#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的aes128cbc加解密操作                   */
/*******************************************************************************************/
jbl_string * jbl_aes_128_cbc_encode(jbl_aes_128_key *w,jbl_uint8 * vi,jbl_string *this,jbl_string *result)
{
	if(!w||!vi)jbl_exception("NULL POINTER");
	if(!this)return result;
	jbl_string *thi=jbl_refer_pull(this);		
	__jbl_aes_128_ex_key *key=&(((jbl_aes_128_key*)jbl_refer_pull(w))->key);	
	jbl_string_size_type len=jbl_string_get_length_force(thi);
	jbl_string *res;result=jbl_string_extend_to(result,len+16,1,&res);jbl_string_hash_clear(res);
	jbl_uint8 s[16],*vii=vi,*sres=jbl_string_get_chars_force(res);
	for(jbl_string_size_type i=0;i<len;i+=16)
	{
		for(jbl_uint8 j=0;j<16;j++)
			s[j]=((i+j>=len)?(16-(len&15)):(jbl_string_get_force(thi,i+j)))^vii[j];
		vii=sres+jbl_string_get_length_force(res);
		__jbl_aes_128_encode_16(key,s,sres+jbl_string_get_length_force(res)),jbl_string_set_length_force(res,jbl_string_get_length_force(res)+16);
	}
	if(len%16==0)
	{
		for(jbl_uint8 j=0;j<16;s[j]=16^vii[j],j++);
		__jbl_aes_128_encode_16(key,s,sres+jbl_string_get_length_force(res)),jbl_string_set_length_force(res,jbl_string_get_length_force(res)+16);
	}
	jbl_string_hash_clear(result);
	return result;	
}
jbl_string * jbl_aes_128_cbc_decode(jbl_aes_128_key *w,jbl_uint8 * vi,jbl_string *this,jbl_string *result)
{
	if(!w||!vi)jbl_exception("NULL POINTER");
	if(!this)return result;
	jbl_string *thi=jbl_refer_pull(this);		
	__jbl_aes_128_ex_key *key=&(((jbl_aes_128_key*)jbl_refer_pull(w))->key);		
	jbl_string_size_type len=jbl_string_get_length_force(thi);
	jbl_string *res;result=jbl_string_extend_to(result,len+16,1,&res);jbl_string_hash_clear(res);
	jbl_uint8 *vii=vi,*sthi=jbl_string_get_chars_force(thi),*sres=jbl_string_get_chars_force(res);
	for(jbl_string_size_type i=0;i<len;i+=16)
	{
		__jbl_aes_128_decode_16(key,sthi+i,sres+jbl_string_get_length_force(res));
		for(jbl_uint8 j=0;j<16;sres[j+jbl_string_get_length_force(res)]^=vii[j],++j);
		vii=sthi+i;
		jbl_string_set_length_force(res,jbl_string_get_length_force(res)+16);
	}
	jbl_string_set_length_force(res,jbl_string_get_length_force(res)-sres[jbl_string_get_length_force(res)-1]);
	jbl_string_hash_clear(result);
	return result;	
}
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现stream的aes128cbc缓冲更新操作                  */
/*******************************************************************************************/
void jbl_aes_128_cbc_update_stream_buf(jbl_stream *this)
{
	if(this->size==(JBL_STREAM_EXCEED_LENGTH+32))this->size-=16;//占用最后16字节保存vi，所以要减掉
}
/*******************************************************************************************/
/*                            以下函数实现stream的aes128cbc加解密操作                    */
/*******************************************************************************************/
void __jbl_aes_128_cbc_seo(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");
	this=jbl_refer_pull(this);
	jbl_stream *nxt=jbl_refer_pull(this->nxt);			
	jbl_uint8 *vii=(jbl_uint8*)this->tmp[0].p;
	__jbl_aes_128_ex_key *key=&(((jbl_aes_128_key*)jbl_refer_pull(this->data))->key);
	if(nxt!=NULL)
	{
		jbl_uint16 i=0,len=(this->en>>4)<<4;
		if(len!=0)
			while(i<len)
			{
				for(jbl_uint8 j=0;j<16;this->buf[i+j]^=vii[j],++j);
				__jbl_aes_128_encode_16(key,this->buf+i,nxt->buf+nxt->en),nxt->en+=16,i+=16;
				if((nxt->en+16)>nxt->size)
					jbl_memory_copy(this->buf+this->size-16,nxt->buf+nxt->en-16,16),vii=this->buf+this->size,jbl_stream_do(nxt,0);//备份vi并下推流
				else
					vii=nxt->buf+nxt->en-16;
			}
		if((flags&jbl_stream_force))
		{
			if((nxt->en+16)>nxt->size)
				jbl_memory_copy(this->buf+this->size,nxt->buf+nxt->en-16,16),vii=this->buf+this->size,jbl_stream_do(nxt,0);//备份vi并下推流
			jbl_uint8 s[16];
			for(jbl_uint8 j=0,x=(this->en==0?16:(16-(this->en&15)));j<16;s[j]=x,++j);
			for(jbl_uint8 j=0;i<this->en;s[j]=this->buf[i],++i,++j);
			for(jbl_uint8 j=0;j<16;s[j]^=vii[j],++j);
			__jbl_aes_128_encode_16(key,s,nxt->buf+nxt->en),nxt->en+=16,i=this->en;
			jbl_stream_do(nxt,flags);
		}
		this->tmp[0].p=vii;
		jbl_memory_copy(this->buf,this->buf+i,this->en-i);
		this->en=this->en-i;
	}
}
void __jbl_aes_128_cbc_sdo(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream *nxt=jbl_refer_pull(this->nxt);			
	jbl_uint8 *vii=(jbl_uint8*)this->tmp[0].p;
	__jbl_aes_128_ex_key *key=&(((jbl_aes_128_key*)jbl_refer_pull(this->data))->key);
	if(nxt!=NULL)
	{
		jbl_uint16 i=0,len=(this->en>>4)<<4;
		if(len!=0)
		{
			while(i<len)
			{
				if(nxt->en+16>nxt->size)jbl_stream_do(nxt,0);
				__jbl_aes_128_decode_16(key,this->buf+i,nxt->buf+nxt->en);
				for(jbl_uint8 j=0;j<16;nxt->buf[nxt->en+j]^=vii[j],++j);
				vii=this->buf+i,nxt->en+=16,i+=16;
			}
		}
		if((flags&jbl_stream_force))
			nxt->en-=nxt->buf[nxt->en-1],jbl_stream_do(nxt,flags);//处理末尾长度，下推流
		jbl_memory_copy(this->buf+this->size,this->buf+i-16,16);this->tmp[0].p=this->buf+this->size;//把最后成功解码的16字节拷到最后面做下一次的vi
		jbl_memory_copy(this->buf,this->buf+i,this->en-i);
		this->en=this->en-i;
	}
}
jbl_stream_operators_new(jbl_stream_aes_128_cbc_encode_operators,__jbl_aes_128_cbc_seo,jbl_aes_128_key_free,jbl_aes_128_cbc_update_stream_buf);
jbl_stream_operators_new(jbl_stream_aes_128_cbc_decode_operators,__jbl_aes_128_cbc_sdo,jbl_aes_128_key_free,jbl_aes_128_cbc_update_stream_buf);
inline jbl_stream * jbl_stream_aes_128_cbc_encode_new(jbl_aes_128_key *w,const unsigned char * v)
{
	jbl_stream *this=jbl_stream_new(&jbl_stream_aes_128_cbc_encode_operators,jbl_aes_128_key_copy(w),JBL_STREAM_EXCEED_LENGTH+32,NULL,1);
	this->tmp[0].p=v;
	return this;
}
inline jbl_stream * jbl_stream_aes_128_cbc_decode_new(jbl_aes_128_key *w,const unsigned char * v)
{
	jbl_stream *this=jbl_stream_new(&jbl_stream_aes_128_cbc_decode_operators,jbl_aes_128_key_copy(w),JBL_STREAM_EXCEED_LENGTH+32,NULL,1);
	this->tmp[0].p=v;
	return this;
}
#if JBL_VAR_ENABLE == 1
inline jbl_var * jbl_Vstream_aes_128_cbc_encode_new(jbl_aes_128_key *w,const unsigned char * v)
{
	jbl_var *this=jbl_Vstream_new(&jbl_stream_aes_128_cbc_encode_operators,jbl_aes_128_key_copy(w),JBL_STREAM_EXCEED_LENGTH+32,NULL,1);
	((jbl_stream*)this)->tmp[0].p=v;
	return this;
}
inline jbl_var * jbl_Vstream_aes_128_cbc_decode_new(jbl_aes_128_key *w,const unsigned char * v)
{
	jbl_var *this=jbl_Vstream_new(&jbl_stream_aes_128_cbc_decode_operators,jbl_aes_128_key_copy(w),JBL_STREAM_EXCEED_LENGTH+32,NULL,1);
	((jbl_stream*)this)->tmp[0].p=v;
	return this;
}
#endif
#endif
#endif
#undef ffmul
#endif