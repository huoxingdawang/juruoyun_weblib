/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jwl_websocket.h"
#if JWL_WEBSOCKET_ENABLE==1
#include "jwl_ying.h"
#include "jwl_http.h"
jbl_string * jwl_websocket_get_sec(jwl_http_head* head,jbl_string *res)
{
	if(!(jwl_http_head_get_connection(head)&JWL_HTTP_CONNECTION_UPGRADE)||jwl_http_head_get_upgrade(head)!=JWL_HTTP_UPGRADE_WEBSOCKET)return NULL;
	void * sec=jwl_http_head_get(head,UC"Sec-WebSocket-Key");
	if(!sec)return NULL;
	sec=jbl_string_add_chars(sec,UC"258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	jbl_string *tmp=jbl_sha1(sec,NULL,true);
	res=jbl_base64_encode(tmp,res);
	sec=jbl_var_free(sec);
	tmp=jbl_string_free(tmp);
	return res;
}
jwl_http_head * jwl_websocket_set_response_head(jwl_http_head* response,jwl_http_head* request)
{
	jwl_http_head_set_status(response,101);
	jwl_http_head_set_connection(response,JWL_HTTP_CONNECTION_UPGRADE);
	jwl_http_head_set_upgrade(response,JWL_HTTP_UPGRADE_WEBSOCKET);
	jwl_http_head_set(response,UC"Sec-WebSocket-Accept",jbl_gc_minus(jwl_websocket_get_sec(request,NULL)));
	return response;
}
jbl_string * jwl_websocket_get_head(jbl_uint64 len,jbl_uint8 is_last,jbl_uint8 need_mask,jwl_websocket_status opcode,jbl_string* head)
{
	head=jbl_string_add_char(head,((is_last?0X80:0X00)|(opcode&0X0F)));
	jbl_uint8 tmp[8];
	if(len<=125)
		head=jbl_string_add_char(head,len|((need_mask&1)<<7));
	else if(len>125&&len<65536)
		head=jbl_string_add_char(head,126|((need_mask&1)<<7)),
		jbl_endian_to_big_uint16(&len,tmp),
		head=jbl_string_add_chars_length(head,tmp,2);
	else
		head=jbl_string_add_char(head,127|((need_mask&1)<<7)),
		jbl_endian_to_big_uint64(&len,tmp),
		head=jbl_string_add_chars_length(head,tmp,8);
	return head;
}
static jbl_uint8 __jwl_websocket_get_data_start(jbl_uint8 *s);
static inline jbl_uint8 __jwl_websocket_get_data_start(jbl_uint8 *s)
{
	if(s[1]&B1000_0000)
	{
		if((s[1]&127)==126)
			return 8;
		else if((s[1]&127)==127)
			return 14;
		return 6;
	}
	else
	{
		if((s[1]&127)==126)
			return 4;
		else if((s[1]&127)==127)
			return 10;
		return 2;		
	}
}
static void __jwl_websocket_get_mask(jbl_uint8 *s,jbl_uint8 * mask)
{
	s+=__jwl_websocket_get_data_start(s)-4;
	mask[0]=s[0];
	mask[1]=s[1];
	mask[2]=s[2];
	mask[3]=s[3];
}
static jbl_uint64 __jwl_websocket_get_length(jbl_uint8 * s)
{
	jbl_uint8 len1=(s[1]&0X7F);
	if(len1==126)
	{
		jbl_uint16 len=0;
		jbl_endian_from_big_uint16(s+2,&len);
		return len;
	}
	else if(len1==127)
	{
		jbl_uint64 len=0;
		jbl_endian_from_big_uint64(s+2,&len);
		return len;
	}
	return len1;
}
#if JBL_STREAM_ENABLE==1
//this->extra[0].c8[0~3]mask
//this->extra[0].c8[4]是否处理头
//this->extra[0].c8[5]下一包是不是extra
//this->extra[0].c8[6]要不要加mask
static void __jwl_websocket_encode_operator(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream *nxt=jbl_refer_pull(this->nxt);
	if(nxt&&(!this->stop)&&this->en)
	{
		jbl_stream_buf_size_type i=0,end=this->en-((flags&jbl_stream_force)?0:16);
		if(!end)goto backup;
		if(!this->extra[0].c8[4])
		{
			jbl_string *head=jwl_websocket_get_head(end,flags&jbl_stream_force,this->extra[0].c8[6],this->extra[0].c8[5],NULL);
			jbl_stream_push_string(nxt,head);
			jbl_string_free(head);
			if(1==(this->stop=nxt->stop))goto backup;
			this->extra[0].c8[4]=1;
			if(this->extra[0].c8[6])
			{
				if(this->extra[0].c8[5]!=JWL_WEBSOCKET_STATUS_EXTRA)
				{
					this->extra[0].c8[0]='a';
					this->extra[0].c8[1]='b';
					this->extra[0].c8[2]='c';
					this->extra[0].c8[3]='d';
				}
				jbl_stream_push_char(nxt,this->extra[0].c8[0]);
				jbl_stream_push_char(nxt,this->extra[0].c8[1]);
				jbl_stream_push_char(nxt,this->extra[0].c8[2]);
				jbl_stream_push_char(nxt,this->extra[0].c8[3]);
			}
			this->extra[0].c8[5]=JWL_WEBSOCKET_STATUS_EXTRA;
		}
		if(this->extra[0].c8[6])
		{
			for(;i<end;++i)
			{
				if(nxt->en>=nxt->size){jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))goto backup;}
				nxt->buf[nxt->en]=this->buf[i]^this->extra[0].c8[i%4];
				++nxt->en;
			}				
		}
		else
		{
			for(;i<end;)
			{
				jbl_stream_buf_size_type len=jbl_min(end-i,nxt->size-nxt->en);	
				jbl_memory_copy(nxt->buf+nxt->en,this->buf+i,len);
				i+=len;
				nxt->en+=len;
				jbl_stream_do(nxt,0);
				if(1==(this->stop=nxt->stop))goto backup;
			}
		}
		this->extra[0].c8[4]=0;
backup:;
		jbl_memory_copy(this->buf,this->buf+i,this->en-=i);
	}
	if(flags&jbl_stream_force)
		this->stop=1,jbl_stream_do(nxt,0);
}
static void __jwl_websocket_decode_operator(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream *nxt=jbl_refer_pull(this->nxt);
	if(nxt&&(!this->stop)&&this->en)
	{
start:;
		jbl_stream_buf_size_type i=0;
		if(this->extra[2].c8[4]==0)//第一帧
		{
			if((i=__jwl_websocket_get_data_start(this->buf))>this->en){i=0;goto backup;}
			if((this->buf[0]&0XF))	this->extra[2].c8[5]=this->buf[0];
			else					this->extra[2].c8[5]=(this->extra[2].c8[5]&0X0F)|(this->buf[0]&0XF0);
			if((this->extra[2].c8[5]&0X0F)==JWL_WEBSOCKET_STATUS_CLOSE){this->extra[0].u=this->extra[1].u=0;this->stop=1;goto backup;}
			if((this->extra[2].c8[5]&0X0F)==JWL_WEBSOCKET_STATUS_PING){this->extra[0].u=this->extra[1].u=0;this->stop=1;goto backup;}
			this->extra[0].u=0;
			this->extra[1].u=__jwl_websocket_get_length(this->buf);
			this->extra[2].c8[4]=1;
			this->extra[2].c8[6]=(this->buf[1]&B1000_0000)?1:0;
			if(this->extra[2].c8[6])
				__jwl_websocket_get_mask(this->buf,this->extra[2].c8);
		}
		if(this->extra[2].c8[6])
			for(;i<this->en;)
			{
				if(nxt->en>=nxt->size){jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))goto backup;}
				nxt->buf[nxt->en]=this->buf[i]^this->extra[2].c8[this->extra[0].u%4];
				++this->extra[0].u;
				++nxt->en;
				++i;
				if(this->extra[0].u==this->extra[1].u)goto end;
			}
		else
			for(;i<this->en;)
			{
				jbl_stream_buf_size_type len=jbl_min(this->en-i,nxt->size-nxt->en);
				jbl_min_update(len,this->extra[1].u-this->extra[0].u);
				jbl_memory_copy(nxt->buf+nxt->en,this->buf+i,len);
				i+=len;
				nxt->en+=len;
				this->extra[0].u+=len;
				if(this->extra[0].u==this->extra[1].u)goto end;
				jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))goto backup;				
			}
		if(this->extra[0].u==this->extra[1].u)
		{
end:;
			this->extra[2].c8[4]=0;
			if(this->extra[2].c8[5]&B1000_0000)
			{
				this->stop=1;
				jbl_stream_do(nxt,flags);
				goto backup;
			}
			jbl_memory_copy(this->buf,this->buf+i,this->en-=i);
			goto start;
		}
backup:;
		jbl_memory_copy(this->buf,this->buf+i,this->en-=i);
	}
}
jbl_stream_operators_new(jwl_stream_websocket_encode_operators,__jwl_websocket_encode_operator,NULL,NULL);
jbl_stream_operators_new(jwl_stream_websocket_decode_operators,__jwl_websocket_decode_operator,NULL,NULL);
#endif



#endif
