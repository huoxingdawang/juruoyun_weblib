/* Copyright (c) [2020] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JWL_DNS_H
#define __JWL_DNS_H
#include "jwl_dns_config.h"
#if JWL_DNS_ENABLE==1
typedef enum
{
	JWB_DNS_QUERY_TYPE_A		=1,
//	JWB_DNS_QUERY_TYPE_NS		=2,
//	JWB_DNS_QUERY_TYPE_CNAME	=5,
//	JWB_DNS_QUERY_TYPE_SOA		=6,
//	JWB_DNS_QUERY_TYPE_WKS		=11,
//	JWB_DNS_QUERY_TYPE_PTR		=12,
//	JWB_DNS_QUERY_TYPE_HINFO	=13,
//	JWB_DNS_QUERY_TYPE_MX		=15,
//	JWB_DNS_QUERY_TYPE_AAAA		=28,
//	JWB_DNS_QUERY_TYPE_AXFR		=252,
//	JWB_DNS_QUERY_TYPE_ANY		=255,
}JWB_DNS_QUERY_TYPE;

jbl_uint32 jwl_dns_get_ip_by_domin_chars(jbl_uint8 *domin,int query_type);



#endif
#endif
