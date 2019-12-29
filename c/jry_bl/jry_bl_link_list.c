/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_bl_link_list.h"
#if JRY_BL_LINK_LIST_ENABLE==1
inline void jry_bl_link_list_init(jry_bl_link_list *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	this->head=NULL,this->tail=NULL;
	this->length=0;this->light_copy=false;
}
inline void jry_bl_link_list_node_init(jry_bl_link_list_node *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_var_init(&this->v);
}
void jry_bl_link_list_free(jry_bl_link_list *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(!this->light_copy)
		jry_bl_link_list_foreach_del(this,i,j)
			jry_bl_link_list_delete_node(this,i);
	this->head=NULL,this->tail=NULL;
}
void jry_bl_link_list_node_free(jry_bl_link_list_node *this)
{
	if(this==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_var_free(&this->v);
	this->nxt=NULL,this->pre=NULL;
}
void jry_bl_link_list_node_delink(jry_bl_link_list *this,jry_bl_link_list_node *node)
{
	if(this==NULL||node==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(node->nxt==NULL)
		this->tail=node->pre;
	else
		node->nxt->pre=node->pre;
	if(node->pre==NULL)
		this->head=node->nxt;
	else
		node->pre->nxt=node->nxt;
	--this->length;
	node->nxt=NULL,node->pre=NULL;
}
void jry_bl_link_list_insert_node(jry_bl_link_list *this,jry_bl_link_list_node *node,jry_bl_link_list_node *after)
{
	if(this==NULL||node==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(after==NULL)
	{
		node->nxt=this->head;
		node->pre=NULL;
		if(node->nxt==NULL)
			this->tail=node;
		else
			node->nxt->pre=node;
		this->head=node;
	}
	else
	{
		node->nxt=after->nxt;
		node->pre=after;
		after->nxt=node;
		if(node->nxt==NULL)
			this->tail=node;
		else
			node->nxt->pre=node;
	}	
	++this->length;
}
void jry_bl_link_list_insert_var(jry_bl_link_list *this,jry_bl_var *var,jry_bl_link_list_node *after)
{
	if(this==NULL||var==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_bl_link_list_node *node=jry_bl_malloc(sizeof(jry_bl_link_list_node));
	if(node==NULL)jry_bl_exception(JRY_BL_ERROR_MEMORY_ERROR);
	jry_bl_link_list_node_init(node);
	jry_bl_var_equal(&(node->v),var);
	jry_bl_link_list_insert_node(this,node,after);
}
void jry_bl_link_list_insert_var_light(jry_bl_link_list *this,jry_bl_var *var,jry_bl_link_list_node *after)
{
	if(this==NULL||var==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_bl_link_list_node *node=jry_bl_malloc(sizeof(jry_bl_link_list_node));
	if(node==NULL)jry_bl_exception(JRY_BL_ERROR_MEMORY_ERROR);
	jry_bl_link_list_node_init(node);
	jry_bl_var_equal_light(&(node->v),var);
	jry_bl_link_list_insert_node(this,node,after);
}
void jry_bl_link_list_insert_var_light_move(jry_bl_link_list *this,jry_bl_var *var,jry_bl_link_list_node *after)
{
	if(this==NULL||var==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_bl_link_list_node *node=jry_bl_malloc(sizeof(jry_bl_link_list_node));
	if(node==NULL)jry_bl_exception(JRY_BL_ERROR_MEMORY_ERROR);
	jry_bl_link_list_node_init(node);
	jry_bl_var_equal_light_move(&(node->v),var);
	jry_bl_link_list_insert_node(this,node,after);
}
inline void jry_bl_link_list_delete_node(jry_bl_link_list *this,jry_bl_link_list_node *node)
{
	if(this==NULL||node==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);	
	jry_bl_link_list_node_delink(this,node);
	jry_bl_link_list_node_free(node);
	jry_bl_free(node);
	node=NULL;
}
inline void jry_bl_link_list_swap_node(jry_bl_link_list *this,jry_bl_link_list_node *a,jry_bl_link_list_node *b)
{
	if(a==b)return;
	if(this==NULL||a==NULL||b==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_link_list_node *c;
	(a->pre==NULL)?(this->head=b,b->pre->nxt=a):((b->pre==NULL)?(this->head=a,a->pre->nxt=b):(a->pre->nxt=b,b->pre->nxt=a));
	(a->nxt==NULL)?(this->tail=b,b->nxt->pre=a):((b->nxt==NULL)?(this->tail=a,a->nxt->pre=b):(a->nxt->pre=b,b->nxt->pre=a));
	c=a->pre,a->pre=b->pre,b->pre=c;
	c=a->nxt,a->nxt=b->nxt,b->nxt=c;
}
char jry_bl_link_list_space_ship(jry_bl_link_list *this,jry_bl_link_list *that)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this==that)
		return 0;	
	if(this->length!=that->length)
		return (this->length>that->length)?(1):(-1);
	char a=0;
	for(jry_bl_link_list_node *i=this->head,*j=that->head;i!=NULL&&a==0;a=jry_bl_var_space_ship(&i->v,&j->v),i=i->nxt,j=j->nxt);
	return a;
}
void jry_bl_link_list_copy(jry_bl_link_list *this,jry_bl_link_list *that,jry_bl_uint8 copytype)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_link_list_free(this);
	if(copytype==JRY_BL_COPY)
		jry_bl_link_list_foreach(that,i)
			jry_bl_link_list_add_var(this,jry_bl_link_list_data(i));
	else
	{
		this->head=that->head;this->tail=that->tail;this->length=that->length;
		if(copytype==JRY_BL_COPY_LIGHT)
			this->light_copy=true;
		else	
			this->light_copy,that->light_copy,that->light_copy=true;
	}
}
void jry_bl_link_list_to_json_ex(jry_bl_link_list *this,jry_bl_string *out,jry_bl_uint8 type)
{
	if(this==NULL||out==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(type==1)
	{		
		(*((jry_bl_string_size_type*)out))+=2;
		if(this->length==0)
			return;
		jry_bl_string_size_type tmp_len=0;
		jry_bl_link_list_foreach(this,i)
			tmp_len+=1,jry_bl_var_to_json_ex(jry_bl_link_list_data(i),(jry_bl_string*)(&tmp_len),1);
		(*((jry_bl_string_size_type*)out))+=tmp_len-1;
		return;
	}
	if(type==0)
	{
		jry_bl_string_size_type tmp_len=0;
		jry_bl_link_list_to_json_ex(this,(jry_bl_string*)(&tmp_len),1);
		jry_bl_string_extend(out,tmp_len);
	}
	jry_bl_string_add_char1(out,'[');
	jry_bl_link_list_foreach(this,i)
		jry_bl_var_to_json_ex(jry_bl_link_list_data(i),out,2),jry_bl_string_add_char1(out,',');
	if(this->length!=0)
		jry_bl_string_delete_1(out);
	jry_bl_string_add_char1(out,']');
}
jry_bl_string_size_type jry_bl_link_list_from_json_start(jry_bl_link_list *this,jry_bl_string *in,jry_bl_string_size_type start)
{
	if(this==NULL||in==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	register jry_bl_string_size_type i=start,n=jry_bl_string_get_length(in),ii;
	jry_bl_link_list that;
	jry_bl_link_list_init(&that);
	for(;(i<n)&&(!(in->s[i]=='['));++i);++i;
	jry_bl_link_list_node *node;
	while(i<n)
	{
begin:
		node=jry_bl_malloc(sizeof(jry_bl_link_list_node));
		jry_bl_link_list_node_init(node);
		ii=jry_bl_var_from_json_start(&(node->v),in,i);
		if(ii==i){jry_bl_free(node);node=NULL;for(;i<n;++i)if(in->s[i]==']')goto success;else if(in->s[i]!=' '&&in->s[i]!='\r'&&in->s[i]!='\t'&&in->s[i]!='\n')goto fail;goto fail;}i=ii;	
		jry_bl_link_list_add_node(&that,node);
		for(;i<n;++i)
			if(in->s[i]==',')
				{++i;goto begin;}
			else if(in->s[i]==']')
				goto success;
			else if(in->s[i]!=' '&&in->s[i]!='\r'&&in->s[i]!='\t'&&in->s[i]!='\n')
				goto fail;
		goto fail;
	}
fail:
	jry_bl_link_list_free(&that);
	return start;
success:
	jry_bl_link_list_merge_light(this,&that);
	jry_bl_link_list_free(&that);
	return i+1;	
}
void jry_bl_link_list_merge(jry_bl_link_list *this,jry_bl_link_list *that)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	jry_bl_link_list_foreach(that,i)
		jry_bl_link_list_add_var(this,jry_bl_link_list_data(i));		
}
void jry_bl_link_list_merge_light(jry_bl_link_list *this,jry_bl_link_list *that)
{
	if(this==NULL||that==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(this->tail!=NULL)
		this->tail->nxt=that->head;
	else
		this->head=that->head;
	if(that->head!=NULL)
		that->head->pre=this->tail;
	this->tail=that->tail;
	this->length+=that->length;
	that->light_copy=true;
}
#if JRY_BL_USE_STDIO==1
void jry_bl_link_list_view_ex(jry_bl_link_list *this,FILE * file,char*str,int a,int tabs)
{
	if(this==NULL||file==NULL||str==NULL)jry_bl_exception(JRY_BL_ERROR_NULL_POINTER);
	if(tabs>=0)
		for(register int i=0;i<tabs;++i,putc('\t',file));
	else
		tabs=-tabs;
	if(a>=0)
		fprintf(file,"jry_bl_link_list %s %d\t:len:%lld\n",str,a,(jry_bl_int64)this->length);
	else
		fprintf(file,"jry_bl_link_list  \t:len:%lld\n",(jry_bl_int64)this->length);
	jry_bl_link_list_size_type i=0;
	jry_bl_link_list_foreach(this,j)
	{
		for(register int i=0;i<tabs+1;++i,putc('\t',file));		
		fprintf(file,"%lld:",(jry_bl_uint64)i++);
		jry_bl_var_view_ex(jry_bl_link_list_data(j),file,"",-1,-(tabs+1));	
	}
	if(tabs==jry_bl_view_default_tabs_num)fputc('\n',file);
}
#endif

#if JRY_BL_USE_STDARG==1
void jry_bl_link_list_inits(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_link_list_init(va_arg(valist,jry_bl_link_list*));
	va_end(valist);	
}
void jry_bl_link_list_node_inits(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_link_list_node_init(va_arg(valist,jry_bl_link_list_node*));
	va_end(valist);
}
void jry_bl_link_list_frees(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_link_list_free(va_arg(valist,jry_bl_link_list*));
	va_end(valist);	
}
void jry_bl_link_list_node_frees(int n,...)
{
	va_list valist;
	va_start(valist,n);
	for(int i=0;i<n;i++)
		jry_bl_link_list_node_free(va_arg(valist,jry_bl_link_list_node*));
	va_end(valist);
}
#endif

#endif
