/* Copyright (c) [2020] juruoyun developer team
	 Juruoyun basic lib is licensed under the Mulan PSL v1.
	 You can use this software according to the terms and conditions of the Mulan PSL v1.
	 You may obtain a copy of Mulan PSL v1 at:
			http://license.coscl.org.cn/MulanPSL
	 THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
	 IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
	 PURPOSE.
	 See the Mulan PSL v1 for more details.*/
#include "jbl_ht.h"
#if JBL_HT_ENABLE==1
/*******************************************************************************************/
/*                            依赖 jbl_gc jbl_malloc jbl_var jbl_ying                      */
/*                                 jbl_exception jbl_string                                */
/*******************************************************************************************/
#include "jbl_gc.h"
#include "jbl_malloc.h"
#include "jbl_var.h"
#include "jbl_ying.h"
#include "jbl_string.h"
#include "jbl_exception.h"
/*******************************************************************************************/
/*                            联动 jbl_stream jbl_ll                                      */
/*******************************************************************************************/
#include "jbl_stream.h"
#if JBL_LL_ENABLE==1
#include "jbl_ll.h"
#endif
/*******************************************************************************************/
/*                            以下函实现哈希表数据节点操作                               */
/*******************************************************************************************/
void __jbl_ht_data_init(jbl_ht_data *this);
void __jbl_ht_data_free(jbl_ht_data *this);
JBL_INLINE void __jbl_ht_data_init(jbl_ht_data *this)
{
	this->k=NULL;
	this->v=NULL;
	this->h=0;
	this->nxt=((jbl_ht_size_type)-1);
}
JBL_INLINE void __jbl_ht_data_free(jbl_ht_data *this)
{
	this->k=jbl_string_free(this->k);
	this->v=jbl_var_free(this->v);
	this->h=0;
	this->nxt=((jbl_ht_size_type)-1);
}
#if JBL_HT_SYS_ENABLE==1
void __jbl_ht_data_free2(jbl_ht_data *this);
void* __jbl_ht_sys_var_copy(void *this);
void* __jbl_ht_sys_var_free(void *this);
#define jbl_ht_is_sys_force(x) (jbl_gc_is_user1(x))		//获取sys标记
JBL_INLINE void __jbl_ht_data_free2(jbl_ht_data *this)
{
	this->k=jbl_string_free(this->k);
	this->h=0;
	this->nxt=((jbl_ht_size_type)-1);
}
JBL_INLINE void* __jbl_ht_sys_var_free(void *this){return NULL;}
JBL_INLINE void* __jbl_ht_sys_var_copy(void *this){return this;}
#endif
/*******************************************************************************************/
/*                            以下函实现哈希表基本操作                                   */
/*******************************************************************************************/
#define htvh(x) for(jbl_ht_size_type i=0;i<(x)->size;jbl_stream_push_char(jbl_stream_stdout,' '),jbl_stream_push_int(jbl_stream_stdout,(jbl_ht_size_type_signed)((jbl_ht_size_type*)(x)->data)[-i-1]),++i);jbl_stream_push_char(jbl_stream_stdout,'\n')		//查看hash值
#define gh2(x,y) ((jbl_ht_size_type_signed)(((jbl_ht_size_type)(y))|(-(jbl_ht_size_type)(x)->size)))
jbl_var_operators_new(jbl_ht_operators,jbl_ht_free,jbl_ht_copy,jbl_ht_space_ship,jbl_ht_json_encode,jbl_ht_view_put,jbl_ht_json_put);
jbl_ht * jbl_ht_new()
{
	jbl_ht * this=jbl_malloc(sizeof(jbl_ht));
	jbl_gc_init(this);
	jbl_gc_plus(this);
	jbl_var_set_operators(this,&jbl_ht_operators);
	this->size=this->len=this->nxt=0;
	this->data=NULL;
	return this;
}
jbl_ht* jbl_ht_free(jbl_ht *this)
{
	if(this==NULL)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		if(jbl_gc_is_ref(this))
			jbl_ht_free((jbl_ht*)(((jbl_reference*)this)->ptr));
		else
			if(this->size!=0)
			{
#if JBL_HT_SYS_ENABLE==1
				if(jbl_ht_is_sys_force(this))
					for(register jbl_ht_size_type i=0;i<this->size;__jbl_ht_data_free2(&this->data[i]),++i);
				else
#endif
					for(register jbl_ht_size_type i=0;i<this->size;__jbl_ht_data_free(&this->data[i]),++i);
				jbl_free((char*)this->data-sizeof(jbl_ht_size_type)*this->size);
			}
		jbl_free(this);
	}
	return NULL;
}
JBL_INLINE jbl_ht *jbl_ht_copy(jbl_ht *that)
{
	if(!that)return NULL;
	jbl_gc_plus(that);
	return that;
}
jbl_ht * jbl_ht_extend_to(jbl_ht *this,jbl_ht_size_type size,jbl_uint8 add,jbl_ht **pthi)
{
//size不够扩
//分离
	if(!this)this=jbl_ht_new();
	jbl_reference *ref=NULL;jbl_ht *thi=jbl_refer_pull_keep_father(this,&ref);	
#if JBL_HT_SYS_ENABLE==1
	void*(*copy)(void*)=(jbl_ht_is_sys_force(this)?__jbl_ht_sys_var_copy:jbl_var_copy);
#else 
	#define copy jbl_var_copy
#endif
	//size计算
	jbl_ht_size_type size_need=size+(thi->len)*(add&1);				//装下当前数据需要的size
	size+=(thi->nxt)*(add&1);										//加上末尾的
	jbl_max_update(size,JBL_HT_MIN_SIZE);							//最小值
	size=1LL<<(jbl_highbit(size-1)+1);								//2的倍数
	jbl_ht *tmp=NULL;
	if(jbl_gc_refcnt(thi)==1)										//不是多个并用
	{
		if(size<=thi->size)											//空间足够
			goto exit;
		else if((size_need<<1)<thi->size)							//有一半的限制节点，可以rehash
		{
			thi=jbl_ht_rehash(thi);
			goto exit;
		}
		//需要重新搞，直接进入下面
	}
	//多个分离
	tmp=jbl_ht_new();
#if JBL_HT_SYS_ENABLE==1
	if(jbl_ht_is_sys_force(thi))
		jbl_ht_set_sys(tmp);
#endif	
	//申请新空间，初始化
	tmp->size=size;
	tmp->data=(jbl_ht_data*)(((char*)jbl_malloc(tmp->size*(sizeof(jbl_ht_data)+sizeof(jbl_ht_size_type))))+sizeof(jbl_ht_size_type)*tmp->size);
	for(register jbl_ht_size_type i=0;i<tmp->size;__jbl_ht_data_init(&tmp->data[i]),((jbl_ht_size_type*)tmp->data)[-i-1]=((jbl_ht_size_type_signed)-1),++i);
	if(thi->data)
	{
		for(register jbl_ht_size_type j=0;j<thi->nxt;++j)	//遍历这个结构
			if(thi->data[j].v!=NULL)						//使用
			{
				jbl_int64 h2=gh2(tmp,tmp->data[tmp->nxt].h=thi->data[j].h);
				tmp->data[tmp->nxt].k=jbl_string_copy(thi->data[j].k);
				tmp->data[tmp->nxt].v=copy(thi->data[j].v);
				tmp->data[tmp->nxt].nxt=((jbl_ht_size_type*)tmp->data)[h2];
				((jbl_ht_size_type*)tmp->data)[h2]=tmp->nxt;
				++tmp->nxt;
				++tmp->len;
			}
	}
	jbl_ht_free(thi);
	thi=tmp;
exit:;	
	if(ref)ref->ptr=thi;else this=thi;
	if(pthi)*pthi=thi;
	return this;
}
jbl_ht * jbl_ht_rehash(jbl_ht *this)
{
	if(!this)return NULL;
	jbl_ht * thi=NULL;this=jbl_ht_extend_to(this,0,1,&thi);
	if(thi->data==NULL)return this;
	register jbl_ht_size_type i=0,j=0;
#if JBL_HT_SYS_ENABLE==1
	void*(*copy)(void*)=(jbl_ht_is_sys_force(this)?__jbl_ht_sys_var_copy:jbl_var_copy);
	void (*dfree)(jbl_ht_data*)=(jbl_ht_is_sys_force(this)?__jbl_ht_data_free2:__jbl_ht_data_free);
#else 
	#define copy jbl_var_copy
	#define dfree __jbl_ht_data_free
#endif
	for(;j<thi->nxt&&thi->data[j].v!=NULL;++j);	
	for(i=j+1;i<thi->nxt;++i)
		if(thi->data[i].v!=NULL&&j<i)
		{
			jbl_int64 h2=gh2(thi,thi->data[j].h=thi->data[i].h);		
			thi->data[j].k=jbl_string_copy(thi->data[i].k);
			thi->data[j].v=copy(thi->data[i].v);
			thi->data[j].nxt=(((jbl_ht_size_type*)thi->data)[h2]>=i)?(thi->data[i].nxt):(((jbl_ht_size_type*)thi->data)[h2]);
			((jbl_ht_size_type*)thi->data)[h2]=j;		
			++j;
			dfree(&thi->data[i]);
		}
	thi->nxt=j;
#if JBL_HT_SYS_ENABLE == 0
	#undef copy
	#undef dfree
#endif
	return this;
}
/*******************************************************************************************/
/*                            以下函实现哈希表插入操作                                   */
/*******************************************************************************************/
jbl_ht * __jbl_ht_insert(jbl_ht *this,jbl_string_hash_type h,jbl_string *k,void *v)
{
	jbl_ht * thi=NULL;this=jbl_ht_extend_to(this,1,1,&thi);
	jbl_ht_size_type_signed h2=gh2(thi,h);
	for(register jbl_ht_size_type i=((jbl_ht_size_type*)thi->data)[h2];i<thi->nxt;i=thi->data[i].nxt)
		if(h==thi->data[i].h&&jbl_string_if_equal(thi->data[i].k,k))
		{
#if JBL_HT_SYS_ENABLE==1
			if(jbl_ht_is_sys_force(thi))
				thi->data[thi->nxt].v=v;
			else
#endif
			jbl_var_free(thi->data[i].v),thi->data[i].v=v?jbl_var_copy(v):jbl_Vnull_new();
			return thi;
		}
	thi->data[thi->nxt].h=h;
	thi->data[thi->nxt].k=jbl_string_copy(jbl_refer_pull(k));
#if JBL_HT_SYS_ENABLE==1
	if(jbl_ht_is_sys_force(thi))
		thi->data[thi->nxt].v=v;
	else
#endif
	thi->data[thi->nxt].v=v?jbl_var_copy(v):jbl_Vnull_new();
	thi->data[thi->nxt].nxt=((jbl_ht_size_type*)thi->data)[h2];
	((jbl_ht_size_type*)thi->data)[h2]=thi->nxt;
	++thi->len;
	++thi->nxt;
	return this;
}
jbl_ht * jbl_ht_insert_force(jbl_ht *this,jbl_string_hash_type h,jbl_string *k,void *v)
{
	jbl_ht * thi=NULL;this=jbl_ht_extend_to(this,1,1,&thi);
	jbl_ht_size_type_signed h2=gh2(thi,h);
	thi->data[thi->nxt].h=h;
	thi->data[thi->nxt].k=jbl_string_copy(jbl_refer_pull(k));
#if JBL_HT_SYS_ENABLE==1
	if(jbl_ht_is_sys_force(thi))
		thi->data[thi->nxt].v=v;
	else
#endif
	thi->data[thi->nxt].v=v?jbl_var_copy(v):jbl_Vnull_new();
	thi->data[thi->nxt].nxt=((jbl_ht_size_type*)thi->data)[h2];
	((jbl_ht_size_type*)thi->data)[h2]=thi->nxt;
	++thi->len;
	++thi->nxt;
	return this;
}
JBL_INLINE jbl_ht  * jbl_ht_insert		(jbl_ht *this,jbl_string *k,void *v)		{return __jbl_ht_insert(this,jbl_string_hash(k),k,v);}
JBL_INLINE jbl_ht  * jbl_ht_insert_chars(jbl_ht *this,unsigned char *kk,void *v)	{jbl_string * k=jbl_string_add_chars(NULL,kk);this=jbl_ht_insert(this,k,v);jbl_string_free(k);return this;}
JBL_INLINE jbl_ht  * jbl_ht_insert_int	(jbl_ht *this,jbl_string_hash_type h,void *v)				{return __jbl_ht_insert(this,h,NULL,v);}
/*******************************************************************************************/
/*                            以下函实现哈希表删除操作                                   */
/*******************************************************************************************/
jbl_ht * __jbl_ht_unset(jbl_ht *this,jbl_string_hash_type h,jbl_string *k)
{
	jbl_ht * thi=NULL;this=jbl_ht_extend_to(this,0,1,&thi);
	if(thi->size!=0)
	{
		jbl_ht_size_type_signed h2=gh2(thi,h);
		for(register jbl_ht_size_type i=((jbl_ht_size_type*)thi->data)[h2],j=((jbl_ht_size_type_signed)-1);i<thi->nxt;j=i,i=thi->data[i].nxt)
			if(h==thi->data[i].h&&jbl_string_if_equal(thi->data[i].k,k))
			{
				if(j==((jbl_ht_size_type_signed)-1))
					(((jbl_ht_size_type*)thi->data)[h2])=thi->data[i].nxt;
				else
					thi->data[j].nxt=thi->data[i].nxt;
#if JBL_HT_SYS_ENABLE==1
				if(jbl_ht_is_sys_force(thi))
					__jbl_ht_data_free2(&thi->data[i]);
				else
#endif
				__jbl_ht_data_free(&thi->data[i]);
				--thi->len;
				break;
			}
	}
	return this;
}
JBL_INLINE jbl_ht  * jbl_ht_unset		(jbl_ht *this,jbl_string *k)				{return __jbl_ht_unset(this,jbl_string_hash(k),k);}
JBL_INLINE jbl_ht  * jbl_ht_unset_chars	(jbl_ht *this,unsigned char *kk)			{jbl_string * k=jbl_string_add_chars(NULL,kk);this=jbl_ht_unset(this,k);jbl_string_free(k);return this;}
JBL_INLINE jbl_ht  * jbl_ht_unset_int	(jbl_ht *this,jbl_string_hash_type h)							{return __jbl_ht_unset(this,h,NULL);}
/*******************************************************************************************/
/*                            以下函实现哈希表获取操作                                   */
/*******************************************************************************************/
JBL_INLINE void * jbl_htv(jbl_ht_data *node){return node->v;}
JBL_INLINE jbl_string * jbl_htk(jbl_ht_data *node){return node->k;}
JBL_INLINE jbl_string_hash_type jbl_hth(jbl_ht_data *node){return node->h;}
jbl_ht_data * jbl_ht_get_ht_data(const jbl_ht *this,jbl_string *k)
{
	this=jbl_refer_pull(this);
	if(this->size!=0&&k)
	{
		jbl_string_hash_type h=jbl_string_hash(k);
		jbl_ht_size_type_signed	h2=gh2(this,h);
		for(register jbl_ht_size_type i=((jbl_ht_size_type*)this->data)[h2];i<this->nxt;i=this->data[i].nxt)
			if(h==this->data[i].h&&jbl_string_if_equal(this->data[i].k,k))
				return &this->data[i];
	}
	return NULL;
}
jbl_ht_data * jbl_ht_get_ht_data_chars(const jbl_ht *this,const jbl_uint8 *chars)
{
	this=jbl_refer_pull(this);
	jbl_string_hash_type h=jbl_strhash(chars,0);
	if(this->size!=0&&chars)
	{
		jbl_ht_size_type_signed	h2=gh2(this,h);
		for(register jbl_ht_size_type i=((jbl_ht_size_type*)this->data)[h2];i<this->nxt;i=this->data[i].nxt)
			if(h==this->data[i].h&&!jbl_string_space_ship_chars(this->data[i].k,(const char *)chars))
				return &this->data[i];
	}
	return NULL;
}
jbl_ht_data * jbl_ht_get_ht_data_int(const jbl_ht *this,jbl_string_hash_type h)
{
	this=jbl_refer_pull(this);
	if(this->size!=0)
	{
		jbl_ht_size_type_signed	h2=gh2(this,h);
		for(register jbl_ht_size_type i=((jbl_ht_size_type*)this->data)[h2];i<this->nxt;i=this->data[i].nxt)
			if(h==this->data[i].h&&!this->data[i].k)
				return &this->data[i];
	}
	return NULL;
}
JBL_INLINE void * jbl_ht_get(const jbl_ht *this,jbl_string *k)
{
	jbl_ht_data *data=jbl_ht_get_ht_data(this,k);
	if(!data)return NULL;
#if JBL_HT_SYS_ENABLE==1
	if(jbl_ht_is_sys(this))
		return data->v;
	else
#endif
	return jbl_var_copy(data->v);
}
JBL_INLINE void * jbl_ht_get_chars(const jbl_ht *this,unsigned char *kk)
{
	jbl_ht_data *data=jbl_ht_get_ht_data_chars(this,kk);
	if(!data)return NULL;
#if JBL_HT_SYS_ENABLE==1
	if(jbl_ht_is_sys(this))
		return data->v;
	else
#endif
	return jbl_var_copy(data->v);
}
JBL_INLINE void * jbl_ht_get_int(const jbl_ht *this,jbl_string_hash_type h)
{
	jbl_ht_data *data=jbl_ht_get_ht_data_int(this,h);
	if(!data)return NULL;
#if JBL_HT_SYS_ENABLE==1
	if(jbl_ht_is_sys(this))
		return data->v;
	else
#endif
	return jbl_var_copy(data->v);
}
/*******************************************************************************************/
/*                            以下函实现哈希表比较操作                                   */
/*******************************************************************************************/
char jbl_ht_space_ship(const jbl_ht *this,const jbl_ht *that)
{
	if(this==that){return 0;}if(this==NULL){return -1;}if(that==NULL){return 1;}
	this=jbl_refer_pull(this);
	that=jbl_refer_pull(that);	
	if(this==that){return 0;}if(this==NULL){return -1;}if(that==NULL){return 1;}
	if(this->len!=that->len)
		return (this->len>that->len)?(1):(-1);
#if JBL_HT_SYS_ENABLE==1
	if(jbl_ht_is_sys_force(this))return -1;
	if(jbl_ht_is_sys_force(that))return 1;
#endif
	void *v2;
	jbl_ht_foreach(this,i)
	{
		v2=jbl_ht_get(that,i->k);
		char tmp=jbl_var_space_ship(i->v,v2);
		v2=jbl_var_free(v2);
		if(!tmp)return tmp;
	}
	return 0;
}
/*******************************************************************************************/
/*                            以下函实现哈希表合并操作                                   */
/*******************************************************************************************/
jbl_ht * jbl_ht_merge(jbl_ht *this,jbl_ht *that)
{
	if(that==NULL)return this;
	that=jbl_refer_pull(that);
	jbl_ht * thi=NULL;this=jbl_ht_extend_to(this,that->len,1,&thi);
#if JBL_HT_SYS_ENABLE==1
	if(jbl_ht_is_sys(this)!=jbl_ht_is_sys(this))return this;
#endif
	jbl_ht_foreach(that,i)
		__jbl_ht_insert(thi,i->h,i->k,i->v);
	return this;
}
jbl_ht * jbl_ht_merge_int(jbl_ht *this)
{
	jbl_ht * thi=NULL;this=jbl_ht_extend_to(this,0,1,&thi);
	if(thi->data==NULL)return this;
#if JBL_HT_SYS_ENABLE==1
	void*(*copy)(void*)=(jbl_ht_is_sys_force(this)?__jbl_ht_sys_var_copy:jbl_var_copy);
	void*(*fre)(void*)=(jbl_ht_is_sys_force(this)?__jbl_ht_sys_var_free:jbl_var_free);
#else 
	#define copy jbl_var_copy
	#define fre jbl_var_free
#endif
	for(register jbl_ht_size_type i=0,j;i<thi->nxt;++i)
		if((!thi->data[i].k)&&thi->data[i].v)
		{
			jbl_string *s=jbl_string_add_uint(NULL,thi->data[i].h);
			char flag=0;
			jbl_string_hash_type h=jbl_string_hash(s);
			jbl_ht_size_type_signed	h2=gh2(thi,h);
			for(j=((jbl_ht_size_type*)thi->data)[h2];j<thi->nxt;j=thi->data[j].nxt)
				if(h==thi->data[j].h&&jbl_string_if_equal(thi->data[j].k,s))
				{flag=1;break;}
			if(flag)
				{if(i>j)fre(thi->data[j].v),thi->data[j].v=copy(thi->data[i].v);}
			else
			{
				this=jbl_ht_extend(this,1);	
				thi=jbl_refer_pull(this);
				h2=gh2(thi,h);
				thi->data[thi->nxt].h=h;
				thi->data[thi->nxt].k=jbl_string_copy(s);
				thi->data[thi->nxt].v=copy(thi->data[i].v);
				thi->data[thi->nxt].nxt=((jbl_ht_size_type*)thi->data)[h2];
				((jbl_ht_size_type*)thi->data)[h2]=thi->nxt;
				++thi->len;
				++thi->nxt;
			}
			__jbl_ht_data_free(&thi->data[i]);
			jbl_string_free(s);
			
		}
#if JBL_HT_SYS_ENABLE == 0
	#undef copy
	#undef fre
#endif
	return this;
}
#if JBL_LL_ENABLE==1
jbl_ht *jbl_ht_merge_ll(jbl_ht *this,jbl_ll *that)
{
#if JBL_HT_SYS_ENABLE==1
	if(this&&jbl_ht_is_sys_force(this))return NULL;
#endif
	jbl_ht * thi=NULL;this=jbl_ht_extend_to(this,jbl_ll_get_length(that),1,&thi);
	jbl_ll_size_type j=0;
	jbl_ll_foreach(that,i)
		jbl_ht_insert_int(thi,j++,i->v);	
	return this;
}
#endif
/*******************************************************************************************/
/*                            以下函实现哈希表JSON操作                                   */
/*******************************************************************************************/
#if JBL_JSON_ENABLE==1
jbl_string* jbl_ht_json_encode(const jbl_ht* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs)
{
#if JBL_HT_SYS_ENABLE==1
	if(this&&jbl_ht_is_sys_force(this))this=NULL;
#endif
	out=jbl_string_json_put_format(this=jbl_refer_pull(this),out,format,tabs);if(!this)return out;
	out=jbl_string_add_char(out,'{');
	if((format&1)||(format&4))out=jbl_string_add_char(out,'\n');
	++tabs;
	jbl_uint8 format2=(format&4)|((format&1)<<2);
	jbl_ht_data * tail=this->data+this->nxt-1;
	jbl_ht_foreach(this,i)
	{
		if(i->k==NULL)continue;
		if((format&1)||(format&4))for(jbl_int32 j=0;j<tabs;out=jbl_string_add_char(out,'\t'),++j);
		if(i->k==NULL)
			out=jbl_string_add_uint(out,i->h);
		else
			out=jbl_string_json_encode(i->k,out,0,0);
		out=jbl_string_add_char(out,':');
		out=jbl_var_json_encode(i->v,out,format2|((i!=tail)<<1),tabs);
	}	
	--tabs;if((format&1)||(format&4))for(jbl_int32 j=0;j<tabs;out=jbl_string_add_char(out,'\t'),++j);//格式化的\t
	out=jbl_string_add_char(out,'}');
	if(format&2){out=jbl_string_add_char(out,',');}if((format&1)||(format&4)){out=jbl_string_add_char(out,'\n');}
	return out;
}
#if JBL_STREAM_ENABLE==1
void jbl_ht_json_put(const jbl_ht* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs)
{
#if JBL_HT_SYS_ENABLE==1
	if(this&&jbl_ht_is_sys_force(this))this=NULL;
#endif	
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,tabs))return;
	jbl_stream_push_char(out,'{');
	if((format&1)||(format&4))jbl_stream_push_char(out,'\n');
	++tabs;
	jbl_uint8 format2=(format&4)|((format&1)<<2);
	jbl_ht_data * tail=this->data+this->nxt-1;
	jbl_ht_foreach(this,i)
	{
		if(i->k==NULL)continue;
		if((format&1)||(format&4))for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
		if(i->k==NULL)
			jbl_stream_push_uint(out,i->h);
		else
			jbl_string_json_put(i->k,out,0,0);
		jbl_stream_push_char(out,':');
		jbl_var_json_put(i->v,out,format2|((i!=tail)<<1),tabs);
	}	
	--tabs;if((format&1)||(format&4))for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
	jbl_stream_push_char(out,'}');
	if(format&2){jbl_stream_push_char(out,',');}if((format&1)||(format&4)){jbl_stream_push_char(out,'\n');}
}
#endif
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现哈希表浏览操作                                   */
/*******************************************************************************************/
jbl_ht* jbl_ht_view_put(jbl_ht* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file)
{
	jbl_ht* thi;if(jbl_stream_view_put_format(thi=jbl_refer_pull(this),out,format,tabs,UC"jbl_ht",line,varname,func,file)){jbl_stream_push_char(out,'\n');return this;}
	jbl_stream_push_chars(out,UC"\tlen:");
	jbl_stream_push_uint(out,thi->len);
	jbl_stream_push_chars(out,UC"\tnxt:");
	jbl_stream_push_uint(out,thi->nxt);
	jbl_stream_push_chars(out,UC"\tsize:");
	jbl_stream_push_uint(out,thi->size);
	jbl_stream_push_char(out,'\n');
	++tabs;
	jbl_ht_foreach(thi,i)
	{
		for(jbl_int32 j=0;j<tabs;jbl_stream_push_char(out,'\t'),++j);//格式化的\t
		if(i->k==NULL)
			jbl_stream_push_uint(out,i->h);
		else
			jbl_stream_push_char(out,'"'),jbl_stream_push_string(out,i->k),jbl_stream_push_char(out,'"');
		jbl_stream_push_char(out,':');
#if JBL_HT_SYS_ENABLE==1
		if(jbl_ht_is_sys_force(thi))jbl_stream_push_uint(out,i->u),jbl_stream_push_char(out,'\n');
		else
#endif
		jbl_var_view_put(i->v,out,0,tabs,0,NULL,NULL,NULL);
	}
	return this;
}
#endif

#endif