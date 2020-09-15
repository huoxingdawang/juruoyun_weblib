/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_log.h"
#if JBL_LOG_ENABLE==1
#include "jbl_stream.h"
#include "jbl_var.h"
#include "jbl_string.h"
#include "jbl_file.h"
#include <stdarg.h>
struct
{
	jbl_uint32 cnt;
	jbl_uint32 pcnt;
	jbl_uint8 start;	
	jbl_log_struct d[JBL_LOG_MAX_LENGTH];
	jbl_log_parameter_struct p[JBL_LOG_MAX_LENGTH*4];
	
	
	
	jbl_stream* fs;
}__jbl_logs;


void jbl_log_start()
{
	__jbl_logs.cnt=0;
	__jbl_logs.start=1;
	__jbl_logs.pcnt=0;
#if JBL_TIME_ENABLE==1
	for(jbl_uint32 i=0;i<JBL_LOG_MAX_LENGTH;++i)
		jbl_time_init(&__jbl_logs.d[i].t);
#endif	
}
void jbl_log_stop()
{
	jbl_log_save();
	jbl_stream_do(__jbl_logs.fs,jbl_stream_force);
	__jbl_logs.fs=jbl_stream_free(__jbl_logs.fs);
}

void jbl_log_add_log(const char * file,const char * func,jbl_uint32 line,unsigned char *s,...)
{
	if(!__jbl_logs.start)return;
	__jbl_logs.d[__jbl_logs.cnt].file=file;
	__jbl_logs.d[__jbl_logs.cnt].func=func;
	__jbl_logs.d[__jbl_logs.cnt].line=line;
	__jbl_logs.d[__jbl_logs.cnt].chars=s;
	va_list arg_ptr;
	va_start(arg_ptr,s);
	for(;*s;)
	{
		jbl_scanner_key key=jbl_scanner(s,&s);
		switch(key)
		{
			case JBL_SCANNER_KEY_UNDEFINED				:							;break;
			case JBL_SCANNER_KEY_END					:goto finish;				;break;
			case JBL_SCANNER_KEY_INT					:__jbl_logs.p[__jbl_logs.pcnt].i=va_arg(arg_ptr,jbl_int64)	;++__jbl_logs.pcnt	;break;
			case JBL_SCANNER_KEY_UINT					:__jbl_logs.p[__jbl_logs.pcnt].u=va_arg(arg_ptr,jbl_uint64)	;++__jbl_logs.pcnt	;break;
			case JBL_SCANNER_KEY_DOUBLE					:__jbl_logs.p[__jbl_logs.pcnt].d=va_arg(arg_ptr,double)		;++__jbl_logs.pcnt	;break;
			case JBL_SCANNER_KEY_CHAR					:__jbl_logs.p[__jbl_logs.pcnt].c=va_arg(arg_ptr,int)		;++__jbl_logs.pcnt	;break;//类型提升
			case JBL_SCANNER_KEY_CHARS					:__jbl_logs.p[__jbl_logs.pcnt].s=va_arg(arg_ptr,char *)		;++__jbl_logs.pcnt	;break;
			case JBL_SCANNER_KEY_HEX					:__jbl_logs.p[__jbl_logs.pcnt].u=va_arg(arg_ptr,jbl_uint64)	;++__jbl_logs.pcnt	;break;
#if JBL_VAR_ENABLE == 1
			case JBL_SCANNER_KEY_VAR					:__jbl_logs.p[__jbl_logs.pcnt].v=jbl_var_copy(va_arg(arg_ptr,void*))	;++__jbl_logs.pcnt	;break;
			case JBL_SCANNER_KEY_JSON					:__jbl_logs.p[__jbl_logs.pcnt].v=jbl_var_copy(va_arg(arg_ptr,void*))	;++__jbl_logs.pcnt	;break;
#endif
		}
	}
	finish:;
	va_end(arg_ptr); 
#if JBL_TIME_ENABLE==1
	jbl_time_now(&__jbl_logs.d[__jbl_logs.cnt].t);
#endif
	++__jbl_logs.cnt;
	if((__jbl_logs.cnt+10)>=JBL_LOG_MAX_LENGTH)
		jbl_log_save();
	else if((__jbl_logs.pcnt+40)>=JBL_LOG_MAX_LENGTH*4)
		jbl_log_save();
}
#include <stdio.h>


void jbl_log_save()
{	
	if((__jbl_logs.start&0x02))return;
	__jbl_logs.start|=0x02;
	jbl_log(UC "Saving logs");
	if(!__jbl_logs.fs)__jbl_logs.fs=jbl_file_stream_new(jbl_file_open_chars(NULL,UC JBL_LOG_DIR,JBL_FILE_WRITE));
#if JBL_STREAM_ENABLE==1
	jbl_uint32 j=0;
	for(jbl_uint32 i=0;i<__jbl_logs.cnt;++i)
	{
#if JBL_TIME_ENABLE==1
		jbl_stream_push_char(__jbl_logs.fs,'[');
		jbl_stream_push_time(__jbl_logs.fs,&__jbl_logs.d[i].t,UC"Y-m-d H:i:s.u");
		jbl_stream_push_char(__jbl_logs.fs,']');
		jbl_stream_push_char(__jbl_logs.fs,'\t');
#endif	
		jbl_stream_push_chars(__jbl_logs.fs,UC "At:"  );jbl_stream_push_chars(__jbl_logs.fs,UC __jbl_logs.d[i].file);jbl_stream_push_char(__jbl_logs.fs,'\t');
		jbl_stream_push_chars(__jbl_logs.fs,UC "Line:");jbl_stream_push_uint (__jbl_logs.fs,   __jbl_logs.d[i].line);jbl_stream_push_char(__jbl_logs.fs,'\t');
		jbl_stream_push_chars(__jbl_logs.fs,UC "Func:");jbl_stream_push_chars(__jbl_logs.fs,UC __jbl_logs.d[i].func);jbl_stream_push_char(__jbl_logs.fs,'\t');
		
		for(unsigned char *s=__jbl_logs.d[i].chars;*s;)
		{
			jbl_scanner_key key=jbl_scanner(s,&s);
			switch(key)
			{
				case JBL_SCANNER_KEY_UNDEFINED				:jbl_stream_push_char(__jbl_logs.fs,*(s-1))									;break;
				case JBL_SCANNER_KEY_END					:goto finish																;break;
				case JBL_SCANNER_KEY_INT					:jbl_stream_push_int	(__jbl_logs.fs,__jbl_logs.p[j].i)			;++j	;break;
				case JBL_SCANNER_KEY_UINT					:jbl_stream_push_uint	(__jbl_logs.fs,__jbl_logs.p[j].u)			;++j	;break;
				case JBL_SCANNER_KEY_DOUBLE					:jbl_stream_push_double	(__jbl_logs.fs,__jbl_logs.p[j].d)			;++j	;break;
				case JBL_SCANNER_KEY_CHAR					:jbl_stream_push_char	(__jbl_logs.fs,__jbl_logs.p[j].c)			;++j	;break;
				case JBL_SCANNER_KEY_HEX					:jbl_stream_push_hex	(__jbl_logs.fs,__jbl_logs.p[j].u)			;++j	;break;
				case JBL_SCANNER_KEY_CHARS					:jbl_stream_push_chars	(__jbl_logs.fs,UC __jbl_logs.p[j].s)		;++j	;break;
#if JBL_VAR_ENABLE == 1
				case JBL_SCANNER_KEY_VAR					:jbl_stream_push_char	(__jbl_logs.fs,'\n');jbl_var_view_put(__jbl_logs.p[j].v,__jbl_logs.fs,1,JBL_VIEW_DEFAULT_TABS,0,NULL,NULL,NULL)	;__jbl_logs.p[j].v=jbl_var_free(__jbl_logs.p[j].v)	;++j	;break;
				case JBL_SCANNER_KEY_JSON					:
#if JBL_JSON_ENABLE == 1
				jbl_var_json_put		(__jbl_logs.p[j].v,__jbl_logs.fs,0,0);
#endif	
				__jbl_logs.p[j].v=jbl_var_free(__jbl_logs.p[j].v)	;++j	;break;
#endif	
			}
		}
		finish:;
		
		
		jbl_stream_push_char(__jbl_logs.fs,'\n');
	}
#if JBL_LOG_DISABLE_STREAM_CACHE ==1
	jbl_stream_do(__jbl_logs.fs,1);
#endif
#endif	
	__jbl_logs.start&=(~0x02);
	__jbl_logs.cnt=0;
	__jbl_logs.pcnt=0;
}

#endif
