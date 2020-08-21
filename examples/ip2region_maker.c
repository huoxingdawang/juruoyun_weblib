#include "main.h"
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jbl_file * f1=jbl_file_open_chars(NULL,UC "testfiles//ip.merge.txt",JBL_FILE_READ);
	jbl_string* origion_string=jbl_file_read(f1,NULL,0,-1);
	jbl_file_free(f1);	
	
	jbl_ll * l1=jbl_string_cut(origion_string,NULL,'\r');//用\r切割，\n落在ip前面不用处理
	origion_string=jbl_string_free(origion_string);
	puint(l1->len);pn();
	
	jbl_uint32 tmp,len=8;
	
	jbl_ht * table=jbl_ht_new();
	jbl_ht_set_sys(table);
	
	
	
	
	jbl_stream*out=jbl_file_stream_new(jbl_file_open_chars(NULL,UC "testfiles//ip2region.db",JBL_FILE_WRITE));
	jbl_ll_foreach(l1,i)
	{
		if((out->size-out->en)<256)jbl_stream_do(out,0);
		jbl_string_size_type j=0;
		jwl_get_binary_ip(jbl_Vstring(i->v),&j);				//第一个ip
		jwl_get_binary_ip(jbl_Vstring(i->v),&j);				//第二个ip
		jbl_string*addr=jbl_string_delete(jbl_string_copy(jbl_Vstring(i->v)),0,j+1);
		
		jbl_ht_data *data=jbl_ht_get_ht_data(table,addr);
		if(!data)
			table=jbl_ht_insert_force(table,jbl_string_hash(addr),addr,(jbl_var *)(((addr->len+4)<<24)|(len&0x00FFFFFF))),len+=addr->len+4/*,jbl_string_view(addr)*/;
		addr=jbl_string_free(addr);
	}
	tmp=len					;jbl_endian_copy_uint32(&tmp,out->buf+out->en),out->en+=4;
	tmp=len+12*(l1->len-1)	;jbl_endian_copy_uint32(&tmp,out->buf+out->en),out->en+=4;
	jbl_ht_foreach(table,i)
	{
		if((out->size-out->en)<256)jbl_stream_do(out,0);
		tmp=0	;jbl_endian_copy_uint32(&tmp,out->buf+out->en),out->en+=4;//4位cityid
		jbl_stream_push_string(out,i->k);
//		jbl_stream_push_string(jbl_stream_stdout,i->k);
	}	
	jbl_ll_foreach(l1,i)
	{
		if((out->size-out->en)<12)jbl_stream_do(out,0);
		jbl_string_size_type j=0;
		tmp=jwl_get_binary_ip(jbl_Vstring(i->v),&j);				//第一个ip
		jbl_endian_swap_uint32(&tmp,out->buf+out->en),out->en+=4;	//转成小段序写入
		tmp=jwl_get_binary_ip(jbl_Vstring(i->v),&j);				//第二个ip
		jbl_endian_swap_uint32(&tmp,out->buf+out->en),out->en+=4;	//转成小段序写入
		jbl_string*addr=jbl_string_delete(jbl_string_copy(jbl_Vstring(i->v)),0,j+1);
		jbl_ht_data *data=jbl_ht_get_ht_data(table,addr);


		addr=jbl_string_free(addr);
		jbl_endian_copy_uint32(&data->u,out->buf+out->en),out->en+=4;	//写入
	}

	
	jbl_stream_do(out,jbl_stream_force);
	out=jbl_stream_free(out);
	table=jbl_ht_free(table);
	l1=jbl_ll_free(l1);
	
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
