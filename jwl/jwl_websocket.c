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
	jbl_var * sec=jwl_http_head_get(head,UC"Sec-WebSocket-Key");
	sec=jbl_V(jbl_string_add_chars(jbl_Vstring(sec),UC"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"));
	jbl_string *tmp=jbl_sha1(jbl_Vstring(sec),NULL,true);
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
	jwl_http_head_set(response,UC"Sec-WebSocket-Accept",jbl_V(jbl_gc_minus(jwl_websocket_get_sec(request,jbl_Vstring(jbl_Vstring_new())))));
	return response;
}
jbl_string * jwl_websocket_get_head(jbl_uint64 len,jbl_uint8 is_last,jwl_websocket_status opcode,jbl_string* head)
{
	head=jbl_string_add_char(head,((is_last?0X80:0X00)|(opcode&0X0F)));
	jbl_uint8 tmp[8];
	if(len<=125)
		head=jbl_string_add_char(head,len);
	else if(len>125&&len<65536)
		head=jbl_string_add_char(head,126),
		jbl_endian_to_big_uint16(&len,tmp),
		head=jbl_string_add_chars_length(head,tmp,2);
	else
		head=jbl_string_add_char(head,127),
		jbl_endian_to_big_uint64(&len,tmp),
		head=jbl_string_add_chars_length(head,tmp,8);
	return head;
}
static jbl_uint8 __jwl_websocket_get_data_start(jbl_uint8 *s);
static inline jbl_uint8 __jwl_websocket_get_data_start(jbl_uint8 *s)
{
	if((s[1]&127)==126)
		return 8;
	else if((s[1]&127)==127)
		return 14;
	return 6;
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
	jbl_uint8 len1=(s[1]&127);
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
static void __jwl_websocket_encode_operator(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream *nxt=jbl_refer_pull(this->nxt);
	if(nxt&&(!this->stop)&&this->en)
	{
		jbl_stream_buf_size_type i=0,end=this->en-((flags&jbl_stream_force)?0:16);
		if(!this->extra[0].c8[0])
		{
			jbl_string *head=jwl_websocket_get_head(end,flags&jbl_stream_force,this->extra[0].c8[1],NULL);
			jbl_stream_push_string(nxt,head);
			jbl_string_free(head);
			if(1==(this->stop=nxt->stop))return;
			this->extra[0].c8[0]=1;
			this->extra[0].c8[1]=JWL_WEBSOCKET_STATUS_EXTRA;
		}
		for(;i<end;)
		{
			jbl_stream_buf_size_type len=jbl_min(end-i,nxt->size-nxt->en);	
			jbl_memory_copy(nxt->buf+nxt->en,this->buf+i,len);
			i+=len;
			nxt->en+=len;
			jbl_stream_do(nxt,0);
			if(1==(this->stop=nxt->stop))return;
		}
		jbl_memory_copy(this->buf,this->buf+i,this->en-=i);
		this->extra[0].c8[0]=0;
	}
	if(flags&jbl_stream_force)
		this->stop=1,jbl_stream_do(nxt,flags);
}
static void __jwl_websocket_decode_operator(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
	jbl_stream *nxt=jbl_refer_pull(this->nxt);
	if(nxt&&(!this->stop)&&this->en)
	{
		jbl_stream_buf_size_type i=0;
		if(this->extra[2].c8[4]==0)//第一帧
		{
			if((i=__jwl_websocket_get_data_start(this->buf))>=this->en)
				return;
			this->extra[2].c8[5]=this->buf[0];
			if((this->extra[2].c8[5]&0X0F)==JWL_WEBSOCKET_STATUS_CLOSE){this->extra[0].u=this->extra[1].u=0;return;}
			if((this->extra[2].c8[5]&0X0F)==JWL_WEBSOCKET_STATUS_PING){this->extra[0].u=this->extra[1].u=0;return;}
			this->extra[1].u=__jwl_websocket_get_length(this->buf);
			this->extra[2].c8[4]=1;
			__jwl_websocket_get_mask(this->buf,this->extra[2].c8);
		}
		for(jbl_stream_buf_size_type len=this->en;i<len;++i)
		{
			if(nxt->en>=nxt->size){jbl_stream_do(nxt,0);if(1==(this->stop=nxt->stop))return;}
			nxt->buf[nxt->en]=this->buf[i]^this->extra[2].c8[this->extra[0].u%4];
			++this->extra[0].u;
			++nxt->en;
		}
		this->en=0;
		if(this->extra[0].u==this->extra[1].u)
		{
			this->extra[2].c8[4]=0;
			if(this->extra[2].c8[5]&B1000_0000)
			{
				this->stop=1;
				jbl_stream_do(nxt,flags);
				return;
			}
		}
		jbl_stream_do(nxt,0);
	}
}
jbl_stream_operators_new(jwl_stream_websocket_encode_operators,__jwl_websocket_encode_operator,NULL,NULL);
jbl_stream_operators_new(jwl_stream_websocket_decode_operators,__jwl_websocket_decode_operator,NULL,NULL);
#endif



#endif
