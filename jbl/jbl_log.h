/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_LOG_H
#define __JBL_LOG_H
#include "jbl_log_config.h"
#if JBL_LOG_ENABLE==1
#include "jbl_string.h"
#include "jbl_time.h"

typedef struct __jbl_log_struct
{
	const char * file;
	const char * func;
	jbl_uint32 line;
	jbl_uint8 type;
#if JBL_TIME_ENABLE==1
	jbl_time t;
#endif
	union
	{	
#if JBL_STRING_ENABLE==1
		jbl_string *s;
#endif
		char * chars;
	};
}jbl_log_struct;
void	jbl_log_start	();
void	jbl_log_stop	();
void	jbl_log_save	();
void	jbl_log_add_log	(const char * file,const char * func,jbl_uint32 line,jbl_uint8 type,void *s);
#define	jbl_log(s)		jbl_log_add_log(__FILE__,__FUNCTION__,__LINE__,0,s)
#define	jbl_logstr(s)	jbl_log_add_log(__FILE__,__FUNCTION__,__LINE__,1,s)




#else
#define	jbl_log_start()
#define	jbl_log_stop()
#define	jbl_log(s)
#define	jbl_logstr(s)

#endif
#endif
