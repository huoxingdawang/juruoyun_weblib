/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_endian.h"
#if JBL_ENDIAN_ENABLE==1
void jbl_endian_copy_uint64(void* in,void* out){((jbl_uint8*)out)[0]=((jbl_uint8*)in)[0];((jbl_uint8*)out)[1]=((jbl_uint8*)in)[1];((jbl_uint8*)out)[2]=((jbl_uint8*)in)[2];((jbl_uint8*)out)[3]=((jbl_uint8*)in)[3];((jbl_uint8*)out)[4]=((jbl_uint8*)in)[4];((jbl_uint8*)out)[5]=((jbl_uint8*)in)[5];((jbl_uint8*)out)[6]=((jbl_uint8*)in)[6];((jbl_uint8*)out)[7]=((jbl_uint8*)in)[7];}
void jbl_endian_copy_uint32(void* in,void* out){((jbl_uint8*)out)[0]=((jbl_uint8*)in)[0];((jbl_uint8*)out)[1]=((jbl_uint8*)in)[1];((jbl_uint8*)out)[2]=((jbl_uint8*)in)[2];((jbl_uint8*)out)[3]=((jbl_uint8*)in)[3];}
void jbl_endian_copy_uint16(void* in,void* out){((jbl_uint8*)out)[0]=((jbl_uint8*)in)[0];((jbl_uint8*)out)[1]=((jbl_uint8*)in)[1];}
void jbl_endian_swap_uint64(void* in,void* out){((jbl_uint8*)out)[0]=((jbl_uint8*)in)[7];((jbl_uint8*)out)[1]=((jbl_uint8*)in)[6];((jbl_uint8*)out)[2]=((jbl_uint8*)in)[5];((jbl_uint8*)out)[3]=((jbl_uint8*)in)[4];((jbl_uint8*)out)[4]=((jbl_uint8*)in)[3];((jbl_uint8*)out)[5]=((jbl_uint8*)in)[2];((jbl_uint8*)out)[6]=((jbl_uint8*)in)[1];((jbl_uint8*)out)[7]=((jbl_uint8*)in)[0];}
void jbl_endian_swap_uint32(void* in,void* out){((jbl_uint8*)out)[0]=((jbl_uint8*)in)[3];((jbl_uint8*)out)[1]=((jbl_uint8*)in)[2];((jbl_uint8*)out)[2]=((jbl_uint8*)in)[1];((jbl_uint8*)out)[3]=((jbl_uint8*)in)[0];}
void jbl_endian_swap_uint16(void* in,void* out){((jbl_uint8*)out)[0]=((jbl_uint8*)in)[1];((jbl_uint8*)out)[1]=((jbl_uint8*)in)[0];}

#endif
