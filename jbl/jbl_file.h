/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_FILE_H
#define __JBL_FILE_H
#include "jbl_file_config.h"
#if JBL_FILE_ENABLE==1
#include "jbl_gc.h"
#include "jbl_time.h"
#include "jbl_string.h"
#include "jbl_file_ct.h"
#include "jbl_var.h"
#include "jbl_pthread.h"
typedef enum
{
	JBL_FILE_CLOSE,
	JBL_FILE_READ,
	JBL_FILE_WRITE,
	JBL_FILE_RW,
	JBL_FILE_WR,
}jbl_file_handle_type;
typedef struct __jbl_file
{
	jbl_gc					gc;		//gc结构
	jbl_var_ops_define			;
	jbl_pthread_lock_define		;
	jbl_string *			dir;
#if JBL_FILE_CACHE_GB2312NAME==1 && defined(_WIN32)
	jbl_string *			dir_gb2312;
#endif
	void *					handle;
	jbl_file_handle_type	type;
	jbl_file_ct				ctid;
	struct
	{
		jbl_uint64			size;
#if JBL_TIME_ENABLE==1
		jbl_time *			time_access;
		jbl_time *			time_modify;
		jbl_time *			time_creat;		
#endif
	}status;
}jbl_file;
typedef struct __jbl_file jbl_file;
jbl_var_operators_extern(jbl_file_operators);
jbl_file *			jbl_file_new						();
jbl_file *			jbl_file_free						(jbl_file *this);
jbl_file *			jbl_file_copy						(jbl_file *that);
jbl_file *			jbl_file_open						(jbl_file *this,jbl_string * dir,jbl_file_handle_type type);
jbl_file *			jbl_file_open_chars					(jbl_file *this,unsigned char * dir,jbl_file_handle_type type);
jbl_file *			jbl_file_close						(jbl_file *this);
jbl_file *			jbl_file_change_handle				(jbl_file *this,jbl_file_handle_type type);
jbl_string *		jbl_file_read						(jbl_file * this,jbl_string*res,jbl_uint64 start,jbl_uint64 end);
jbl_file *			jbl_file_write						(jbl_file * this,jbl_string*out);
jbl_file *			jbl_file_set_offset					(jbl_file * this,jbl_uint64 start);
jbl_file *			jbl_file_update_status				(jbl_file *this);
#define				jbl_file_get_dir(this)				(jbl_string_copy(((jbl_file*)jbl_refer_pull(this))->dir))
#define				jbl_file_get_handle(this)			(((jbl_file*)jbl_refer_pull(this))->handle)
#define				jbl_file_get_handle_type(this)		(((jbl_file*)jbl_refer_pull(this))->type)
#define				jbl_file_get_ct(this)				(((jbl_file*)jbl_refer_pull(this))->ctid)
#define				jbl_file_get_size(this)				(((jbl_file*)jbl_refer_pull(this))->status.size)
#define				jbl_file_get_time_access(this)		(jbl_time_copy(((jbl_file*)jbl_refer_pull(this))->status.time_access))
#define				jbl_file_get_time_modify(this)		(jbl_time_copy(((jbl_file*)jbl_refer_pull(this))->status.time_modify))
#define				jbl_file_get_time_creat(this)		(jbl_time_copy(((jbl_file*)jbl_refer_pull(this))->status.time_creat))
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现问价的浏览操作                                 */
/*******************************************************************************************/
jbl_file*			jbl_file_view_put					(jbl_file* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个字符串
#define				jbl_file_view(x)					jbl_file_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个字符串
#endif
jbl_stream *		jbl_file_stream_new					(jbl_file *file);
extern const 		jbl_stream_operater					jbl_file_stream_operators;											//string的流处理器
#define				jbl_file_stream_set_offset(x,l)		(((jbl_stream*)jbl_refer_pull(x))->extra[0].u=l)
#define				jbl_file_stream_set_end(x,l)		(((jbl_stream*)jbl_refer_pull(x))->extra[1].u=l)



#endif
#endif
