//////////////////////////////////////////////////////////////////////////
//File: StrUtil.h
//Project: Only XML (The XML Lib For Only Game Engine) 
//Author:Zipxin
//Desc ：很轻巧的一个String处理函数集合，属于OnlyXML项目
//       本结构的Util函数集可以被其他单元共享，但是每个项目
//		 共享方式不同，考虑到这个Util集合比较小，所以以后其他项目
//		 如果需要，直接包含即可，不提供外在的接口了。
//////////////////////////////////////////////////////////////////////////
#ifndef __STR_UTIL_H__
#define __STR_UTIL_H__

//////////////////////////////////////////////////////////////////////////
//命名规范：
//		Char_ 处理字符
//	    Str_ 处理字符串

#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#define INCLUDE_STD
#include <string>
#include <iostream>
#define ONLY_STRING	    std::string
#define ONLY_ISTREAM	std::istream
#define ONLY_OSTREAM	std::ostream

/*
UTF-8转换表表示如下： 
　　UNICODE UTF-8 
　　00000000 - 0000007F 0xxxxxxx
　　00000080 - 000007FF 110xxxxx 10xxxxxx 
　　00000800 - 0000FFFF 1110xxxx 10xxxxxx 10xxxxxx 
　　00010000 - 001FFFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
　　00200000 - 03FFFFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
　　04000000 - 7FFFFFFF 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
*/

//对于UTF-8可变字节的编码方式，我需要知道一个字符它到底有几个字节
//下面可以通过查表可以获取
//来自：TinyXml
const int UTF8ByteNumTable[256] = 
{
	//	0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x00
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x10
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x20
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x30
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x40
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x50
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x60
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x70	End of ASCII range
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x80 0x80 to 0xc1 invalid
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x90 
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xa0 
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xb0 
	1,	1,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xc0 0xc2 to 0xdf 2 byte
	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xd0
	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	// 0xe0 0xe0 to 0xef 3 byte
	4,	4,	4,	4,	4,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1	// 0xf0 0xf0 to 0xf4 4 byte, 0xf5 and higher invalid
};

enum OnlyEncoding
{
	ONLY_ENCODING_UTF8,
	ONLY_ENCODING_UNKNOW,
	ONLY_ENCODING_ASCII
};

/*
const int ONLY_MAJOR_VERSION = 0;
const int ONLY_MINOR_VERSION = 0;
const int ONLY_PATCH_VERSION = 1;
*/

const OnlyEncoding ONLY_DEFAULT_ENCODING = ONLY_ENCODING_UNKNOW;

#ifndef TRUE
#	define TRUE                    1          
#endif

#ifndef FALSE
#	define FALSE                   0           
#endif

#define RES_ID   int 
#define RES_STR  (char *)

//---------------------------字符判断------
//返回字符是否是空白
inline int Char_IsBlank ( char ch )
{
	if ( ch == ' ' || ch == '\t' )
		return TRUE;
	return FALSE;
}
//返回字符是否是数字
inline int Char_IsNumeric ( char ch )
{
	if ( ch >= '0' && ch <= '9' )
		return TRUE;
	return FALSE;
}
//是否是合法标识符里的字符
//不能判断首字母是否合法
inline int Char_IsIdentity ( char ch )
{
	if ( ( ch >= '0' && ch <= '9' ) ||
		( ch >= 'A' && ch <= 'Z' ) ||
		( ch >= 'a' && ch <= 'z' ) ||
		ch == '_' )
		return TRUE;
	return FALSE;
}

//字符是否是分割符
inline int Char_IsDelimiter ( char cChar )
{

	if ( cChar == ':' || cChar == ',' || cChar == '"' ||
		cChar == '[' || cChar == ']' ||
		cChar == '{' || cChar == '}' ||
		Char_IsBlank ( cChar ) || cChar == '\n' )
		return TRUE;

	return FALSE;
}

inline char Char_ToUpper(char ch)
{
	if( ch>='a' && ch<='z')
		return ch - 32;
	return ch;
}

inline static bool	Char_IsWhiteSpace( char c )		
{ 
	return ( isspace( (unsigned char) c ) || c == '\n' || c == '\r' ); 
}

inline int Char_IsAlphaNum(char ch, OnlyEncoding encoding)
{
	return isalnum( ch );
}

inline int Char_IsAlpha(char ch, OnlyEncoding encoding)
{
	if ( ( ch >= 'A' && ch <= 'Z' ) ||
		( ch >= 'a' && ch <= 'z' ) )
		return true;
	return false;
}

inline int Char_ToLowerEx( int v, OnlyEncoding encoding )
{
	if ( encoding == ONLY_ENCODING_UTF8 )
	{
		if ( v < 128 ) return tolower( v );
		return v;
	}
	else
	{
		return tolower( v );
	}
}

//--------------------------------判定字符串属性

//修剪字符串，除去字符串前后空格
void Str_Trim(char *str);


int Str_IsInteger(char *str);

//注意：此函数无法分辨整形和字符串型
int Str_IsFloat(char *str);

int Str_IsBlank(const char *str);

int Str_IsIdentity(char *str);

void Str_ToUpper(char *str);

//如果是UTF-8编码，会跳过前面3个字节的BOM头
const char * Str_SkipBlank(const char *pStr);

bool Str_Equal(const char *lh, const char *rh, bool ignoreCase, OnlyEncoding encoding);

#endif /*File Is Over*/