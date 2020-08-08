/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_STREAM_CONFIG_H
#define __JBL_STREAM_CONFIG_H
#include "jbl_config.h"
#if JBL_STREAM_ENABLE==1
#include "jbl_ying.h"

#define JBL_STREAM_EXCEED_LENGTH	64
#define JBL_VIEW_DEFAULT_TABS		1
#define JBL_VIEW_NAME_LENGTH		16
#define JBL_VIEW_DISPLAY_VARNAME	0
#define JBL_VIEW_DISPLAY_FUNC		1
#define JBL_VIEW_DISPLAY_FILE		1
#define JBL_VIEW_DISPLAY_LINE		1
typedef jbl_uint32 jbl_stream_buf_size_type;

//#define JBL_STREAM_STDOUT	fopen("tmp/out.tmp","wb")
#define JBL_STREAM_STDIN	stdin
#ifndef JBL_STREAM_STDOUT
	#define JBL_STREAM_STDOUT	stdout
#endif

#endif
#endif
