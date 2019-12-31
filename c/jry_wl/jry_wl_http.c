/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_wl_http.h"
#if JRY_WL_HTTP_ENABLE==1
#define sg(x,y)	jry_bl_string_get2(x,y)
#define _(x)	this->status=0;	\
				this->cache=0;\
				this->cache_max_age=0;\
				jry_bl_string_##x(&this->content_type);\
				jry_bl_string_##x(&this->etag);\
				jry_bl_hash_table_##x(&this->v);
inline void jry_wl_http_response_head_init(jry_wl_http_response_head *this){_(init);}
inline void jry_wl_http_response_head_free(jry_wl_http_response_head *this){_(free);}
inline void jry_wl_http_response_head_clear(jry_wl_http_response_head *this){_(clear);}
#undef _
#define _(x)	this->method=0;	\
				this->protocol=0;\
				this->connection=0;\
				this->cache=0;\
				this->cache_max_age=0;\
				jry_bl_string_##x(&this->url);\
				jry_bl_string_##x(&this->host);\
				jry_bl_string_##x(&this->user_agent);\
				jry_bl_string_##x(&this->referer);\
				jry_bl_string_##x(&this->etag);\
				jry_bl_hash_table_##x(&this->cookie);\
				jry_bl_hash_table_##x(&this->v);
inline void jry_wl_http_request_head_init(jry_wl_http_request_head *this){_(init);}
inline void jry_wl_http_request_head_free(jry_wl_http_request_head *this){_(free);}
inline void jry_wl_http_request_head_clear(jry_wl_http_request_head *this){_(clear);}
#undef _
void jry_wl_http_send_response_head(jry_bl_socket_handle *client,jry_wl_http_response_head *head,jry_bl_string *data)
{
#define rn() jry_bl_string_add_char1(&buf,'\r');jry_bl_string_add_char1(&buf,'\n')
	if(client==NULL||head==NULL||data==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_bl_string buf;jry_bl_string_init(&buf);jry_bl_string_extend_to(&buf,JRY_WL_HTTP_MAX_HEADER_LENGTH);
	jry_bl_string_add_char_pointer(&buf,"HTTP/1.1 ");
	jry_bl_string_add_uint64(&buf,head->status);
	switch(head->status)
	{
		case 200:
			jry_bl_string_add_char_pointer(&buf,"OK");
			break;
		case 304:
			jry_bl_string_add_char_pointer(&buf,"Not Modified");
			break;
	}
	rn();
	jry_bl_string_add_char_pointer(&buf,"Server:"JRY_WL_HTTP_RESPONSE_SERVER_NAME"\r\n");
	jry_bl_string_add_char_pointer(&buf,"Accept-Ranges: bytes\r\n");
	jry_bl_string_add_char_pointer(&buf,"Content-Length: ");jry_bl_string_add_uint64(&buf,jry_bl_string_get_length(data));rn();
	jry_bl_string_add_char_pointer(&buf,"Connection: close\r\n");
	jry_bl_string_add_char_pointer(&buf,"Content-Type: ");jry_bl_string_add_string(&buf,&head->content_type);rn();
	jry_bl_string_add_char_pointer(&buf,"ETag: ");jry_bl_string_add_string(&buf,&head->etag);rn();
	jry_bl_string_add_char_pointer(&buf,"Cache-Control: ");if(head->cache==JRY_WL_HTTP_CACHE_MAX_AGE)jry_bl_string_add_char_pointer(&buf,"max-age="),jry_bl_string_add_uint64(&buf,head->cache_max_age);else jry_bl_string_add_char_pointer(&buf,"no-cache");rn();
	jry_bl_hash_table_foreach(&head->v,i)
	{
		jry_bl_string_add_string(&buf,jry_bl_hash_table_get_key(i));
		jry_bl_string_add_char1(&buf,':');jry_bl_string_add_char1(&buf,' ');
		jry_bl_string_add_var(&buf,jry_bl_hash_table_get_var(i));
		rn();
	}	
	rn();
	jry_wl_socket_send(client,&buf);
#undef rn
}
void jry_wl_http_send_response(jry_bl_socket_handle *client,jry_wl_http_response_head *head,jry_bl_string *data)
{
	if(client==NULL||head==NULL||data==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_wl_http_send_response_head(client,head,data);
	jry_wl_socket_send(client,data);
}

jry_bl_uint8 jry_wl_http_get_request_head(jry_bl_socket_handle *client,jry_wl_http_request_head *head,jry_bl_string *remain_data,jry_bl_uint8 enable_ex)
{
	if(client==NULL||head==NULL||remain_data==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_bl_string tmp1,tmp2,tmp3,tmp4,tmp5;jry_bl_string_init(&tmp1);jry_bl_string_init(&tmp2);jry_bl_string_init(&tmp3);jry_bl_string_init(&tmp4);jry_bl_string_init(&tmp5);
	jry_wl_socket_receive_length(client,&tmp1,JRY_WL_HTTP_MAX_HEADER_LENGTH);
	jry_bl_string_size_type i=0,n=jry_bl_string_get_length(&tmp1),j,ii;
	jry_bl_uint8 flag=(n==JRY_WL_HTTP_MAX_HEADER_LENGTH);
	const jry_bl_string cn=jry_bl_string_const("Connection"			,10);
	const jry_bl_string ua=jry_bl_string_const("User-Agent"			,10);
	const jry_bl_string ho=jry_bl_string_const("Host"				,4);
	const jry_bl_string re=jry_bl_string_const("Referer"			,7);
	const jry_bl_string pr=jry_bl_string_const("Pragma"				,6);
	const jry_bl_string cc=jry_bl_string_const("Cache-Control"		,13);
	const jry_bl_string co=jry_bl_string_const("Cookie"				,6);
	const jry_bl_string et=jry_bl_string_const("ETag"				,4);
	const jry_bl_string nm=jry_bl_string_const("If-None-Match"		,13);
	
	const jry_bl_string ac=jry_bl_string_const("Accept"				,6);
	const jry_bl_string ae=jry_bl_string_const("Accept-Encoding"	,15);
	const jry_bl_string al=jry_bl_string_const("Accept-Language"	,15);
	const jry_bl_string uir=jry_bl_string_const("Upgrade-Insecure-Requests",25);
method_start:
	if(((i+2)<n)&&sg(&tmp1,i+0)=='G'&&sg(&tmp1,i+1)=='E'&&sg(&tmp1,i+2)=='T')
		head->method=JRY_WL_HTTP_METHOD_GET,i+=3;
	else if(((i+3)<n)&&sg(&tmp1,i+0)=='P'&&sg(&tmp1,i+1)=='O'&&sg(&tmp1,i+2)=='S'&&sg(&tmp1,i+1)=='T')
		head->method=JRY_WL_HTTP_METHOD_GET,i+=4;
	else
		goto fail;
	++i;
url_start:
	j=jry_bl_string_find_char_start(&tmp1,' ',i);
	jry_bl_string_equal_char_pointer_length(&head->url,jry_bl_string_get_char_pointer(&tmp1)+i,j-i);
	i=j+1;
protocol_start:
	if(((i+8)<n)&&	sg(&tmp1,i+0)=='H'&&sg(&tmp1,i+1)=='T'&&sg(&tmp1,i+2)=='T'&&sg(&tmp1,i+3)=='P'&&sg(&tmp1,i+4)=='/'&&
					sg(&tmp1,i+5)=='1'&&sg(&tmp1,i+6)=='.'&&sg(&tmp1,i+7)=='1')
		head->protocol=JRY_WL_HTTP_PROTOCOL_HTTP,i+=8;
	else
		goto fail;
	i+=2;
value_start:
	while((i+4)<n&&(!(sg(&tmp1,i)=='\r'&&sg(&tmp1,i+1)=='\n'&&sg(&tmp1,i+2)=='\r'&&sg(&tmp1,i+3)=='\n')))
	{
		j=jry_bl_string_find_char_start(&tmp1,':',i);
		jry_bl_string_equal_char_pointer_length(&tmp2,jry_bl_string_get_char_pointer(&tmp1)+i,j-i);
		i=j+1;
		while(i<n&&(sg(&tmp1,i)==' '))++i;
		j=jry_bl_string_find_char_start(&tmp1,'\r',i);
		jry_bl_string_equal_char_pointer_length(&tmp3,jry_bl_string_get_char_pointer(&tmp1)+i,j-i);
		i=j+2;
		if(jry_bl_string_if_equal(&tmp2,&cn))
		{
			ii=jry_bl_string_get_length(&tmp3);
			if((9<ii)&&	sg(&tmp3,0)=='k'&&sg(&tmp3,1)=='e'&&sg(&tmp3,2)=='e'&&sg(&tmp3,3)=='p'&&sg(&tmp3,4)=='-'&&
						sg(&tmp3,5)=='a'&&sg(&tmp3,6)=='l'&&sg(&tmp3,7)=='i'&&sg(&tmp3,8)=='v'&&sg(&tmp3,9)=='e')
				head->connection=JRY_WL_HTTP_CONNECTION_KEEP_ALIVE;
			else 
				goto fail;
		}
		else if(jry_bl_string_if_equal(&tmp2,&ua))
			jry_bl_string_equal_light_move(&head->user_agent,&tmp3);
		else if(jry_bl_string_if_equal(&tmp2,&ho))
			jry_bl_string_equal_light_move(&head->host,&tmp3);
		else if(jry_bl_string_if_equal(&tmp2,&re))
			jry_bl_string_equal_light_move(&head->referer,&tmp3);
		else if(jry_bl_string_if_equal(&tmp2,&et)||jry_bl_string_if_equal(&tmp2,&nm))
			jry_bl_string_equal_light_move(&head->etag,&tmp3);		
		else if(jry_bl_string_if_equal(&tmp2,&pr)||jry_bl_string_if_equal(&tmp2,&cc))
		{
			ii=jry_bl_string_get_length(&tmp3);
			if((7<ii)&&	sg(&tmp3,0)=='n'&&sg(&tmp3,1)=='o'&&sg(&tmp3,2)=='-'&&sg(&tmp3,3)=='c'&&sg(&tmp3,4)=='a'&&
						sg(&tmp3,5)=='c'&&sg(&tmp3,6)=='h'&&sg(&tmp3,7)=='e')
				head->cache=JRY_WL_HTTP_CACHE_NO;
			else if ((7<ii)&&	sg(&tmp3,0)=='m'&&sg(&tmp3,1)=='a'&&sg(&tmp3,2)=='x'&&sg(&tmp3,3)=='-'&&sg(&tmp3,4)=='a'&&
								sg(&tmp3,5)=='g'&&sg(&tmp3,6)=='e'&&sg(&tmp3,7)=='=')
				head->cache=JRY_WL_HTTP_CACHE_MAX_AGE,head->cache_max_age=jry_bl_string_get_uint64(&tmp3);
			else 
				goto fail;			
		}	
		else if(jry_bl_string_if_equal(&tmp2,&co))
		{
			ii=jry_bl_string_get_length(&tmp3);
			jry_bl_var v;jry_bl_var_init(&v);
			for(register jry_bl_string_size_type i1=0,j1;i1<ii;i1++)
			{
				while(i1<ii&&(sg(&tmp3,i1)==' '))++i1;
				j1=jry_bl_string_find_char_start(&tmp3,'=',i1);
				jry_bl_string_equal_char_pointer_length(&tmp4,jry_bl_string_get_char_pointer(&tmp3)+i1,j1-i1);
				i1=j1+1;
				j1=jry_bl_string_find_char_start(&tmp3,';',i1);
				jry_bl_string_equal_char_pointer_length(&tmp5,jry_bl_string_get_char_pointer(&tmp3)+i1,j1-i1);
				i1=j1+1;
				jry_bl_var_equal_string_light_move(&v,&tmp5);
				jry_bl_hash_table_insert(&head->cookie,&tmp4,&v,JRY_BL_COPY_LIGHT_MOVE,JRY_BL_COPY_LIGHT_MOVE);				
			}
			jry_bl_var_free(&v);
		}
		else if(jry_bl_string_if_equal(&tmp2,&ac)||jry_bl_string_if_equal(&tmp2,&ae)||jry_bl_string_if_equal(&tmp2,&al)||jry_bl_string_if_equal(&tmp2,&uir)){}
		else if(enable_ex==true)
		{
			jry_bl_var v;jry_bl_var_init(&v);
			jry_bl_var_equal_string_light_move(&v,&tmp3);
			jry_bl_hash_table_insert(&head->v,&tmp2,&v,JRY_BL_COPY_LIGHT_MOVE,JRY_BL_COPY_LIGHT_MOVE);
			jry_bl_var_free(&v);
		}
	}
	i+=4;
	
	
	
	
	
	
	
	
	
	goto success;
fail:
	jry_wl_http_request_head_clear(head);
	i=0;
success:
	if(i<n)
		jry_bl_string_add_char_pointer_length(remain_data,jry_bl_string_get_char_pointer(&tmp1)+i,n-i);
	jry_bl_string_free(&tmp1);jry_bl_string_free(&tmp2);jry_bl_string_free(&tmp3);jry_bl_string_free(&tmp4);jry_bl_string_free(&tmp5);
	return flag;
}
#if JRY_BL_USE_STDIO==1
void jry_wl_request_head_view_ex(jry_wl_http_request_head *this,FILE * file,char*str,int a,int tabs)
{
	if(this==NULL||file==NULL||str==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(tabs>=0)
		for(register int i=0;i<tabs;++i,putc('\t',file));
	else
		tabs=-tabs;
	if(a>=0)
		fprintf(file,"jry_bl_request_head    %s %d:",str,a);
	else
		fprintf(file,"jry_bl_request_head     \t:");	
	++tabs;
	putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));	fprintf(file,"method:");	
	if(this->method==JRY_WL_HTTP_METHOD_GET)			fprintf(file,"get");
	else if(this->method==JRY_WL_HTTP_METHOD_POST)		fprintf(file,"post");
	else												fprintf(file,"unknow");
	putc('\n',file);

	for(register int i=0;i<tabs;++i,putc('\t',file));	fprintf(file,"protocol:");
	if(this->protocol==JRY_WL_HTTP_PROTOCOL_HTTP)		fprintf(file,"http");
	else if(this->protocol==JRY_WL_HTTP_PROTOCOL_HTTPS)	fprintf(file,"https");
	else												fprintf(file,"unknow");
	putc('\n',file);
	
	for(register int i=0;i<tabs;++i,putc('\t',file));		fprintf(file,"connection:");
	if(this->connection==JRY_WL_HTTP_CONNECTION_KEEP_ALIVE)	fprintf(file,"keep-alive");
	else													fprintf(file,"unknow");
	putc('\n',file);
	
	for(register int i=0;i<tabs;++i,putc('\t',file));		fprintf(file,"cache_control:");
	if(this->cache==JRY_WL_HTTP_CACHE_NO)					fprintf(file,"no");
	else if(this->cache==JRY_WL_HTTP_CACHE_MAX_AGE)			fprintf(file,"max_age:%d",this->cache_max_age);
	else													fprintf(file,"unknow");
	putc('\n',file);	
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"url:")		;jry_bl_string_print(&this->url,file)			;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"host:")		;jry_bl_string_print(&this->host,file)			;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"user_agent:");jry_bl_string_print(&this->user_agent,file)	;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"referer:")	;jry_bl_string_print(&this->referer,file)		;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"etag:")		;jry_bl_string_print(&this->etag,file)		;putc('\n',file);
	for(register int i=0;i<tabs;++i,putc('\t',file));fprintf(file,"cookie:\n");	
	jry_bl_hash_table_foreach(&this->cookie,i)
	{
		for(register int i=0;i<tabs+1;++i,putc('\t',file));	
		jry_bl_string_print(jry_bl_hash_table_get_key(i),file);
		putc(':',file);
		jry_bl_var_view_ex(jry_bl_hash_table_get_var(i),file,"",-1,-(tabs+2));
	}	
	jry_bl_hash_table_foreach(&this->v,i)
	{
		for(register int i=0;i<tabs;++i,putc('\t',file));	
		jry_bl_string_print(jry_bl_hash_table_get_key(i),file);
		putc(':',file);
		jry_bl_var_view_ex(jry_bl_hash_table_get_var(i),file,"",-1,-(tabs+1));
	}
	

	
}
#endif

#endif
