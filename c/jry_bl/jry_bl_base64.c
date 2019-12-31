/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_bl_base64.h"
#if JRY_BL_BASE64_ENABLE==1
void jry_bl_base64_encode(const jry_bl_string *this,jry_bl_string *result)
{
	if(this==NULL||result==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	register jry_bl_string_size_type i=0,len=jry_bl_string_get_length(this);
	jry_bl_string_extend(result,len/3*4+4);
	const char table[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
	while(len>2)
		jry_bl_string_add_char1(result,table[(unsigned char)jry_bl_string_get1(this,i)>>2]),jry_bl_string_add_char1(result,table[(((unsigned char)jry_bl_string_get1(this,i)&0x03)<<4)+((unsigned char)jry_bl_string_get1(this,i+1)>>4)]),jry_bl_string_add_char1(result,table[(((unsigned char)jry_bl_string_get1(this,i+1)&0x0f)<<2)+((unsigned char)jry_bl_string_get1(this,i+2)>>6)]),jry_bl_string_add_char1(result,table[(unsigned char)jry_bl_string_get1(this,i+2)&0x3f]),i+=3,len-=3;
	if(len>0)
	{
		jry_bl_string_add_char1(result,table[(unsigned char)jry_bl_string_get1(this,i)>>2]);
		if(len%3==1)
			jry_bl_string_add_char1(result,table[((unsigned char)jry_bl_string_get1(this,i)&0x03)<<4]),jry_bl_string_add_char1(result,'='),jry_bl_string_add_char1(result,'=');
		else if(len%3==2)
			jry_bl_string_add_char1(result,table[(((unsigned char)jry_bl_string_get1(this,i)&0x03)<<4)+((unsigned char)jry_bl_string_get1(this,i+1)>>4)]),jry_bl_string_add_char1(result,table[((unsigned char)jry_bl_string_get1(this,i+1)&0x0f)<<2]),jry_bl_string_add_char1(result,'=');
	}
}

void jry_bl_base64_decode(const jry_bl_string *this,jry_bl_string *result)
{
	if(this==NULL||result==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	const char table[]={-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-2,-2,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,62,-2,-2,-2,63,52,53,54,55,56,57,58,59,60,61,-2,-2,-2,-2,-2,-2,-2,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-2,-2,-2,-2,-2,-2,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2};
	register jry_bl_string_size_type i=0,len=jry_bl_string_get_length(this);
	register char bin=0,ch;
	jry_bl_string_extend(result,len/4*3+3);
	while(len-->0)
	{
		ch=jry_bl_string_get1(this,i);
		if(ch=='=')
		{
			if(jry_bl_string_get1(this,i+1)!='='&&(i%4)==1)
				return jry_bl_string_clear(result);
			continue;
		}
		ch=table[ch];
		if(ch<0)
			continue;
		switch(i%4)
		{
			case 0:
				bin=ch<<2;
				break;
			case 1:
				bin|=ch>>4,jry_bl_string_add_char1(result,bin),bin=(ch&0x0f)<<4;
				break;
			case 2:
				bin|=ch>>2,jry_bl_string_add_char1(result,bin),bin=(ch&0x03)<<6;
				break;
			case 3:
				bin|=ch,jry_bl_string_add_char1(result,bin);
				break;
		}
		i++;
	}	
}
#endif
