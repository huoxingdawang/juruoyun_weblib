#include "main.h"
jwl_socket *sock=NULL;
void stop()
{
	sock=jwl_socket_free(sock);
}
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	jbl_exception_add_exit_function(stop);	
	pchars("--------------------------------" __FILE__ "--------------------------------\n");pf();
	jbl_string *ip=jbl_string_add_chars(NULL,UC((argc>1)?argv[1]:"0.0.0.0"));
	jbl_string *port=jbl_string_add_chars(NULL,UC((argc>2)?argv[2]:"1217"));
	sock=jwl_socket_bind(sock,jwl_get_binary_ip(ip),jbl_string_get_uint64(port));
	ip=jbl_string_free(ip);
	port=jbl_string_free(port);	
	jwl_socket_view(sock);pf();

	jbl_uint32 count=0;
	while(1)
	{
		jwl_socket *client=jwl_socket_accept(sock);
		if(!client)continue;
		puint(++count);pn();
		jwl_socket_view(client);
pl();
///////////////////////////////////////////////////////////////////////
//windows环境下，会卡在这里，导致ctrl+c关闭的时候，无法释放所有内存
///////////////////////////////////////////////////////////////////////
		jbl_string *get=jwl_socket_receive(client,NULL);
pl();
//		jbl_string_view(get);
		jwl_http_reqh * reqh=jwl_http_reqh_decode(get,NULL);
		get=jbl_string_free(get);
		jwl_http_reqh_view(reqh);	
		
		
		
		jbl_string *	res	=NULL;							//输出缓冲
		jwl_http_resh *	resh=jwl_http_resh_new();			//响应头
		if(jbl_string_space_ship_chars(reqh->url,"/favicon.ico")==0)
		{
			resh=jwl_http_resh_set_content_type	(resh,jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_ICO));
			resh=jwl_http_resh_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
			resh=jwl_http_resh_set_cache_max_age(resh,3600);
			resh=jwl_http_resh_set_etag			(resh,jbl_string_cache_get(UC"456"));\
			if(jbl_string_space_ship_chars(reqh->etag,"456")==0)
				resh=jwl_http_resh_set_status		(resh,304);				
			else
			{
				resh=jwl_http_resh_set_status		(resh,200);
				FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//logo.ico","rb"));fclose(fp);
			}
		}
		else if(jbl_string_space_ship_chars(reqh->url,"/pic")==0)
		{
			resh=jwl_http_resh_set_content_type	(resh,jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_PNG));
			resh=jwl_http_resh_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
			resh=jwl_http_resh_set_cache_max_age(resh,3600);
			resh=jwl_http_resh_set_etag			(resh,jbl_string_cache_get(UC"789"));
			if(jbl_string_space_ship_chars(reqh->etag,"789")==0)
				resh=jwl_http_resh_set_status		(resh,304);
			else
			{
				resh=jwl_http_resh_set_status		(resh,200);
				FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//test.png","rb"));fclose(fp);
			}
		}
		else if(jbl_string_space_ship_chars(reqh->url,"/video")==0)
		{
			resh=jwl_http_resh_set_content_type	(resh,jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_MP4));
			resh=jwl_http_resh_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
			resh=jwl_http_resh_set_cache_max_age(resh,3600);
			resh=jwl_http_resh_set_etag			(resh,jbl_string_cache_get(UC"135"));
			if(jbl_string_space_ship_chars(reqh->etag,"135")==0)
				resh=jwl_http_resh_set_status		(resh,304);
			else
			{
				resh=jwl_http_resh_set_status		(resh,200);
				FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//test.mp4","rb"));fclose(fp);
			}			
		}		
		else
		{
			resh=jwl_http_resh_set_status		(resh,200);
			resh=jwl_http_resh_set_cache		(resh,JWL_HTTP_CACHE_NO);
			resh=jwl_http_resh_set_content_type	(resh,jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_HTML));
			resh=jwl_http_resh_set_charset		(resh,JWL_HTTP_CHARSET_UTF8);
			resh=jwl_http_resh_set_etag			(resh,jbl_string_cache_get(UC"123123"));			
			res=jbl_string_add_uint64(res,count);
			res=jbl_string_add_chars(res,UC"Hello world,Juruoyun!<br>蒟蒻云");
		}
		
		jwl_http_resh_view(resh);
		
		
		jwl_http_send_res(client,resh,res);

		reqh=jwl_http_reqh_free(reqh);
		resh=jwl_http_resh_free(resh);
		client=jwl_socket_free(client);
		res=jbl_string_free(res);
		pf();
		
	}
	
	sock=jwl_socket_free(sock);
	jwl_stop();
	jbl_stop();		
}
