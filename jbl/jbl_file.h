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
#include "jbl_string.h"
#include "jbl_var.h"
#include "jbl_ying.h"
#include "jbl_time.h"
#include "jbl_ht.h"
#include <dirent.h>
#define JBL_FILE_TYPE_UNKNOW	0
#define JBL_FILE_TYPE_FILE	1
#define JBL_FILE_TYPE_DIR	2
typedef struct __jbl_file
{
	jbl_uint8 type:2;
	jbl_uint8 light_copy:1;
	jbl_string name;
	struct __jbl_file *f;
	union
	{
		struct
		{
			FILE *handle;
			jbl_uint64 size;
			jbl_time last_modify;
		}file;
		struct
		{
			jbl_ht child;
			
		}dir;
	};
}jbl_file;
void	jbl_file_init					(jbl_file *this);
void	jbl_file_clear					(jbl_file *this);
void	jbl_file_free					(jbl_file *this);
void	jbl_file_copy					(jbl_file *this,jbl_file *that,jbl_copy_type cpt);
#define	jbl_file_equal(x,y)				jbl_file_copy(x,y,copy)
#define	jbl_file_equal_light(x,y)		jbl_file_copy(x,y,light)
#define	jbl_file_equal_light_copy(x,y)	jbl_file_copy(x,y,move)
char	jbl_file_space_ship				(const jbl_file *this,const jbl_file *that);
#define	jbl_file_view(x,y)			 	jbl_file_view_ex(x,y,#x " @ "__FILE__,__LINE__,JBL_VIEW_DEFAULT_TABS)
void 	jbl_file_view_ex					(const jbl_file *this,FILE * file,char*str,int a,int tabs);
#define	jbl_file_open(x,y,z)				jbl_file_file_open_ex(x,NULL,y,z,-1)
void	jbl_file_file_open_ex			(jbl_file *this,jbl_file *f,jbl_string *name,jbl_copy_type ncpt,jbl_uint32 recursive_time);



#define jbl_var_get_file(this)			((jbl_file*)(this)->data.p)

#endif
#endif
