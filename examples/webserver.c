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
	sock=jwl_socket_bind(sock,jwl_get_binary_ip(ip),jbl_string_get_uint(port));
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
		jbl_stream * client_stream=jwl_socket_stream_new(client);
#ifdef _WIN32
pl();
#endif
///////////////////////////////////////////////////////////////////////
//windows环境下，会卡在这里，导致ctrl+c关闭的时候，无法释放所有内存
///////////////////////////////////////////////////////////////////////
	jbl_string *get=jbl_string_new();
	jbl_stream *tmp=jbl_string_stream_new(get);
	jbl_stream_connect(client_stream,tmp);
	jbl_stream_do(client_stream,jbl_stream_force);
	tmp->data=NULL;
	tmp=jbl_stream_free(tmp);

//	get=jbl_string_free(get);
//	get=jwl_socket_receive(client,NULL);



#ifdef _WIN32
pl();
#endif
//		jbl_string_view(get);
		jwl_http_head * reqh=jwl_http_head_decode(get,NULL);
		jwl_http_head_view(reqh);	
		
		
		jbl_string *	res	=NULL;							//输出缓冲
		jwl_http_head *	resh=jwl_http_head_new();			//响应头
		jbl_uint64		res_start=0,res_end=-1;
		if(jbl_string_space_ship_chars(reqh->url,"/favicon.ico")==0)
		{
			resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_ICO)));
			resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
			resh=jwl_http_head_set_cache_max_age(resh,36000);
			resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"456")));
			if(jbl_string_space_ship_chars(reqh->etag,"456")==0)
				resh=jwl_http_head_set_status		(resh,304);				
			else
			{
				resh=jwl_http_head_set_status		(resh,200);
				FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//logo.ico","rb"));fclose(fp);
			}
		}
		else if(jbl_string_space_ship_chars(reqh->url,"/pic")==0)
		{
			resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_PNG)));
			resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
			resh=jwl_http_head_set_cache_max_age(resh,36000);
			resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"789")));
			if(jbl_string_space_ship_chars(reqh->etag,"789")==0)
				resh=jwl_http_head_set_status		(resh,304);
			else
			{
				resh=jwl_http_head_set_status		(resh,200);
				FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//test.png","rb"));fclose(fp);
			}
		}
		else if(jbl_string_space_ship_chars(reqh->url,"/download")==0)
		{
			resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_TXT)));
			resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
			resh=jwl_http_head_set_cache_max_age(resh,36000);
			resh=jwl_http_head_set_status		(resh,200);
			resh=jwl_http_head_set_filename		(resh,jbl_gc_minus(jbl_string_cache_get("download.txt")));
			res =jbl_rand_string				(NULL,1024*1024*10,UC jbl_rand_dict_small jbl_rand_dict_big  jbl_rand_dict_number jbl_rand_dict_symbol);
		}
		else if(jbl_string_space_ship_chars(reqh->url,"/video")==0)
		{
			resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_MP4)));
			resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
			resh=jwl_http_head_set_cache_max_age(resh,36000);
			resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"135")));
			if(jbl_string_space_ship_chars(reqh->etag,"135")==0)
				resh=jwl_http_head_set_status		(resh,304);
			else
			{
pl();
				resh=jwl_http_head_set_status		(resh,200);
				jbl_string_view(res);
				FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//test.mp4","rb"));fclose(fp);
				if(reqh->range.start!=0)
				{
					if(reqh->range.start>jbl_string_get_length(res)||reqh->range.end>jbl_string_get_length(res))
						resh=jwl_http_head_set_status		(resh,416),res=jbl_string_free(res),jbl_string_view(get);
					else
					{
pl();
						resh=jwl_http_head_set_status		(resh,206);
						res_start=reqh->range.start;
						res_end=reqh->range.start+jbl_min(409600,reqh->range.end-reqh->range.start);
						jbl_min_update(res_end,jbl_string_get_length(res));
						resh=jwl_http_head_set_range		(resh,res_start,res_end);
					}
				}
			}
		}	
		else if(jbl_string_space_ship_chars(reqh->url,"/")==0)
		{
			resh=jwl_http_head_set_status		(resh,200);
			resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_NO);
			resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_HTML)));
			resh=jwl_http_head_set_charset		(resh,JWL_HTTP_CHARSET_UTF8);
			res=jbl_string_add_uint(res,count);
			res=jbl_string_add_chars(res,UC
				"Hello world,Juruoyun!<br>蒟蒻云<br>"
				"<a href=\"pic\">pic</a><br>"
				"<a href=\"video\">video</a><br>"
				"<a href=\"download\">download</a><br>"
				"注意F5或刷新按钮会导致缓存失败，退化为304<br>"
			);
		}
		else
		{
			resh=jwl_http_head_set_status		(resh,404);
			resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_NO);
			resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_HTML)));
			resh=jwl_http_head_set_charset		(resh,JWL_HTTP_CHARSET_UTF8);
			resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"123123")));			
			res=jbl_string_add_uint(res,count);
			res=jbl_string_add_chars(res,UC
				"<body bgcolor='#000' style='background-color:#000;'>"
				"<div style='font-size:200px; color:#F00;background-color:#000;' align='center'>WARNING!<br></div>"
				"<div style='font-size:50px; color:#F00;background-color:#000;' align='center'>You are trying to get something that we don't have.<br>Please stop now!<br>(╯°Д°)╯︵ ┻━┻</div>"
				"<div style='font-size:25px; color:#F00;background-color:#000;' align='center'>If you sure that the URL:"
			);
			res=jbl_string_add_chars(res,UC"http://");
			res=jbl_string_add_string(res,reqh->host);
			res=jbl_string_add_string(res,reqh->url);			
			res=jbl_string_add_chars(res,UC
				" is right,send an email to develop group.<br>"
				"<a href='/'>Click here to back</a><br><span style='color:white;'></a></div></body>"
			);
		}
		jwl_http_head_view(resh);pf();
		jwl_http_head_encode(client_stream,resh,jbl_string_get_length(res));
		jbl_stream_push_string_start_end(client_stream,res,res_start,res_end);
		jbl_stream_do(client_stream,1);		
		
		get=jbl_string_free(get);		
		client_stream=jbl_stream_free(client_stream);
		reqh=jwl_http_head_free(reqh);
		resh=jwl_http_head_free(resh);
		client=jwl_socket_free(client);
		res=jbl_string_free(res);
		
	}
	
	sock=jwl_socket_free(sock);
	jwl_stop();
	jbl_stop();		
}
