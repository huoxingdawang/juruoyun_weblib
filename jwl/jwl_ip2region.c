/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
	  http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
//本模块基于开源项目ip2region(https://gitee.com/lionsoul/ip2region/)魔改而成
#include "jwl_ip2region.h"
#if JWL_IP2REGION_ENABLE==1
#define JWL_IP2REGION_DB_INDEX_BLOCK_LENGTH 12
#include<stdio.h>
struct
{
	jbl_string * db;
	jbl_uint32 firstindexptr;	//first index ptr
	jbl_uint32 lastindexptr;	//last index ptr
	jbl_uint32 totalblocks;		//total index blocks number	
	
}__jwl_ip2region_data;
void jwl_ip2region_start()
{
	jbl_file * f1=jbl_file_open_chars(NULL,UC JWL_IP2REGION_DB_DIR,JBL_FILE_READ);
	__jwl_ip2region_data.db=jbl_file_read(f1,NULL,0,-1);
	jbl_file_free(f1);	
	
	jbl_uint8* buf=__jwl_ip2region_data.db->s;
	jbl_endian_copy_uint32(buf,&__jwl_ip2region_data.firstindexptr);
	jbl_endian_copy_uint32(buf+4,&__jwl_ip2region_data.lastindexptr);
	__jwl_ip2region_data.totalblocks   =(__jwl_ip2region_data.lastindexptr-__jwl_ip2region_data.firstindexptr)/JWL_IP2REGION_DB_INDEX_BLOCK_LENGTH+1;
//	jbl_log(UC"ip2region database loaded successfully first index ptr:%d,last index ptr:%d,total blocks:%d",__jwl_ip2region_data.firstindexptr,__jwl_ip2region_data.lastindexptr,__jwl_ip2region_data.totalblocks);
}
void jwl_ip2region_stop()
{
	__jwl_ip2region_data.db=jbl_string_free(__jwl_ip2region_data.db);
}

jbl_string* jwl_ip2region(jbl_uint32 ip_big,jbl_string*result)
{
	jbl_uint32 ip;
	jbl_endian_swap_uint32(&ip_big,&ip);//ip2region的数据库ip是小端序,jwl_ying处理出的ip默认是大端序
	
	jbl_uint32 dptr=0;
	for(jbl_uint32 l=0,h=__jwl_ip2region_data.totalblocks;l<=h;)
	{
		jbl_uint32 m=(l+h)>>1;
		jbl_uint8 *buf=__jwl_ip2region_data.db->s+__jwl_ip2region_data.firstindexptr+m*JWL_IP2REGION_DB_INDEX_BLOCK_LENGTH;
		jbl_uint32 lip,hip;
		jbl_endian_copy_uint32(buf,&lip);
		jbl_endian_copy_uint32(buf+4,&hip);
		if(ip<lip)
			h=m-1;
		else
			if(ip>hip)
				l=m+1;
			else
				{jbl_endian_copy_uint32(buf+8,&dptr);break;}
	}
	if(!dptr)return result;
	jbl_uint32 datalen = ((dptr >> 24) & 0xFF);
	jbl_uint8 * buf=__jwl_ip2region_data.db->s+(dptr&0x00FFFFFF);

//	city_id = jbl_getuint32_raw(buf);
	result=jbl_string_add_chars_length(result,buf+4,datalen-4);

	return result;
}
#endif
