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
jbl_uint32 __jbl_logs_cnt;
jbl_uint8 __jbl_logs_start;
jbl_log_struct jbl_logs[JBL_LOG_MAX_LENGTH];
void jbl_log_start()
{
	__jbl_logs_cnt=0;
	__jbl_logs_start=1;
	fclose(fopen(JBL_LOG_DIR,"wb"));
#if JBL_TIME_ENABLE==1
	for(jbl_uint32 i=0;i<JBL_LOG_MAX_LENGTH;++i)
		jbl_time_init(&jbl_logs[i].t);
#endif	
}
void jbl_log_stop()
{
	jbl_log_save();	
}

void jbl_log_add_log(const char * file,const char * func,jbl_uint32 line,jbl_uint8 type,void *s)
{
	if(!__jbl_logs_start)return;
	jbl_logs[__jbl_logs_cnt].file=file;
	jbl_logs[__jbl_logs_cnt].func=func;
	jbl_logs[__jbl_logs_cnt].line=line;
	jbl_logs[__jbl_logs_cnt].type=type;
	jbl_logs[__jbl_logs_cnt].s=s;
#if JBL_TIME_ENABLE==1
	jbl_time_now(&jbl_logs[__jbl_logs_cnt].t);
#endif	
	++__jbl_logs_cnt;
	if((__jbl_logs_cnt+10)>=JBL_LOG_MAX_LENGTH)
		jbl_log_save();
}
#include <stdio.h>
void jbl_log_save()
{
	if((__jbl_logs_start&0x02))return;
	__jbl_logs_start|=0x02;
	jbl_log("Saving logs");
#if JBL_STREAM_ENABLE==1
	jbl_stream*out=jbl_stream_new(&jbl_stream_file_operators,fopen(JBL_LOG_DIR,"ab"),8192-jbl_stream_caculate_size(0),NULL,0);	
	for(jbl_uint32 i=0;i<__jbl_logs_cnt;++i)
	{
#if JBL_TIME_ENABLE==1
		jbl_time_decoded tt;
		jbl_time_decode(&jbl_logs[i].t,&tt);
		jbl_stream_push_char(out,'[');
		jbl_stream_push_uint(out,tt.year)	;jbl_stream_push_char(out,'-');
		jbl_stream_push_uint(out,tt.month)	;jbl_stream_push_char(out,'-');
		jbl_stream_push_uint(out,tt.day)	;jbl_stream_push_char(out,' ');
		jbl_stream_push_uint(out,tt.hour)	;jbl_stream_push_char(out,':');
		jbl_stream_push_uint(out,tt.minute)	;jbl_stream_push_char(out,':');
		jbl_stream_push_uint(out,tt.second)	;jbl_stream_push_char(out,'.');
		jbl_stream_push_uint(out,tt.ms)		;jbl_stream_push_char(out,']');
		jbl_stream_push_char(out,'\t');
#endif	
		jbl_stream_push_chars(out,UC "At:"  );jbl_stream_push_chars(out,UC jbl_logs[i].file);jbl_stream_push_char(out,'\t');
		jbl_stream_push_chars(out,UC "Line:");jbl_stream_push_uint (out,   jbl_logs[i].line);jbl_stream_push_char(out,'\t');
		jbl_stream_push_chars(out,UC "Func:");jbl_stream_push_chars(out,UC jbl_logs[i].func);jbl_stream_push_char(out,'\t');
#if JBL_STRING_ENABLE==1
		if(jbl_logs[i].type)
			jbl_stream_push_string(out,jbl_logs[i].s),jbl_logs[i].s=jbl_string_free(jbl_logs[i].s);
		else
#endif	
			jbl_stream_push_chars(out,UC jbl_logs[i].chars);
		jbl_stream_push_char(out,'\n');
	}
	jbl_stream_do(out,jbl_stream_force);	
	out=jbl_stream_free(out);
#endif	
	__jbl_logs_start&=(~0x02);
	__jbl_logs_cnt=0;
}

#endif