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
void jbl_file_init(jbl_file *this)
{
	if(this==NULL)jbl_exception("NULL POINTER");
	this->type=JBL_FILE_TYPE_UNKNOW;
	this->f=NULL;
	this->light_copy=0;
	jbl_string_init(&this->name);
}
void jbl_file_init_as(jbl_file *this,jbl_uint8 type)
{
	if(this==NULL)jbl_exception("NULL POINTER");
	jbl_string_clear(&this->name);
	this->f=NULL;
	this->light_copy=0;
	if(this->type==type)
	{
		if(type==JBL_FILE_TYPE_FILE)
			if(this->file.handle!=NULL)
				fclose(this->file.handle),this->file.handle=NULL;		
		else if(type==JBL_FILE_TYPE_DIR)
			jbl_ht_clear(&this->dir.child);
	}
	else if(this->type==JBL_FILE_TYPE_UNKNOW)
	{
		if(type==JBL_FILE_TYPE_FILE)
			this->file.handle=NULL;
		else if(type==JBL_FILE_TYPE_DIR)
			jbl_ht_init(&this->dir.child);
		this->type=type;		
	}
	else
	{
		if(this->type==JBL_FILE_TYPE_FILE)
			if(this->file.handle!=NULL)
				fclose(this->file.handle),this->file.handle=NULL;
		else if(this->type==JBL_FILE_TYPE_DIR)
			jbl_ht_free(&this->dir.child);			
		this->type=type;		
	}
}
void jbl_file_clear(jbl_file *this)
{
	if(this==NULL)jbl_exception("NULL POINTER");
	if(this->f!=NULL)
		return;
	if(this->type==JBL_FILE_TYPE_FILE)
	{
		if(this->file.handle!=NULL)
			fclose(this->file.handle),this->file.handle=NULL;
	}
	else if(this->type==JBL_FILE_TYPE_DIR)
		jbl_ht_free(&this->dir.child);
	this->light_copy=0;
	this->type=JBL_FILE_TYPE_UNKNOW;
	jbl_string_clear(&this->name);	
}
void jbl_file_free(jbl_file *this)
{
	if(this==NULL)jbl_exception("NULL POINTER");
	if(this->type==JBL_FILE_TYPE_FILE)
	{
		if(this->file.handle!=NULL&&this->light_copy==0)
			fclose(this->file.handle),this->file.handle=NULL;
	}
	else if(this->type==JBL_FILE_TYPE_DIR)
		jbl_ht_free(&this->dir.child);	
	this->type=JBL_FILE_TYPE_UNKNOW;
	jbl_string_free(&this->name);
	this->f=NULL;
}
void jbl_file_copy(jbl_file *this,jbl_file *that,jbl_copy_type cpt)
{
	if(this==NULL||that==NULL)jbl_exception("NULL POINTER");
	jbl_file_clear(this);
	jbl_file_init_as(this,that->type);
	this->f=that->f;
	jbl_string_copy(&this->name,&that->name,cpt);
	if(that->type==JBL_FILE_TYPE_FILE)
	{
		if(cpt==copy)
			this->file.handle=fopen(jbl_string_get_chars(&that->name),"rb+"),this->light_copy=0;
		else
			this->light_copy=that->light_copy,this->file.handle=that->file.handle,((cpt==move)?that:this)->light_copy=1;
	}
	else if(that->type==JBL_FILE_TYPE_DIR)
		jbl_ht_copy(&this->dir.child,&that->dir.child,cpt);
}
inline char jbl_file_space_ship(const jbl_file *this,const jbl_file *that)
{
	if(this==NULL||that==NULL)jbl_exception("NULL POINTER");
	return jbl_string_space_ship(&this->name,&that->name);
}
void jbl_file_view_ex(const jbl_file *this,FILE * file,char*str,int a,int tabs)
{
	if(this==NULL||file==NULL||str==NULL)jbl_exception("NULL POINTER");
	if(tabs>=0)
		for(register int i=0;i<tabs;++i,putc('\t',file));
	else
		tabs=-tabs;
	if(a>=0)
		fprintf(file,"jbl_%s    %s %d:",((this->type==JBL_FILE_TYPE_FILE)?"file  ":((this->type==JBL_FILE_TYPE_DIR)?"dir   ":"filenw")),str,a);
	else
		fprintf(file,"jbl_%s     \t:",((this->type==JBL_FILE_TYPE_FILE)?"file  ":((this->type==JBL_FILE_TYPE_DIR)?"dir   ":"filenw")));
	fprintf(file,"\tname:");
	jbl_string_print(&this->name,file);
	fputc('\t',file);
	if(this->type==JBL_FILE_TYPE_FILE)
	{
		fputc('\n',file);
	}
	else if(this->type==JBL_FILE_TYPE_DIR)
	{
		fputc('\n',file);
		jbl_ht_foreach(&this->dir.child,i)
			jbl_file_view_ex(jbl_var_get_file(jbl_ht_get_var(i)),file,"",-1,tabs+1);
	}
}
void jbl_file_file_open_ex(jbl_file *this,jbl_file *f,jbl_string *name,jbl_copy_type ncpt,jbl_uint32 recursive_time)
{
	if(this==NULL||name==NULL)jbl_exception("NULL POINTER");
	jbl_file_clear(this);
	jbl_string_set(name,jbl_string_get_length(name),'\0');
	FILE	*file	=fopen(jbl_string_get_chars(name),"rb+");
	DIR		*dir	=opendir(jbl_string_get_chars(name));
	if(file!=NULL)
	{
		if(dir!=NULL)closedir(dir);	
		jbl_file_init_as(this,JBL_FILE_TYPE_FILE);
		jbl_string_copy(&this->name,name,ncpt);this->f=f;
		this->file.handle=file;
	}
	else if(dir!=NULL)
	{
		jbl_file_init_as(this,JBL_FILE_TYPE_DIR);
		jbl_string_copy(&this->name,name,ncpt);this->f=f;
		if(recursive_time>0)
		{	
			struct dirent *dirp;
			jbl_string tmp1,tmp2;jbl_string_init(&tmp1);jbl_string_init(&tmp2);jbl_string_equal_string(&tmp1,&this->name);
#ifdef __linux__
				jbl_string_add_char(&tmp1,'/');
#else
				jbl_string_add_char(&tmp1,'\\');
#endif
			jbl_string_size_type nn=jbl_string_get_length(&tmp1);
			jbl_ht_size_type cnt=0;
			jbl_var tv;jbl_var_init(&tv);
			while((dirp=readdir(dir)))++cnt;
			jbl_ht_extend(&this->dir.child,cnt);
			rewinddir(dir);
			while((dirp=readdir(dir)))
			{
				jbl_string_equal_chars(&tmp2,dirp->d_name);
				if(jbl_string_get1(&tmp2,0)=='.'||(2<jbl_string_get_length(&tmp2)&&jbl_string_get1(&tmp2,0)=='.'&&jbl_string_get1(&tmp2,1)=='.'))
					continue;
				jbl_var_init_as(&tv,JBL_VAR_TYPE_FILE);
				jbl_string_add_chars(&tmp1,dirp->d_name);
				jbl_file_file_open_ex(jbl_var_get_file(&tv),this,&tmp1,move,recursive_time-1);
				jbl_ht_insert(&this->dir.child,&tmp2,&tv,move,move);
				jbl_string_get_length(&tmp1)=nn;
			}
			jbl_var_free(&tv);
			jbl_string_free(&tmp1);
			jbl_string_free(&tmp2);
		}
		closedir(dir);
	}
	else
		jbl_string_print(name,stderr),jbl_file_clear(this)/*,jbl_exception(JBL_ERROR_FILE_NOT_EXIST)*/;
}
void jbl_file_file_cd(jbl_file *this,jbl_file *that,jbl_string *name,jbl_copy_type ncpt)
{
	
	
}

#endif
