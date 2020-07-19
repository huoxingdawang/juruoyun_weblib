#include "main.h"
#define ip "192.168.1.2"
int main()
{
	printf("--------------------------------" __FILE__ "--------------------------------\n");
	jbl_start();
	jwl_start();	
	jbl_string a;jbl_string_inits(1,&a);
	jbl_var v;jbl_var_inits(1,&v);
	jwl_http_request_head	request_head	;jwl_http_request_head_init(&request_head);
	jwl_http_response_head	response_head	;jwl_http_response_head_init(&response_head);	
	
	jwl_http_request_head_set_method(&request_head,JRY_WL_HTTP_METHOD_GET);
	jwl_http_request_head_set_protocol(&request_head,JRY_WL_HTTP_PROTOCOL_HTTP);
	jwl_http_request_head_set_connection(&request_head,JRY_WL_HTTP_CONNECTION_CLOSE);
	jwl_http_request_head_set_cache(&request_head,JRY_WL_HTTP_CACHE_NO);
	
/*	jbl_string_equal_chars(&a,"/jry_wb/jry_wb_tools/jry_wb_get_ip_address.php?ip=112.224.67.110");jwl_http_request_head_set_url(&request_head,&a,move);
	jbl_string_equal_chars(&a,"www.juruoyun.top");jwl_http_request_head_set_host(&request_head,&a,move);
	jbl_string_equal_chars_light(&a,"60.205.219.36");
	jbl_socket_handle sock;jwl_socket_init(&sock,jwl_get_binary_ip(&a),80,JRY_BL_SOCKET_MODE_CLIENT);
*/

	jbl_string_equal_chars(&a,"/");jwl_http_request_head_set_url(&request_head,&a,move);
	jbl_string_equal_chars(&a,"192.168.56.2");jwl_http_request_head_set_host(&request_head,&a,move);
	jbl_string_equal_chars_light(&a,"192.168.56.2");
	jbl_socket_handle sock;jwl_socket_init(&sock,jwl_get_binary_ip(&a),1217,JRY_BL_SOCKET_MODE_CLIENT);
	
	jwl_http_send_request_head(&sock,&request_head);
	jbl_string_clear(&a);
	jwl_socket_receive(&sock,&a);
	jbl_string_view(&a,stderr);	
	jbl_string_clear(&a);
	jwl_socket_receive_length(&sock,&a,20029134);
	printf("%lld\n",jbl_string_get_length(&a));
	FILE * fp;
	fp=fopen ("testfiles/tmp","wb");
	jbl_string_print(&a,fp);
	fclose(fp);	

//	jbl_var_from_json(&v,&a);jbl_var_view(&v,stderr);


	jwl_socket_close(&sock);
	jwl_http_request_head_free(&request_head);
	jbl_string_frees(1,&a);
	jbl_var_frees(1,&v);
	jbl_stop();		
}
