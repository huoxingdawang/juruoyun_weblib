/* Copyright (c) [2020] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jwl_dns.h"
#if JWL_DNS_ENABLE==1
#include "jwl_socket.h"
#include "jwl_ying.h"
struct __jwl_dns_header
{
	jbl_uint16	id			;	//会话标识
	jbl_uint8	rd		:1	;	//表示期望递归
	jbl_uint8	tc		:1	;	//表示可截断的 
	jbl_uint8	aa		:1	;	//表示授权回答
	jbl_uint8	opcode	:4	;
	jbl_uint8	qr		:1	;	//查询/响应标志，0为查询，1为响应
	jbl_uint8	rcode	:4	;	//应答码
	jbl_uint8	cd		:1	;
	jbl_uint8	ad		:1	;
	jbl_uint8	z		:1	;	//保留值
	jbl_uint8	ra		:1	;	//表示可用递归
	jbl_uint16	q_count		;	//表示查询问题区域节的数量 
	jbl_uint16	ans_count	;	//表示回答区域的数量
	jbl_uint16	auth_count	;	//表示授权区域的数量
	jbl_uint16	add_count	;	//表示附加区域的数量
};
struct __jwl_dns_question
{
	jbl_uint16	qtype		;//查询类型
	jbl_uint16	qclass		;//查询类
};
#pragma pack(push, 1)
struct __jwl_dns_receive_data
{
	jbl_uint16	name;
	jbl_uint16	type;			//资源记录的类型
	jbl_uint16	cla;			//类
	jbl_uint32	ttl;			//表示资源记录可以缓存的时间
	jbl_uint16	len;			//数据长度
};
#pragma pack(pop)				//恢复对齐状态
struct __jwl_dns_receive_record
{
	jbl_uint8     *	name;		//资源记录包含的域名
	struct __jwl_dns_receive_data *	resource;	//资源数据
	jbl_uint8     *	rdata;
};

jbl_uint64 jwl_dns_get_ip_domin_chars(jbl_uint8 *domin,int query_type)
{
	jbl_uint8 *buf=jbl_malloc(65536); 
	struct __jwl_dns_header *dns=(struct __jwl_dns_header *)buf;
	dns->qr			=0;	//查询
	dns->opcode		=0;	//标准查询
	dns->aa			=0;	//不授权回答
	dns->tc			=0;	//不可截断
	dns->rd			=1;	//期望递归
	dns->ra			=0;	//不可用递归
	dns->z			=0;	//必须为0
	dns->ad			=0;
	dns->cd			=0;
	dns->rcode		=0;	//没有差错
	dns->ans_count	=0; 
	dns->auth_count	=0;
	dns->add_count	=0;
	jbl_uint16 tmp;
	tmp=1;jbl_endian_to_big_uint16(&tmp,&dns->q_count);
	tmp=1;jbl_endian_to_big_uint16(&tmp,&dns->id);
	jbl_uint8 *qname=buf+sizeof(struct __jwl_dns_header);
	jbl_uint32 len=0;
	for(int i=0,j=0;;++i)
		if(domin[i]=='.'||(!domin[i]))
		{
			for(qname[len]=i-j,++len;j<i;qname[len]=domin[j],++j,++len);
			j=i+1;
			if(!domin[i])break;
		}
	++len,qname[len]=0;	
	tmp=1;jbl_endian_to_big_uint16(&tmp,buf+sizeof(struct __jwl_dns_header)+len);
	tmp=query_type;jbl_endian_to_big_uint16(&tmp,buf+sizeof(struct __jwl_dns_header)+len+2);
	jwl_socket * socket=jwl_socket_connect(NULL,jwl_get_binary_ip_chars(UC"10.200.0.1"),53,JWL_SOCKET_MODE_UDP);
	jwl_socket_send_chars(socket,buf,sizeof(struct __jwl_dns_header)+len+sizeof(struct __jwl_dns_question));
	jwl_socket_receive_chars(socket,buf,65536);
	socket=jwl_socket_free(socket);
	dns=(struct __jwl_dns_header*)buf;
	jbl_uint8 *reader=buf+sizeof(struct __jwl_dns_header)+len+sizeof(struct __jwl_dns_question);

	jbl_uint16 ans_cnt=0;jbl_endian_from_big_uint16(&dns->ans_count,&ans_cnt);
//	printf("%d个回答\n",ans_cnt);
	jbl_uint64 ip=0;
	for(int i=0;i<ans_cnt;++i)
	{
		struct __jwl_dns_receive_data *	resource=(struct __jwl_dns_receive_data *)reader;
		jbl_endian_to_big_uint16(&resource->type,&tmp);resource->type=tmp;
		jbl_endian_to_big_uint16(&resource->cla ,&tmp);resource->cla =tmp;
		jbl_endian_to_big_uint16(&resource->ttl ,&tmp);resource->ttl =tmp;
		jbl_endian_to_big_uint16(&resource->len ,&tmp);resource->len =tmp;
//		printf("type %d\tcla  %d\tttl  %d\tlen  %d\n",resource->type,resource->cla,resource->ttl,resource->len);
		reader+=sizeof(struct __jwl_dns_receive_data);
		if(resource->type==JWB_DNS_QUERY_TYPE_A)	{ip=*((jbl_uint64*)reader);break;}
		reader+=resource->len;
	}
	jbl_free(buf);
	return ip;
}
#endif
