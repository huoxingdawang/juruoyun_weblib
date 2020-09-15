/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_malloc.h"
#if JBL_MALLOC_ENABLE==1
/*******************************************************************************************/
/*                            依赖 jbl_ying jbl_bitset jbl_exception                         */
/*******************************************************************************************/
#include "jbl_ying.h"
#include "jbl_bitset.h"
#include "jbl_exception.h"
#include "jbl_log.h"
/*******************************************************************************************/
/*                            联动 jbl_stream                                               */
/*******************************************************************************************/
#include "jbl_stream.h"
jbl_uint64 __jbl_malloc_get_ignore_size();
#if JBL_MALLOC_FAST==1
/*******************************************************************************************/
/*                            系统文件包含                                                   */
/*******************************************************************************************/

#ifdef _WIN32
	#include <windows.h>		
	#include <wincrypt.h>
#elif __APPLE__
  #include <sys/mman.h>
#elif __linux__
	#include <unistd.h>
	#include <sys/mman.h>
#endif
/*******************************************************************************************/
/*                            结构体定义                                                   */
/*******************************************************************************************/
#if  jbl_bitset_bits==32
	#define jbl_malloc_fmap_len		16	//(512/jbl_bitset_bits)
#else
	#define jbl_malloc_fmap_len		8	//(512/jbl_bitset_bits)
#endif
typedef struct __jbl_malloc_free_slot		jbl_malloc_free_slot;
typedef struct __jbl_malloc_heap_struct		jbl_malloc_heap_struct;
typedef struct __jbl_malloc_huge_struct		jbl_malloc_huge_struct;
typedef struct __jbl_malloc_chunk_struct	jbl_malloc_chunk_struct;

typedef struct __jbl_malloc_free_slot
{
	struct __jbl_malloc_free_slot	*next;
}jbl_malloc_free_slot;
typedef struct __jbl_malloc_heap_struct
{
#if JBL_MALLOC_COUNT==1
	jbl_malloc_size_type size;			//实际使用
	jbl_malloc_size_type peak;			//峰值
	
	jbl_malloc_size_type applied_size;	//申请
	jbl_malloc_size_type applied_peak;	//申请峰值
#endif
	struct __jbl_malloc_huge_struct *huge_list;
	
	jbl_malloc_chunk_struct	*main_chunk;
	jbl_malloc_chunk_struct	*cached_chunk;
	
	jbl_malloc_free_slot		slot[30];
	
	jbl_uint32	cached_chunk_count;
}jbl_malloc_heap_struct;
typedef struct __jbl_malloc_huge_struct
{
	void *ptr;
	jbl_malloc_size_type size;
	struct __jbl_malloc_huge_struct *next;
}jbl_malloc_huge_struct;
typedef struct __jbl_malloc_chunk_struct
{
	struct __jbl_malloc_chunk_struct	*next;
	struct __jbl_malloc_chunk_struct	*pre;
	jbl_uint16						free_pages;		
	jbl_uint32						map[512];					//2KB=512*4
	jbl_bitset_type					fmap[jbl_malloc_fmap_len];	//16*32bit
}jbl_malloc_chunk_struct;
static const struct
{
	jbl_uint8 num;
	jbl_uint8 pages;
	jbl_uint16 size;
	jbl_uint16 count;
}jbl_malloc_small_bins[30]={{ 0,1,   8,512},{ 1,1,  16,256},{ 2,1,  24,170},{ 3,1,  32,128},{ 4,1,  40,102}, 
							{ 5,1,  48, 85},{ 6,1,  56, 73},{ 7,1,  64, 64},{ 8,1,  80, 51},{ 9,1,  96, 42},
							{10,1, 112, 36},{11,1, 128, 32},{12,1, 160, 25},{13,1, 192, 21},{14,1, 224, 18}, 
							{15,1, 256, 16},{16,5, 320, 64},{17,3, 384, 32},{18,1, 448,  9},{19,1, 512,  8}, 
							{20,5, 640, 32},{21,3, 768, 16},{22,2, 896,  9},{23,2,1024,  8},{24,5,1280, 16}, 
							{25,3,1536,  8},{26,7,1792, 16},{27,4,2048,  8},{28,5,2560,  8},{29,3,3072,  4}};
/*******************************************************************************************/
/*                            fast内存管理声明                                                   */
/*******************************************************************************************/
void*					__jbl_malloc_mmap			(jbl_malloc_size_type size);
void					__jbl_malloc_munmap			(void *ptr, jbl_malloc_size_type size);
void*					__jbl_malloc_aligned		(jbl_malloc_size_type size,jbl_malloc_size_type alignment);
void					__jbl_free_aligned			(void* ptr,jbl_malloc_size_type size);
void*					__jbl_malloc_chunk			();											//该函数不会操作size              但会操作applied_size
void					__jbl_free_chunk			(void *ptr);								//该函数不会操作size              但会操作applied_size
void					__jbl_free_chunks			();											//该函数不会操作size和applied_size
void					__jbl_free_cached_chunks	();											//该函数不会操作size              但会操作applied_size
void*					__jbl_malloc_page			(jbl_uint16 nums,jbl_uint8 type);			//该函数不会操作size和applied_size
jbl_uint16				__jbl_free_page				(void *ptr);								//该函数不会操作size和applied_size
void*					__jbl_malloc_large			(jbl_malloc_size_type size);				//该函数  会操作size              但不会操作applied_size
extern JBL_INLINE void		__jbl_free_large			(void *ptr);								//该函数  会操作size              但不会操作applied_size
void*					__jbl_malloc_small			(jbl_uint16 size);							//该函数  会操作size              但不会操作applied_size
void					__jbl_free_small			(void* ptr);								//该函数  会操作size              但不会操作applied_size
void					__jbl_free_smalls			();											//该函数不会操作size和applied_size
void*					__jbl_malloc_huge			(jbl_malloc_size_type size);				//该函数  会操作size和applied_size
jbl_uint8				__jbl_free_huge				(void* ptr);								//该函数  会操作size和applied_size
#define					is_aligned_2M(ptr)			((((jbl_malloc_size_type)(ptr))&0X1fffff)==0)
#define					aligned_to_2M(ptr)			((void*)(((jbl_malloc_size_type)(ptr))&(~0X1fffff)))
#define					is_aligned_4K(ptr)			((((jbl_malloc_size_type)(ptr))&(0XFFF))==0)
#define					aligned_to_4K(ptr)			((void*)(((jbl_malloc_size_type)(ptr))&(~0XFFF)))
#define					get_page_i(page,chunk)		(((jbl_malloc_size_type)((char*)page-(char*)chunk))>>12)
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
#if JBL_MALLOC_COUNT==1
	jbl_uint64 __jbl_malloc_count[3];
	jbl_uint64 __jbl_malloc_small_count[30];
#endif
jbl_malloc_heap_struct jbl_malloc_heap;
#if JBL_MALLOC_COUNT==1
jbl_malloc_size_type	jbl_malloc_used_size	(){return jbl_malloc_heap.size;}
#endif
/*******************************************************************************************/
/*                            以下函数完成内存管理组件启动和停止                           */
/*******************************************************************************************/
void jbl_malloc_start()
{
	jbl_malloc_heap.cached_chunk_count=0;
	jbl_malloc_heap.huge_list=NULL;
	jbl_malloc_heap.main_chunk=NULL;
	jbl_malloc_heap.cached_chunk=NULL;
#if JBL_MALLOC_COUNT==1
	jbl_malloc_heap.size=0;
	jbl_malloc_heap.peak=0;
	jbl_malloc_heap.applied_size=0;
	jbl_malloc_heap.applied_peak=0;
	__jbl_malloc_count[0]=0;
	__jbl_malloc_count[1]=0;
	__jbl_malloc_count[2]=0;
	for(unsigned char i=0;i<30;__jbl_malloc_small_count[i]=0,++i);
#endif
}
void jbl_malloc_stop()
{
#if JBL_STREAM_ENABLE==1
//输出内存统计
	jbl_stream_push_chars(jbl_stream_stdout,UC"\n\n");
#if JBL_MALLOC_COUNT==1
	jbl_stream_push_chars(jbl_stream_stdout,UC"Memory            :");jbl_stream_push_uint(jbl_stream_stdout,jbl_malloc_heap.size-__jbl_malloc_get_ignore_size());jbl_stream_push_chars(jbl_stream_stdout,UC"B\n");	
	jbl_stream_push_chars(jbl_stream_stdout,UC"Max memory        :");jbl_stream_push_uint(jbl_stream_stdout,jbl_malloc_heap.peak);jbl_stream_push_chars(jbl_stream_stdout,UC"B(");jbl_stream_push_double(jbl_stream_stdout,(double)jbl_malloc_heap.peak/1048576);jbl_stream_push_chars(jbl_stream_stdout,UC"M)\n");	
	jbl_stream_push_chars(jbl_stream_stdout,UC"Applied max memory:");jbl_stream_push_uint(jbl_stream_stdout,jbl_malloc_heap.applied_peak>>20);jbl_stream_push_chars(jbl_stream_stdout,UC"M\n");	
	jbl_uint64 n=__jbl_malloc_count[0]+__jbl_malloc_count[1]+__jbl_malloc_count[2];
	jbl_stream_push_chars(jbl_stream_stdout,UC"Small             :");jbl_stream_push_uint(jbl_stream_stdout,__jbl_malloc_count[0]);jbl_stream_push_chars(jbl_stream_stdout,UC" (");jbl_stream_push_double(jbl_stream_stdout,(double)__jbl_malloc_count[0]*100/n);jbl_stream_push_chars(jbl_stream_stdout,UC"%)\n");	
	jbl_stream_push_chars(jbl_stream_stdout,UC"Large             :");jbl_stream_push_uint(jbl_stream_stdout,__jbl_malloc_count[1]);jbl_stream_push_chars(jbl_stream_stdout,UC" (");jbl_stream_push_double(jbl_stream_stdout,(double)__jbl_malloc_count[1]*100/n);jbl_stream_push_chars(jbl_stream_stdout,UC"%)\n");	
	jbl_stream_push_chars(jbl_stream_stdout,UC"Huge              :");jbl_stream_push_uint(jbl_stream_stdout,__jbl_malloc_count[2]);jbl_stream_push_chars(jbl_stream_stdout,UC" (");jbl_stream_push_double(jbl_stream_stdout,(double)__jbl_malloc_count[2]*100/n);jbl_stream_push_chars(jbl_stream_stdout,UC"%)\n");	
	for(jbl_uint8 i=0;i<30;++i)
	{
		if(!__jbl_malloc_small_count[i])
			continue;
		jbl_stream_push_char(jbl_stream_stdout,'\t');
		jbl_stream_push_uint(jbl_stream_stdout,jbl_malloc_small_bins[i].size);
		jbl_stream_push_chars(jbl_stream_stdout,UC"B\t:");
		jbl_stream_push_uint(jbl_stream_stdout,__jbl_malloc_small_count[i]);
		jbl_stream_push_char(jbl_stream_stdout,'\n');
	}
#endif
//统计完成关流
	jbl_stream_do(jbl_stream_stdout,jbl_stream_force);
	jbl_stream_stdout=jbl_stream_free(jbl_stream_stdout);//强推，关闭
#endif
	for(;jbl_malloc_heap.huge_list;__jbl_free_huge(jbl_malloc_heap.huge_list->ptr));
	for(void*ptr;jbl_malloc_heap.main_chunk;ptr=jbl_malloc_heap.main_chunk->next,__jbl_free_aligned(jbl_malloc_heap.main_chunk,0X200000),jbl_malloc_heap.main_chunk=ptr/*,jbl_malloc_heap.applied_size-=0X200000*/);
	for(void*ptr;jbl_malloc_heap.cached_chunk;ptr=jbl_malloc_heap.cached_chunk->next,__jbl_free_aligned(jbl_malloc_heap.cached_chunk,0X200000),jbl_malloc_heap.cached_chunk=ptr/*,jbl_malloc_heap.applied_size-=0X200000*/);
}
/*******************************************************************************************/
/*                            fast内存管理实现                                                   */
/*******************************************************************************************/
void* jbl_malloc(jbl_malloc_size_type size)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!size)jbl_exception("MEMORY ERROR");
#endif
	void *ptr;
	if(size<=3072)//small
		ptr=__jbl_malloc_small(size);
	else if(size<=2093056)//large 2*1024*1024-4*1024(2M-4K)
		ptr=__jbl_malloc_large(size);
	else//huge
		ptr=__jbl_malloc_huge(size);
#if JBL_MALLOC_LOG ==1
	jbl_log(UC "addr:0X%X\tsize:%d",ptr,jbl_malloc_size(ptr));
#endif
	return ptr;
}
jbl_malloc_size_type jbl_malloc_size(void* ptr)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("NULL POINTER");	
#endif
	if(is_aligned_2M(ptr))//按照2M对齐，huge
		for(jbl_malloc_huge_struct *huge=jbl_malloc_heap.huge_list;huge;huge=huge->next)
			if(ptr==huge->ptr)
				return huge->size;
	void *page=aligned_to_4K(ptr);
	jbl_malloc_chunk_struct *chunk=aligned_to_2M(page);
	jbl_uint16 i=get_page_i(page,chunk);
//	printf("%d 0X%X 0X%X 0X%X %d %X\n",__LINE__,ptr,page,chunk,i,chunk->map[i]);
	if(chunk->map[i]&0X20000000)//small
		return jbl_malloc_small_bins[chunk->map[i]&0X1F].size;
	return (chunk->map[i]&0X1ff)<<12;//large
}
void* jbl_realloc(void* ptr,jbl_malloc_size_type size)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("NULL POINTER");	
	if(size==0)jbl_exception("MEMORY ERROR");
#endif
	jbl_malloc_size_type size_now=jbl_malloc_size(ptr);
	if(size_now>size)
		return ptr;
	
	if(3072<size_now&&size_now<=2093056&&3072<size&&size<=2093056)//全在large
	{
#if JBL_MALLOC_COUNT==1		
		++__jbl_malloc_count[1];
#endif
		jbl_malloc_chunk_struct *chunk=aligned_to_2M(ptr);
		jbl_uint16 i=get_page_i(ptr,chunk);
		jbl_uint16 n=chunk->map[i]&(0X1FF);				//有的
		jbl_uint16 page=((size&(0X1FF))!=0)+(size>>12);	//现在需要page个page
		if(jbl_bitset_find1(chunk->fmap,i+n,jbl_malloc_fmap_len)>=i+page)
		{
#if JBL_MALLOC_COUNT ==1
			jbl_malloc_heap.size+=((page-n)<<12);/*新增page-n个page*/jbl_max_update(jbl_malloc_heap.peak,jbl_malloc_heap.size);
#endif
			//在chunk第i个位置标记page个large类型的内存块
			jbl_uint32 tmp=(0X40000000)|page;
			jbl_bitset_set(chunk->fmap,i+n,page-n);
			for(jbl_uint16 j=0;j<page;++j)
				chunk->map[i+j]=tmp|(j<<10);//[30,29]U[19,10]U[9,0]
#if JBL_MALLOC_LOG ==1
			jbl_log(UC "addr:0X%X\tto addr:0X%X\tsize:%d",ptr,ptr,jbl_malloc_size(ptr));
#endif
			return ptr;
		}
	}
	
	void * ptr2=jbl_malloc(size);
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr2)jbl_exception("MEMORY ERROR");	
#endif
	jbl_malloc_size_type size_new=jbl_malloc_size(ptr2);
	jbl_min_update(size_new,size_now);
	jbl_memory_copy(ptr2,ptr,size_new);
#if JBL_MALLOC_LOG ==1
	jbl_log(UC "addr:0X%X\tto addr:0X%X\tsize:%d",ptr,ptr2,jbl_malloc_size(ptr2));
#endif
//重写free减少查表
	if(size_now<=3072)//small
		__jbl_free_small(ptr);
	else if(size_now<=2093056)//large 2*1024*1024-4*1024(2M-4K)
		__jbl_free_large(ptr);
	else//huge
		__jbl_free_huge(ptr);
		
	return ptr2;	
}
void jbl_free(void * ptr)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("NULL POINTER");
#endif
#if JBL_MALLOC_LOG ==1
	jbl_log(UC "addr:0X%X",ptr);
#endif
	if(!is_aligned_4K(ptr))//没有4k，small
		return __jbl_free_small(ptr);
	if(is_aligned_2M(ptr))//按照2M对齐，因为large的第一个page被用于管理自身，所以一定不会用这个函数释放
		if(__jbl_free_huge(ptr))
			return;
	void *page=aligned_to_4K(ptr);
	jbl_malloc_chunk_struct *chunk=aligned_to_2M(page);
	jbl_uint16 i=get_page_i(page,chunk);
//	printf("%d 0X%X 0X%X 0X%X %d %X\n",__LINE__,ptr,page,chunk,i,chunk->map[i]);
	if(chunk->map[i]&0X20000000)//small
		return __jbl_free_small(ptr);
	__jbl_free_large(ptr);
}
//申请一段mmap
void* __jbl_malloc_mmap(jbl_malloc_size_type size)
{
#ifdef _WIN32
	void* ptr=VirtualAlloc(NULL,size,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("MEMORY ERROR");
#endif
	return ptr;
#elif defined(__APPLE__) || defined(__linux__)
	void* ptr=mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON,-1,0);
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(ptr==MAP_FAILED)jbl_exception("MEMORY ERROR");
#endif
	return ptr;		
#endif
}
//释放指定尺寸的一部分mmap
void __jbl_malloc_munmap(void *ptr, jbl_malloc_size_type size)
{
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("NULL POINTER");	
#endif
#ifdef _WIN32
	if(VirtualFree(ptr,0,MEM_RELEASE)==0)	jbl_exception("MEMORY ERROR");
#elif defined(__APPLE__) || defined(__linux__)
	if(munmap(ptr,size)!=0)					jbl_exception("MEMORY ERROR");
#endif
}
//对齐方式申请mmap
void *__jbl_malloc_aligned(jbl_malloc_size_type size,jbl_malloc_size_type alignment)
{
#ifdef _WIN32
	void *ptr=__jbl_malloc_mmap(size+alignment-1);
	jbl_malloc_size_type offset=(((jbl_malloc_size_type)(ptr))&((alignment)-1));
	VirtualFree(ptr,0,MEM_RELEASE);		
	if(offset!=0)
	{
		offset=alignment-offset;
		ptr=VirtualAlloc((char*)ptr+offset,size,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	}
	else
		ptr=VirtualAlloc(ptr,size,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
#elif defined(__APPLE__) || defined(__linux__)
	void *ptr=__jbl_malloc_mmap(size+alignment-1);
	jbl_malloc_size_type offset=(((jbl_malloc_size_type)(ptr))&((alignment)-1));
	if(offset!=0)
	{
		offset=alignment-offset;
		__jbl_malloc_munmap(ptr,offset);
		ptr=(char*)ptr+offset;
		alignment-=offset;
	}
	if(alignment>0)
		__jbl_malloc_munmap(ptr+size,alignment-1);
#endif
#if JBL_MALLOC_NULL_PTR_CHECK ==1
	if(!ptr)jbl_exception("MEMORY ERROR");
#endif
	return ptr;
}
//释放指定尺寸的对齐过的mmap
JBL_INLINE void __jbl_free_aligned(void* ptr,jbl_malloc_size_type size)
{
	__jbl_malloc_munmap(ptr,size);
}
//申请一个chunk
void *__jbl_malloc_chunk()
{
	void *ptr;
	jbl_malloc_chunk_struct *chunk;
	if(jbl_malloc_heap.cached_chunk)									//有缓冲chunk
	{
		--jbl_malloc_heap.cached_chunk_count;							//更新计数
		ptr=jbl_malloc_heap.cached_chunk;								//获取
		chunk=ptr;														//保存为chunk结构
		if(chunk->next)chunk->next->pre=chunk->pre;						//从缓冲chunk链表中移除
		if(!chunk->pre)jbl_malloc_heap.cached_chunk=chunk->next;		//从缓冲chunk链表中移除
		else           chunk->pre->next=chunk->next;					//从缓冲chunk链表中移除
		chunk->next=chunk->pre=NULL;									//清空链表指针
	}
	else
	{
		ptr=__jbl_malloc_aligned(0X200000,0X200000);					//对齐申请2M
#if JBL_MALLOC_COUNT ==1
		jbl_malloc_heap.applied_size+=0X200000;jbl_max_update(jbl_malloc_heap.applied_peak,jbl_malloc_heap.applied_size);//更新申请内存
#endif
		chunk=ptr;														//保存为chunk结构
	}
	for(register jbl_uint16 i=0;i<512;chunk->map[i]=0,++i);				//初始化标记区
	jbl_bitset_init(chunk->fmap,jbl_malloc_fmap_len);					//初始化标记区
	chunk->free_pages=511;												//第一个page保存chunk_struct
	chunk->map[0]=0X40000000|0X01;										//维护map标记
	jbl_bitset_set(chunk->fmap,0,1);									//维护map标记
	if(jbl_malloc_heap.main_chunk)jbl_malloc_heap.main_chunk->pre=ptr;	//插入链表
	chunk->next=jbl_malloc_heap.main_chunk;chunk->pre=NULL;jbl_malloc_heap.main_chunk=chunk;//插入链表
	return ptr;
}
//释放一个chunk
void __jbl_free_chunk(void *ptr)
{
	jbl_malloc_chunk_struct *chunk=ptr;						//保存为chunk结构
	if(chunk->next)chunk->next->pre=chunk->pre;				//从主chunk链表中移除
	if(chunk->pre) chunk->pre->next=chunk->next;			//从主chunk链表中移除
	else          jbl_malloc_heap.main_chunk=chunk->next;	//从主chunk链表中移除
/*TODO:自动决策是否放进缓存 */
//	if(false)
//	{//不用缓存,直接释放
//		__jbl_free_aligned(ptr,0X200000);
//	jbl_malloc_heap.applied_size-=0X200000;
//	}
//	else
//	{
		++jbl_malloc_heap.cached_chunk_count;														//增加缓冲统计
		if(jbl_malloc_heap.cached_chunk)jbl_malloc_heap.cached_chunk->pre=ptr;						//插入缓冲chunk链表
		chunk->next=jbl_malloc_heap.cached_chunk;chunk->pre=NULL;jbl_malloc_heap.cached_chunk=ptr;	//插入缓冲chunk链表
//	}
}
//回收可以回收的chunk
void __jbl_free_chunks()
{
	for(jbl_malloc_chunk_struct *chunk=jbl_malloc_heap.main_chunk,*chunk2;chunk;)	//遍历表
	{
		if(chunk->fmap[0]!=(1LL<<(jbl_bitset_bits-1)))goto not_free;				//第一个块除系统标记page有其他块在被占用
		for(jbl_uint8 i=1;i<jbl_malloc_fmap_len;++i)if(chunk->fmap[i])goto not_free;//其他块有占用
		chunk2=chunk->next;__jbl_free_chunk(chunk);chunk=chunk2;					//释放当前chunk
		continue;
not_free:;
		chunk=chunk->next;
	}	
}
//释放缓冲的chunk
void __jbl_free_cached_chunks()
{
	for(void*ptr;jbl_malloc_heap.cached_chunk;ptr=jbl_malloc_heap.cached_chunk->next,__jbl_free_aligned(jbl_malloc_heap.cached_chunk,0X200000),jbl_malloc_heap.cached_chunk=ptr)//遍历,释放更新申请内存
#if JBL_MALLOC_COUNT ==1
		jbl_malloc_heap.applied_size-=0X200000;
#endif
	;
	jbl_malloc_heap.cached_chunk_count=0;//更新计数
}
//申请一个page(从chunk中找)
void *__jbl_malloc_page(jbl_uint16 nums,jbl_uint8 type)//type为0XFF用于large，type为其他用于small
{
	void * ptr=NULL;
	jbl_uint16 cnt0=-1,i0=0;
	jbl_malloc_chunk_struct *chunk0=NULL;
	for(jbl_malloc_chunk_struct *chunk=jbl_malloc_heap.main_chunk;chunk;chunk=chunk->next)//遍历表
		if(chunk->free_pages>=nums)										//剩余页足够
 			for(jbl_uint16 i=0,cnt1=0,j=0;i<512;++i)					//遍历所有页
			{
				j=jbl_bitset_find0(chunk->fmap,i,jbl_malloc_fmap_len);	//找到第一个0
				i=jbl_bitset_find1(chunk->fmap,j,jbl_malloc_fmap_len);	//找到0后的第一个1
				cnt1=i-j;												//两者做差是这一段空区间的长度
				if(cnt1>=nums&&cnt1<cnt0)								//够了，并且比已有的较小
					cnt0=cnt1,i0=j,chunk0=chunk;						//更新
			}
	if(!chunk0)chunk0=__jbl_malloc_chunk(),i0=1;						//chunk为空说明未找到，申请一个chunk，并从第1个page开始分配
	chunk0->free_pages-=nums;											//更新剩余的page
	//在chunk0第i0个位置标记nums个type类型的内存块
	jbl_bitset_set(chunk0->fmap,i0,nums);								//标记占用
	jbl_uint32 tmp;
	//[ 9, 0]在large中表示有几个连续page
	//[ 9, 0]在small中表示type
	//[19,10]表示当前page是第几个连续的page
	//[29,20]用于回收small时计数
	//[31,30]表示page类型 10b是large 01b是第一个small 11b是其余的small
	if(type==0XFF)
		chunk0->map[i0]=tmp=(0X40000000)|nums;							//标记[31,30]U[9,0] 10b<<29
	else
		chunk0->map[i0]=(0X20000000)|(type),							//标记[31,30]U[9,0] 01b<<29
		tmp=(0X60000000)|(type);										//                  11b<<29
	for(jbl_uint16 i=1;i<nums;++i)
		chunk0->map[i+i0]=tmp|(i<<10);									//标记[31,30]U[19,10]U[9,0]
	
	ptr=chunk0;															//获取chunk基址
	ptr=(char*)ptr+(i0<<12);											//偏移
	return ptr;															//返回
}
//释放一个page
jbl_uint16 __jbl_free_page(void *ptr)
{
	jbl_malloc_chunk_struct *chunk=aligned_to_2M(ptr);	//因为chunk按照2M对齐，这样可以直接获得chunk头所在位置
	jbl_uint16 i=get_page_i(ptr,chunk);						//计算page编号
	jbl_uint16 n=chunk->map[i]&(0X1FF);													//获取连续page数量
	chunk->free_pages+=n;																//标记以释放
	if(chunk->map[i]&0X20000000)n=jbl_malloc_small_bins[n].pages;						//如果是small n指的是type，取到num	
	jbl_bitset_reset(chunk->fmap,i,n);													//维护二进制表
	return n;
}
//申请一个large内存
void* __jbl_malloc_large(jbl_malloc_size_type size)
{
	jbl_uint16 page=((size&(0XFFF))!=0)+(size>>12);												//4K对齐并计算所需page个数
#if JBL_MALLOC_COUNT ==1
	++__jbl_malloc_count[1];																	//计数
	jbl_malloc_heap.size+=(page<<12);jbl_max_update(jbl_malloc_heap.peak,jbl_malloc_heap.size);	//更新内存占用
#endif
	return __jbl_malloc_page(page,0XFF);														//申请，返回
}
//释放一个large内存
JBL_INLINE void __jbl_free_large(void *ptr)
{
#if JBL_MALLOC_COUNT ==1
	jbl_malloc_heap.size-=(__jbl_free_page(ptr)<<12);
#else
	__jbl_free_page(ptr);
#endif
}
//申请small内存(从缓冲表和page中获取)
void* __jbl_malloc_small(jbl_uint16 size)
{	
	jbl_uint8 type=0;
	for(jbl_uint8 i=0;i<30;++i)					//查找类型
		if(jbl_malloc_small_bins[i].size>=size)	//最小尺寸
			{type=i;break;}						//找到了
#if JBL_MALLOC_COUNT==1
	++__jbl_malloc_count[0];
	++__jbl_malloc_small_count[type];			//计数
	jbl_malloc_heap.size+=jbl_malloc_small_bins[type].size;jbl_max_update(jbl_malloc_heap.peak,jbl_malloc_heap.size);	//更新内存占用
#endif
	jbl_uint8 num=jbl_malloc_small_bins[type].num;								//获取占用的缓冲槽编号
	if(jbl_malloc_heap.slot[num].next)											//有缓冲
	{
		void *ptr=jbl_malloc_heap.slot[num].next;								//取出
		jbl_malloc_heap.slot[num].next=jbl_malloc_heap.slot[num].next->next;	//断开链表
		return ptr;																//返回
	}
	void *ptr=__jbl_malloc_page(jbl_malloc_small_bins[type].pages,type);		//没有缓冲就申请一个新的页
	for(jbl_uint16 i=1,n=jbl_malloc_small_bins[type].count;i<n;++i)				//除了最后一个逐一加入
	{
		((jbl_malloc_free_slot*)ptr)->next=jbl_malloc_heap.slot[num].next;		//头插法插入
		jbl_malloc_heap.slot[num].next=ptr;										//头插法插入
		ptr=(char*)ptr+jbl_malloc_small_bins[type].size;						//计算偏移
	}
	return ptr;																	//返回最后一个
}
//释放small内存(把small内存加入缓冲表)
void __jbl_free_small(void* ptr)
{
	void *page=aligned_to_4K(ptr);					//计算page地址
	jbl_malloc_chunk_struct *chunk=aligned_to_2M(page);//计算chunk地址
	jbl_uint16 i=get_page_i(page,chunk);				//计算page编号
	jbl_uint8 type=chunk->map[i]&0X1F;											//获取类型
	jbl_uint8 num=jbl_malloc_small_bins[type].num;								//获取槽编号
#if JBL_MALLOC_COUNT ==1
	jbl_malloc_heap.size-=jbl_malloc_small_bins[type].size;						//更新内存占用
#endif
	((jbl_malloc_free_slot*)ptr)->next=jbl_malloc_heap.slot[num].next;			//头插法插入
	jbl_malloc_heap.slot[num].next=ptr;											//头插法插入
//	__jbl_free_smalls();														//极为激进的释放策略
}
//释放缓冲的small
void __jbl_free_smalls()
{
	for(jbl_uint8 type=0;type<30;++type)										//遍历30个槽
	{
		for(jbl_malloc_free_slot *ptr=jbl_malloc_heap.slot[type].next;ptr;)		//遍历每一个small
		{
			void *page=aligned_to_4K(ptr);			//计算page地址
			jbl_malloc_chunk_struct *chunk=aligned_to_2M(page);//计算chunk地址
			jbl_uint16 i=get_page_i(page,chunk);		//计算page编号
			jbl_uint16 fi=i-(jbl_uint32)((chunk->map[i]>>10)&0X1FF);			//计算father page编号
			jbl_uint16 cnt=(jbl_uint32)((chunk->map[fi]>>20)&0X1FF);			//获取已经释放的small类型数量
			++cnt;																//加一释放当前
			//printf("0X%X 0X%X 0X%X %d %d %d %d\n",ptr,page,chunk,i,fi,type,cnt);
			if(cnt==jbl_malloc_small_bins[type].count)							//如果small都闲置，把father page所属的small全部移出slot，并标记释放所有page
			{
				//printf("FREE 0X%X 0X%X 0X%X %d %d %d\n",ptr,page,chunk,i,fi,type);
				for(jbl_malloc_free_slot *ptr2=jbl_malloc_heap.slot[type].next,*ptr3=NULL;ptr2;)	//遍历每一个small
				{
					void *page2=(void*)(((jbl_malloc_size_type)ptr2)&(~0XFFF));						//计算page
					jbl_malloc_chunk_struct *chunk2=(void*)(((jbl_malloc_size_type)page2)&(~0X1fffff));	//计算chunk地址
					jbl_uint16 i2=((jbl_malloc_size_type)((char*)page2-(char*)chunk2))>>12;				//计算page编号
					jbl_uint16 fi2=i2-(jbl_uint32)((chunk2->map[i2]>>10)&0X1FF);					//计算father page编号
					//printf("\t0X%X 0X%X 0X%X %d %d\n",ptr2,page2,chunk2,i2,fi2);
					jbl_uint8 flag=0;
					if(ptr2==ptr)ptr=ptr2->next,flag=1;												//在当前情况下,所有属于当前father page的small都在ptr之前，遍历到ptr说明都放完了，标记flag退出
					if(fi2==fi)																		//是当前father page
						if(ptr3)																	//前面有small
							(ptr3)->next=ptr2->next,ptr2=ptr2->next;								//从slot中删除ptr2，ptr3不动，ptr2下移
						else
							jbl_malloc_heap.slot[type].next=ptr2->next,ptr2=ptr2->next;				//从slot中删除ptr2，ptr3不动，ptr2下移
					else
						ptr3=ptr2,ptr2=ptr2->next;													//ptr2,ptr3下移
					if(flag)break;																	//有标记就退出
				}
				__jbl_free_page((char*)chunk+(fi<<12));												//释放page
				continue;																			//因为上面维护了ptr，直接continue
			}
			//因为上面continue了,运行至此，一定是槽没全释放
			chunk->map[fi]=0X20000000|type|(cnt<<20);												//维护fi的计数
			ptr=ptr->next;																			//下移ptr
		}
		for(jbl_malloc_free_slot *ptr=jbl_malloc_heap.slot[type].next;ptr;ptr=ptr->next)			//这些small没释放，充值father page的计数
		{
			void *page=aligned_to_4K(ptr);															//计算page地址
			jbl_malloc_chunk_struct *chunk=aligned_to_2M(page);										//计算chunk地址
			jbl_uint16 i=get_page_i(page,chunk);													//计算page编号
			jbl_uint16 fi=i-(jbl_uint32)((chunk->map[i]>>10)&0X1FF);								//计算father page编号
			chunk->map[fi]=0X20000000|type;															//重置
		}
	}
}
//申请一段huge内存
void *__jbl_malloc_huge(jbl_malloc_size_type size)
{
#if JBL_MALLOC_COUNT==1		
	++__jbl_malloc_count[2];															//计数
#endif
	jbl_malloc_huge_struct* this=__jbl_malloc_small((sizeof (jbl_malloc_huge_struct)));	//申请huge链表头
	this->size=size=(((size&(0XFFF))!=0)+(size>>12))<<12;								//4K对齐
	this->ptr=__jbl_malloc_aligned(size,0X200000);										//申请内存
	this->next=jbl_malloc_heap.huge_list,jbl_malloc_heap.huge_list=this;				//插入链表
#if JBL_MALLOC_COUNT ==1
	jbl_malloc_heap.size+=size;jbl_max_update(jbl_malloc_heap.peak,jbl_malloc_heap.size);							//更新内存占用
	jbl_malloc_heap.applied_size+=size;jbl_max_update(jbl_malloc_heap.applied_peak,jbl_malloc_heap.applied_size);	//更新内存申请
#endif
//	printf("M %0 16llX\n",this->ptr);
	return this->ptr;
}
//释放一段huge内存
jbl_uint8 __jbl_free_huge(void* ptr)
{
	for(jbl_malloc_huge_struct *huge=jbl_malloc_heap.huge_list,*pre=NULL;huge;pre=huge,huge=huge->next)//查找整个链表
		if(ptr==huge->ptr)//找到
		{
//			printf("F %0 16llX\n",huge->ptr);
			if(pre)pre->next=huge->next;					//链表删除
			else   jbl_malloc_heap.huge_list=huge->next;	//链表删除
			__jbl_free_aligned(huge->ptr,huge->size);		//释放
#if JBL_MALLOC_COUNT ==1
			jbl_malloc_heap.applied_size-=huge->size;		//更新内存占用
			jbl_malloc_heap.size-=huge->size;				//更新内存申请
#endif
			__jbl_free_small(huge);							//释放huge链表头
			return true;
		}
	jbl_log(UC "HUGE ERROR");
	return false;
}
#endif




#endif
