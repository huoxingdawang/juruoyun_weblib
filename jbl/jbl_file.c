/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_file.h"
#if JBL_FILE_ENABLE==1
#include <sys/stat.h>
#include "jbl_log.h"
#include "jbl_string.h"
#include "jbl_string_cc.h"
#include "jbl_file_ct.h"
#include <errno.h>
#include <stdio.h>
jbl_var_operators_new(jbl_file_operators,jbl_file_free,jbl_file_copy,NULL,NULL,jbl_file_view_put,NULL);
jbl_file * jbl_file_new()
{
	jbl_file * this=jbl_malloc(sizeof(jbl_file));
	jbl_gc_init(this);
	jbl_gc_plus(this);
	jbl_var_set_operators(this,&jbl_file_operators);
	this->dir					=NULL;
	this->handle				=NULL;
	this->type					=JBL_FILE_CLOSE;
	this->status.size			=0;
	this->ctid					=JBL_FILE_CT_UNKNOW;
#if JBL_TIME_ENABLE==1
	this->status.time_access	=NULL;
	this->status.time_modify	=NULL;
	this->status.time_creat		=NULL;
#endif
#if JBL_FILE_CACHE_GB2312NAME==1 && defined(_WIN32)
	this->dir_gb2312			=NULL;
#endif
	return this;
}
jbl_file* jbl_file_free(jbl_file *this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this))
			jbl_file_free((jbl_file*)(((jbl_reference*)this)->ptr));
		else
			this=jbl_file_close(this);
		jbl_free(this);
	}
	return NULL;
}
JBL_INLINE jbl_file *jbl_file_copy(jbl_file *that)
{
	if(!that)return NULL;
	jbl_gc_plus(that);
	return that;
}
jbl_file* jbl_file_close(jbl_file *this)
{
	if(!this)jbl_exception("NULL POINTER");
	jbl_file *thi=jbl_refer_pull(this);	
	thi->dir					=jbl_string_free(thi->dir);
	thi->status.size			=0;
	thi->ctid					=JBL_FILE_CT_UNKNOW;
	if(thi->handle)				fclose(thi->handle);
	thi->handle					=NULL;
	thi->type					=JBL_FILE_CLOSE;
#if JBL_TIME_ENABLE==1
	thi->status.time_access		=jbl_time_free(thi->status.time_access);
	thi->status.time_modify		=jbl_time_free(thi->status.time_modify);
	thi->status.time_creat		=jbl_time_free(thi->status.time_creat);
#endif	
#if JBL_FILE_CACHE_GB2312NAME==1 && defined(_WIN32)
	thi->dir_gb2312				=jbl_string_free(thi->dir_gb2312);
#endif
	return this;
}
jbl_file* jbl_file_open(jbl_file *this,jbl_string * dir,jbl_file_handle_type type)
{
	if(!this)this=jbl_file_new();
	jbl_file *thi=jbl_refer_pull(this);
	if(thi->dir)thi	=jbl_refer_pull(this=jbl_file_close(this));
	thi->dir		=jbl_string_copy(dir);
	thi->dir		=jbl_string_set_tail(thi->dir);	
	thi->ctid		=jbl_file_get_ctid_by_name(thi->dir);
#if defined(_WIN32) && JBL_FILE_CACHE_GB2312NAME==1
	thi->dir_gb2312			=jbl_string_to_gb2312_from_utf8(NULL,thi->dir);
	thi->dir_gb2312			=jbl_string_set_tail(thi->dir_gb2312);
#endif
	thi=jbl_file_change_handle(thi,type);
	return this;
}
JBL_INLINE jbl_file* jbl_file_open_chars(jbl_file *this,unsigned char * dir,jbl_file_handle_type type)
{
	return jbl_file_open(this,jbl_gc_minus(jbl_string_add_const(NULL,dir)),type);
}
jbl_file* jbl_file_change_handle(jbl_file *this,jbl_file_handle_type type)
{
	if(!this)jbl_exception("NULL POINTER");
	jbl_file *thi=jbl_refer_pull(this);
	if(type!=thi->type&&thi->dir&&thi->type<=JBL_FILE_WRITE)
	{
		if(thi->handle||this->type!=JBL_FILE_CLOSE)fclose(thi->handle),thi->handle=NULL,this->type=JBL_FILE_CLOSE;
#ifdef _WIN32
#if JBL_FILE_CACHE_GB2312NAME==1
		jbl_string * gb2312name	=jbl_string_copy(thi->dir_gb2312);
#else
		jbl_string * gb2312name	=jbl_string_to_gb2312_from_utf8(NULL,thi->dir);//windows的系统要求gb2312
		gb2312name				=jbl_string_set_tail(gb2312name);
#endif
		#define name gb2312name
#elif defined(__APPLE__) || defined(__linux__)
		#define name thi->dir
#endif	
		switch(thi->type=type)
		{
			case JBL_FILE_CLOSE	:break;
			case JBL_FILE_READ	:thi->handle=fopen((char*)jbl_string_get_chars(name),"rb");break;
			case JBL_FILE_WRITE	:thi->handle=fopen((char*)jbl_string_get_chars(name),"wb");break;
			case JBL_FILE_RW	:thi->handle=fopen((char*)jbl_string_get_chars(name),"rb+");break;
			case JBL_FILE_WR	:thi->handle=fopen((char*)jbl_string_get_chars(name),"wb+");break;
		}
#undef name
#ifdef _WIN32
		gb2312name=jbl_string_free(gb2312name);	
#endif
	}
	if(thi->handle)
		thi=jbl_file_update_status(thi);
	else
	{
		jbl_log(UC"open file %j failed",thi->dir);		
		thi=jbl_file_close(thi);
	}
	return this;
}
jbl_file* jbl_file_update_status(jbl_file *this)
{
	if(!this)jbl_exception("NULL POINTER");
	jbl_file *thi=jbl_refer_pull(this);
#ifdef _WIN32
	struct __stat64 buf;
#if JBL_FILE_CACHE_GB2312NAME==1
	jbl_string * gb2312name	=jbl_string_copy(thi->dir_gb2312);
#else
	jbl_string * gb2312name	=jbl_string_to_gb2312_from_utf8(NULL,thi->dir);//windows的系统要求gb2312
	gb2312name				=jbl_string_set_tail(gb2312name);
#endif
	if(_stat64((char*)jbl_string_get_chars(gb2312name),&buf))
	{
		thi->dir=jbl_string_free(thi->dir);
		gb2312name=jbl_string_free(gb2312name);	
		jbl_log(UC"open file %j failed with errno %d",thi->dir,errno);
		return this;
	}
	gb2312name=jbl_string_free(gb2312name);	
#elif defined(__APPLE__) || defined(__linux__)
	struct stat buf;
	if(stat((char*)jbl_string_get_chars(thi->dir),&buf))
	{
		jbl_log(UC"open file %j failed with errno %d",thi->dir,errno);
		thi->dir=jbl_string_free(thi->dir);
		return this;
	}
#endif
	thi->status.size		=buf.st_size;
#if JBL_TIME_ENABLE==1
	thi->status.time_access	=jbl_time_set(thi->status.time_access,buf.st_atime*1000);
	thi->status.time_modify	=jbl_time_set(thi->status.time_modify,buf.st_mtime*1000);
	thi->status.time_creat	=jbl_time_set(thi->status.time_creat ,buf.st_ctime*1000);
#endif
	return this;
}
jbl_string * jbl_file_read(jbl_file * this,jbl_string*res,jbl_uint64 start,jbl_uint64 end)
{
	if(!this)jbl_exception("NULL POINTER");
	this=jbl_file_change_handle(this,JBL_FILE_READ);
	jbl_file *thi=jbl_refer_pull(this);
	if(thi->type==JBL_FILE_WRITE||thi->type==JBL_FILE_CLOSE)jbl_exception("Unreadable file");
	jbl_min_update(end,thi->status.size);
	if(start==-1)
		start=0;
	else
	{
		jbl_max_update(end,start);
		jbl_min_update(start,end);
		thi=jbl_file_set_offset(thi,start);
	}
	jbl_string *re;res=jbl_string_extend_to(res,end-start,1,&re);jbl_string_hash_clear(re);
	re->len+=fread(re->s+re->len,1,end-start,thi->handle);
	return res;
}
jbl_file * jbl_file_write(jbl_file * this,jbl_string*out)
{
	if(!this)jbl_exception("NULL POINTER");
	this=jbl_file_change_handle(this,JBL_FILE_WRITE);
	jbl_file *thi=jbl_refer_pull(this);
	if(thi->type==JBL_FILE_READ||thi->type==JBL_FILE_CLOSE)jbl_exception("Unwriteable file");
	out=jbl_refer_pull(out);
	fwrite(out->s,1,out->len,this->handle);
	return this;
}
JBL_INLINE jbl_file * jbl_file_set_offset(jbl_file * this,jbl_uint64 start)
{
	if(!this)jbl_exception("NULL POINTER");
	jbl_file *thi=jbl_refer_pull(this);	
	jbl_min_update(start,thi->status.size);
#ifdef _WIN64
	_fseeki64(thi->handle,start,SEEK_SET);
#else
	fseek(thi->handle,start,SEEK_SET);
#endif	
	return this;
}
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现文件的浏览操作                               */
/*******************************************************************************************/
jbl_file* jbl_file_view_put(jbl_file* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jbl_file *thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jbl_file",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_char(out,'\n');
	++tabs;
	char * type		[]={"close","read","write","RW","WR"};
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j){}	jbl_stream_push_chars(out,UC"handle type  :");jbl_stream_push_chars(out,UC type[thi->type])								;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j){}	jbl_stream_push_chars(out,UC"dir          :");jbl_stream_push_string(out,thi->dir)										;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j){}	jbl_stream_push_chars(out,UC"size         :");jbl_stream_push_uint(out,thi->status.size)								;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j){}	jbl_stream_push_chars(out,UC"content type :");jbl_stream_push_chars(out,jbl_file_get_ct_chars_by_ctid(thi->ctid))		;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j){}	jbl_stream_push_chars(out,UC"suffix       :");jbl_stream_push_chars(out,jbl_file_get_suffix_chars_by_ctid(thi->ctid))	;jbl_stream_push_char(out,'\n');
#if JBL_TIME_ENABLE==1
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j){}	jbl_stream_push_chars(out,UC"time_creat   :");jbl_stream_push_time(out,thi->status.time_creat ,UC"Y-m-d H:i:s.u")		;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j){}	jbl_stream_push_chars(out,UC"time_modify  :");jbl_stream_push_time(out,thi->status.time_modify,UC"Y-m-d H:i:s.u")		;jbl_stream_push_char(out,'\n');
	for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j){}	jbl_stream_push_chars(out,UC"time_access  :");jbl_stream_push_time(out,thi->status.time_access,UC"Y-m-d H:i:s.u")		;jbl_stream_push_char(out,'\n');
#endif

	return this;
}
jbl_stream * jbl_file_stream_new(jbl_file *file)
{
	jbl_stream* this=jbl_stream_new(&jbl_file_stream_operators,(file),JBL_FILE_STREAM_BUF_LENGTH,NULL,2);
	this->extra[0].u=0;
	this->extra[1].u=-1;
	return this;
}

void __jbl_file_stream_operator(jbl_stream* this,jbl_uint8 flags)
{
	this=jbl_refer_pull(this);
	jbl_stream* nxt=jbl_refer_pull(this->nxt);
	jbl_file *file=jbl_refer_pull((jbl_file*)this->data);
	if(this->en)
	{
		file=jbl_file_change_handle(file,JBL_FILE_WRITE);
		if(file->type==JBL_FILE_READ||file->type==JBL_FILE_CLOSE)jbl_exception("Unwriteable file");
		fwrite(this->buf,1,this->en,file->handle);
		this->en=0;
	}
	else if(nxt)
	{
		file=jbl_file_change_handle(file,JBL_FILE_READ);
		if(file->type==JBL_FILE_WRITE||file->type==JBL_FILE_CLOSE)jbl_exception("Unreadable file");
		file=jbl_file_set_offset(file,this->extra[0].u);
		jbl_min_update(this->extra[1].u,file->status.size);
		while(this->extra[0].u<this->extra[1].u)
		{
			jbl_stream_buf_size_type len=jbl_min((this->extra[1].u-this->extra[0].u),(nxt->size-nxt->en));
			len=fread((char*)nxt->buf+nxt->en,1,len,file->handle);
			nxt->en+=len;this->extra[0].u+=len;
			jbl_stream_do(nxt,0);
			if(nxt->stop)return;
		}
		jbl_stream_do(nxt,flags);	
	}
}
jbl_stream_operators_new(jbl_file_stream_operators,__jbl_file_stream_operator,jbl_file_free,NULL);
#endif
#endif
