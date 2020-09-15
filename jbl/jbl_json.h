/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_JSON_H
#define __JBL_JSON_H
#include "jbl_json_config.h"
#if JBL_JSON_ENABLE==1
#include "jbl_stream.h"

#if JBL_STREAM_ENABLE==1
extern					const jbl_stream_operater			jbl_json_decode_stream_operaters;
jbl_stream *			jbl_json_decode_stream_new			();
void *					jbl_json_decode_stream_var			(jbl_stream* this);
#endif






#endif
#endif
