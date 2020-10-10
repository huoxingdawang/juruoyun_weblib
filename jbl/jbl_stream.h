/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_STREAM_H
#define __JBL_STREAM_H
#include "jbl_stream_config.h"
#if JBL_STREAM_ENABLE==1
#include "jbl_ying.h"
#include "jbl_malloc.h"
#include "jbl_gc.h"
#include "jbl_var.h"
#include "jbl_pthread.h"
#include "jbl_exception.h"
#include "jbl_pthread.h"
typedef struct __jbl_stream_operater jbl_stream_operater;
typedef union
{
	void const *			p;
	jbl_uint64				u;
	jbl_uint8				c8[8];
}jbl_stream_extra_struct;
typedef struct __jbl_stream
{
	jbl_gc						gc;
	jbl_var_ops_define			;
	jbl_pthread_lock_define		;
	const jbl_stream_operater *	op;
	struct __jbl_stream *		nxt;
	jbl_stream_buf_size_type	en;
	jbl_stream_buf_size_type	size;
	void *						data;
	unsigned char *				buf;
	jbl_uint8					stop:1;
	jbl_stream_extra_struct		extra[0];
}jbl_stream;

typedef struct __jbl_stream_operater
{
	void  (*op)(jbl_stream*,jbl_uint8);
	void  (*free)(void*);
	void  (*usb)(jbl_stream*);// update stream buf
}jbl_stream_operater;

#define			jbl_stream_operators_new(name,op,free,usb)	\
				const jbl_stream_operater name={			\
					op,										\
					(void (*)(void *))free,					\
					usb										\
				}

void			jbl_stream_start					();
void			jbl_stream_stop						();
jbl_stream *	jbl_stream_new						(const jbl_stream_operater *op,void *data,jbl_stream_buf_size_type size,unsigned char *buf,jbl_uint8 tmplen);
jbl_stream * 	jbl_stream_copy						(jbl_stream* this);
jbl_stream * 	jbl_stream_free						(jbl_stream* this);
const jbl_stream_operater * jbl_stream_get_ops(jbl_stream* this);
void			jbl_stream_do						(jbl_stream* this,jbl_uint8 flag);
#define			jbl_stream_caculate_size(y)			((sizeof(jbl_stream))+(sizeof(jbl_stream_extra_struct)*(y)))
jbl_stream *	jbl_stream_connect					(jbl_stream* this,jbl_stream* next);
jbl_stream *	jbl_stream_disconnect				(jbl_stream* this);
#define 		jbl_stream_reset(this)				jbl_stream_disconnect(this),(((jbl_stream*)jbl_refer_pull(this)))->en=0,(((jbl_stream*)jbl_refer_pull(this)))->tmp=0
		
#define 		jbl_stream_force					B0000_0001

void			jbl_stream_push_char				(jbl_stream * this,unsigned char c);
void			jbl_stream_push_chars				(jbl_stream * this,const unsigned char *str);
#define			jbl_stream_push_uint(this,in)		jbl_stream_push_uint_length(this,in,0,0)
void			jbl_stream_push_uint_length			(jbl_stream * this,jbl_uint64 in,jbl_uint8 len,char c);
void			jbl_stream_push_int					(jbl_stream * this,jbl_int64 in);
void			jbl_stream_push_double				(jbl_stream * this,double in);
void			jbl_stream_push_hex					(jbl_stream * this,jbl_uint64 in);
void			jbl_stream_push_hex_8bits			(jbl_stream * this,jbl_uint8 in);
#define			pchars(x)							jbl_stream_push_chars(jbl_stream_stdout,UC(x))
#define			pint(x)								jbl_stream_push_int(jbl_stream_stdout,x)
#define			puint(x)							jbl_stream_push_uint(jbl_stream_stdout,x)
#define			pchar(x)							jbl_stream_push_char(jbl_stream_stdout,(unsigned char)x)
#define			pdouble(x)							jbl_stream_push_double(jbl_stream_stdout,x)
#define			phex(x)								jbl_stream_push_hex(jbl_stream_stdout,x)
#define			phex8(x)							jbl_stream_push_hex_8bits(jbl_stream_stdout,x)
#define			pn()								pchar('\n')
#define			pt()								pchar('\t')
#define			pf()								jbl_stream_do(jbl_stream_stdout,jbl_stream_force)
#define			pl()								puint(__LINE__),pchars(" @ "__FILE__),pn(),pf()
#define			louts()								jbl_refer_pull_wrlock(jbl_stream_stdout)
#define			ulouts()							jbl_refer_pull_unlock(jbl_stream_stdout)
#define			lins()								jbl_refer_pull_wrlock(jbl_stream_stdin)
#define			ulins()								jbl_refer_pull_unlock(jbl_stream_stdin)

char			jbl_stream_view_put_format			(const void *this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,unsigned char * typename,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);
#if JBL_JSON_ENABLE==1
char			jbl_stream_json_put_format			(const void *this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);
#endif

extern			jbl_stream *						jbl_stream_stdout;
extern			jbl_stream *						jbl_stream_stdin;
extern			jbl_stream *						jbl_stream_stdin_link;


#else
#define			jbl_stream_start()
#define			jbl_stream_stop()
#endif
#endif
