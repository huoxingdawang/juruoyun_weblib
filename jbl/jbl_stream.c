/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_stream.h"
#if JBL_STREAM_ENABLE==1
#include "jbl_var.h"
#include <stdio.h>
#include "jbl_string_cc.h"
extern			const jbl_stream_operater			jbl_stream_file_operators;
jbl_var_operators_new(jbl_stream_operators,jbl_stream_free,jbl_stream_copy,NULL,NULL,NULL,NULL);
void jbl_stream_start()
{
	jbl_stream_stdout=jbl_stream_new(&jbl_stream_file_operators,JBL_STREAM_STDOUT,JBL_STREAM_EXCEED_LENGTH,NULL,0);
	jbl_stream_stdin =jbl_stream_new(&jbl_stream_file_operators,JBL_STREAM_STDIN ,0,NULL,0);
#ifdef _WIN32
#if JBL_STRING_CC_ENABLE ==1
	jbl_stream_stdin_link=jbl_stream_gb2312_to_utf8_new();
	jbl_stream_connect(jbl_stream_stdin,jbl_stream_stdin_link);

	jbl_stream * tmp=jbl_stream_stdout;
	jbl_stream_stdout=jbl_stream_utf8_to_gb2312_new();
	jbl_stream_connect(jbl_stream_stdout,tmp);
	jbl_stream_free(tmp);
#else
	jbl_stream_stdin_link=jbl_stream_copy(jbl_stream_stdin);
#endif
#elif __APPLE__
	jbl_stream_stdin_link=jbl_stream_copy(jbl_stream_stdin);
#elif __linux__
	jbl_stream_stdin_link=jbl_stream_copy(jbl_stream_stdin);
#endif
}
void jbl_stream_stop()
{
	/*不关闭stdout,给malloc留条活路jbl_stream_do(jbl_stream_stdout,jbl_stream_force),jbl_stream_stdout=jbl_stream_free(jbl_stream_stdout),*/
	jbl_stream_stdin		=jbl_stream_free(jbl_stream_stdin);	
	jbl_stream_stdin_link	=jbl_stream_free(jbl_stream_stdin_link);	
}
jbl_stream * jbl_stream_new(const jbl_stream_operater *op,void *data,jbl_stream_buf_size_type size,unsigned char *buf,jbl_uint8 tmplen)
{
	jbl_stream* this=jbl_malloc(jbl_stream_caculate_size(tmplen)+((buf)?0:size));
	jbl_gc_init(this);
	jbl_gc_plus(this);//增加引用计数		
	jbl_var_set_operators(this,&jbl_stream_operators);
	jbl_pthread_lock_init(this);
	this->op	=op;
	this->data	=data;
	this->size	=size;
	this->en	=0;
	this->stop	=0;
	this->buf	=((buf)?buf:(((jbl_uint8*)this)+(sizeof(jbl_uint64)*tmplen)+(sizeof(jbl_stream))));
	this->nxt	=NULL;
	while(tmplen--)this->extra[tmplen].u=0;
	if(this->op->usb)this->op->usb(this);
	return this;
}
jbl_stream * jbl_stream_free(jbl_stream* this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this))
			jbl_stream_free((jbl_stream*)(((jbl_reference*)this)->ptr));
		else
		{
			if(this->op->free)
				this->op->free(this->data);
			jbl_stream_free(((jbl_stream *)jbl_refer_pull(this))->nxt);
		}
		jbl_free(this);
	}
	return NULL;
}
jbl_stream * jbl_stream_copy(jbl_stream* this)
{
	if(!this)return NULL;
	jbl_gc_plus(this);
	return this;
/*	
	if(jbl_gc_is_ref(this))
	{
		jbl_gc_plus(this);
	}
	jbl_stream_buf_size_type size=jbl_malloc_size(this);
	jbl_stream *that=jbl_malloc(size);
	jbl_gc_init(that);
	jbl_gc_plus(that);//增加引用计数	
	that->op	=this->op;
	that->tmpv	=this->tmpv;
	that->nxt	=jbl_stream_copy(this->nxt);
	if(that->op->scp)that->data=that->op->scp(this->data);
	if(this->buf==(((jbl_uint8*)this)+(sizeof(jbl_stream))))
	{
		that->en	=this->en;
		that->size	=this->size;
		that->buf	=(((jbl_uint8*)that)+(sizeof(jbl_stream)));
		jbl_memory_copy(that->buf,this->buf,this->en);
	}
	else
		that->en=0,that->size=0,that->buf=NULL;
	if(that->op->usb)that->op->usb(that);
	return that;
*/
}
JBL_INLINE const jbl_stream_operater *jbl_stream_get_ops(jbl_stream* this)
{
	if(!this)return NULL;
	const jbl_stream_operater *ops=((jbl_stream*)jbl_refer_pull(this))->op;
	return ops;
}
void jbl_stream_do(jbl_stream* this,jbl_uint8 flag)
{
	const jbl_stream_operater *ops=jbl_stream_get_ops(this);
	if(ops&&ops->op)ops->op(this,flag);
}
jbl_stream * jbl_stream_connect(jbl_stream* this,jbl_stream* next)
{
	this=jbl_stream_disconnect(this);
	jbl_stream* thi=jbl_refer_pull(this);
	thi->nxt=jbl_stream_copy(next);
	return this;
}
JBL_INLINE jbl_stream * jbl_stream_disconnect(jbl_stream* this)
{
	jbl_stream* thi=jbl_refer_pull(this);
	thi->nxt=jbl_stream_free(thi->nxt);
	return this;
}

void jbl_stream_push_char(jbl_stream* this,unsigned char c)
{
	if(!this)jbl_exception("NULL POINTER");
	jbl_stream*thi=jbl_refer_pull(this);
	if(thi->stop)return;
	thi->buf[thi->en]=c;
	++thi->en;
	if(thi->en>=thi->size)jbl_stream_do(thi,0);
}
void jbl_stream_push_chars(jbl_stream* this,const unsigned char *str)
{
	if(!str)return;
	if(!this)jbl_exception("NULL POINTER");
	jbl_stream*thi=jbl_refer_pull(this);
	if(thi->stop)return;
	for(;*str;)
	{
		for(;*str&&thi->en<thi->size;thi->buf[thi->en]=*str,++str,++thi->en);
		jbl_stream_do(thi,0);if(thi->stop)return;
	}
}
void jbl_stream_push_uint_length(jbl_stream *this,jbl_uint64 in,jbl_uint8 len,char c)
{
	if(in==0)return jbl_stream_push_char(this,'0');
	int cnt=20;
	unsigned char b[21];
	b[cnt--]=0;
	while(in)b[cnt--]=in%10+'0',in/=10;
	for(jbl_uint8 i=19-cnt;i<len;jbl_stream_push_char(this,c),++i);
	jbl_stream_push_chars(this,b+cnt+1);	
}
void jbl_stream_push_int(jbl_stream* this,jbl_int64 in)
{
	if(in<0)jbl_stream_push_char(this,'-'),in=-in;
	jbl_stream_push_uint(this,in);
}
void jbl_stream_push_double(jbl_stream* this,double in)
{
	if(in<0)jbl_stream_push_char(this,'-'),in=-in;
	jbl_stream_push_uint(this,in);
	in-=(jbl_uint64)in;
	jbl_stream_push_char(this,'.');
	jbl_uint64 t=(in*1000000+0.5)/10;
	if(t==0)
		return jbl_stream_push_char(this,'0');
	unsigned char b[21];
	for(register unsigned char i=0;i<21;b[i]='0',++i);
	register unsigned char cnt=20;
	b[cnt--]=0;
	while(t)b[cnt--]=t%10+'0',t/=10;
	jbl_stream_push_chars(this,b+20-7+2);
}
void jbl_stream_push_hex(jbl_stream *this,jbl_uint64 in)
{
	unsigned char n=1;
	while((in>>(n<<2)))++n;
	const char hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	for(;n--;jbl_stream_push_char(this,hex[(in>>(n<<2))&15]));
}
void jbl_stream_push_hex_8bits(jbl_stream *this,jbl_uint8 in)
{
	const char hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	jbl_stream_push_char(this,hex[(in>>4)&15]),jbl_stream_push_char(this,hex[in&15]);	
}
JBL_INLINE char jbl_stream_view_put_format(const void *this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,unsigned char * typename,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	if(!out)jbl_exception("NULL POINTER");
	if(format)for(jbl_uint32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);
	if(!this)typename=UC"null";
	jbl_stream_push_chars(out,typename);
	int i=0;while(typename[i])++i;
	if(i<JBL_VIEW_NAME_LENGTH)
		for(;i<JBL_VIEW_NAME_LENGTH;++i,jbl_stream_push_char(out,' '));
#if JBL_VIEW_DISPLAY_VARNAME == 1
	if(format)jbl_stream_push_chars(out,varname);
#endif
#if JBL_VIEW_DISPLAY_FUNC == 1
	if(format&&func)jbl_stream_push_chars(out,UC" in func:"),jbl_stream_push_chars(out,func);
#endif
#if JBL_VIEW_DISPLAY_FILE == 1
	if(format&&file)jbl_stream_push_chars(out,UC" at file:"),jbl_stream_push_chars(out,file);
#endif
#if JBL_VIEW_DISPLAY_LINE == 1
	if(format&&file)jbl_stream_push_chars(out,UC" on line:"),jbl_stream_push_uint(out,line);
#endif
	return this?0:1;
}
#if JBL_JSON_ENABLE==1
JBL_INLINE char jbl_stream_json_put_format(const void *this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(!out)jbl_exception("NULL POINTER");
	if(format&1)for(jbl_uint32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);
	if(!this)return jbl_stream_push_chars(out,UC"null"),1;
	return 0;
}
#endif
void jbl_stream_file_operator(jbl_stream* this,jbl_uint8 flags)
{
	jbl_stream* thi=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(thi->nxt);
	if(nxt)
		for(;;)
		{
			if((nxt->en+1)>nxt->size){jbl_stream_do(nxt,0);if(nxt->stop)break;}
			jbl_int8 c=fgetc(thi->data);
			if(c==EOF||(c=='\n'&&thi->data==stdin)){jbl_stream_do(nxt,flags);break;}
			nxt->buf[nxt->en++]=c;
		}
	else
	{
		fwrite(thi->buf,1,thi->en,thi->data);
		thi->en=0;
	}
}
jbl_stream_operators_new(jbl_stream_file_operators,jbl_stream_file_operator,fclose,NULL);
jbl_stream *jbl_stream_stdout;
jbl_stream *jbl_stream_stdin;
jbl_stream *jbl_stream_stdin_link;

#endif
