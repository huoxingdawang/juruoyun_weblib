#include "main.h"
#include <time.h>
jry_bl_socket_handle sock,client;
#ifdef __linux__
#include <signal.h>
void jry_wb_signal_callback(int sign)
{
	jry_wl_socket_close(&sock);	
	fputs("Bye juruoyun webserve! \nSAFE EXIT\n",stdout);
	exit(0);
}


#endif
int main(int argc,char** argv)
{
	printf("--------------------------------" __FILE__ "--------------------------------\n");
	jry_bl_start();
	jry_wl_start();	
	jry_bl_string 				data,data2		;jry_bl_string_inits(2,&data,&data2);
	jry_bl_string 				et				=jry_bl_string_const("435434"		,6);
	const jry_bl_string 		ip				=jry_bl_string_const("0.0.0.0"		,7);
	const jry_bl_string 		fa				=jry_bl_string_const("/favicon.ico"	,12);
	jry_bl_var					v1				;jry_bl_var_inits	(1,&v1);
	jry_wl_http_request_head	request_head	;jry_wl_http_request_head_init(&request_head);	
	jry_wl_http_response_head	response_head	;jry_wl_http_response_head_init(&response_head);	
	FILE 						*fp;
	jry_wl_socket_init(&sock,jry_wl_get_binary_ip(&ip),1217,JRY_BL_SOCKET_MODE_SERVER);
#ifdef __linux__
	signal(SIGINT,jry_wb_signal_callback);
	signal(SIGTERM,jry_wb_signal_callback);	
#endif
	int count=0;
	clock_t __start;	
	while(1)
	{
		jry_wl_socket_accept(&sock,&client);
		printf("%d\n",++count);
		jry_wl_http_request_head_clear(&request_head);
		__start=clock();
		if(jry_wl_http_get_request_head(&client,&request_head,&data,true))
			jry_wl_socket_receive(&client,&data);
		fprintf(stderr,"Accept used time:%fs\n",((double)(clock()-__start)/CLOCKS_PER_SEC));		
		jry_wl_request_head_view(&request_head,stderr);
		jry_bl_string_view(&data,stderr);
		jry_bl_string_clear(&data);
		
		jry_wl_http_response_head_set_max_age(&response_head,86400);
		jry_wl_http_response_head_set_etag(&response_head,&et,JRY_BL_COPY_LIGHT);
		if(jry_bl_string_if_equal(jry_wl_http_request_head_get_etag(&request_head),&et))
			jry_wl_http_response_head_set_status(&response_head,304),jry_bl_string_clears(2,&data,&data2);
		else
		{
			__start=clock();
			jry_wl_http_response_head_set_status(&response_head,200);
			if(jry_bl_string_if_equal(jry_wl_http_request_head_get_url(&request_head),&fa))
				fp=fopen(logo_ico,"rb"),jry_wl_http_response_head_set_content_type(&response_head,JRY_WL_HTTP_CONTENT_TYPE_ICO);
			else
				fp=fopen(filename,"rb"),jry_wl_http_response_head_set_content_type(&response_head,JRY_WL_HTTP_CONTENT_TYPE_PNG);		
			jry_bl_string_equal_file(&data2,fp);
			fclose(fp);
			fprintf(stderr,"Read %lld Byte data used time:%fs\n",jry_bl_string_get_length(&data2),((double)(clock()-__start)/CLOCKS_PER_SEC));		
		}
		__start=clock();
		jry_wl_http_send_response(&client,&response_head,&data2);
		fprintf(stderr,"Send %lld Byte data used time:%fs\n",jry_bl_string_get_length(&data2),((double)(clock()-__start)/CLOCKS_PER_SEC));
		jry_bl_string_clears(2,&data,&data2);
		
		jry_wl_socket_close(&client);		
	}	
	jry_wl_socket_close(&sock);	
	jry_bl_string_frees(3,&data,&data2,&et);
	jry_bl_var_frees(1,&v1);
	jry_wl_http_response_head_free(&response_head);
	jry_wl_http_request_head_free(&request_head);
	printf("\nMEMEORY:%lld\n",jry_bl_malloced_size);	
}
