/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_json.h"
#if JBL_JSON_ENABLE==1
#include "jbl_string.h"
#include "jbl_string_cc.h"
#include "jbl_stream.h"
#include "jbl_ll.h"
#include "jbl_ht.h"
#include "jbl_log.h"
#if JBL_JSON_DEBUG ==0
	#undef jbl_log
	#define jbl_log(x,...)
#endif
struct __jbl_json_decode_yy
{
	jbl_uint8		cond;
	jbl_uint8		state;
	jbl_uint8		ch;
	struct
	{
		jbl_int8	f:2;
		jbl_int8	type:4;	
	};
	unsigned char *	marker;
//	jbl_string    *	k;
	void          *	v;
	struct __jbl_json_decode_yy *pre;
	union
	{
		jbl_uint64	u;
		double		d;
	}tmp;
	jbl_string* htk;
	jbl_uint8	dl;
};
#define ARRAY	1
#define HTK		2
#define HTV		3
struct __jbl_json_decode_yy* __jbl_json_decode_yy_new()
{
	struct __jbl_json_decode_yy*yy=jbl_malloc(sizeof(struct __jbl_json_decode_yy));
	yy->cond	=0;
	yy->state	=0;		
	yy->ch		=0;
	yy->marker	=NULL;
	yy->v		=NULL;
	yy->pre		=NULL;
	yy->htk		=NULL;
	return yy;
	
}
struct __jbl_json_decode_yy* __jbl_json_decode_yy_free(struct __jbl_json_decode_yy* yy)
{
	if(!yy)return NULL;
	jbl_var_free(yy->v);
	jbl_string_free(yy->htk);
	__jbl_json_decode_yy_free(yy->pre);
	jbl_free(yy);
	return NULL;
}
/*!types:re2c */
//-1 完成
//-2致命错误
//-3更多输入
struct __jbl_json_decode_yy* __jbl_json_decode(struct __jbl_json_decode_yy* yy,unsigned char* in,unsigned char*YYLIMIT,jbl_string_size_type *start)
{
	jbl_string_size_type _=0;if(!start)start=&_;
start:;
#define upd *start=YYCURSOR-in	
	unsigned char	*	YYCURSOR=in+*start;
///*!re2c
	re2c:condenumprefix						=jbl_json_condition_;
	re2c:define:YYCONDTYPE					=jbl_json_condition;
	re2c:define:YYSETCONDITION				="yy->cond = ";
	re2c:define:YYGETCONDITION				=yy->cond ;
	re2c:define:YYGETCONDITION:naked		=1;
	re2c:define:YYSETSTATE					="yy->state = ";
	re2c:define:YYGETSTATE					=yy->state ;
	re2c:define:YYGETSTATE:naked			=1;
	re2c:define:YYMARKER					=yy->marker;
	re2c:yyfill:enable						=1;
	re2c:define:YYFILL:naked				=1;
	re2c:define:YYFILL						="{upd;return yy;}";
	re2c:variable:yych						=yy->ch;
	<> :=> start
	<start>			[\x01-\x20]									{upd																						;goto yyc_start;}
	<start>			"\""						=>string		{upd;yy->v=jbl_var_free(yy->v);yy->v=jbl_string_new(yy->v)									;goto yyc_string;}
	<start>			"0x"|"0X"					=>hex			{upd;yy->v=jbl_var_free(yy->v);yy->tmp.u=0													;goto yyc_hex;}
	<start>			[0-9]						=>num			{upd;yy->v=jbl_var_free(yy->v);yy->tmp.u=*(YYCURSOR-1)-'0';yy->f=1							;goto yyc_num;}
	<start>			"-"							=>num			{upd;yy->v=jbl_var_free(yy->v);yy->tmp.u=0;yy->f=-1											;goto yyc_num;}
	<start>			"+"							=>num			{upd;yy->v=jbl_var_free(yy->v);yy->tmp.u=0;yy->f=1											;goto yyc_num;}
	<start>			"."							=>flo			{upd;yy->v=jbl_var_free(yy->v);yy->f=1;yy->dl=0;											;goto yyc_flo;}
	<start>			"true"										{upd;yy->v=jbl_var_free(yy->v);yy->v=jbl_Vtrue_new()										;goto yyc_start;}
	<start>			"null"										{upd;yy->v=jbl_var_free(yy->v);yy->v=jbl_Vnull_new()										;goto yyc_start;}
	<start>			"false"										{upd;yy->v=jbl_var_free(yy->v);yy->v=jbl_Vfalse_new()										;goto yyc_start;}
	<start>			[]											{																							;return yy;}	
	
	
	
	<string>		"\""						=>start			{upd																						;goto yyc_start;}
	<string>		[]											{upd;yy->v=jbl_string_add_char(yy->v,*YYCURSOR)			;++YYCURSOR;upd						;goto yyc_string;}
	<string>		"\\"						=>string_turn	{upd																						;goto yyc_string_turn;}
	<string_turn>	"\\"						=>string		{upd;yy->v=jbl_string_add_char(yy->v,'\\')													;goto yyc_string;}
	<string_turn>	"\""						=>string		{upd;yy->v=jbl_string_add_char(yy->v,'"')													;goto yyc_string;}
	<string_turn>	"b"							=>string		{upd;yy->v=jbl_string_add_char(yy->v,'\b')													;goto yyc_string;}
	<string_turn>	"f"							=>string		{upd;yy->v=jbl_string_add_char(yy->v,'\f')													;goto yyc_string;}
	<string_turn>	"n"							=>string		{upd;yy->v=jbl_string_add_char(yy->v,'\n')													;goto yyc_string;}
	<string_turn>	"r"							=>string		{upd;yy->v=jbl_string_add_char(yy->v,'\r')													;goto yyc_string;}
	<string_turn>	"t"							=>string		{upd;yy->v=jbl_string_add_char(yy->v,'\t')													;goto yyc_string;}
	<string_turn>	"u"							=>string_u		{upd;yy->tmp.u=0																			;goto yyc_string_u;}
	<string_turn>	[]							=>string		{upd;																						;goto yyc_string;}
	<string_u>		[0-9]										{upd;yy->tmp.u=(yy->tmp.u<<4)+*(YYCURSOR-1)-'0'												;goto yyc_string_u;}
	<string_u>		[a-f]										{upd;yy->tmp.u=(yy->tmp.u<<4)+*(YYCURSOR-1)-'a'+10											;goto yyc_string_u;}
	<string_u>		[A-F]										{upd;yy->tmp.u=(yy->tmp.u<<4)+*(YYCURSOR-1)-'A'+10											;goto yyc_string_u;}
	<string_u>		[\x00]										{																							;return yy;}	
	<string_u>		[]							=>string		{upd;yy->v=jbl_string_add_utf8_from_unicode(yy->v,yy->tmp.u)								;goto yyc_string;}

	<hex>			[0-9]										{upd;yy->tmp.u=(yy->tmp.u<<4)+*(YYCURSOR-1)-'0'												;goto yyc_hex;}
	<hex>			[a-f]										{upd;yy->tmp.u=(yy->tmp.u<<4)+*(YYCURSOR-1)-'a'+10											;goto yyc_hex;}
	<hex>			[A-F]										{upd;yy->tmp.u=(yy->tmp.u<<4)+*(YYCURSOR-1)-'A'+10											;goto yyc_hex;}
	<hex>			[]							=>start			{upd;yy->v=jbl_Vuint_set(yy->v,yy->tmp.u)													;goto yyc_start;}
	<hex>			[\x00]										{   ;yy->v=jbl_Vuint_set(yy->v,yy->tmp.u)													;yy->cond=-1;return yy;}	
	
	<num>			[0-9]										{upd;yy->tmp.u=(yy->tmp.u<<3)+(yy->tmp.u<<1)+*(YYCURSOR-1)-'0'								;goto yyc_num;}
	<num>			[]							=>start			{upd;yy->v=((yy->f==1)?jbl_Vuint_set(yy->v,yy->tmp.u):jbl_Vint_set(yy->v,yy->tmp.u*-1))		;goto yyc_start;}
	<num>			[\x00]										{   ;yy->v=((yy->f==1)?jbl_Vuint_set(yy->v,yy->tmp.u):jbl_Vint_set(yy->v,yy->tmp.u*-1))		;yy->cond=-1;return yy;}	
	<num>			"."							=>flo			{upd;yy->v=jbl_var_free(yy->v);yy->dl=0														;goto yyc_flo;}
	<num>			[eE]						=>e				{upd;yy->v=jbl_var_free(yy->v);yy->dl=0;yy->tmp.d=yy->tmp.u;yy->f=1;yy->dl=0				;goto yyc_e;}

	<flo>			[0-9]										{upd;yy->tmp.u=(yy->tmp.u<<3)+(yy->tmp.u<<1)+*(YYCURSOR-1)-'0';++yy->dl;					;goto yyc_flo;}
	<flo>			[]							=>start			{upd;yy->v=jbl_Vdouble_set(yy->v,yy->f*((double)yy->tmp.u)/jbl_pow(10,yy->dl))				;goto yyc_start;}
	<flo>			[\x00]										{   ;yy->v=jbl_Vdouble_set(yy->v,yy->f*((double)yy->tmp.u)/jbl_pow(10,yy->dl))				;yy->cond=-1;pl();return yy;}	
	<flo>			[eE]						=>e				{upd;yy->tmp.d=yy->f*((double)yy->tmp.u)/jbl_pow(10,yy->dl);yy->f=1;yy->dl=0				;goto yyc_e;}
	
	<e>				"-"											{upd;yy->f=-1																				;goto yyc_e;}
	<e>				"+"											{upd;yy->f=1																				;goto yyc_e;}
	<e>				[0-9]										{upd;yy->dl=(yy->dl<<3)+(yy->dl<<1)+*(YYCURSOR-1)-'0'										;goto yyc_e;}
	<e>				[]							=>start			{upd;yy->v=jbl_Vdouble_set(yy->v,yy->tmp.d*jbl_pow((yy->f==1)?10:0.1,yy->dl))				;goto yyc_start;}
	<e>				[\x00]										{   ;yy->v=jbl_Vdouble_set(yy->v,yy->tmp.d*jbl_pow((yy->f==1)?10:0.1,yy->dl))				;yy->cond=-1;return yy;}	
	
	<start>			"["											{
																	upd;yy->v=jbl_var_free(yy->v);yy->v=jbl_ll_new(yy->v);
																	struct __jbl_json_decode_yy* yy2=__jbl_json_decode_yy_new();
																	yy2->type=ARRAY;
																	yy2->pre=yy;
																	yy=yy2;
																	goto start;
																}
	<start>			"]"											{
																	if(yy->type==ARRAY)
																	{
																		upd;
																		struct __jbl_json_decode_yy* yy2=yy->pre;
																		if(yy->v)
																			yy2->v=jbl_ll_add(yy2->v,yy->v);
																		yy->pre=NULL;
																		yy=__jbl_json_decode_yy_free(yy);
																		yy=yy2;
																		goto start;
																	}
																	else
																	{
																		yy->cond=-2;
																		jbl_log(UC"JSON decode error %d %j %s",yy->type,yy->v,in);
																		return yy;
																	}
																}
	<start>			"{"											{
																	upd;yy->v=jbl_var_free(yy->v);yy->v=jbl_ht_new(yy->v);
																	struct __jbl_json_decode_yy* yy2=__jbl_json_decode_yy_new();
																	yy2->type=HTK;
																	yy2->pre=yy;
																	yy=yy2;
																	goto start;
																}
	<start>			"}"											{
																	if((yy->type==HTK&&!yy->htk)||(yy->type==HTV&&yy->v))
																	{
																		upd;
																		struct __jbl_json_decode_yy* yy2=yy->pre;
																		if(yy->v)
																			yy2->v=jbl_ht_insert(yy2->v,yy->htk,yy->v);
																		yy->pre=NULL;
																		yy=__jbl_json_decode_yy_free(yy);
																		yy=yy2;
																		goto start;																					
																	}
																	else
																	{
																		jbl_log(UC"JSON decode error %d %j %j %s",yy->type,yy->htk,yy->v,in);
																		yy->cond=-2;
																		return yy;
																	}
																}
	<start>			":"											{
																	if(yy->type==HTK&&jbl_var_is(yy->v,jbl_string_operators))
																	{
																		upd;
																		yy->htk=(jbl_string*)yy->v;
																		yy->v=NULL;
																		yy->type=HTV;
																		goto start;
																	}
																	else
																	{
																		jbl_log(UC"JSON decode error %s",in);
																		yy->cond=-2;
																		return yy;
																	}
																}																
	<start>			","											{
																	if(yy->type==ARRAY)
																	{
																		upd;
																		struct __jbl_json_decode_yy* yy2=yy->pre;
																		if(yy->v)
																			yy2->v=jbl_ll_add(yy2->v,yy->v);
																		else
																		{
																			jbl_log(UC"JSON decode error %s",in);
																			yy->cond=-2;
																			return yy;
																		}
																		yy->v=jbl_var_free(yy->v);
																		goto yyc_start;
																	}
																	else if(yy->type==HTV&&yy->htk)
																	{
																		upd;
																		struct __jbl_json_decode_yy* yy2=yy->pre;
																		if(yy->v)
																			yy2->v=jbl_ht_insert(yy2->v,yy->htk,yy->v);
																		else
																		{
																			jbl_log(UC"JSON decode error %s",in);
																			yy->cond=-2;
																			return yy;
																		}
																		yy->v=jbl_var_free(yy->v);
																		yy->htk=jbl_string_free(yy->htk);
																		yy->type=HTK;
																		goto yyc_start;																		
																	}
																	else
																	{
																		jbl_log(UC"JSON decode error %s",in);
																		yy->cond=-2;
																		return yy;
																	}
																}


	<*>				[\x00]										{yy->cond=-1;return yy;}	
 */
	#undef YYCTYPE	


	return yy;
}
jbl_stream *jbl_json_decode_stream_new()
{
	struct __jbl_json_decode_yy* yy=__jbl_json_decode_yy_new();
	jbl_stream * this=jbl_stream_new(&jbl_json_decode_stream_operaters,yy,JBL_JSON_DECODE_STREAM_BUF_LENGTH+1,NULL,1);
	this->extra[0].p=yy;
	return this;
}
static void __jbl_json_decode_stream_usf(jbl_stream* this){this->size=JBL_JSON_DECODE_STREAM_BUF_LENGTH;}
static void __jbl_json_decode_stream_operater(jbl_stream* this,jbl_uint8 flags)
{
	if(!this)jbl_exception("NULL POINTER");	
	this=jbl_refer_pull(this);
//	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jbl_string_size_type i=0;
	if((!this->stop)&&this->en)
	{
		this->buf[this->en]=0;
		this->data=__jbl_json_decode(this->data,this->buf,this->buf+this->size,&i);
		if(((struct __jbl_json_decode_yy*)this->data)->cond==(jbl_uint8)-2)
		{
			jbl_log(UC"%v",((struct __jbl_json_decode_yy*)this->extra[0].p)->v);
			this->extra[0].p=__jbl_json_decode_yy_free((struct __jbl_json_decode_yy*)this->extra[0].p);
			this->extra[0].p=this->data=__jbl_json_decode_yy_new();
			this->stop=1;
			return;
		}
		if((((struct __jbl_json_decode_yy*)this->data)->cond==(jbl_uint8)-1)&&(this->extra[0].p==this->data)){this->stop=1;return;}
	}
	jbl_memory_copy(this->buf,this->buf+i,this->en-=i);
}
jbl_stream_operators_new(jbl_json_decode_stream_operaters,__jbl_json_decode_stream_operater,__jbl_json_decode_yy_free,__jbl_json_decode_stream_usf);
inline void * jbl_json_decode_stream_var(jbl_stream* this)
{
	this=jbl_refer_pull(this);
	return jbl_var_copy(((struct __jbl_json_decode_yy*)(this->extra[0].p))->v);
}


#endif
