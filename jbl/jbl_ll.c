/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_ll.h"
#if JBL_LL_ENABLE==1
/*******************************************************************************************/
/*                            依赖 jbl_gc jbl_malloc jbl_ying jbl_exception jbl_var        */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_gc.h"
#include "jbl_malloc.h"
#include "jbl_ying.h"
#include "jbl_var.h"
/*******************************************************************************************/
/*                            联动jbl_stream jbl_string jbl_ht                             */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_stream.h"
#include "jbl_ht.h"
/*******************************************************************************************/
/*                            以下函实现链节点基本操作                                   */
/*******************************************************************************************/
jbl_ll_node * jbl_ll_node_init(jbl_ll_node *node)
{
	node->pre=NULL;
	node->nxt=NULL;
	node->v=NULL;
	return node;
}
jbl_ll_node * jbl_ll_node_new()
{
	return jbl_ll_node_init(jbl_malloc(sizeof(jbl_ll_node)));
}
void jbl_ll_node_free(jbl_ll_node *node)
{
	node->v=jbl_var_free(node->v);
	jbl_free(node);
}
void jbl_ll_node_delink(jbl_ll *this,jbl_ll_node *node)
{
	if(node->nxt==NULL)	this->tail=node->pre;
	else				node->nxt->pre=node->pre;
	if(node->pre==NULL)	this->head=node->nxt;
	else				node->pre->nxt=node->nxt;
	--this->len;
	node->nxt=NULL,node->pre=NULL;
}
void jbl_ll_node_insert(jbl_ll *this,jbl_ll_node *node,jbl_ll_node *after)
{
	if(after==NULL)
	{
		node->nxt=this->head;
		node->pre=NULL;
		if(node->nxt==NULL)	this->tail=node;
		else				node->nxt->pre=node;
		this->head=node;
	}
	else
	{
		node->nxt=after->nxt;
		node->pre=after;
		after->nxt=node;
		if(node->nxt==NULL)	this->tail=node;
		else				node->nxt->pre=node;
	}	
	++this->len;
}
void jbl_ll_node_delete(jbl_ll *this,jbl_ll_node *node)
{
	jbl_ll_node_delink(this,node);
	jbl_ll_node_free(node);
}



/*******************************************************************************************/
/*                            以下函实现链表基本操作                                      */
/*******************************************************************************************/
jbl_var_operators_new(jbl_ll_operators,jbl_ll_free,jbl_ll_copy,jbl_ll_space_ship,jbl_ll_json_encode,jbl_ll_view_put,jbl_ll_json_put);
inline jbl_ll* jbl_Vll(jbl_var * this){if(this&&!Vis_jbl_ll(this))jbl_exception("VAR TYPE ERROR");return((jbl_ll*)this);}
inline jbl_ll * jbl_ll_new()
{
	return jbl_ll_init(jbl_malloc((sizeof(jbl_ll))));	
}
jbl_var * jbl_Vll_new()
{
	jbl_var *this=(jbl_var*)((char*)(jbl_malloc((sizeof(jbl_ll))+(sizeof(jbl_var)))+(sizeof(jbl_var))));
	jbl_ll_init((jbl_ll*)this);
	jbl_gc_set_var((jbl_ll*)this);
	jbl_var_set_operators(this,&jbl_ll_operators);
	return this;		
}
inline jbl_ll* jbl_ll_init(jbl_ll *this)
{
	jbl_gc_init(this);
	jbl_gc_plus(this);	
	this->head=NULL,this->tail=NULL;
	this->len=0;
	return this;
}
jbl_ll* jbl_ll_free(jbl_ll *this)
{
	if(!this)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this)||jbl_gc_is_pvar(this))
			jbl_ll_free((jbl_ll *)jbl_refer_pull(this));
		else
			jbl_ll_foreach_del(this,i,j)
				jbl_ll_node_delete(this,i);	
		jbl_free(((jbl_gc_is_var(this))?(jbl_ll*)((char*)this-sizeof(jbl_var)):this));
	}
	return NULL;	
}
inline jbl_ll *jbl_ll_copy(jbl_ll *that)
{
	if(that==NULL)return NULL;
	jbl_gc_plus(that);
	return that;
}
jbl_ll *jbl_ll_extend(jbl_ll *this,jbl_ll_node **a,jbl_ll_node **b,jbl_ll **pthi)
{
	if(!this){this=jbl_ll_new();if(pthi)*pthi=this;return this;}
	jbl_reference *ref=NULL;jbl_ll *thi=jbl_refer_pull_keep_father(this,&ref);
	jbl_uint8 is_var=jbl_gc_is_var(this);
	if(jbl_gc_is_pvar(thi))
	{
		jbl_ll *ttt=((jbl_reference*)thi)->ptr;
		if(jbl_gc_refcnt(thi)==1)
			((jbl_reference*)thi)->ptr=NULL;
		jbl_ll_free(thi);
		thi=ttt;
	}
	if((jbl_gc_refcnt(thi)<=1)){if(pthi)*pthi=thi;return this;}
	jbl_ll *tmp=(is_var)?jbl_Vll(jbl_Vll_new()):jbl_ll_new();
	jbl_ll_foreach(thi,i)
	{
		jbl_ll_node *node=jbl_ll_node_new();
		node->v=jbl_var_copy(i->v);
		if(a&&(*a==i))*a=node;
		if(b&&(*b==i))*b=node;
		jbl_ll_node_insert(tmp,node,tmp->tail);		
	}
	jbl_ll_free(thi);
	if(ref)		ref->ptr=tmp;
	else		this=tmp;
	if(pthi)	*pthi=tmp;
	return this;
}
/*******************************************************************************************/
/*                            以下函实现链表插入操作                                     */
/*******************************************************************************************/
jbl_ll * jbl_ll_insert(jbl_ll *this,jbl_var *var,jbl_ll_node *after)
{
	jbl_ll *thi;this=jbl_ll_extend(this,&after,NULL,&thi);
	jbl_ll_node *node=jbl_ll_node_new();
	node->v=(var?jbl_var_copy(var):jbl_Vnull_new());
	jbl_ll_node_insert(thi,node,after);
	return this;
}
/*******************************************************************************************/
/*                            以下函实现链表合并操作                                     */
/*******************************************************************************************/
jbl_ll *jbl_ll_merge(jbl_ll *this,jbl_ll *that)
{
	if(that==NULL)return this;
	jbl_ll * tha=jbl_refer_pull(that);
	jbl_ll *thi;this=jbl_ll_extend(this,NULL,NULL,&thi);
	jbl_ll_foreach(tha,i)
		jbl_ll_insert(thi,i->v,thi->tail);
	return this;
}
#if JBL_HT_ENABLE==1
jbl_ll *jbl_ll_merge_ht(jbl_ll *this,jbl_ht *that)
{
#if JBL_HT_SYS_ENABLE==1
	if(!that||jbl_ht_is_sys(that))return this;
#endif
	jbl_ht_foreach(that,i)
		this=jbl_ll_add(this,i->v);	
	return this;
}
#endif
/*******************************************************************************************/
/*                            以下函实现链表删除操作                                     */
/*******************************************************************************************/
jbl_ll * jbl_ll_delete(jbl_ll *this,jbl_ll_node *node)
{
	if(!node)return this;
	jbl_ll *thi;this=jbl_ll_extend(this,&node,NULL,&thi);
	jbl_ll_node_delete(thi,node);
	return this;
}
/*******************************************************************************************/
/*                            以下函实现链表获取操作                                      */
/*******************************************************************************************/
inline jbl_var* jbl_llv(jbl_ll_node *node){return node->v;}
/*******************************************************************************************/
/*                            以下函实现链表交换操作                                     */
/*******************************************************************************************/
jbl_ll * jbl_ll_swap_node(jbl_ll *this,jbl_ll_node *a,jbl_ll_node *b)
{
	if(a==b)return this;
	this=jbl_ll_extend(this,&a,&b,NULL);
	jbl_var *v;
	v=a->v;
	a->v=b->v;
	b->v=v;
	return this;
}
/*******************************************************************************************/
/*                            以下函实现链表比较操作                                     */
/*******************************************************************************************/
char jbl_ll_space_ship(const jbl_ll *this,const jbl_ll *that)
{
	if(this==that){return 0;}if(this==NULL){return -1;}if(that==NULL){return 1;}
	const jbl_ll *thi=jbl_refer_pull(this);
	const jbl_ll *tha=jbl_refer_pull(that);	
	if(thi==tha){return 0;}if(thi==NULL){return -1;}if(tha==NULL){return 1;}
	if(thi->len!=tha->len)
		return (thi->len>tha->len)?(1):(-1);
	char a=0;
	for(jbl_ll_node *i=thi->head,*j=tha->head;i!=NULL&&a==0;a=jbl_var_space_ship(i->v,j->v),i=i->nxt,j=j->nxt);
	return a;
}
#if JBL_JSON_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现链表JSON操作                                      */
/*******************************************************************************************/
#if JBL_STRING_ENABLE==1
jbl_string* jbl_ll_json_encode(const jbl_ll* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
	out=jbl_string_json_put_format(this=jbl_refer_pull(this),out,format,tabs);if(!this)return out;
	out=jbl_string_add_char(out,'[');
	if((format&1)||(format&4))out=jbl_string_add_char(out,'\n');
	++tabs;
	jbl_uint8 format2=(format&4)|((format&1)<<2);
	jbl_ll_foreach(this,i)
	{
		if(format){out=jbl_string_add_char(out,'\n');for(jbl_int32 i=0;i<tabs;out=jbl_string_add_char(out,'\t'),++i);}//¸ñÊ½»¯µÄ\tºÍ\n
		out=jbl_var_json_encode(i->v,out,format2|((i!=this->tail)<<1),tabs);

	}	
	--tabs;if((format&1)||(format&4))for(jbl_int32 j=0;j<tabs;out=jbl_string_add_char(out,'\t'),++j);//格式化的\t
	out=jbl_string_add_char(out,']');
	if(format&2){out=jbl_string_add_char(out,',');}if((format&1)||(format&4)){out=jbl_string_add_char(out,'\n');}
	return out;
}
jbl_ll* jbl_ll_json_decode(jbl_ll *this,jbl_string* in,jbl_string_size_type *start)
{
	char flag=0;
	if(!this)this=jbl_ll_new(),flag=1;
	in=jbl_refer_pull(in);	
	jbl_string_size_type i=start?(*start):0,n=in->len;
	for(;i<n&&in->s[i]<=32;++i);
//	pchar(in->s[i]);pn();
	if(in->s[i]!='[')goto fail;
	++i;
	for(;i<n;)
	{
		for(;i<n&&in->s[i]<=32;++i);
		if(in->s[i]==']'){++i;goto success;}
		jbl_var *v=jbl_var_json_decode(NULL,in,&i);
		if(v==NULL)goto fail;
		this=jbl_ll_add(this,v);
		v=jbl_var_free(v);	
		for(;i<n&&in->s[i]<=32;++i);
		if(in->s[i]==']'){++i;goto success;}
		if(in->s[i]!=',')goto fail;
		++i;
	}
	goto fail;
success:;
	start?(*start=i):0;
	return this;
fail:;
	if(flag)this=jbl_ll_free(this);
	return NULL;
}
#endif
#if JBL_STREAM_ENABLE==1
void jbl_ll_json_put(const jbl_ll* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,tabs))return;	
	jbl_stream_push_char(out,'[');
	if((format&1)||(format&4))jbl_stream_push_char(out,'\n');
	++tabs;
	jbl_uint8 format2=(format&4)|((format&1)<<2);
	jbl_ll_foreach(this,i)
	{
		if((format&1)||(format&4))for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);
		jbl_var_json_put(i->v,out,format2|((i!=this->tail)<<1),tabs);
	}	
	--tabs;if((format&1)||(format&4))for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);
	jbl_stream_push_char(out,']');
	if(format&2){jbl_stream_push_char(out,',');}if((format&1)||(format&4)){jbl_stream_push_char(out,'\n');}
}
#endif
#endif
/*******************************************************************************************/
/*                            以下函实现链表插看操作                                      */
/*******************************************************************************************/
#if JBL_STREAM_ENABLE==1
jbl_ll*jbl_ll_view_put(jbl_ll* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jbl_ll* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jbl_ll",line,varname,func,file))return this;
	jbl_stream_push_chars(out,UC"\tlen:");jbl_stream_push_uint(out,thi->len);
	jbl_stream_push_char(out,'\n');
	jbl_ll_size_type j=0;
	++tabs;
	jbl_ll_foreach(thi,i)
	{
		for(jbl_int32 i=0;i<tabs;jbl_stream_push_char(out,'\t'),++i);
		jbl_stream_push_uint(out,j++);
		jbl_stream_push_char(out,':');
		jbl_var_view_put(i->v,out,0,tabs,0,NULL,NULL,NULL);
	}
	return this;
}
#endif
#endif
