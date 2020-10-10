#include "main.h"
jwl_socket *host=NULL;
jwl_socket_poll *poll=NULL;
jbl_string *video_big=NULL;
jbl_uint64 count=0;
void stop()
{
	host=jwl_socket_free(host);
	poll=jwl_socket_poll_free(poll);
	video_big=jbl_string_free(video_big);
}
void handle_websocket(jwl_socket_poll_data *spd)
{
	jwl_socket_view(spd->socket);
	jbl_string * get=jbl_string_new();
	jbl_stream * get_stream=jbl_string_stream_new(jbl_refer(&get));
	jbl_stream_connect(spd->ins,get_stream);
	do
	{
		puint(++count);pn();pf();
		jbl_log(UC"--------------------------------------------request:%d---------------------------------------------------------------\n",count);
		get=jbl_string_clear(get);
		jbl_string_update_stream_buf(get_stream);
		spd->sockets->stop=spd->ins->stop=spd->outs->stop=0;
		do{jbl_stream_do(spd->sockets,jbl_stream_force);}while((!spd->ins->stop)&&jwl_socket_opened(spd->socket));
		if(jwl_websocket_decode_stream_get_status(spd->ins)==JWL_WEBSOCKET_STATUS_CLOSE)
		{
			jbl_log(UC "websocket close");
			spd->socket=jwl_socket_close(spd->socket),spd=NULL;
		}
		else if(jwl_websocket_decode_stream_get_status(spd->ins)==JWL_WEBSOCKET_STATUS_PING)
		{
			jbl_log(UC "websocket ping");
			jbl_string *head=jwl_websocket_get_head(0,1,0,JWL_WEBSOCKET_STATUS_PONG,NULL);
			jbl_stream_push_string(spd->ins,head);
			jbl_stream_do(spd->ins,1);	
			head=jbl_string_free(head);					
		}
		else if((jwl_websocket_decode_stream_get_status(spd->ins)==JWL_WEBSOCKET_STATUS_TEXT)||
				(jwl_websocket_decode_stream_get_status(spd->ins)==JWL_WEBSOCKET_STATUS_BIN))
		{
			jbl_string_view(get);
//jbl_log(UC "%v",jbl_gc_minus(jbl_string_copy_as_var(get)));jbl_log_save();
			jbl_string *res=NULL;
			res=jbl_string_add_chars(res,UC"Receive from ip:");
			res=jwl_get_string_ip(jwl_socket_get_ip(spd->socket),res);
			res=jbl_string_add_char(res,'(');
			res=jwl_ip2region(jwl_socket_get_ip(spd->socket),res);
			res=jbl_string_add_chars(res,UC")port:");
			res=jbl_string_add_uint(res,jwl_socket_get_port(spd->socket));
			res=jbl_string_add_chars(res,UC",at ");
			jbl_time * t1=jbl_time_now(NULL);
			res=jbl_time_to_string_format(t1,res,UC"Y-m-d H:i:s.u");
			t1=jbl_time_free(t1);
			res=jbl_string_add_char(res,',');
			res=jbl_string_add_uint(res,jbl_string_get_length(get));
			res=jbl_string_add_chars(res,UC"bytes intotal");
			res=jbl_string_add_chars(res,UC" data:<br>");
			res=jbl_string_add_string(res,get);
//					res=jbl_string_add_chars(res,UC"<br>ex:<br>");
//					res=jbl_rand_string(res,4096,UC jbl_rand_dict_small jbl_rand_dict_big jbl_rand_dict_number jbl_rand_dict_symbol);
			jwl_socket_poll_foreach(poll,i)
				if(jwl_socket_opened(i->socket)&&jbl_stream_is_jwl_websocket_encode_stream(i->outs))
				{
					i->outs->stop=i->sockets->stop=0;
					jwl_socket_view(i->socket);pf();
					jwl_websocket_encode_stream_set_opcode(i->outs,JWL_WEBSOCKET_STATUS_BIN);
					jbl_stream_push_string(i->outs,res);
					jbl_stream_do(i->outs,jbl_stream_force);
					jbl_stream_do(i->sockets,jbl_stream_force);
				}
			res=jbl_string_free(res);
		}
	}while(spd&&jwl_socket_opened(spd->socket)&&(spd->ins->en));
	get=jbl_string_free(get);
	get_stream=jbl_stream_free(get_stream);
}
void handle_webserve(jwl_socket_poll_data *spd)
{
	jwl_http_head * reqh=NULL;
	jwl_http_head * resh=NULL;
	if(!spd->sockets)
	{
		reqh		=jwl_http_head_new();
		resh		=jwl_http_head_new();
		spd->sockets=jwl_socket_stream_new(jbl_refer(&spd->socket));
		spd->ins	=jwl_http_decode_stream_new(jbl_refer(&reqh));
		spd->outs	=jwl_http_encode_stream_new(jbl_refer(&resh));
		jbl_stream_connect(spd->sockets,spd->ins);
		jbl_stream_connect(spd->outs,spd->sockets);
	}
	else
	{
		reqh=jwl_http_head_copy(spd->ins->data);
		resh=jwl_http_head_copy(spd->outs->data);
	}
	jwl_socket_view(spd->socket);
	jbl_string    * get=jbl_string_new();
	jbl_stream    * get_stream=jbl_string_stream_new		(jbl_refer(&get));
	jbl_stream_connect(spd->ins,get_stream);
	do
	{
		puint(++count);pn();
		jbl_log(UC"--------------------------------------------request:%d---------------------------------------------------------------\n",count);
		spd->sockets->stop=spd->ins->stop=spd->outs->stop=0;
		get=jbl_string_clear(get);
		jbl_string_update_stream_buf(get_stream);
		reqh=jwl_http_head_clear(reqh);
		do{jbl_stream_do(spd->sockets,jbl_stream_force);}while(jwl_socket_opened(spd->socket)&&(!spd->ins->stop));
		if(jwl_socket_opened(spd->socket)&&jwl_http_head_get_protocol(reqh))
		{
			jwl_http_head_view(reqh);pf();
			jbl_string_view(get);
			if(jwl_http_head_get_connection(reqh)&JWL_HTTP_CONNECTION_CLOSE)
				spd->socket=jwl_socket_close(spd->socket),spd=NULL;
			else if(jwl_http_head_get_connection(reqh)&JWL_HTTP_CONNECTION_UPGRADE)
			{
				resh=jwl_http_head_clear(resh);
				switch(jwl_http_head_get_upgrade(reqh))
				{
					case JWL_HTTP_UPGRADE_WEBSOCKET:
						resh=jwl_websocket_set_response_head(resh,reqh);
						break;
					case JWL_HTTP_UPGRADE_UNKNOW:
					default:
						break;
				}
				jwl_http_head_view(resh);pf();
				jbl_stream_do(spd->outs,jbl_stream_force);		
				jbl_stream_do(spd->sockets,jbl_stream_force);		
				spd->ins	=jbl_stream_free(spd->ins);
				spd->outs	=jbl_stream_free(spd->outs);
				spd->ins	=jwl_websocket_decode_stream_new();
				spd->outs	=jwl_websocket_encode_stream_new();
				jbl_stream_connect(spd->sockets,spd->ins);
				jbl_stream_connect(spd->outs,spd->sockets);
			}
			else
			{
				resh=jwl_http_head_clear				(resh);
				resh=jwl_http_head_set_response			(resh);
				resh=jwl_http_head_set_connection		(resh,JWL_HTTP_CONNECTION_CLOSE);
				resh=jwl_http_head_set_tren				(resh,JWL_HTTP_TREN_CHUNKED);
				jbl_stream *outs=NULL;
				jbl_string * url=jwl_http_head_get_url(reqh);
				if(jbl_string_space_ship_chars(url,"/")==0)
					url=jbl_string_free(url),url=jbl_string_cache_get(UC"/index.html");
				else if(jbl_string_space_ship_chars(url,"/favicon.ico")==0)
					url=jbl_string_free(url),url=jbl_string_cache_get(UC"/logo.ico");
				else if(jbl_string_space_ship_chars(url,"/download.txt")==0)
					resh=jwl_http_head_set_filename		(resh,jbl_gc_minus(jbl_string_cache_get(UC"download.txt")));
				jbl_string* dir=jbl_string_add_chars(NULL,UC"testfiles");dir=jbl_string_add_string(dir,url);
				jbl_file *f1=jbl_file_open(NULL,dir,JBL_FILE_READ);
				if(jbl_file_get_handle_type(f1))
				{
					outs=jbl_file_stream_new(jbl_refer(&f1));
					resh=jwl_http_head_set_status		(resh,200);
					resh=jwl_http_head_set_cache		(resh,JWL_HTTP_CACHE_NO);
					resh=jwl_http_head_set_content_type	(resh,jbl_file_get_ct(f1));
					resh=jwl_http_head_set_charset		(resh,JWL_HTTP_CHARSET_UTF8);
					resh=jwl_http_head_set_length		(resh,jbl_file_get_size(f1));
					//jbl_file_view(f1);
					
					if(	jwl_http_head_get_range(reqh).start||jwl_http_head_get_range(reqh).end||
						jbl_file_is_video(jbl_file_get_ct(f1))||
						jbl_file_is_audio(jbl_file_get_ct(f1)))
					{
						resh=jwl_http_head_set_tren				(resh,JWL_HTTP_TREN_UNKNOW);
						if(jwl_http_head_get_range(reqh).start>jbl_file_get_size(f1)||jwl_http_head_get_range(reqh).end>jbl_file_get_size(f1))
							resh=jwl_http_head_set_status		(resh,416),jbl_log(UC "[%d,%d) out of [%d,%d)",jwl_http_head_get_range(reqh).start,jwl_http_head_get_range(reqh).end,0LL,jbl_file_get_size(f1));
						else
						{
							jbl_uint64 start=jwl_http_head_get_range(reqh).start;
							jbl_uint64 end=jwl_http_head_get_range(reqh).end?jwl_http_head_get_range(reqh).end:jwl_http_head_get_range(reqh).start+1024*1024*4;
							jbl_min_update(end,jbl_file_get_size(f1));
							resh=jwl_http_head_set_status		(resh,206);
							resh=jwl_http_head_set_length		(resh,end-start);
							resh=jwl_http_head_set_total_length	(resh,jbl_file_get_size(f1));
							resh=jwl_http_head_set_range		(resh,(jwl_http_head_range){start,end});
							jbl_file_stream_set_offset(outs,start);
							jbl_file_stream_set_end(outs,end);
						}
					}								
					jbl_stream_connect(outs,spd->outs);
				}
				f1=jbl_file_free(f1);
				dir=jbl_string_free(dir);
				url=jbl_string_free(url);
				
				if(jwl_http_head_get_status(resh))
				{
					jwl_http_head_view(resh);pf();
					jbl_stream_do(outs,jbl_stream_force);
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
					jbl_stream_push_string(spd->outs,res);
					jbl_stream_do(spd->outs,jbl_stream_force);
					res=jbl_string_free(res);
				}
				jbl_stream_do(spd->sockets,jbl_stream_force);
				outs=jbl_stream_free(outs);
			}
		}
	}while(spd&&jwl_socket_opened(spd->socket)&&(spd->ins->en)&&(spd->sockets->en));
	get_stream=jbl_stream_free(get_stream);
	get=jbl_string_free(get);
	reqh=jwl_http_head_free(reqh);
	resh=jwl_http_head_free(resh);
}
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	jbl_exception_add_exit_function(stop);	
	pchars("--------------------------------" __FILE__ "--------------------------------\n");pf();
	host=jwl_socket_bind(host,jwl_get_binary_ip_chars(UC((argc>1)?argv[1]:"0.0.0.0")),jbl_getuint64(UC((argc>2)?argv[2]:"1217")),JWL_SOCKET_MODE_TCP);
	jwl_socket_view(host);pf();
	
	poll=jwl_socket_poll_new();
	poll=jwl_socket_poll_add(poll,host);


	while(1)
	{
pl();
		poll=jwl_socket_poll_wait(poll);
		for(jwl_socket_poll_data *spd=NULL;NULL!=(spd=jwl_socket_poll_get(poll));)
		{
			if(jwl_socket_if_equal(spd->socket,host))
			{
				jwl_socket * client=jwl_socket_accept(spd->socket);
				poll=jwl_socket_poll_remove_closed(poll);
				poll=jwl_socket_poll_add(poll,client);
				jwl_socket_poll_view(poll);
				jwl_socket_free(client);
			}
			else if(jbl_stream_is_jwl_websocket_encode_stream(spd->outs))//websoket
				handle_websocket(spd);
			else
				handle_webserve(spd);
		}
	}
	host=jwl_socket_free(host);
	poll=jwl_socket_poll_free(poll);
	jwl_stop();
	jbl_stop();		
}
