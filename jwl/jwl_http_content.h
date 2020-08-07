/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JWL_HTTP_CONTENT_H
#define __JWL_HTTP_CONTENT_H
#include "jwl_http_config.h"
#if JWL_HTTP_ENABLE==1
#define JWL_HTTP_CONTENT_TYPE_RSS		"application/xml"
#define JWL_HTTP_CONTENT_TYPE_EZ		"application/andrew-inset"
#define JWL_HTTP_CONTENT_TYPE_HQX		"application/mac-binhex40"
#define JWL_HTTP_CONTENT_TYPE_CPT		"application/mac-compactpro"
#define JWL_HTTP_CONTENT_TYPE_DOC		"application/msword"
#define JWL_HTTP_CONTENT_TYPE_BIN		"application/octet-stream"
#define JWL_HTTP_CONTENT_TYPE_DMS		"application/octet-stream"
#define JWL_HTTP_CONTENT_TYPE_LHA		"application/octet-stream"
#define JWL_HTTP_CONTENT_TYPE_LZH		"application/octet-stream"
#define JWL_HTTP_CONTENT_TYPE_EXE		"application/octet-stream"
#define JWL_HTTP_CONTENT_TYPE_CLASS		"application/octet-stream"
#define JWL_HTTP_CONTENT_TYPE_SO		"application/octet-stream"
#define JWL_HTTP_CONTENT_TYPE_DLL		"application/octet-stream"
#define JWL_HTTP_CONTENT_TYPE_ODA		"application/oda"
#define JWL_HTTP_CONTENT_TYPE_PDF		"application/pdf"
#define JWL_HTTP_CONTENT_TYPE_AI		"application/postscript"
#define JWL_HTTP_CONTENT_TYPE_EPS		"application/postscript"
#define JWL_HTTP_CONTENT_TYPE_PS		"application/postscript"
#define JWL_HTTP_CONTENT_TYPE_SMI		"application/smil"
#define JWL_HTTP_CONTENT_TYPE_SMIL		"application/smil"
#define JWL_HTTP_CONTENT_TYPE_MIF		"application/vnd.mif"
#define JWL_HTTP_CONTENT_TYPE_XLS		"application/vnd.ms-excel"
#define JWL_HTTP_CONTENT_TYPE_PPT		"application/vnd.ms-powerpoint"
#define JWL_HTTP_CONTENT_TYPE_WBXML		"application/vnd.wap.wbxml"
#define JWL_HTTP_CONTENT_TYPE_WMLC		"application/vnd.wap.wmlc"
#define JWL_HTTP_CONTENT_TYPE_WMLSC		"application/vnd.wap.wmlscriptc"
#define JWL_HTTP_CONTENT_TYPE_BCPIO		"application/x-bcpio"
#define JWL_HTTP_CONTENT_TYPE_VCD		"application/x-cdlink"
#define JWL_HTTP_CONTENT_TYPE_PGN		"application/x-chess-pgn"
#define JWL_HTTP_CONTENT_TYPE_CPIO		"application/x-cpio"
#define JWL_HTTP_CONTENT_TYPE_CSH		"application/x-csh"
#define JWL_HTTP_CONTENT_TYPE_DCR		"application/x-director"
#define JWL_HTTP_CONTENT_TYPE_DIR		"application/x-director"
#define JWL_HTTP_CONTENT_TYPE_DXR		"application/x-director"
#define JWL_HTTP_CONTENT_TYPE_DVI		"application/x-dvi"
#define JWL_HTTP_CONTENT_TYPE_SPL		"application/x-futuresplash"
#define JWL_HTTP_CONTENT_TYPE_GTAR		"application/x-gtar"
#define JWL_HTTP_CONTENT_TYPE_HDF		"application/x-hdf"
#define JWL_HTTP_CONTENT_TYPE_JS		"application/x-javascript"
#define JWL_HTTP_CONTENT_TYPE_SKP		"application/x-koan"
#define JWL_HTTP_CONTENT_TYPE_SKD		"application/x-koan"
#define JWL_HTTP_CONTENT_TYPE_SKT		"application/x-koan"
#define JWL_HTTP_CONTENT_TYPE_SKM		"application/x-koan"
#define JWL_HTTP_CONTENT_TYPE_LATEX		"application/x-latex"
#define JWL_HTTP_CONTENT_TYPE_NC		"application/x-netcdf"
#define JWL_HTTP_CONTENT_TYPE_CDF		"application/x-netcdf"
#define JWL_HTTP_CONTENT_TYPE_SH		"application/x-sh"
#define JWL_HTTP_CONTENT_TYPE_SHAR		"application/x-shar"
#define JWL_HTTP_CONTENT_TYPE_SWF		"application/x-shockwave-flash"
#define JWL_HTTP_CONTENT_TYPE_SIT		"application/x-stuffit"
#define JWL_HTTP_CONTENT_TYPE_SV4CPIO	"application/x-sv4cpio"
#define JWL_HTTP_CONTENT_TYPE_SV4CRC	"application/x-sv4crc"
#define JWL_HTTP_CONTENT_TYPE_TAR		"application/x-tar"
#define JWL_HTTP_CONTENT_TYPE_TCL		"application/x-tcl"
#define JWL_HTTP_CONTENT_TYPE_TEX		"application/x-tex"
#define JWL_HTTP_CONTENT_TYPE_TEXINFO	"application/x-texinfo"
#define JWL_HTTP_CONTENT_TYPE_TEXI		"application/x-texinfo"
#define JWL_HTTP_CONTENT_TYPE_T			"application/x-troff"
#define JWL_HTTP_CONTENT_TYPE_TR		"application/x-troff"
#define JWL_HTTP_CONTENT_TYPE_ROFF		"application/x-troff"
#define JWL_HTTP_CONTENT_TYPE_MAN		"application/x-troff-man"
#define JWL_HTTP_CONTENT_TYPE_ME		"application/x-troff-me"
#define JWL_HTTP_CONTENT_TYPE_MS		"application/x-troff-ms"
#define JWL_HTTP_CONTENT_TYPE_USTAR		"application/x-ustar"
#define JWL_HTTP_CONTENT_TYPE_SRC		"application/x-wais-source"
#define JWL_HTTP_CONTENT_TYPE_XHTML		"application/xhtml+xml"
#define JWL_HTTP_CONTENT_TYPE_XHT		"application/xhtml+xml"
#define JWL_HTTP_CONTENT_TYPE_ZIP		"application/zip"
#define JWL_HTTP_CONTENT_TYPE_AU		"audio/basic"
#define JWL_HTTP_CONTENT_TYPE_SND		"audio/basic"
#define JWL_HTTP_CONTENT_TYPE_MID		"audio/midi"
#define JWL_HTTP_CONTENT_TYPE_MIDI		"audio/midi"
#define JWL_HTTP_CONTENT_TYPE_KAR		"audio/midi"
#define JWL_HTTP_CONTENT_TYPE_MPGA		"audio/mpeg"
#define JWL_HTTP_CONTENT_TYPE_MP2		"audio/mp2"
#define JWL_HTTP_CONTENT_TYPE_MP3		"audio/mp3"
#define JWL_HTTP_CONTENT_TYPE_MP4		"video/mp4"
#define JWL_HTTP_CONTENT_TYPE_AIF		"audio/x-aiff"
#define JWL_HTTP_CONTENT_TYPE_AIFF		"audio/x-aiff"
#define JWL_HTTP_CONTENT_TYPE_AIFC		"audio/x-aiff"
#define JWL_HTTP_CONTENT_TYPE_M3U		"audio/x-mpegurl"
#define JWL_HTTP_CONTENT_TYPE_RAM		"audio/x-pn-realaudio"
#define JWL_HTTP_CONTENT_TYPE_RM		"audio/x-pn-realaudio"
#define JWL_HTTP_CONTENT_TYPE_RPM		"audio/x-pn-realaudio-plugin"
#define JWL_HTTP_CONTENT_TYPE_RA		"audio/x-realaudio"
#define JWL_HTTP_CONTENT_TYPE_WAV		"audio/x-wav"
#define JWL_HTTP_CONTENT_TYPE_PDB		"chemical/x-pdb"
#define JWL_HTTP_CONTENT_TYPE_XYZ		"chemical/x-xyz"
#define JWL_HTTP_CONTENT_TYPE_BMP		"image/bmp"
#define JWL_HTTP_CONTENT_TYPE_GIF		"image/gif"
#define JWL_HTTP_CONTENT_TYPE_GIF		"image/gif"
#define JWL_HTTP_CONTENT_TYPE_IEF		"image/ief"
#define JWL_HTTP_CONTENT_TYPE_JPEG		"image/jpeg"
#define JWL_HTTP_CONTENT_TYPE_JPG		"image/jpeg"
#define JWL_HTTP_CONTENT_TYPE_JPE		"image/jpeg"
#define JWL_HTTP_CONTENT_TYPE_PNG		"image/png"
#define JWL_HTTP_CONTENT_TYPE_TIFF		"image/tiff"
#define JWL_HTTP_CONTENT_TYPE_TIF		"image/tif"
#define JWL_HTTP_CONTENT_TYPE_DJVU		"image/vnd.djvu"
#define JWL_HTTP_CONTENT_TYPE_DJV		"image/vnd.djvu"
#define JWL_HTTP_CONTENT_TYPE_WBMP		"image/vnd.wap.wbmp"
#define JWL_HTTP_CONTENT_TYPE_RAS		"image/x-cmu-raster"
#define JWL_HTTP_CONTENT_TYPE_PNM		"image/x-portable-anymap"
#define JWL_HTTP_CONTENT_TYPE_PBM		"image/x-portable-bitmap"
#define JWL_HTTP_CONTENT_TYPE_PGM		"image/x-portable-graymap"
#define JWL_HTTP_CONTENT_TYPE_PPM		"image/x-portable-pixmap"
#define JWL_HTTP_CONTENT_TYPE_RGB		"image/x-rgb"
#define JWL_HTTP_CONTENT_TYPE_XBM		"image/x-xbitmap"
#define JWL_HTTP_CONTENT_TYPE_XPM		"image/x-xpixmap"
#define JWL_HTTP_CONTENT_TYPE_XWD		"image/x-xwindowdump"
#define JWL_HTTP_CONTENT_TYPE_IGS		"model/iges"
#define JWL_HTTP_CONTENT_TYPE_IGES		"model/iges"
#define JWL_HTTP_CONTENT_TYPE_MSH		"model/mesh"
#define JWL_HTTP_CONTENT_TYPE_MESH		"model/mesh"
#define JWL_HTTP_CONTENT_TYPE_SILO		"model/mesh"
#define JWL_HTTP_CONTENT_TYPE_WRL		"model/vrml"
#define JWL_HTTP_CONTENT_TYPE_VRML		"model/vrml"
#define JWL_HTTP_CONTENT_TYPE_CSS		"text/css"
#define JWL_HTTP_CONTENT_TYPE_HTML		"text/html"
#define JWL_HTTP_CONTENT_TYPE_HTM		"text/html"
#define JWL_HTTP_CONTENT_TYPE_ASC		"text/plain"
#define JWL_HTTP_CONTENT_TYPE_TXT		"text/plain"
#define JWL_HTTP_CONTENT_TYPE_IN		"text/plain"
#define JWL_HTTP_CONTENT_TYPE_OUT		"text/plain"
#define JWL_HTTP_CONTENT_TYPE_RTX		"text/richtext"
#define JWL_HTTP_CONTENT_TYPE_RTF		"text/rtf"
#define JWL_HTTP_CONTENT_TYPE_SGML		"text/sgml"
#define JWL_HTTP_CONTENT_TYPE_SGM		"text/sgml"
#define JWL_HTTP_CONTENT_TYPE_TSV		"text/tab-separated-values"
#define JWL_HTTP_CONTENT_TYPE_WML		"text/vnd.wap.wml"
#define JWL_HTTP_CONTENT_TYPE_WMLS		"text/vnd.wap.wmlscript"
#define JWL_HTTP_CONTENT_TYPE_ETX		"text/x-setext"
#define JWL_HTTP_CONTENT_TYPE_XSL		"text/xml"
#define JWL_HTTP_CONTENT_TYPE_XML		"text/xml"
#define JWL_HTTP_CONTENT_TYPE_MPEG		"video/mpeg"
#define JWL_HTTP_CONTENT_TYPE_MPG		"video/mpeg"
#define JWL_HTTP_CONTENT_TYPE_MPE		"video/mpeg"
#define JWL_HTTP_CONTENT_TYPE_FLV		"video/x-flv"
#define JWL_HTTP_CONTENT_TYPE_QT		"video/quicktime"
#define JWL_HTTP_CONTENT_TYPE_MOV		"video/quicktime"
#define JWL_HTTP_CONTENT_TYPE_MXU		"video/vnd.mpegurl"
#define JWL_HTTP_CONTENT_TYPE_AVI		"video/x-msvideo"
#define JWL_HTTP_CONTENT_TYPE_MOVIE		"video/x-sgi-movie"
#define JWL_HTTP_CONTENT_TYPE_ICE		"x-conference/x-cooltalk"
#define JWL_HTTP_CONTENT_TYPE_ICO		"image/x-icon"
#endif
#endif