/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_HT_CONFIG_H
#define __JBL_HT_CONFIG_H
#include "jbl_config.h"
#if JBL_HT_ENABLE==1
#include "jbl_ying.h"
#if __SIZEOF_POINTER__ == 8
	typedef jbl_uint64 			jbl_ht_size_type;
	typedef jbl_int64 			jbl_ht_size_type_signed;
#else
	typedef jbl_uint32 			jbl_ht_size_type;
	typedef jbl_int32 			jbl_ht_size_type_signed;
#endif

#define JBL_HT_MIN_SIZE			8
#define JBL_HT_SYS_ENABLE		1

#endif
#endif