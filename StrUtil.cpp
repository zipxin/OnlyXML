#include "stdafx.h"
#include "StrUtil.h"

void Str_Trim(char *str)
{
	unsigned int curCharIndex = 0;
	unsigned int size = 0;
	unsigned int offsetLeftMove = 0;

	if (!str) return;
	size = strlen(str);
	if(size == 0) return ;

	while(Char_IsBlank(str[curCharIndex])) curCharIndex++;

	offsetLeftMove = curCharIndex;

	if(offsetLeftMove)
	{
		for(;curCharIndex < size; curCharIndex ++)
		{
			str[curCharIndex - offsetLeftMove ] = str[curCharIndex];
		}

		for(curCharIndex = size - offsetLeftMove; curCharIndex < size; curCharIndex ++)
		{
			str[curCharIndex] = ' ';
		}
	}
	curCharIndex = size - 1; //过滤/0
	while(Char_IsBlank(str[curCharIndex]))
	{
		//跳出循环的变量不能写在if里
		if(curCharIndex>0)
			curCharIndex--;
		else break;
	}

	str[curCharIndex+1] = '\0';
}

int Str_IsInteger(char *str)
{
	int curIndex = 0;
	int size = 0;

	if (!str) return FALSE;
	size =  strlen(str);
	if(size == 0) return FALSE;



	if(str[curIndex] == '-') curIndex ++;

	while(Char_IsNumeric(str[curIndex])) curIndex ++;

	//	printf("curindex:%d, size : %d",curIndex,size);
	if(curIndex == size) return TRUE;
	else return FALSE;
}


int Str_IsFloat(char *str)
{
	int curIndex = 0;
	int size = 0;

	if (!str) return FALSE;
	size = strlen(str);
	if(size == 0) return FALSE;



	if(str[curIndex] == '-') 
	{
		curIndex ++;
		if(	curIndex == size ) return FALSE; //防止 -
	}

	while(Char_IsNumeric(str[curIndex])) curIndex ++;

	if(str[curIndex] == '.') 
	{
		curIndex ++;
		if(	curIndex == size ) return FALSE; //防止 -213.
	}


	while(Char_IsNumeric(str[curIndex])) curIndex ++;

	if( str[curIndex] == 'e' || str[curIndex] == 'E' ) 
	{
		curIndex ++;
		if( curIndex == size ) return FALSE; //防止 -213.23E
	}

	if(str[curIndex] == '-') 
	{
		curIndex ++;
		if(	curIndex == size ) return FALSE; //防止 -
	}

	while(Char_IsNumeric(str[curIndex])) curIndex ++;

	//printf("curindex:%d, size : %d",curIndex,size);

	if(curIndex == size) return TRUE;
	else return FALSE;
}

int Str_IsBlank(const char *str)
{
	int curIndex = 0;
	int size = 0;

	if (!str) return FALSE;
	size = strlen(str);
	if(size == 0) return FALSE;

	for(; curIndex < size; curIndex++)
	{
		if(!Char_IsBlank(str[curIndex])) return FALSE;
	}

	return TRUE;
}

int Str_IsIdentity(char *str)
{
	int curIndex = 0;
	int size = 0;

	if (!str) return FALSE;
	size = strlen(str);
	if(size == 0) return FALSE;

	if(Char_IsNumeric(str[curIndex])) return FALSE;

	for(; curIndex < size; curIndex++)
	{
		if(!Char_IsIdentity(str[curIndex])) return FALSE;
	}

	return TRUE;
}

void Str_ToUpper(char *str)
{
	int size =0 ;
	int curIndex = 0;
	if(!str) return ;
	size = strlen(str);
	for(; curIndex < size; curIndex++)
	{
		//str[curIndex] = str[curIndex] & 0x4F;
		//if( str[curIndex]>='A' && str[curIndex]<='Z')
		//	str[curIndex] = str[curIndex]+32;

		if( str[curIndex]>='a' &&str[curIndex]<='z')
			str[curIndex] = str[curIndex]-32; 

		//default 不做处理
	}
}

bool Str_Equal(const char *lh, const char *rh, bool ignoreCase, OnlyEncoding encoding)
{
	assert(lh);
	assert(rh);

	if(!lh || !*lh)
	{
		assert(!"操作数为NULL，程序不应该到达此处！");
		return false;
	}

	if(ignoreCase)
	{
		while(*lh && *rh && (Char_ToUpper(*lh) == Char_ToUpper(*rh) ))
		{
			++lh ;
			++rh ;
		}
		if(*rh == 0) return true;
	}else
	{
		while(*lh && *rh && (*lh == *rh))
		{
			++lh ;
			++rh ;
		}
		if(*rh == 0) return true;
	}
	return false;
}


const char * Str_SkipBlank(const char *pStr)
{
	if ( !pStr || !*pStr )
		return 0;
	while ( *pStr )
	{
		const unsigned char* pU = (const unsigned char*)pStr;

		if (	*(pU+0)==0xefU
			&& *(pU+1)==0xbbU 
			&& *(pU+2)==0xbfU )
		{
			pStr += 3;
			continue;
		}
		else if(*(pU+0)==0xefU
			&& *(pU+1)==0xbfU
			&& *(pU+2)==0xbeU )
		{
			pStr += 3;
			continue;
		}
		else if(*(pU+0)==0xefU
			&& *(pU+1)==0xbfU
			&& *(pU+2)==0xbfU )
		{
			pStr += 3;
			continue;
		}

		if ( Char_IsBlank( *pStr ) || *pStr == '\n' || *pStr =='\r' )		
			++pStr;
		else
			break;
	}
	return pStr;
}