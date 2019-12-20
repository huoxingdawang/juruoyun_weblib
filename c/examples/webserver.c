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
	jry_bl_string ip,data,data2;jry_bl_string_inits(3,&ip,&data,&data2);
	jry_bl_string_equal_char_pointer_light(&ip,"0.0.0.0");
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
		__start=clock();
		printf("%d\n",++count);
		jry_wl_socket_receive(&client,&data);
		jry_bl_string_view(&data,stderr);
		jry_bl_string_clear(&data);
		fprintf(stderr,"Accept used time:%fs\n",((double)(clock()-__start)/CLOCKS_PER_SEC));		
		
		FILE * fp;
		fp=fopen(filename,"rb");
		jry_bl_string_equal_file(&data2,fp);
		fclose(fp);
		fprintf(stderr,"Read %lld Byte data used time:%fs\n",jry_bl_string_get_length(&data2),((double)(clock()-__start)/CLOCKS_PER_SEC));		

		jry_wl_http_response_head head=jry_wl_http_response_head_const(200,jry_bl_string_const(JRY_WL_HTTP_CONTENT_TYPE_PNG,jry_bl_strlen(JRY_WL_HTTP_CONTENT_TYPE_PNG)));
		jry_wl_http_send_respose(&client,&head,&data2);
		fprintf(stderr,"Send %lld Byte data used time:%fs\n",jry_bl_string_get_length(&data2),((double)(clock()-__start)/CLOCKS_PER_SEC));
		jry_bl_string_clears(2,&data,&data2);
		
		jry_wl_socket_close(&client);		
	}	
	jry_wl_socket_close(&sock);	
	jry_bl_string_frees(3,&ip,&data,&data2);
	printf("\nMEMEORY:%lld\n",jry_bl_malloced_size);	
}
