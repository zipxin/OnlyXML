//////////////////////////////////////////////////////////////////////////
//File: StrUtil.h
//Project: Only XML (The XML Lib For Only Game Engine) 
//Author:Zipxin
//Desc �������ɵ�һ��String���������ϣ�����OnlyXML��Ŀ
//       ���ṹ��Util���������Ա�������Ԫ��������ÿ����Ŀ
//		 ����ʽ��ͬ�����ǵ����Util���ϱȽ�С�������Ժ�������Ŀ
//		 �����Ҫ��ֱ�Ӱ������ɣ����ṩ���ڵĽӿ��ˡ�
//////////////////////////////////////////////////////////////////////////
#ifndef __STR_UTIL_H__
#define __STR_UTIL_H__

//////////////////////////////////////////////////////////////////////////
//�����淶��
//		Char_ �����ַ�
//	    Str_ �����ַ���

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
UTF-8ת�����ʾ���£� 
����UNICODE UTF-8 
����00000000 - 0000007F 0xxxxxxx
����00000080 - 000007FF 110xxxxx 10xxxxxx 
����00000800 - 0000FFFF 1110xxxx 10xxxxxx 10xxxxxx 
����00010000 - 001FFFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
����00200000 - 03FFFFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
����04000000 - 7FFFFFFF 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
*/

//����UTF-8�ɱ��ֽڵı��뷽ʽ������Ҫ֪��һ���ַ��������м����ֽ�
//�������ͨ�������Ի�ȡ
//���ԣ�TinyXml
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

//---------------------------�ַ��ж�------
//�����ַ��Ƿ��ǿհ�
inline int Char_IsBlank ( char ch )
{
	if ( ch == ' ' || ch == '\t' )
		return TRUE;
	return FALSE;
}
//�����ַ��Ƿ�������
inline int Char_IsNumeric ( char ch )
{
	if ( ch >= '0' && ch <= '9' )
		return TRUE;
	return FALSE;
}
//�Ƿ��ǺϷ���ʶ������ַ�
//�����ж�����ĸ�Ƿ�Ϸ�
inline int Char_IsIdentity ( char ch )
{
	if ( ( ch >= '0' && ch <= '9' ) ||
		( ch >= 'A' && ch <= 'Z' ) ||
		( ch >= 'a' && ch <= 'z' ) ||
		ch == '_' )
		return TRUE;
	return FALSE;
}

//�ַ��Ƿ��Ƿָ��
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

//--------------------------------�ж��ַ�������

//�޼��ַ�������ȥ�ַ���ǰ��ո�
void Str_Trim(char *str);


int Str_IsInteger(char *str);

//ע�⣺�˺����޷��ֱ����κ��ַ�����
int Str_IsFloat(char *str);

int Str_IsBlank(const char *str);

int Str_IsIdentity(char *str);

void Str_ToUpper(char *str);

//�����UTF-8���룬������ǰ��3���ֽڵ�BOMͷ
const char * Str_SkipBlank(const char *pStr);

bool Str_Equal(const char *lh, const char *rh, bool ignoreCase, OnlyEncoding encoding);

#endif /*File Is Over*/