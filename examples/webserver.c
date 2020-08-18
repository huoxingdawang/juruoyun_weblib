#include "main.h"
jwl_socket *host=NULL;
jwl_socket_poll *poll=NULL;
jbl_string *video_big=NULL;
void stop()
{
	host=jwl_socket_free(host);
	poll=jwl_socket_poll_free(poll);
	video_big=jbl_string_free(video_big);
}
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	jbl_exception_add_exit_function(stop);	
	pchars("--------------------------------" __FILE__ "--------------------------------\n");pf();
	jbl_string *ip=jbl_string_add_chars(NULL,UC((argc>1)?argv[1]:"0.0.0.0"));
	jbl_string *port=jbl_string_add_chars(NULL,UC((argc>2)?argv[2]:"1217"));
	host=jwl_socket_bind(host,jwl_get_binary_ip(ip),jbl_string_get_uint(port));
	ip=jbl_string_free(ip);
	port=jbl_string_free(port);	
	jwl_socket_view(host);pf();
	
	poll=jwl_socket_poll_new();
	poll=jwl_socket_poll_add(poll,host);



	jbl_uint64 count=0;
	while(1)
	{
pl() ;
		poll=jwl_socket_poll_wait(poll);
		jwl_socket *client=NULL;
		while(NULL!=(client=jwl_socket_poll_get(poll)))
		{
			if(jwl_socket_if_equal(client,host))
			{
				client=jwl_socket_free(client);
				client=jwl_socket_accept(host);
				poll=jwl_socket_poll_add(poll,client);
				poll=jwl_socket_poll_remove_closed(poll);
				jwl_socket_poll_view(poll);
				client=jwl_socket_free(client);
			}
			else if(jwl_socket_get_payload(client)==1)//websoket
			{
				puint(++count);pn();
				jbl_log(UC	"\n-----------------------------------------------------------------------------------------------------------------------------------\n"
							"-----------------------------------------------------------------------------------------------------------------------------------\n"
							"request:%d",count);
				jwl_socket_view(client);
				jbl_stream * client_stream=jwl_socket_stream_new(jbl_refer(&client));
				if(!client_stream)
				{
					client=jwl_socket_free(client);
					continue;
				}
				jbl_stream * websocket_stream=jwl_websocket_stream_new();
				jbl_string *get=jbl_string_new();
				jbl_stream *tmp=jbl_string_stream_new(get);
				jbl_stream_connect(client_stream,websocket_stream);
				jbl_stream_connect(websocket_stream,tmp);
				while(!jwl_websocket_stream_finished(websocket_stream)&&jwl_websocket_stream_get_status(websocket_stream)!=JWL_WEBSOCKET_STATUS_CLOSE)
					jbl_stream_do(client_stream,jbl_stream_force);
				tmp->data=NULL;
				tmp=jbl_stream_free(tmp);
				jbl_stream_disconnect(client_stream);//断开连接				
				if(jwl_websocket_stream_get_status(websocket_stream)==JWL_WEBSOCKET_STATUS_CLOSE)
				{
					jbl_log(UC "websocket close");jbl_log_save();
					client=jwl_socket_close(client);
					websocket_stream=jbl_stream_free(websocket_stream);
					goto exit_of_websocket;
				}
				websocket_stream=jbl_stream_free(websocket_stream);
jbl_log(UC "%v",jbl_gc_minus(jbl_string_copy_as_var(get)));jbl_log_save();
//				jbl_string_view(get);

//				jbl_string *res=jbl_rand_string(NULL,6,UC jbl_rand_dict_small jbl_rand_dict_big  jbl_rand_dict_number jbl_rand_dict_symbol);
				jbl_string *res=jbl_string_add_chars(NULL,"Receive from ip:");
				res=jwl_get_string_ip(jwl_socket_get_ip(client),res);
				res=jbl_string_add_chars(res," port:");
				res=jbl_string_add_uint(res,jwl_socket_get_port(client));
				res=jbl_string_add_chars(res,",at ");
				jbl_time * t1=jbl_time_now(NULL);
				res=jbl_time_to_string_format(t1,res,UC"Y-m-d H:i:s.u");
				t1=jbl_time_free(t1);
				res=jbl_string_add_chars(res,",");
				res=jbl_string_add_uint(res,jbl_string_get_length(get));
				res=jbl_string_add_chars(res,"bytes intotal");
				res=jbl_string_add_chars(res," data:<br>");
				res=jbl_string_add_string(res,get);
				
//				jbl_string_view(res);
				jbl_string *head=jwl_websocket_get_head(jbl_string_get_length(res),1,JWL_WEBSOCKET_STATUS_TEXT,NULL);
				jbl_stream_push_string(client_stream,head);
				jbl_stream_push_string(client_stream,res);
				jbl_stream_do(client_stream,1);			
				res=jbl_string_free(res);
				head=jbl_string_free(head);
				
exit_of_websocket:;
				client=jwl_socket_free(client);
				get=jbl_string_free(get);
				client_stream=jbl_stream_free(client_stream);
			}
			else
			{
				puint(++count);pn();
				jbl_log(UC	"\n-----------------------------------------------------------------------------------------------------------------------------------\n"
							"-----------------------------------------------------------------------------------------------------------------------------------\n"
							"request:%d",count);
				jwl_socket_view(client);
				jbl_stream * client_stream=jwl_socket_stream_new(jbl_refer(&client));
				if(!client_stream)
				{
					client=jwl_socket_free(client);
					continue;
				}
				jbl_string *get=jbl_string_new();
				jbl_stream *tmp=jbl_string_stream_new(get);
				jbl_stream_connect(client_stream,tmp);
				jbl_stream_do(client_stream,jbl_stream_force);
				tmp->data=NULL;
				tmp=jbl_stream_free(tmp);
				jbl_stream_disconnect(client_stream);//断开连接
jbl_log(UC "%v",jbl_gc_minus(jbl_string_copy_as_var(get)));jbl_log_save();
				jwl_http_head * reqh=jwl_http_head_decode(get,NULL);
				jwl_http_head_view(reqh);pf();
				jbl_string *	res	=NULL;							//输出缓冲
				jwl_http_head *	resh=jwl_http_head_new();			//响应头
				jbl_uint64		res_start=0,res_end=-1;
				resh=jwl_http_head_set_connection		(resh,JWL_HTTP_CONNECTION_KEEP_ALIVE);
				if(jwl_http_head_get_connection(reqh)==JWL_HTTP_CONNECTION_UPGRADE)
				{
					switch(jwl_http_head_get_upgrade(reqh))
					{
						case JWL_HTTP_UPGRADE_WEBSOCKET:
							resh=jwl_websocket_set_response_head(resh,reqh);
							client=jwl_socket_set_payload(client,1);
							break;
						case JWL_HTTP_UPGRADE_UNKNOW:
						default:
							break;
					}
				}
				else
				{
					if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_url(reqh)),"/")==0)
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
							"<a href=\"videobig\">videobig</a><br>"
							"<a href=\"download\">download</a><br>"
							"<a href=\"websocket.html\">websocket</a><br>"
							"<a href=\"http://www.yhdm.tv/v/4466-1.html\">videoout</a><br>"
							"<a href=\"http://111.30.158.158/vmtt.tc.qq.com/1098_d9b5d5438b47c14c899213c2c27cd941.f0.mp4?vkey=6704C21D9CACA82931587D2DBFE6825BAF46064206F39B3BB81B78529139D3EF7B80A4034695C9E26BD217FFBC29C0C5EEABE6ACCE9570942FD0D23DC1AFA2B6819CE6C73FFE78B83FD133F031650FB2BC5612A49171971C\">video out</a><br>"
							"注意F5或刷新按钮会导致缓存失败，退化为304<br>"
						);
					}
					else if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_url(reqh)),"/favicon.ico")==0)
					{
						resh=jwl_http_head_set_status		(resh,200);
						resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
						resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_ICO)));
						resh=jwl_http_head_set_cache_max_age(resh,36000);
						resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"456")));
						if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_etag(reqh)),"456")==0)
							resh=jwl_http_head_set_status		(resh,304);				
						else
						{
							FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//logo.ico","rb"));fclose(fp);
						}
					}
					else if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_url(reqh)),"/websocket.html")==0)
					{
						resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_HTML)));
						resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
						resh=jwl_http_head_set_cache_max_age(resh,36000);
						resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"456")));
						if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_etag(reqh)),"456")==0)
							resh=jwl_http_head_set_status		(resh,304);				
						else
						{
							resh=jwl_http_head_set_status		(resh,200);
							FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//websocket.html","rb"));fclose(fp);
						}
					}
					else if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_url(reqh)),"/js/jwb_websocket.js")==0)
					{
						resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_JS)));
						resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
						resh=jwl_http_head_set_cache_max_age(resh,36000);
						resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"456")));
						if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_etag(reqh)),"456")==0)
							resh=jwl_http_head_set_status		(resh,304);				
						else
						{
							resh=jwl_http_head_set_status		(resh,200);
							FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//js//jwb_websocket.js","rb"));fclose(fp);
						}
					}
					else if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_url(reqh)),"/pic")==0)
					{
						resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_PNG)));
						resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
						resh=jwl_http_head_set_cache_max_age(resh,36000);
						resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"789")));
						if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_etag(reqh)),"789")==0)
							resh=jwl_http_head_set_status		(resh,304);
						else
						{
							resh=jwl_http_head_set_status		(resh,200);
							FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles//test.png","rb"));fclose(fp);
						}
					}
					else if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_url(reqh)),"/download")==0)
					{
						resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_TXT)));
						resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
						resh=jwl_http_head_set_cache_max_age(resh,36000);
						resh=jwl_http_head_set_status		(resh,200);
						resh=jwl_http_head_set_filename		(resh,jbl_gc_minus(jbl_string_cache_get(UC"download.txt")));
						res =jbl_rand_string				(NULL,1024*1024*10,UC jbl_rand_dict_small jbl_rand_dict_big  jbl_rand_dict_number jbl_rand_dict_symbol);
					}
					else if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_url(reqh)),"/video")==0)
					{
						resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_MP4)));
						resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
						resh=jwl_http_head_set_cache_max_age(resh,36000);
						resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"135")));
						if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_etag(reqh)),"135")==0)
							resh=jwl_http_head_set_status		(resh,304);
						else
						{
							resh=jwl_http_head_set_status		(resh,200);
							FILE *fp;res=jbl_string_add_file(res,fp=fopen("testfiles/test.mp4","rb"));fclose(fp);
							if(jwl_http_head_get_range(reqh).start)
							{
								if(jwl_http_head_get_range(reqh).start>jbl_string_get_length(res)||jwl_http_head_get_range(reqh).end>jbl_string_get_length(res))
									resh=jwl_http_head_set_status		(resh,416),jbl_log(UC "[%d,%d) out of [%d,%d)",jwl_http_head_get_range(reqh).start,jwl_http_head_get_range(reqh).end,0LL,jbl_string_get_length(res)),res=jbl_string_free(res);
								else
								{
									resh=jwl_http_head_set_status		(resh,206);
									res_start=jwl_http_head_get_range(reqh).start;
									res_end=jbl_max(jwl_http_head_get_range(reqh).start+409600,jwl_http_head_get_range(reqh).end);
									jbl_min_update(res_end,jbl_string_get_length(res));
									resh=jwl_http_head_set_range		(resh,(jwl_http_head_range){res_start,res_end});
								}
							}
						}
					}
					else if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_url(reqh)),"/videobig")==0)
					{
						resh=jwl_http_head_set_content_type	(resh,jbl_gc_minus(jbl_string_cache_get(UC JWL_HTTP_CONTENT_TYPE_MP4)));
						resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_MAX_AGE);
						resh=jwl_http_head_set_cache_max_age(resh,36000);
						resh=jwl_http_head_set_etag			(resh,jbl_gc_minus(jbl_string_cache_get(UC"135")));
						if(jbl_string_space_ship_chars(jbl_gc_minus(jwl_http_head_get_etag(reqh)),"135")==0)
							resh=jwl_http_head_set_status		(resh,304);
						else
						{
							resh=jwl_http_head_set_status		(resh,200);
							if(!video_big)
								{FILE *fp;video_big=jbl_string_add_file(video_big,fp=fopen("testfiles/video_big.mp4","rb"));fclose(fp);}
							res=jbl_string_copy(video_big);
							if(jwl_http_head_get_range(reqh).start||jwl_http_head_get_range(reqh).end)
							{
								if(jwl_http_head_get_range(reqh).start>jbl_string_get_length(res)||jwl_http_head_get_range(reqh).end>jbl_string_get_length(res))
									resh=jwl_http_head_set_status		(resh,416),jbl_log(UC "[%d,%d) out of [%d,%d)",jwl_http_head_get_range(reqh).start,jwl_http_head_get_range(reqh).end,0LL,jbl_string_get_length(res)),res=jbl_string_free(res);
								else
								{
									resh=jwl_http_head_set_status		(resh,206);
									res_start=jwl_http_head_get_range(reqh).start;
									res_end=jbl_max(jwl_http_head_get_range(reqh).start+409600,jwl_http_head_get_range(reqh).end);
									jbl_min_update(res_end,jwl_http_head_get_range(reqh).start+4096000);
									jbl_min_update(res_end,jbl_string_get_length(res));
									resh=jwl_http_head_set_range		(resh,(jwl_http_head_range){res_start,res_end});
								}
							}
						}
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
						res=jbl_string_add_string(res,jbl_gc_minus(jwl_http_head_get_host(reqh)));
						res=jbl_string_add_string(res,jbl_gc_minus(jwl_http_head_get_url(reqh)));			
						res=jbl_string_add_chars(res,UC
							" is right,send an email to develop group.<br>"
							"<a href='/'>Click here to back</a><br><span style='color:white;'></a></div></body>"
						);
					}
				}
				jwl_http_head_view(resh);pf();
				jwl_http_head_encode(client_stream,resh,jbl_string_get_length(res));
				jbl_stream_push_string_start_end(client_stream,res,res_start,res_end);
				jbl_stream_do(client_stream,1);		
				
				resh=jwl_http_head_free(resh);
				res=jbl_string_free(res);
				reqh=jwl_http_head_free(reqh);
				get=jbl_string_free(get);
				client_stream=jbl_stream_free(client_stream);
				client=jwl_socket_free(client);
			}
		}
	}
	host=jwl_socket_free(host);
	poll=jwl_socket_poll_free(poll);
	jwl_stop();
	jbl_stop();		
}
