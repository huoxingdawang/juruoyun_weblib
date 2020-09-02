#include "main.h"
#include <setjmp.h>
jmp_buf env,env2;
void stop(){if(!setjmp(env2))longjmp(env,1);}
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_socket *socket=jwl_socket_connect(NULL,jwl_get_binary_ip_chars(UC((argc>1)?argv[1]:"127.0.0.1")),jbl_getuint64(UC((argc>2)?argv[2]:"1217")));
	jwl_socket_view(socket);
	jbl_stream * socket_stream=jwl_socket_stream_new(jbl_refer(&socket));

	jwl_http_head *	reqh=jwl_http_head_new();
	jbl_var * sec_key=jbl_V(jbl_rand_string(jbl_Vstring(jbl_Vstring_new()),16,UC jbl_rand_dict_small jbl_rand_dict_big));
	jbl_stream * reqh_stream=jwl_http_encode_stream_new(jbl_refer(&reqh));
	jbl_stream_connect(reqh_stream,socket_stream);
	reqh=jwl_http_head_set_request		(reqh);
	reqh=jwl_http_head_set_method		(reqh,JWL_HTTP_METHOD_GET);
	reqh=jwl_http_head_set_url			(reqh,jbl_gc_minus(jbl_string_cache_get(UC"/")));
	reqh=jwl_http_head_set_cache		(reqh,JWL_HTTP_CACHE_NO);
	reqh=jwl_http_head_set_protocol		(reqh,JWL_HTTP_PROTOCOL_HTTP_1_1);
	reqh=jwl_http_head_set_connection	(reqh,JWL_HTTP_CONNECTION_UPGRADE);
	reqh=jwl_http_head_set_upgrade		(reqh,JWL_HTTP_UPGRADE_WEBSOCKET);
	reqh=jwl_http_head_set_charset		(reqh,JWL_HTTP_CHARSET_UTF8);
	reqh=jwl_http_head_set				(reqh,UC"Sec-WebSocket-Key",sec_key);
	jwl_http_head_view(reqh);
	jbl_stream_do(reqh_stream,jbl_stream_force);
	
	
	
	jwl_http_head *	resh=jwl_http_head_new();
	jbl_stream * resh_stream=jwl_http_decode_stream_new(jbl_refer(&resh));	
	jbl_stream_connect(socket_stream,resh_stream);
	jbl_stream_do(socket_stream,jbl_stream_force);
	jbl_stream_disconnect(socket_stream);
	jwl_http_head_view(resh);
	

	sec_key=jbl_var_free(sec_key);
	reqh=jwl_http_head_free(reqh);
	reqh_stream=jbl_stream_free(reqh_stream);
	resh_stream=jbl_stream_free(resh_stream);
	resh=jwl_http_head_free(resh);

	jbl_stream * wbesocket_send_stream=jwl_websocket_encode_stream_new();
	jwl_websocket_encode_stream_set_mask(wbesocket_send_stream,1);
	jwl_websocket_encode_stream_set_opcode(wbesocket_send_stream,JWL_WEBSOCKET_STATUS_BIN);
	jbl_stream_connect(wbesocket_send_stream,socket_stream);
	
	jbl_string * s1=jbl_string_new();

	for(jbl_uint8 i=0;i<128;++i)
	{
		s1=jbl_string_clear(s1);
		s1=jbl_rand_string(s1,128,UC jbl_rand_dict_small jbl_rand_dict_big);
		jbl_string_view(s1);pf();
		jbl_stream_push_string(wbesocket_send_stream,s1);
		jbl_stream_do(wbesocket_send_stream,jbl_stream_force);
		wbesocket_send_stream->stop=0;
	}
	jbl_string_free(s1);
	jbl_stream_do(socket_stream,jbl_stream_force);	
	wbesocket_send_stream=jbl_stream_free(wbesocket_send_stream);
	
	
jbl_exception_add_exit_function(stop);	
	jbl_stream * wbesocket_receive_stream=jwl_websocket_decode_stream_new();
	jbl_string * get=jbl_string_new();
	jbl_stream * get_stream=jbl_string_stream_new(jbl_refer(&get));
	jbl_stream_connect(socket_stream,wbesocket_receive_stream);
	jbl_stream_connect(wbesocket_receive_stream,get_stream);
	jbl_uint8 _=0;
	while(!jwl_socket_closed(socket)&&!(_=setjmp(env)))
	{
		do
		{
			wbesocket_receive_stream->stop=0;
			get=jbl_string_clear(get);
			jbl_string_update_stream_buf(get_stream);
			do{jbl_stream_do(socket_stream,jbl_stream_force);}while((socket_stream->en||wbesocket_receive_stream->en)&&(!wbesocket_receive_stream->stop));
			jbl_string_view(get);pf();
		}while((socket_stream->en||wbesocket_receive_stream->en)&&(wbesocket_receive_stream->stop));
	}
	get_stream=jbl_stream_free(get_stream);
	get=jbl_string_free(get);
	wbesocket_receive_stream=jbl_stream_free(wbesocket_receive_stream);
	socket_stream=jbl_stream_free(socket_stream);
	socket=jwl_socket_free(socket);
	if(_)longjmp(env2,1);


	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
