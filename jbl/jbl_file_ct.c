/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_file_ct.h"
#if JBL_FILE_ENABLE==1
#include "jbl_log.h"
#include "jbl_var.h"
#define jbl_fctdst jbl_uint8			//juruoyun basic lib file content type data size type
static struct
{
	jbl_fctdst size;
	struct
	{
		jbl_uint8		ctid;
		jbl_uint8		ctl:5;
		jbl_uint8		sul:3;
		unsigned char	ct[32];
		unsigned char	su[8];
		
		
	}d[147];
}
ctd={
	147,
	{
		{JBL_FILE_CT_UNKNOW,0,0,"",""},
		{JBL_FILE_CT_MP2,9,3,"audio/mp2","mp2"},
		{JBL_FILE_CT_MP3,9,3,"audio/mp3","mp3"},
		{JBL_FILE_CT_MP4,9,3,"video/mp4","mp4"},
		{JBL_FILE_CT_ODA,15,3,"application/oda","oda"},
		{JBL_FILE_CT_MPGA,10,4,"audio/mpeg","mpga"},
		{JBL_FILE_CT_LHA,24,3,"application/octet-stream","lha"},
		{JBL_FILE_CT_RA,17,2,"audio/x-realaudio","ra"},
		{JBL_FILE_CT_PDB,14,3,"chemical/x-pdb","pdb"},
		{JBL_FILE_CT_RGB,11,3,"image/x-rgb","rgb"},
		{JBL_FILE_CT_AIFC,12,4,"audio/x-aiff","aifc"},
		{JBL_FILE_CT_WMLC,24,4,"application/vnd.wap.wmlc","wmlc"},
		{JBL_FILE_CT_NC,20,2,"application/x-netcdf","nc"},
		{JBL_FILE_CT_DOC,18,3,"application/msword","doc"},
		{JBL_FILE_CT_SV4CRC,20,6,"application/x-sv4crc","sv4crc"},
		{JBL_FILE_CT_SRC,25,3,"application/x-wais-source","src"},
		{JBL_FILE_CT_ASC,10,3,"text/plain","asc"},
		{JBL_FILE_CT_WMLSC,30,5,"application/vnd.wap.wmlscriptc","wmlsc"},
		{JBL_FILE_CT_VCD,20,3,"application/x-cdlink","vcd"},
		{JBL_FILE_CT_MID,10,3,"audio/midi","mid"},
		{JBL_FILE_CT_SKD,18,3,"application/x-koan","skd"},
		{JBL_FILE_CT_SND,11,3,"audio/basic","snd"},
		{JBL_FILE_CT_XWD,19,3,"image/x-xwindowdump","xwd"},
		{JBL_FILE_CT_ICE,23,3,"x-conference/x-cooltalk","ice"},
		{JBL_FILE_CT_MOVIE,17,5,"video/x-sgi-movie","movie"},
		{JBL_FILE_CT_ME,22,2,"application/x-troff-me","me"},
		{JBL_FILE_CT_JPE,10,3,"image/jpeg","jpe"},
		{JBL_FILE_CT_MPE,10,3,"video/mpeg","mpe"},
		{JBL_FILE_CT_EXE,24,3,"application/octet-stream","exe"},
		{JBL_FILE_CT_CDF,20,3,"application/x-netcdf","cdf"},
		{JBL_FILE_CT_HDF,17,3,"application/x-hdf","hdf"},
		{JBL_FILE_CT_PDF,15,3,"application/pdf","pdf"},
		{JBL_FILE_CT_IEF,9,3,"image/ief","ief"},
		{JBL_FILE_CT_AIFF,12,4,"audio/x-aiff","aiff"},
		{JBL_FILE_CT_TIFF,10,4,"image/tiff","tiff"},
		{JBL_FILE_CT_ROFF,19,4,"application/x-troff","roff"},
		{JBL_FILE_CT_AIF,12,3,"audio/x-aiff","aif"},
		{JBL_FILE_CT_GIF,9,3,"image/gif","gif"},
		{JBL_FILE_CT_MIF,19,3,"application/vnd.mif","mif"},
		{JBL_FILE_CT_TIF,9,3,"image/tif","tif"},
		{JBL_FILE_CT_RTF,8,3,"text/rtf","rtf"},
		{JBL_FILE_CT_SWF,29,3,"application/x-shockwave-flash","swf"},
		{JBL_FILE_CT_JPEG,10,4,"image/jpeg","jpeg"},
		{JBL_FILE_CT_MPEG,10,4,"video/mpeg","mpeg"},
		{JBL_FILE_CT_PNG,9,3,"image/png","png"},
		{JBL_FILE_CT_JPG,10,3,"image/jpeg","jpg"},
		{JBL_FILE_CT_MPG,10,3,"video/mpeg","mpg"},
		{JBL_FILE_CT_SH,16,2,"application/x-sh","sh"},
		{JBL_FILE_CT_CSH,17,3,"application/x-csh","csh"},
		{JBL_FILE_CT_MESH,10,4,"model/mesh","mesh"},
		{JBL_FILE_CT_MSH,10,3,"model/mesh","msh"},
		{JBL_FILE_CT_LZH,24,3,"application/octet-stream","lzh"},
		{JBL_FILE_CT_AI,22,2,"application/postscript","ai"},
		{JBL_FILE_CT_MIDI,10,4,"audio/midi","midi"},
		{JBL_FILE_CT_SMI,16,3,"application/smil","smi"},
		{JBL_FILE_CT_AVI,15,3,"video/x-msvideo","avi"},
		{JBL_FILE_CT_DVI,17,3,"application/x-dvi","dvi"},
		{JBL_FILE_CT_TEXI,21,4,"application/x-texinfo","texi"},
		{JBL_FILE_CT_TCL,17,3,"application/x-tcl","tcl"},
		{JBL_FILE_CT_SMIL,16,4,"application/smil","smil"},
		{JBL_FILE_CT_DLL,24,3,"application/octet-stream","dll"},
		{JBL_FILE_CT_SGML,9,4,"text/sgml","sgml"},
		{JBL_FILE_CT_VRML,10,4,"model/vrml","vrml"},
		{JBL_FILE_CT_HTML,9,4,"text/html","html"},
		{JBL_FILE_CT_XHTML,21,5,"application/xhtml+xml","xhtml"},
		{JBL_FILE_CT_WML,16,3,"text/vnd.wap.wml","wml"},
		{JBL_FILE_CT_XML,8,3,"text/xml","xml"},
		{JBL_FILE_CT_WBXML,25,5,"application/vnd.wap.wbxml","wbxml"},
		{JBL_FILE_CT_SPL,26,3,"application/x-futuresplash","spl"},
		{JBL_FILE_CT_WRL,10,3,"model/vrml","wrl"},
		{JBL_FILE_CT_XSL,8,3,"text/xml","xsl"},
		{JBL_FILE_CT_RAM,20,3,"audio/x-pn-realaudio","ram"},
		{JBL_FILE_CT_WEBM,10,4,"video/webm","webm"},
		{JBL_FILE_CT_PBM,23,3,"image/x-portable-bitmap","pbm"},
		{JBL_FILE_CT_XBM,15,3,"image/x-xbitmap","xbm"},
		{JBL_FILE_CT_PGM,24,3,"image/x-portable-graymap","pgm"},
		{JBL_FILE_CT_SGM,9,3,"text/sgml","sgm"},
		{JBL_FILE_CT_SKM,18,3,"application/x-koan","skm"},
		{JBL_FILE_CT_PNM,23,3,"image/x-portable-anymap","pnm"},
		{JBL_FILE_CT_PPM,23,3,"image/x-portable-pixmap","ppm"},
		{JBL_FILE_CT_RPM,27,3,"audio/x-pn-realaudio-plugin","rpm"},
		{JBL_FILE_CT_XPM,15,3,"image/x-xpixmap","xpm"},
		{JBL_FILE_CT_RM,20,2,"audio/x-pn-realaudio","rm"},
		{JBL_FILE_CT_HTM,9,3,"text/html","htm"},
		{JBL_FILE_CT_MAN,23,3,"application/x-troff-man","man"},
		{JBL_FILE_CT_PGN,23,3,"application/x-chess-pgn","pgn"},
		{JBL_FILE_CT_IN,10,2,"text/plain","in"},
		{JBL_FILE_CT_BIN,24,3,"application/octet-stream","bin"},
		{JBL_FILE_CT_JSON,16,4,"application/json","json"},
		{JBL_FILE_CT_ICO,12,3,"image/x-icon","ico"},
		{JBL_FILE_CT_TEXINFO,21,7,"application/x-texinfo","texinfo"},
		{JBL_FILE_CT_CPIO,18,4,"application/x-cpio","cpio"},
		{JBL_FILE_CT_SV4CPIO,21,7,"application/x-sv4cpio","sv4cpio"},
		{JBL_FILE_CT_BCPIO,19,5,"application/x-bcpio","bcpio"},
		{JBL_FILE_CT_SILO,10,4,"model/mesh","silo"},
		{JBL_FILE_CT_SO,24,2,"application/octet-stream","so"},
		{JBL_FILE_CT_ZIP,15,3,"application/zip","zip"},
		{JBL_FILE_CT_SKP,18,3,"application/x-koan","skp"},
		{JBL_FILE_CT_BMP,9,3,"image/bmp","bmp"},
		{JBL_FILE_CT_WBMP,18,4,"image/vnd.wap.wbmp","wbmp"},
		{JBL_FILE_CT_SHAR,18,4,"application/x-shar","shar"},
		{JBL_FILE_CT_KAR,10,3,"audio/midi","kar"},
		{JBL_FILE_CT_TAR,17,3,"application/x-tar","tar"},
		{JBL_FILE_CT_GTAR,18,4,"application/x-gtar","gtar"},
		{JBL_FILE_CT_USTAR,19,5,"application/x-ustar","ustar"},
		{JBL_FILE_CT_DCR,22,3,"application/x-director","dcr"},
		{JBL_FILE_CT_DIR,22,3,"application/x-director","dir"},
		{JBL_FILE_CT_TR,19,2,"application/x-troff","tr"},
		{JBL_FILE_CT_DXR,22,3,"application/x-director","dxr"},
		{JBL_FILE_CT_RAS,18,3,"image/x-cmu-raster","ras"},
		{JBL_FILE_CT_IGES,10,4,"model/iges","iges"},
		{JBL_FILE_CT_IGS,10,3,"model/iges","igs"},
		{JBL_FILE_CT_JS,24,2,"application/x-javascript","js"},
		{JBL_FILE_CT_WMLS,22,4,"text/vnd.wap.wmlscript","wmls"},
		{JBL_FILE_CT_XLS,24,3,"application/vnd.ms-excel","xls"},
		{JBL_FILE_CT_MS,22,2,"application/x-troff-ms","ms"},
		{JBL_FILE_CT_DMS,24,3,"application/octet-stream","dms"},
		{JBL_FILE_CT_PS,22,2,"application/postscript","ps"},
		{JBL_FILE_CT_EPS,22,3,"application/postscript","eps"},
		{JBL_FILE_CT_CLASS,24,5,"application/octet-stream","class"},
		{JBL_FILE_CT_CSS,8,3,"text/css","css"},
		{JBL_FILE_CT_RSS,15,3,"application/xml","rss"},
		{JBL_FILE_CT_T,19,1,"application/x-troff","t"},
		{JBL_FILE_CT_XHT,21,3,"application/xhtml+xml","xht"},
		{JBL_FILE_CT_SIT,21,3,"application/x-stuffit","sit"},
		{JBL_FILE_CT_SKT,18,3,"application/x-koan","skt"},
		{JBL_FILE_CT_CPT,26,3,"application/mac-compactpro","cpt"},
		{JBL_FILE_CT_PPT,29,3,"application/vnd.ms-powerpoint","ppt"},
		{JBL_FILE_CT_QT,15,2,"video/quicktime","qt"},
		{JBL_FILE_CT_OUT,10,3,"text/plain","out"},
		{JBL_FILE_CT_TXT,10,3,"text/plain","txt"},
		{JBL_FILE_CT_M3U,15,3,"audio/x-mpegurl","m3u"},
		{JBL_FILE_CT_AU,11,2,"audio/basic","au"},
		{JBL_FILE_CT_DJVU,14,4,"image/vnd.djvu","djvu"},
		{JBL_FILE_CT_MXU,17,3,"video/vnd.mpegurl","mxu"},
		{JBL_FILE_CT_WAV,11,3,"audio/x-wav","wav"},
		{JBL_FILE_CT_DJV,14,3,"image/vnd.djvu","djv"},
		{JBL_FILE_CT_FLV,11,3,"video/x-flv","flv"},
		{JBL_FILE_CT_MOV,15,3,"video/quicktime","mov"},
		{JBL_FILE_CT_TSV,25,3,"text/tab-separated-values","tsv"},
		{JBL_FILE_CT_TEX,17,3,"application/x-tex","tex"},
		{JBL_FILE_CT_LATEX,19,5,"application/x-latex","latex"},
		{JBL_FILE_CT_HQX,24,3,"application/mac-binhex40","hqx"},
		{JBL_FILE_CT_ETX,13,3,"text/x-setext","etx"},
		{JBL_FILE_CT_RTX,13,3,"text/richtext","rtx"},
		{JBL_FILE_CT_EZ,24,2,"application/andrew-inset","ez"},
		{JBL_FILE_CT_XYZ,14,3,"chemical/x-xyz","xyz"},
	}
};
jbl_file_ct jbl_file_get_ctid_by_name(jbl_string *name)
{
	if(!name)return JBL_FILE_CT_UNKNOW;
	name=jbl_refer_pull(name);
	if(name->len==0)return JBL_FILE_CT_UNKNOW;
	for(jbl_fctdst j=0;j<ctd.size;++j)
	{
		if(name->len-ctd.d[j].sul<1)
			goto failed;
		for(jbl_uint8 i=1;i<=ctd.d[j].sul;++i)
			if(name->len<i||name->s[name->len-i]!=ctd.d[j].su[ctd.d[j].sul-i])
				goto failed;
		if(name->s[name->len-ctd.d[j].sul-1]=='.')
			return ctd.d[j].ctid;
		failed:;
	}
#if JBL_FILE_CT_DEBUG ==1 && JBL_VAR_ENABLE==1
	jbl_log(UC "unknow content type%j",name);
#endif
	return JBL_FILE_CT_UNKNOW;
}
jbl_file_ct jbl_file_get_ctid_by_ct(jbl_string *ct)
{
	if(!ct)return JBL_FILE_CT_UNKNOW;
	ct=jbl_refer_pull(ct);
	if(ct->len==0)return JBL_FILE_CT_UNKNOW;
	for(jbl_fctdst j=0;j<ctd.size;++j)
	{
		if(ct->len!=ctd.d[j].ctl)
			goto failed;
		for(jbl_uint8 i=1;i<=ctd.d[j].ctl;++i)
			if(ct->len<i||ct->s[ct->len-i]!=ctd.d[j].ct[ctd.d[j].ctl-i])
				goto failed;
		return ctd.d[j].ctid;
		failed:;
	}
#if JBL_FILE_CT_DEBUG ==1 && JBL_VAR_ENABLE==1
	jbl_log(UC "unknow content type%j",ct);
#endif
	return JBL_FILE_CT_UNKNOW;
}
JBL_INLINE unsigned char *	jbl_file_get_ct_chars_by_ctid		(jbl_file_ct ctid)	{return ctd.d[ctid].ct;}
JBL_INLINE unsigned char *	jbl_file_get_suffix_chars_by_ctid	(jbl_file_ct ctid)	{return ctd.d[ctid].su;}
JBL_INLINE jbl_string    *	jbl_file_get_ct_by_ctid				(jbl_file_ct ctid)	{return jbl_string_cache_get(ctd.d[ctid].ct);}
JBL_INLINE jbl_string    *	jbl_file_get_suffix_by_ctid			(jbl_file_ct ctid)	{return jbl_string_cache_get(ctd.d[ctid].su);}
jbl_uint8 jbl_file_is_video(jbl_file_ct ctid)
{
	if(	(ctid==JBL_FILE_CT_FLV	)||
		(ctid==JBL_FILE_CT_WEBM	)||
		(ctid==JBL_FILE_CT_MP4	)
		)
		return 1;
	return 0;
}
jbl_uint8 jbl_file_is_audio(jbl_file_ct ctid)
{
	if(	(ctid==JBL_FILE_CT_M3U	)||
		(ctid==JBL_FILE_CT_WAV	)||
		(ctid==JBL_FILE_CT_MP3	)
		)
		return 1;
	return 0;
}

#endif
