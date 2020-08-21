/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_FILE_CT_H
#define __JBL_FILE_CT_H
#include "jbl_file_config.h"
#if JBL_FILE_ENABLE==1
#include "jbl_string.h"
typedef enum
{
	JBL_FILE_CT_UNKNOW,
	JBL_FILE_CT_MP2,
	JBL_FILE_CT_MP3,
	JBL_FILE_CT_MP4,
	JBL_FILE_CT_ODA,
	JBL_FILE_CT_MPGA,
	JBL_FILE_CT_LHA,
	JBL_FILE_CT_RA,
	JBL_FILE_CT_PDB,
	JBL_FILE_CT_RGB,
	JBL_FILE_CT_AIFC,
	JBL_FILE_CT_WMLC,
	JBL_FILE_CT_NC,
	JBL_FILE_CT_DOC,
	JBL_FILE_CT_SV4CRC,
	JBL_FILE_CT_SRC,
	JBL_FILE_CT_ASC,
	JBL_FILE_CT_WMLSC,
	JBL_FILE_CT_VCD,
	JBL_FILE_CT_MID,
	JBL_FILE_CT_SKD,
	JBL_FILE_CT_SND,
	JBL_FILE_CT_XWD,
	JBL_FILE_CT_ICE,
	JBL_FILE_CT_MOVIE,
	JBL_FILE_CT_ME,
	JBL_FILE_CT_JPE,
	JBL_FILE_CT_MPE,
	JBL_FILE_CT_EXE,
	JBL_FILE_CT_CDF,
	JBL_FILE_CT_HDF,
	JBL_FILE_CT_PDF,
	JBL_FILE_CT_IEF,
	JBL_FILE_CT_AIFF,
	JBL_FILE_CT_TIFF,
	JBL_FILE_CT_ROFF,
	JBL_FILE_CT_AIF,
	JBL_FILE_CT_GIF,
	JBL_FILE_CT_MIF,
	JBL_FILE_CT_TIF,
	JBL_FILE_CT_RTF,
	JBL_FILE_CT_SWF,
	JBL_FILE_CT_JPEG,
	JBL_FILE_CT_MPEG,
	JBL_FILE_CT_PNG,
	JBL_FILE_CT_JPG,
	JBL_FILE_CT_MPG,
	JBL_FILE_CT_SH,
	JBL_FILE_CT_CSH,
	JBL_FILE_CT_MESH,
	JBL_FILE_CT_MSH,
	JBL_FILE_CT_LZH,
	JBL_FILE_CT_AI,
	JBL_FILE_CT_MIDI,
	JBL_FILE_CT_SMI,
	JBL_FILE_CT_AVI,
	JBL_FILE_CT_DVI,
	JBL_FILE_CT_TEXI,
	JBL_FILE_CT_TCL,
	JBL_FILE_CT_SMIL,
	JBL_FILE_CT_DLL,
	JBL_FILE_CT_SGML,
	JBL_FILE_CT_VRML,
	JBL_FILE_CT_HTML,
	JBL_FILE_CT_XHTML,
	JBL_FILE_CT_WML,
	JBL_FILE_CT_XML,
	JBL_FILE_CT_WBXML,
	JBL_FILE_CT_SPL,
	JBL_FILE_CT_WRL,
	JBL_FILE_CT_XSL,
	JBL_FILE_CT_RAM,
	JBL_FILE_CT_WEBM,
	JBL_FILE_CT_PBM,
	JBL_FILE_CT_XBM,
	JBL_FILE_CT_PGM,
	JBL_FILE_CT_SGM,
	JBL_FILE_CT_SKM,
	JBL_FILE_CT_PNM,
	JBL_FILE_CT_PPM,
	JBL_FILE_CT_RPM,
	JBL_FILE_CT_XPM,
	JBL_FILE_CT_RM,
	JBL_FILE_CT_HTM,
	JBL_FILE_CT_MAN,
	JBL_FILE_CT_PGN,
	JBL_FILE_CT_IN,
	JBL_FILE_CT_BIN,
	JBL_FILE_CT_ICO,
	JBL_FILE_CT_TEXINFO,
	JBL_FILE_CT_CPIO,
	JBL_FILE_CT_SV4CPIO,
	JBL_FILE_CT_BCPIO,
	JBL_FILE_CT_SILO,
	JBL_FILE_CT_SO,
	JBL_FILE_CT_ZIP,
	JBL_FILE_CT_SKP,
	JBL_FILE_CT_BMP,
	JBL_FILE_CT_WBMP,
	JBL_FILE_CT_SHAR,
	JBL_FILE_CT_KAR,
	JBL_FILE_CT_TAR,
	JBL_FILE_CT_GTAR,
	JBL_FILE_CT_USTAR,
	JBL_FILE_CT_DCR,
	JBL_FILE_CT_DIR,
	JBL_FILE_CT_TR,
	JBL_FILE_CT_DXR,
	JBL_FILE_CT_RAS,
	JBL_FILE_CT_IGES,
	JBL_FILE_CT_IGS,
	JBL_FILE_CT_JS,
	JBL_FILE_CT_WMLS,
	JBL_FILE_CT_XLS,
	JBL_FILE_CT_MS,
	JBL_FILE_CT_DMS,
	JBL_FILE_CT_PS,
	JBL_FILE_CT_EPS,
	JBL_FILE_CT_CLASS,
	JBL_FILE_CT_CSS,
	JBL_FILE_CT_RSS,
	JBL_FILE_CT_T,
	JBL_FILE_CT_XHT,
	JBL_FILE_CT_SIT,
	JBL_FILE_CT_SKT,
	JBL_FILE_CT_CPT,
	JBL_FILE_CT_PPT,
	JBL_FILE_CT_QT,
	JBL_FILE_CT_OUT,
	JBL_FILE_CT_TXT,
	JBL_FILE_CT_M3U,
	JBL_FILE_CT_AU,
	JBL_FILE_CT_DJVU,
	JBL_FILE_CT_MXU,
	JBL_FILE_CT_WAV,
	JBL_FILE_CT_DJV,
	JBL_FILE_CT_FLV,
	JBL_FILE_CT_MOV,
	JBL_FILE_CT_TSV,
	JBL_FILE_CT_TEX,
	JBL_FILE_CT_LATEX,
	JBL_FILE_CT_HQX,
	JBL_FILE_CT_ETX,
	JBL_FILE_CT_RTX,
	JBL_FILE_CT_EZ,
	JBL_FILE_CT_XYZ,
}jbl_file_ct;
jbl_file_ct jbl_file_get_ct_by_name					(jbl_string *name);
unsigned char * jbl_file_get_ct_chars_by_ctid		(jbl_file_ct ctid);
unsigned char * jbl_file_get_suffic_chars_by_ctid	(jbl_file_ct ctid);
jbl_string    *	jbl_file_get_ct_by_ctid				(jbl_file_ct ctid);
jbl_string    *	jbl_file_get_suffic_by_ctid			(jbl_file_ct ctid);


#endif
#endif
