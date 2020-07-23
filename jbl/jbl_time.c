/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_time.h"
#if JBL_TIME_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_gc jbl_malloc jbl_ying jbl_exception                */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_gc.h"
#include "jbl_malloc.h"
#include "jbl_ying.h"
#include <sys/time.h>
/*******************************************************************************************/
/*                            联动jbl_string jbl_var                                       */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_var.h"
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
jbl_int8		jbl_time_time_zone=JBL_TIME_DEFAULT_TIME_ZONE;
/*******************************************************************************************/
/*                            以下函实现时间基本操作                                     */
/*******************************************************************************************/
inline jbl_time * jbl_time_new()
{
	return jbl_time_init(jbl_malloc(sizeof(jbl_time)));
}
inline jbl_time * jbl_time_init(jbl_time *this)
{
	jbl_gc_init(this);
	jbl_gc_plus(this);
	this->t=0;
	return this;
}
inline jbl_time * jbl_time_free(jbl_time *this)
{
	if(this==NULL)return NULL;
	jbl_gc_minus(this);
	if(!jbl_gc_refcnt(this))
	{
		((jbl_gc_is_ref(this))?jbl_time_free((jbl_time *)jbl_refer_pull(this)):0);
#if JBL_VAR_ENABLE==1
		if(jbl_gc_is_var(this))
			jbl_free((char*)this-sizeof(jbl_var));
		else
#endif
			jbl_free(this);
	}
	return NULL;
}
inline jbl_time * jbl_time_set(jbl_time * this,jbl_uint64 time)
{
	if(!this)this=jbl_time_new();
	((jbl_time *)(jbl_refer_pull(this)))->t=time;
	return this;
}
inline jbl_time * jbl_time_copy(jbl_time * that)
{
	if(that==NULL)return NULL;
#if JBL_VAR_ENABLE==1
	if(jbl_gc_is_var(that))
		return jbl_time_set(jbl_Vtime(jbl_Vtime_new()),((jbl_time*)jbl_refer_pull(that))->t);
	else
#endif
	return jbl_time_set(NULL,((jbl_time*)jbl_refer_pull(that))->t);
}
/*******************************************************************************************/
/*                            以下函实现时间获取操作                                     */
/*******************************************************************************************/
inline jbl_time * jbl_time_now(jbl_time *this)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return jbl_time_set(this,(((jbl_int64)tv.tv_sec)*1000)+(tv.tv_usec/1000));
}
void jbl_time_decode(const jbl_time *this,jbl_time_decoded *tt)
{
	if(this==NULL)return;
	jbl_int64 ts=jbl_time_to_unix(this);
	int days=0,dayTmp=0,secs=0;
	if(jbl_time_time_zone==JBL_TIME_ZONE_ASIA_SHANGHAI)
		ts+=8*3600000;
	if(ts>0)
	{
		tt->ms=ts%1000,ts/=1000,days=ts/86400,secs=ts%86400;
		for(tt->year=1970;days>0;++tt->year)
			if(days>=(dayTmp=(365+jbl_time_if_leap_year(tt->year))))
				days-=dayTmp;
			else
				break;
		for(tt->month=1;tt->month<12;++tt->month)
			if(days>=(dayTmp=jbl_time_get_day_of_month(tt->month,tt->year)))
				days-=dayTmp;
			else
				break;
		tt->month=tt->month,tt->day=days+1;
	}
	else
	{
		tt->ms=(1000-(-ts)%1000),ts=((ts-(1000-tt->ms))/1000),((tt->ms==1000)?(tt->ms=0):0),secs=-(ts%86400),secs=(secs==0)?0:(86400-secs),days=(ts-secs)/86400;
		for(tt->year=1969;days<0;--tt->year)
			if((-days)>=(dayTmp=(365+jbl_time_if_leap_year(tt->year))))
				days+=dayTmp;
			else
				break;
		for(tt->month=12;tt->month>0;--tt->month)
			if(days<(dayTmp=jbl_time_get_day_of_month(tt->month,tt->year)))
				days+=dayTmp;
			else
				break;
		if(days>jbl_time_get_day_of_month(tt->month+1,tt->year))
			++tt->month,days-=jbl_time_get_day_of_month(tt->month,tt->year);
		tt->month=tt->month+1,tt->day=days+1;
	}
	tt->hour=secs/3600,secs%=3600,tt->minute=secs/60,tt->second=secs%60;	
}
/*******************************************************************************************/
/*                            以下函实现时间比较操作                                      */
/*******************************************************************************************/
inline char jbl_time_space_ship(jbl_time *this,jbl_time *that)
{
	jbl_uint64 t=jbl_time_minus(this,that);
	return (t>0)?1:((t<0)?(-1):0);
}
/*******************************************************************************************/
/*                            以下函实现特殊时间判断操作                                 */
/*******************************************************************************************/
inline jbl_uint8	jbl_time_if_leap_year		(jbl_uint32 year){return (((year%400)==0)?1:(((year%100)==0)?0:(((year%4)==0)?1:0)));}
inline jbl_uint8	jbl_time_get_day_of_month	(jbl_uint8 month,jbl_uint32 year){static const jbl_uint8 dpm[12]={31,28,31,30,31,30,31,31,30,31,30,31};return (month==2||(month==0)||(month>12))?(dpm[1]+jbl_time_if_leap_year(year)):(dpm[month-1]);}
inline jbl_uint8	jbl_time_get_weekday		(jbl_uint8 day,jbl_uint8 month,jbl_uint32 year){if((month==1)||(month==2))month+=12,year--;return (day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7+1;}
jbl_uint8 jbl_time_if_dst(jbl_uint8 day,jbl_uint8 month,jbl_uint32 year)
{
	if(jbl_time_time_zone==JBL_TIME_ZONE_ASIA_SHANGHAI)
	{
		
		
		
	}
	
}


#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现时间对字符串操作                                */
/*******************************************************************************************/
jbl_string * jbl_time_to_string(const jbl_time *this,jbl_string *result)
{
	jbl_time_decoded tt;
	jbl_time_decode(this,&tt);
	result=jbl_string_extend(result,64);
	result=jbl_string_add_uint64_length(result,tt.year,4,'0');
	result=jbl_string_add_char(result,'-');
	result=jbl_string_add_uint64_length(result,tt.month,2,'0');
	result=jbl_string_add_char(result,'-');
	result=jbl_string_add_uint64_length(result,tt.day,2,'0');
	result=jbl_string_add_char(result,' ');
	result=jbl_string_add_uint64_length(result,tt.hour,2,'0');
	result=jbl_string_add_char(result,':');
	result=jbl_string_add_uint64_length(result,tt.minute,2,'0');
	result=jbl_string_add_char(result,':');
	result=jbl_string_add_uint64_length(result,tt.second,2,'0');
	result=jbl_string_add_char(result,'.');
	result=jbl_string_add_uint64_length(result,tt.ms,3,'0');
	result=jbl_string_add_char(result,' ');
	
	result=jbl_string_add_uint64_length(result,jbl_time_get_weekday(tt.day,tt.month,tt.year),1,'0');
	return result;
}
#endif
#if JBL_JSON_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现时间JSON操作                                      */
/*******************************************************************************************/
#if JBL_STRING_ENABLE==1
inline jbl_string* jbl_time_json_encode(const jbl_time* this,jbl_string *out,char format,jbl_int32 tabs)
{
	jbl_string *s1=jbl_time_to_string(this,NULL);
	out=jbl_string_json_encode(s1,out,format,tabs);
	jbl_string_free(s1);
	return out;
}
#endif
#if JBL_STREAM_ENABLE==1
void jbl_time_json_put(const jbl_time* this,jbl_stream *out,char format,jbl_int32 tabs)
{
#if JBL_STRING_ENABLE==1
	jbl_string *s1=jbl_time_to_string(this,NULL);
	jbl_string_json_put(s1,out,format,tabs);
	jbl_string_free(s1);	
#else
	if(jbl_stream_json_put_format(this=jbl_refer_pull(this),out,format,&tabs))return;
	jbl_stream_push_int(out,this->t);
#endif
}
#endif

#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现时间的浏览操作                                 */
/*******************************************************************************************/
void jbl_time_view_put(const jbl_time* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs)
{
	if(jbl_stream_view_put_format(this=jbl_refer_pull(this),out,"jbl_time      ",format,str,&tabs))return;
	jbl_stream_push_chars(out,UC"\ttime stamp:");
	jbl_stream_push_int(out,this->t);
#if JBL_STRING_ENABLE==1
	jbl_stream_push_chars(out,UC"\tdate:");
	jbl_string *s1=jbl_time_to_string(this,NULL);
	jbl_stream_push_string(out,s1);
	jbl_string_free(s1);
#endif
}
#endif

#if JBL_VAR_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现时间的var操作                                    */
/*******************************************************************************************/
jbl_var_operators_new(jbl_time_operators,jbl_time_free,jbl_time_copy,jbl_time_space_ship,jbl_time_json_encode,jbl_time_view_put,jbl_time_json_put);
inline jbl_time* jbl_Vtime(jbl_var * this){if(this&&!Vis_jbl_time(this))jbl_exception("VAR TYPE ERROR");return((jbl_time*)this);}
jbl_var * jbl_Vtime_new()
{
	jbl_var *this=(jbl_var*)((char*)(jbl_malloc((sizeof(jbl_time))+(sizeof(jbl_var)))+(sizeof(jbl_var))));
	jbl_time_init((jbl_time*)this);
	jbl_gc_set_var((jbl_time*)this);
	jbl_var_set_operators(this,&jbl_time_operators);
	return this;
}
jbl_var * jbl_time_copy_as_var(jbl_time *that)
{
	if(that==NULL)return NULL;
	if(jbl_gc_is_var(that))
		return jbl_V(jbl_time_copy(that));
	if(jbl_gc_is_ref(that))
	{
		void *data=that;
		jbl_refer_as_var(&data,&jbl_time_operators);
		jbl_gc_minus((jbl_reference*)data);//出来的data ref_cnt一定是2
		return jbl_V(data);
	}	
	return jbl_V(jbl_time_set(jbl_Vtime(jbl_Vtime_new()),((jbl_time*)jbl_refer_pull(that))->t));
}
#endif
#endif
