/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_BASE64_H
#define __JRY_BL_BASE64_H
#include "jry_bl_base64_config.h"
#if JRY_BL_BASE64_ENABLE==1
#include "jry_bl_exception.h"
#include "jry_bl_malloc.h"
#include "jry_bl_string.h"
#include "jry_bl_ying.h"
void jry_bl_base64_encode(const jry_bl_string *this,jry_bl_string *result);
void jry_bl_base64_decode(const jry_bl_string *this,jry_bl_string *result);
#endif
#endif