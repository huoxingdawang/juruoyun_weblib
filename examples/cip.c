#include "main.h"
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_socket *socket=jwl_socket_connect(NULL,jwl_dns_get_ip_by_domin_chars(UC"cip.cc",JWB_DNS_QUERY_TYPE_A),80,JWL_SOCKET_MODE_TCP);
	jwl_socket_view(socket);pf();
	jbl_stream * socket_stream=jwl_socket_stream_new(jbl_refer(&socket));
	socket=jwl_socket_free(socket);

	jwl_http_head *	reqh=jwl_http_head_new();
	reqh=jwl_http_head_set_request		(reqh);
	reqh=jwl_http_head_set_method		(reqh,JWL_HTTP_METHOD_GET);
	reqh=jwl_http_head_set_host			(reqh,jbl_gc_minus(jbl_string_cache_get(UC"cip.cc")));
	reqh=jwl_http_head_set_url			(reqh,jbl_gc_minus(jbl_string_cache_get(UC"/60.205.217.184")));
	reqh=jwl_http_head_set_ua			(reqh,jbl_gc_minus(jbl_string_cache_get(UC"curl/7.68.0")));
	reqh=jwl_http_head_set_cache		(reqh,JWL_HTTP_CACHE_NO);
	reqh=jwl_http_head_set_protocol		(reqh,JWL_HTTP_PROTOCOL_HTTP_1_1);
	reqh=jwl_http_head_set_connection	(reqh,JWL_HTTP_CONNECTION_CLOSE);
	reqh=jwl_http_head_set_charset		(reqh,JWL_HTTP_CHARSET_UTF8);
	jwl_http_head_view(reqh);pf();

	jbl_stream * reqh_stream=jwl_http_encode_stream_new(jbl_refer(&reqh));
	jbl_stream_connect(reqh_stream,socket_stream);
	jbl_stream_do(reqh_stream,jbl_stream_force);
	jbl_stream_do(socket_stream,jbl_stream_force);
	reqh=jwl_http_head_free(reqh);
	reqh_stream=jbl_stream_free(reqh_stream);
	
	
	
	
	jwl_http_head *	resh=jwl_http_head_new();
	jbl_stream * resh_stream=jwl_http_decode_stream_new(jbl_refer(&resh));
	jbl_string *get=jbl_string_new();
	jbl_stream *get_stream=jbl_string_stream_new(jbl_refer(&get));


	jbl_stream_connect(socket_stream,resh_stream);
	jbl_stream_connect(resh_stream,get_stream);
	jbl_stream_do(socket_stream,jbl_stream_force);
	jbl_stream_do(resh_stream,jbl_stream_force);
	jbl_stream_disconnect(socket_stream);//断开连接

	jwl_http_head_view(resh);pf();

	jbl_string_view(get);
	get=jbl_string_free(get);


	get_stream=jbl_stream_free(get_stream);
	resh_stream=jbl_stream_free(resh_stream);
	socket_stream=jbl_stream_free(socket_stream);
	resh=jwl_http_head_free(resh);



	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
