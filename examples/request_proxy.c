#include "main.h"
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_socket *socket=jwl_socket_connect(NULL,jwl_get_binary_ip_chars(UC"192.168.56.1"),7890);
	jwl_socket_view(socket);
	jbl_stream * socket_stream=jwl_socket_stream_new(jbl_refer(&socket));
	socket=jwl_socket_free(socket);

	jwl_http_head *	reqh		=jwl_http_head_new();
	jwl_http_head *	resh		=jwl_http_head_new();
	jbl_stream *	reqh_stream	=jwl_http_encode_stream_new(jbl_refer(&reqh));
	jbl_stream *	resh_stream	=jwl_http_decode_stream_new(jbl_refer(&resh));
	reqh=jwl_http_head_set_request		(reqh);
	reqh=jwl_http_head_set_status		(reqh,JWL_HTTP_STATUS_PROXY);
	reqh=jwl_http_head_set_host			(reqh,jbl_gc_minus(jbl_string_cache_get(UC"192.168.56.150:1217")));
	jbl_stream_connect(reqh_stream,socket_stream);
	jbl_stream_connect(socket_stream,resh_stream);
	jbl_stream_do(reqh_stream,jbl_stream_force);
	jbl_stream_do(socket_stream,jbl_stream_force);
	jbl_stream_disconnect(socket_stream);//断开连接
	jwl_http_head_view(resh);
	
	reqh=jwl_http_head_clear			(reqh);
	resh=jwl_http_head_clear			(resh);
	reqh_stream->stop=0;
	resh_stream->stop=0;
	
	reqh=jwl_http_head_set_request		(reqh);
	reqh=jwl_http_head_set_method		(reqh,JWL_HTTP_METHOD_POST);
	reqh=jwl_http_head_set_url			(reqh,jbl_gc_minus(jbl_string_cache_get(UC"/")));
	reqh=jwl_http_head_set_cache		(reqh,JWL_HTTP_CACHE_NO);
	reqh=jwl_http_head_set_protocol		(reqh,JWL_HTTP_PROTOCOL_HTTP_1_1);
	reqh=jwl_http_head_set_connection	(reqh,JWL_HTTP_CONNECTION_CLOSE);
	reqh=jwl_http_head_set_charset		(reqh,JWL_HTTP_CHARSET_UTF8);
//	reqh=jwl_http_head_set_range		(reqh,(jwl_http_head_range){2,16});
	jwl_http_head_view(reqh);


	jbl_string *s1=jbl_string_add_const(NULL,UC"{\"sno\":\"2017370201882530205\",\"userType\":\"0\",\"pageNum\":1,\"pageSize\":100}");jbl_stream_push_string(reqh_stream,s1);s1=jbl_string_free(s1);
	jbl_stream_do(reqh_stream,jbl_stream_force);
	
	
	jbl_string *get=jbl_string_new();
	jbl_stream *get_stream=jbl_string_stream_new(jbl_refer(&get));
	
	

	jbl_stream_connect(socket_stream,resh_stream);
	jbl_stream_connect(resh_stream,get_stream);
	jbl_stream_do(socket_stream,jbl_stream_force);
	
	jbl_stream_disconnect(socket_stream);//断开连接

	jwl_http_head_view(resh);
	jbl_string_view(get);



	get=jbl_string_free(get);
	get_stream=jbl_stream_free(get_stream);
	resh=jwl_http_head_free(resh);
	reqh=jwl_http_head_free(reqh);
	resh_stream=jbl_stream_free(resh_stream);
	reqh_stream=jbl_stream_free(reqh_stream);	
	socket_stream=jbl_stream_free(socket_stream);



	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
