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
	host=jwl_socket_bind(host,jwl_get_binary_ip_chars(UC((argc>1)?argv[1]:"0.0.0.0")),jbl_getuint64(UC((argc>2)?argv[2]:"1217")));
	jwl_socket_view(host);pf();
	
	poll=jwl_socket_poll_new();
	poll=jwl_socket_poll_add(poll,host);



	jbl_uint64 count=0;
	while(1)
	{
pl();
		poll=jwl_socket_poll_wait(poll);
		for(jwl_socket *client=NULL;NULL!=(client=jwl_socket_poll_get(poll));client=jwl_socket_free(client))
		{
			if(jwl_socket_if_equal(client,host))
			{
				client=jwl_socket_free(client);
				client=jwl_socket_accept(host);
				poll=jwl_socket_poll_remove_closed(poll);
				poll=jwl_socket_poll_add(poll,client);
				jwl_socket_poll_view(poll);
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
				while(!jwl_websocket_stream_finished(websocket_stream))
					jbl_stream_do(client_stream,jbl_stream_force);
				tmp->data=NULL;
				tmp=jbl_stream_free(tmp);
				jbl_stream_disconnect(client_stream);//断开连接				
				if(jwl_websocket_stream_get_status(websocket_stream)==JWL_WEBSOCKET_STATUS_CLOSE)
				{
					jbl_log(UC "websocket close");
					client=jwl_socket_close(client);
				}
				else if(jwl_websocket_stream_get_status(websocket_stream)==JWL_WEBSOCKET_STATUS_PING)
				{
					jbl_log(UC "websocket ping");
					jbl_string *head=jwl_websocket_get_head(0,1,JWL_WEBSOCKET_STATUS_PONG,NULL);
					jbl_stream_push_string(websocket_stream,head);
					jbl_stream_do(websocket_stream,1);	
					head=jbl_string_free(head);					
				}
				else if((jwl_websocket_stream_get_status(websocket_stream)==JWL_WEBSOCKET_STATUS_TEXT)||
						(jwl_websocket_stream_get_status(websocket_stream)==JWL_WEBSOCKET_STATUS_BIN))
				{
//jbl_log(UC "%v",jbl_gc_minus(jbl_string_copy_as_var(get)));jbl_log_save();
					jbl_string *res=jbl_string_add_chars(NULL,UC"Receive from ip:");
					res=jwl_get_string_ip(jwl_socket_get_ip(client),res);
					res=jbl_string_add_char(res,'(');
					res=jwl_ip2region(jwl_socket_get_ip(client),res);
					res=jbl_string_add_chars(res,UC")port:");
					res=jbl_string_add_uint(res,jwl_socket_get_port(client));
					res=jbl_string_add_chars(res,UC",at ");
					jbl_time * t1=jbl_time_now(NULL);
					res=jbl_time_to_string_format(t1,res,UC"Y-m-d H:i:s.u");
					t1=jbl_time_free(t1);
					res=jbl_string_add_char(res,',');
					res=jbl_string_add_uint(res,jbl_string_get_length(get));
					res=jbl_string_add_chars(res,UC"bytes intotal");
					res=jbl_string_add_chars(res,UC" data:<br>");
					res=jbl_string_add_string(res,get);
					
					jbl_string *head=jwl_websocket_get_head(jbl_string_get_length(res),1,JWL_WEBSOCKET_STATUS_TEXT,NULL);
					jwl_socket_poll_foreach(poll,i)
						if(jwl_socket_get_payload(i->socket)==1)
						{
							jwl_socket * websoccket_send=jwl_socket_copy(i->socket);
							jbl_stream * websocket_send_stream=jwl_socket_stream_new(jbl_refer(&websoccket_send));						
							jbl_stream_push_string(websocket_send_stream,head);
							jbl_stream_push_string(websocket_send_stream,res);
							jbl_stream_do(websocket_send_stream,1);	
							websoccket_send=jwl_socket_free(websoccket_send);
							websocket_send_stream=jbl_stream_free(websocket_send_stream);
						}
					res=jbl_string_free(res);
					head=jbl_string_free(head);
				}
				websocket_stream=jbl_stream_free(websocket_stream);
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
				jbl_stream    * client_stream=jwl_socket_stream_new(jbl_refer(&client));
				jwl_http_head * reqh=jwl_http_head_new();
				jbl_stream    * reqh_stream=jwl_http_decode_stream_new(jbl_refer(&reqh));
				jbl_string    * get=jbl_string_new();
				jbl_stream    * get_stream=jbl_string_stream_new(jbl_refer(&get));
		while(1)
		{
				jbl_stream_connect(client_stream,reqh_stream);
				jbl_stream_connect(reqh_stream,get_stream);
				jbl_stream_do(client_stream,jbl_stream_force);
				jbl_stream_disconnect(client_stream);
		if(!jwl_http_head_get_protocol(reqh))break;

				if((!jwl_socket_closed(client))&&jwl_http_head_get_protocol(reqh))
				{
					jwl_http_head_view(reqh);pf();
					jbl_string_view(get);
					if(jwl_http_head_get_connection(reqh)&JWL_HTTP_CONNECTION_UPGRADE)
					{
						jwl_http_head *	resh=jwl_http_head_new();			//响应头
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
						jwl_http_head_view(resh);pf();
						jwl_http_head_encode(resh,client_stream);
						jbl_stream_do(client_stream,1);		
						resh=jwl_http_head_free(resh);
					}
					else
					{
						jwl_http_head *	resh=jwl_http_head_new();			//响应头
						resh=jwl_http_head_set_response			(resh);
						resh=jwl_http_head_set_connection		(resh,JWL_HTTP_CONNECTION_KEEP_ALIVE);
						resh=jwl_http_head_set_tren				(resh,JWL_HTTP_TREN_CHUNKED);
						jbl_stream * resh_stream=jwl_http_encode_stream_new(jbl_refer(&resh));
						jbl_stream_connect(resh_stream,client_stream);
						jbl_stream *outs=NULL;

						jbl_string * url=jwl_http_head_get_url(reqh);
						if(jbl_string_space_ship_chars(url,"/")==0)
							url=jbl_string_free(url),url=jbl_string_cache_get(UC"/index.html");
						else if(jbl_string_space_ship_chars(url,"/favicon.ico")==0)
							url=jbl_string_free(url),url=jbl_string_cache_get(UC"/logo.ico");
						else if(jbl_string_space_ship_chars(url,"/download.txt")==0)
							resh=jwl_http_head_set_filename		(resh,jbl_gc_minus(jbl_string_cache_get(UC"download.txt")));
						jbl_string* dir=jbl_string_add_chars(NULL,UC"testfiles");dir=jbl_string_add_string(dir,url);
						jbl_string_view(dir);
						jbl_file *f1=jbl_file_open(NULL,dir,JBL_FILE_READ);
						if(jbl_file_get_handle_type(f1))
						{
							outs=jbl_file_stream_new(jbl_refer(&f1));
							resh=jwl_http_head_set_status		(resh,200);
							resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_NO);
							resh=jwl_http_head_set_content_type	(resh,jbl_file_get_ct(f1));
							resh=jwl_http_head_set_charset		(resh,JWL_HTTP_CHARSET_UTF8);
							resh=jwl_http_head_set_length		(resh,jbl_file_get_size(f1));
							jbl_file_view(f1);
							
							if(	jwl_http_head_get_range(reqh).start||jwl_http_head_get_range(reqh).end||
								jbl_file_is_video(jbl_file_get_ct(f1))||
								jbl_file_is_audio(jbl_file_get_ct(f1)))
							{
								resh=jwl_http_head_set_tren				(resh,JWL_HTTP_TREN_UNKNOW);
								if(jwl_http_head_get_range(reqh).start>jbl_file_get_size(f1)||jwl_http_head_get_range(reqh).end>jbl_file_get_size(f1))
									resh=jwl_http_head_set_status		(resh,416),jbl_log(UC "[%d,%d) out of [%d,%d)",jwl_http_head_get_range(reqh).start,jwl_http_head_get_range(reqh).end,0LL,jbl_file_get_size(f1));
								else
								{
									resh=jwl_http_head_set_status		(resh,206);
									jbl_uint64 start=jwl_http_head_get_range(reqh).start;
									jbl_uint64 end=jwl_http_head_get_range(reqh).end?jwl_http_head_get_range(reqh).end:jwl_http_head_get_range(reqh).start+409600;
									jbl_min_update(end,jbl_file_get_size(f1));
									resh=jwl_http_head_set_range		(resh,(jwl_http_head_range){start,end});
									jbl_file_stream_set_offset(outs,start);
									jbl_file_stream_set_end(outs,end);
								}
							}								
							jbl_stream_connect(outs,resh_stream);
						}
						f1=jbl_file_free(f1);
						dir=jbl_string_free(dir);
						url=jbl_string_free(url);
						
						if(jwl_http_head_get_status(resh))
						{
							jwl_http_head_view(resh);pf();
							jbl_stream_do(outs,1);
						}
						else if(reqh)
						{
							resh=jwl_http_head_set_status		(resh,404);
							resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_NO);
							resh=jwl_http_head_set_content_type	(resh,JBL_FILE_CT_HTML);
							resh=jwl_http_head_set_charset		(resh,JWL_HTTP_CHARSET_UTF8);
							jbl_string *res=NULL;
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
							resh=jwl_http_head_set_length(resh,jbl_string_get_length(res));						
							jwl_http_head_view(resh);pf();
							jbl_stream_push_string(resh_stream,res);
							res=jbl_string_free(res);
						}						
						jbl_stream_do(resh_stream,1);
						outs=jbl_stream_free(outs);
						resh_stream=jbl_stream_free(resh_stream);
						resh=jwl_http_head_free(resh);
					}
				}
				
				reqh=jwl_http_head_clear(reqh);
		}
				reqh_stream=jbl_stream_free(reqh_stream);
				get_stream=jbl_stream_free(get_stream);
				get=jbl_string_free(get);
				reqh=jwl_http_head_free(reqh);
				client_stream=jbl_stream_free(client_stream);
	
			}
		}
	}
	host=jwl_socket_free(host);
	poll=jwl_socket_poll_free(poll);
	jwl_stop();
	jbl_stop();		
}
