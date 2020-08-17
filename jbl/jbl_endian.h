/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_ENDIAN_H
#define __JBL_ENDIAN_H
#include "jbl_endian_config.h"
#if JBL_ENDIAN_ENABLE==1
#include "jbl_ying.h"
void	jbl_endian_copy_uint64(void* in,void* out);
void	jbl_endian_copy_uint32(void* in,void* out);
void	jbl_endian_copy_uint16(void* in,void* out);
void	jbl_endian_swap_uint64(void* in,void* out);
void	jbl_endian_swap_uint32(void* in,void* out);
void	jbl_endian_swap_uint16(void* in,void* out);
#if __BYTE_ORDER__ ==__ORDER_BIG_ENDIAN__
#define jbl_endian_to_big_uint64(x,y)		jbl_endian_copy_uint64(x,y)
#define jbl_endian_to_big_uint32(x,y)		jbl_endian_copy_uint32(x,y)
#define jbl_endian_to_big_uint16(x,y)		jbl_endian_copy_uint16(x,y)
#define jbl_endian_to_small_uint64(x,y)		jbl_endian_swap_uint64(x,y)
#define jbl_endian_to_small_uint32(x,y)		jbl_endian_swap_uint32(x,y)
#define jbl_endian_to_small_uint16(x,y)		jbl_endian_swap_uint16(x,y)

#define jbl_endian_from_big_uint64(x,y)		jbl_endian_copy_uint64(x,y)
#define jbl_endian_from_big_uint32(x,y)		jbl_endian_copy_uint32(x,y)
#define jbl_endian_from_big_uint16(x,y)		jbl_endian_copy_uint16(x,y)
#define jbl_endian_from_small_uint64(x,y)	jbl_endian_swap_uint64(x,y)
#define jbl_endian_from_small_uint32(x,y)	jbl_endian_swap_uint32(x,y)
#define jbl_endian_from_small_uint16(x,y)	jbl_endian_swap_uint16(x,y)

#else
#define jbl_endian_to_big_uint64(x,y)		jbl_endian_swap_uint64(x,y)
#define jbl_endian_to_big_uint32(x,y)		jbl_endian_swap_uint32(x,y)
#define jbl_endian_to_big_uint16(x,y)		jbl_endian_swap_uint16(x,y)
#define jbl_endian_to_small_uint64(x,y)		jbl_endian_copy_uint64(x,y)
#define jbl_endian_to_small_uint32(x,y)		jbl_endian_copy_uint32(x,y)
#define jbl_endian_to_small_uint16(x,y)		jbl_endian_copy_uint16(x,y)

#define jbl_endian_from_big_uint64(x,y)		jbl_endian_swap_uint64(x,y)
#define jbl_endian_from_big_uint32(x,y)		jbl_endian_swap_uint32(x,y)
#define jbl_endian_from_big_uint16(x,y)		jbl_endian_swap_uint16(x,y)
#define jbl_endian_from_small_uint64(x,y)	jbl_endian_copy_uint64(x,y)
#define jbl_endian_from_small_uint32(x,y)	jbl_endian_copy_uint32(x,y)
#define jbl_endian_from_small_uint16(x,y)	jbl_endian_copy_uint16(x,y)

#endif



#endif
#endif
