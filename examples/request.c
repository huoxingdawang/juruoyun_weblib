#include "main.h"
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_socket *socket=jwl_socket_connect(NULL,jwl_get_binary_ip_chars(UC"27.221.57.108"),10009);
//	jwl_socket *socket=jwl_socket_connect(NULL,jwl_get_binary_ip_chars(UC"127.0.0.1"),1217);
	jwl_socket_view(socket);
	jbl_stream * stream=jwl_socket_stream_new(jbl_refer(&socket));
	socket=jwl_socket_free(socket);


	jwl_http_head *	reqh=jwl_http_head_new();
	reqh=jwl_http_head_set_request		(reqh);
	reqh=jwl_http_head_set_method		(reqh,JWL_HTTP_METHOD_POST);
	reqh=jwl_http_head_set_url			(reqh,jbl_gc_minus(jbl_string_cache_get(UC"/app/trades")));
	reqh=jwl_http_head_set_host			(reqh,jbl_gc_minus(jbl_string_cache_get(UC"27.221.57.108:10009")));
	reqh=jwl_http_head_set_cache		(reqh,JWL_HTTP_CACHE_NO);
	reqh=jwl_http_head_set_protocol		(reqh,JWL_HTTP_PROTOCOL_HTTP_1_1);
	reqh=jwl_http_head_set_connection	(reqh,JWL_HTTP_CONNECTION_CLOSE);
	reqh=jwl_http_head_set_content_type	(reqh,JBL_FILE_CT_JSON);
	reqh=jwl_http_head_set_charset		(reqh,JWL_HTTP_CHARSET_UTF8);

	jwl_http_head_view(reqh);
	jbl_string *s1=jbl_string_add_const(NULL,UC"{\"sno\":\"2017370201882530205\",\"userType\":\"0\",\"pageNum\":1,\"pageSize\":100}");
	jwl_http_head_encode(reqh,stream,jbl_string_get_length(s1));
	jbl_stream_push_string(stream,s1);
	s1=jbl_string_free(s1);
	jbl_stream_do(stream,jbl_stream_force);
	pl();
	
	
	reqh=jwl_http_head_free(reqh);

	jbl_string *get=jbl_string_new();
	jbl_stream *tmp=jbl_string_stream_new(jbl_refer(&get));
	jbl_stream_connect(stream,tmp);
	jbl_stream_do(stream,jbl_stream_force);
	jbl_stream_disconnect(stream);//断开连接
	tmp=jbl_stream_free(tmp);
	stream=jbl_stream_free(stream);
	jbl_string_view(get);







	jwl_http_head * resh=jwl_http_head_decode(get,NULL);
	jwl_http_head_view(resh);pf();
	resh=jwl_http_head_free(resh);



	get=jbl_string_free(get);



	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
