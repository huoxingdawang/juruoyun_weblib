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
#include <stdarg.h>
jbl_uint32 __jbl_logs_cnt;
jbl_uint32 __jbl_log_parameter_cnt;
jbl_uint8 __jbl_logs_start;
jbl_log_struct __jbl_logs[JBL_LOG_MAX_LENGTH];
jbl_log_parameter_struct __jbl_log_parameter[JBL_LOG_MAX_LENGTH*4];







void jbl_log_start()
{
	__jbl_logs_cnt=0;
	__jbl_logs_start=1;
	__jbl_log_parameter_cnt=0;
	fclose(fopen(JBL_LOG_DIR,"wb"));
#if JBL_TIME_ENABLE==1
	for(jbl_uint32 i=0;i<JBL_LOG_MAX_LENGTH;++i)
		jbl_time_init(&__jbl_logs[i].t);
#endif	
}
void jbl_log_stop()
{
	jbl_log_save();	
}

void jbl_log_add_log(const char * file,const char * func,jbl_uint32 line,unsigned char *s,...)
{
	if(!__jbl_logs_start)return;
	__jbl_logs[__jbl_logs_cnt].file=file;
	__jbl_logs[__jbl_logs_cnt].func=func;
	__jbl_logs[__jbl_logs_cnt].line=line;
	__jbl_logs[__jbl_logs_cnt].chars=s;
	va_list arg_ptr;
	va_start(arg_ptr,s);
	for(;*s;)
	{
		jbl_var_scanner_key key=jbl_var_scanner(s,&s);
		switch(key)
		{
			case JBL_VAR_SCANNER_KEY_UNDEFINED				:							;break;
			case JBL_VAR_SCANNER_KEY_END					:goto finish;				;break;
			case JBL_VAR_SCANNER_KEY_INT					:__jbl_log_parameter[__jbl_log_parameter_cnt].i=va_arg(arg_ptr,jbl_int64)	;++__jbl_log_parameter_cnt	;break;
			case JBL_VAR_SCANNER_KEY_UINT					:__jbl_log_parameter[__jbl_log_parameter_cnt].u=va_arg(arg_ptr,jbl_uint64)	;++__jbl_log_parameter_cnt	;break;
			case JBL_VAR_SCANNER_KEY_DOUBLE					:__jbl_log_parameter[__jbl_log_parameter_cnt].d=va_arg(arg_ptr,double)		;++__jbl_log_parameter_cnt	;break;
			case JBL_VAR_SCANNER_KEY_CHAR					:__jbl_log_parameter[__jbl_log_parameter_cnt].c=va_arg(arg_ptr,int)			;++__jbl_log_parameter_cnt	;break;//类型提升
			case JBL_VAR_SCANNER_KEY_CHARS					:__jbl_log_parameter[__jbl_log_parameter_cnt].s=va_arg(arg_ptr,char *)		;++__jbl_log_parameter_cnt	;break;
			case JBL_VAR_SCANNER_KEY_HEX					:__jbl_log_parameter[__jbl_log_parameter_cnt].u=va_arg(arg_ptr,jbl_uint64)	;++__jbl_log_parameter_cnt	;break;
			case JBL_VAR_SCANNER_KEY_VAR					:__jbl_log_parameter[__jbl_log_parameter_cnt].v=jbl_var_copy(va_arg(arg_ptr,jbl_var*))	;++__jbl_log_parameter_cnt	;break;
		}
	}
	finish:;
	va_end(arg_ptr); 
#if JBL_TIME_ENABLE==1
	jbl_time_now(&__jbl_logs[__jbl_logs_cnt].t);
#endif	
	++__jbl_logs_cnt;
	if((__jbl_logs_cnt+10)>=JBL_LOG_MAX_LENGTH)
		jbl_log_save();
	else if((__jbl_log_parameter_cnt+40)>=JBL_LOG_MAX_LENGTH*4)
		jbl_log_save();
}
#include <stdio.h>


void jbl_log_save()
{
	if((__jbl_logs_start&0x02))return;
	__jbl_logs_start|=0x02;
	jbl_log(UC "Saving logs");
#if JBL_STREAM_ENABLE==1
	jbl_stream*out=jbl_stream_new(&jbl_stream_file_operators,fopen(JBL_LOG_DIR,"ab"),8192-jbl_stream_caculate_size(0),NULL,0);	
	jbl_uint32 j=0;
	for(jbl_uint32 i=0;i<__jbl_logs_cnt;++i)
	{
#if JBL_TIME_ENABLE==1
		jbl_stream_push_char(out,'[');
		jbl_stream_push_time(out,&__jbl_logs[i].t,UC"Y-m-d H:i:s.u");
		jbl_stream_push_char(out,']');
		jbl_stream_push_char(out,'\t');
#endif	
		jbl_stream_push_chars(out,UC "At:"  );jbl_stream_push_chars(out,UC __jbl_logs[i].file);jbl_stream_push_char(out,'\t');
		jbl_stream_push_chars(out,UC "Line:");jbl_stream_push_uint (out,   __jbl_logs[i].line);jbl_stream_push_char(out,'\t');
		jbl_stream_push_chars(out,UC "Func:");jbl_stream_push_chars(out,UC __jbl_logs[i].func);jbl_stream_push_char(out,'\t');
		
		for(unsigned char *s=__jbl_logs[i].chars;*s;)
		{
			jbl_var_scanner_key key=jbl_var_scanner(s,&s);
			switch(key)
			{
				case JBL_VAR_SCANNER_KEY_UNDEFINED				:jbl_stream_push_char(out,*(s-1))										;break;
				case JBL_VAR_SCANNER_KEY_END					:goto finish															;break;
				case JBL_VAR_SCANNER_KEY_INT					:jbl_stream_push_int(out,__jbl_log_parameter[j].i)				;++j	;break;
				case JBL_VAR_SCANNER_KEY_UINT					:jbl_stream_push_uint(out,__jbl_log_parameter[j].u)				;++j	;break;
				case JBL_VAR_SCANNER_KEY_DOUBLE					:jbl_stream_push_double(out,__jbl_log_parameter[j].d)			;++j	;break;
				case JBL_VAR_SCANNER_KEY_CHAR					:jbl_stream_push_char(out,__jbl_log_parameter[j].c)				;++j	;break;
				case JBL_VAR_SCANNER_KEY_CHARS					:jbl_stream_push_chars(out,UC __jbl_log_parameter[j].s)			;++j	;break;
				case JBL_VAR_SCANNER_KEY_HEX					:jbl_stream_push_hex(out,__jbl_log_parameter[j].u)				;++j	;break;
				case JBL_VAR_SCANNER_KEY_VAR					:jbl_stream_push_char(out,'\n');jbl_var_view_put(__jbl_log_parameter[j].v,out,1,JBL_VIEW_DEFAULT_TABS,0,NULL,NULL,NULL)	;__jbl_log_parameter[j].v=jbl_var_free(__jbl_log_parameter[j].v)	;++j	;break;
			}
		}
		finish:;
		
		
		jbl_stream_push_char(out,'\n');
	}
	jbl_stream_do(out,jbl_stream_force);	
	out=jbl_stream_free(out);
#endif	
	__jbl_logs_start&=(~0x02);
	__jbl_logs_cnt=0;
	__jbl_log_parameter_cnt=0;
}

#endif
