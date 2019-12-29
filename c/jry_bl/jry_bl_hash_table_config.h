/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_HASH_TABLE_CONFIG_H
#define __JRY_BL_HASH_TABLE_CONFIG_H
#include "jry_bl_config.h"
#if JRY_BL_HASH_TABLE_ENABLE==1
#include "jry_bl_ying.h"
typedef jry_bl_uint64 jry_bl_hash_table_size_type;
#define JRY_BL_HASH_TABLE_MIN_SIZE 8

#endif
#endif