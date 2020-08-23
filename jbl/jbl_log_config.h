/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_LOG_CONFIG_H
#define __JBL_LOG_CONFIG_H
#include "jbl_config.h"
#if JBL_LOG_ENABLE==1
#include "jbl_ying.h"
#define JBL_LOG_MAX_LENGTH 1024
#define JBL_LOG_DIR "jbl.log"
#define JBL_LOG_DISABLE_STREAM_CACHE 1


#endif
#endif
