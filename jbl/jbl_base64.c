/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_base64.h"
#if JBL_BASE64_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_ying jbl_exception                                   */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_ying.h"
/*******************************************************************************************/
/*                            联动jbl_stream jbl_string                                    */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_stream.h"

/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
static const char __jbl_base64_encode_table[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
static const char __jbl_base64_decode_table[]={-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-2,-2,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,62,-2,-2,-2,63,52,53,54,55,56,57,58,59,60,61,-2,-2,-2,-2,-2,-2,-2,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-2,-2,-2,-2,-2,-2,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2};
#define det __jbl_base64_decode_table
#define ent __jbl_base64_encode_table
#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的base64编解码操作                      */
/*******************************************************************************************/
jbl_string * jbl_base64_encode(jbl_string *this,jbl_string *result)
{
	if(this==NULL)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);		
	jbl_string_size_type i=0,len=jbl_string_get_length_force(thi);
	jbl_string *res;result=jbl_string_extend_to(result,len*4/3+4,1,&res);jbl_string_hash_clear(res);
	while(len>2)
		jbl_string_add_char_force(result,ent[(unsigned char)jbl_string_get_force(thi,i)>>2]),jbl_string_add_char_force(result,ent[(((unsigned char)jbl_string_get_force(thi,i)&0x03)<<4)+((unsigned char)jbl_string_get_force(thi,i+1)>>4)]),jbl_string_add_char_force(result,ent[(((unsigned char)jbl_string_get_force(thi,i+1)&0x0f)<<2)+((unsigned char)jbl_string_get_force(thi,i+2)>>6)]),jbl_string_add_char_force(result,ent[(unsigned char)jbl_string_get_force(thi,i+2)&0x3f]),i+=3,len-=3;
	if(len>0)
	{
		jbl_string_add_char_force(res,ent[(unsigned char)jbl_string_get_force(thi,i)>>2]);
		if(len%3==1)
			jbl_string_add_char_force(res,ent[((unsigned char)jbl_string_get_force(thi,i)&0x03)<<4]),jbl_string_add_char_force(res,'='),jbl_string_add_char_force(res,'=');
		else if(len%3==2)
			jbl_string_add_char_force(res,ent[(((unsigned char)jbl_string_get_force(thi,i)&0x03)<<4)+((unsigned char)jbl_string_get_force(thi,i+1)>>4)]),jbl_string_add_char_force(res,ent[((unsigned char)jbl_string_get_force(thi,i+1)&0x0f)<<2]),jbl_string_add_char_force(res,'=');
	}
	return result;
}
jbl_string * jbl_base64_decode(jbl_string *this,jbl_string *result)
{
	if(this==NULL)jbl_exception("NULL POINTER");	
	jbl_string *thi=jbl_refer_pull(this);		
	jbl_string_size_type i=0,len=jbl_string_get_length_force(thi);
	jbl_uint8 bin=0,ch;
	jbl_string *res;result=jbl_string_extend_to(result,len*3/4+3,1,&res);jbl_string_hash_clear(res);
	while(len-->0)
	{
		ch=jbl_string_get_force(thi,i);
		if(ch=='=')
		{
			if(jbl_string_get_force(thi,i+1)!='='&&(i%4)==1)
				return jbl_string_clear(res);
			continue;
		}
		ch=det[ch];
		if(ch<0)
			continue;
		switch(i&3)
		{
			case 0:bin=ch<<2;break;
			case 1:bin|=ch>>4,jbl_string_add_char_force(res,bin),bin=(ch&0x0f)<<4;break;
			case 2:bin|=ch>>2,jbl_string_add_char_force(res,bin),bin=(ch&0x03)<<6;break;
			case 3:bin|=ch,jbl_string_add_char_force(res,bin);break;
		}
		++i;
	}	
	return result;
}
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现stream的编解码操作                       */
/*******************************************************************************************/
void __jbl_base64_seo(jbl_stream* this,jbl_uint8 flags)
{
	if(this==NULL)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream *nxt=(this->nxt!=NULL?jbl_refer_pull(this->nxt):NULL);	
	jbl_uint8 *s=this->buf;
	if(nxt&&(!this->stop))
	{
		jbl_stream_buf_size_type i=0,len=this->en;
		while(len>2)
		{
			if((nxt->en+4)>=nxt->size){jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))goto backup;}
			nxt->buf[nxt->en++]=(ent[s[i]>>2]),nxt->buf[nxt->en++]=(ent[((s[i]&0x03)<<4)+(s[i+1]>>4)]),nxt->buf[nxt->en++]=(ent[((s[i+1]&0x0f)<<2)+(s[i+2]>>6)]),nxt->buf[nxt->en++]=(ent[s[i+2]&0x3f]),i+=3,len-=3;
		}
		if((flags&jbl_stream_force))
		{
			if(len>0)
			{
			if((nxt->en+4)>nxt->size){jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))goto backup;}
				nxt->buf[nxt->en++]=(ent[s[i]>>2]);
				if(len%3==1)
					nxt->buf[nxt->en++]=(ent[(s[i]&0x03)<<4]),nxt->buf[nxt->en++]=('='),nxt->buf[nxt->en++]=('='),len-=1,i+=1;
				else if(len%3==2)
					nxt->buf[nxt->en++]=(ent[((s[i]&0x03)<<4)+(s[i+1]>>4)]),nxt->buf[nxt->en++]=(ent[(s[i+1]&0x0f)<<2]),nxt->buf[nxt->en++]=('='),len-=2,i+=2;
			}
			jbl_stream_do(nxt,flags);
			this->stop=1;
			goto backup;
		}
backup:;
		jbl_memory_copy(this->buf,this->buf+i,this->en=len);
	}
}
void __jbl_base64_sdo(jbl_stream* this,jbl_uint8 flags)
{
	if(this==NULL)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream *nxt=(this->nxt!=NULL?jbl_refer_pull(this->nxt):NULL);		
	if(nxt&&(!this->stop))
	{
		jbl_stream_buf_size_type i=0,len=this->en;
		jbl_uint8 ch,bin=0;
		while(i<len)
		{
			if((nxt->en+3)>=nxt->size){jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))goto backup;}
			ch=this->buf[i];
			if(ch=='=')
			{
				if(this->buf[i+1]!='='&&(i%4)==1)
				{
					if(i<len&&(flags&jbl_stream_force))
						return jbl_exception("STREAM ERROR");
					goto backup;
				}
				++i;
				continue;
			}
			ch=det[ch];
			if(ch<0)
				continue;
			switch(i&3)
			{
				case 0:bin=ch<<2;break;
				case 1:bin|=ch>>4,nxt->buf[nxt->en++]=(bin),bin=(ch&0x0f)<<4;break;
				case 2:bin|=ch>>2,nxt->buf[nxt->en++]=(bin),bin=(ch&0x03)<<6;break;
				case 3:bin|=ch,nxt->buf[nxt->en++]=(bin);break;
			}
			++i;
		}
		if((flags&jbl_stream_force))
			jbl_stream_do(nxt,flags),this->stop=1;
backup:;
		jbl_memory_copy(this->buf,this->buf+i,this->en=len-i);
	}
}
jbl_stream_operators_new(jbl_stream_base64_encode_operators,__jbl_base64_seo,NULL,NULL);
jbl_stream_operators_new(jbl_stream_base64_decode_operators,__jbl_base64_sdo,NULL,NULL);
#endif
#endif
