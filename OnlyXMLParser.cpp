//////////////////////////////////////////////////////////////////////////
//File: OnlyXMLParser.cpp
//Project: Only XML (The XML Lib For Only Game Engine) 
//Author:Zipxin
//Desc ：XML节点解释函数的实现
//		 本来可以直接放在OnlyXML.cpp中的，但是方便阅览，
//		 就单独形成一个文件
//       本项目是OnlyGameEngine项目的子项目
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "OnlyXML.h"


const char* GetChar( const char* p, char* _value, int* length, OnlyEncoding encoding )
{
	assert( p );
	if ( encoding == ONLY_ENCODING_UTF8 )
	{
		*length = UTF8ByteNumTable[ *((unsigned char*)p) ];
		assert( *length >= 0 && *length < 5 );
	}
	else
	{
		*length = 1;
	}

	if ( *length == 1 )
	{
		if ( *p == '&' )
			return GetEntity( p, _value, length, encoding );
		*_value = *p;
		return p+1;
	}
	else if ( *length )
	{
		strncpy( _value, p, *length );
		return p + (*length);
	}
	else
	{
		return 0;
	}
}

const char* GetName(const char * pstr, ONLY_STRING *name, OnlyEncoding encoding)
{
	if(!pstr || !*pstr) return 0;

	*name = "";
	
	if( Char_IsAlpha(*pstr, encoding) || *pstr == '_')
	{
		while(pstr && *pstr && (Char_IsIdentity(*pstr) 
			|| *pstr == '-' 
			|| *pstr == ':'
			|| *pstr == '.'))
		{
			*name += *pstr;
			pstr++;
		}
		return pstr;
	}
	return pstr;
}

const char* GetEntity( const char* pstr, char* &value, int* length, OnlyEncoding encoding )
{
	//ONLY_STRING entity;
	for( int i=0; i<ONLYXML_ENTITY_NUM; ++i )
	{
		if ( strncmp( entity[i].str, pstr, entity[i].strLength ) == 0 )
		{
			assert( strlen( entity[i].str ) == entity[i].strLength );
			*value = entity[i].chr;
			*length = 1;
			return ( pstr + entity[i].strLength );
		}
	}
	*length = 1;
	*value = *pstr;	
	return pstr+1;
}

/**/
const char *GetText( const char *pstr, 
					 ONLY_STRING *text, 
					 bool isIgnoreBlank, 
					 const char *endtag, 
					 bool isIgnoreCase,
					 OnlyEncoding encoding )
{
	if(!pstr || !*pstr) return 0;
	//重置string
	*text = "";
	if(!isIgnoreBlank)
	{
		while (	 pstr && *pstr
			&& !Str_Equal( pstr, endtag, isIgnoreCase, encoding )
			)
		{
			int len;
			char Char[4] = { 0, 0, 0, 0 };
			pstr = GetChar( pstr, Char, &len, encoding );
			text->append( Char, len );
		}
	}else
	{
		bool HasBlank = false;
		pstr = Str_SkipBlank(pstr);
		while( pstr && *pstr
			&& !Str_Equal( pstr, endtag, isIgnoreCase, encoding ))
		{
			if(Char_IsBlank(*pstr))
			{
				HasBlank = true;
				++pstr;
			}else if(*pstr == '\n' || *pstr == '\r')
			{
				HasBlank = true;
				++pstr;
			}else
			{
				if(HasBlank)
				{
					*text += ' ';
					HasBlank = false;
				}
					int len;
				char Char[4] = { 0, 0, 0, 0 };
				pstr = GetChar( pstr, Char, &len, encoding );
				if ( len == 1 )
					(*text) += Char[0]; 
				else
					text->append( Char, len );
			}
		}
	}
	return pstr + strlen( endtag );
}


void PutString( const ONLY_STRING& str, ONLY_STRING* outString )
{
	int i=0;
	while( i<(int)str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
		{
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString->append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString->append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString->append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			char buf[ 32 ];
			sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


//////////////////////////////////////////////////////////////////////////

const char* XMLElement::Parser(	const char* p,  OnlyEncoding encoding  )
{
	p = Str_SkipBlank( p );
	XMLDoc* doc= GetDocument();

	if ( !p || !(*p) )
	{
		if ( doc ) doc->SetError( ONLY_RES_NULL );
		return 0;
	}
	if ( *p != '<' )
	{
		if ( doc ) doc->SetError( ONLY_RES_READING_ELEMENTVALUE );
		return 0;
	}
	p = Str_SkipBlank( p+1);
	p = GetName( p, &value, encoding );
	if ( !p || !*p )
	{
		if ( doc ) doc->SetError( 1 );
		return 0;
	};


	ONLY_STRING endTag ("</");
	endTag += value;
	endTag += ">";

	while ( p && *p )
	{
		p = Str_SkipBlank( p );
		if ( !p || !*p )
		{
			if ( doc ) doc->SetError( 1 );
			return 0;
		}
		if ( *p == '/' )
		{
			++p;
			if ( *p  != '>' )
			{
				if ( doc ) doc->SetError( 1 );
				return 0;
			}
			return (p+1);
		}
		else if ( *p == '>' )
		{
			++p;
			p = GetValue( p, doc, encoding );	
			if ( !p || !*p )
				return 0;

			// We should find the end tag now
			if ( Str_Equal( p, endTag.c_str(), false, encoding ) )
			{
				p += endTag.length();
				return p;
			}
			else
			{
				if ( doc ) doc->SetError( 1 );
				return 0;
			}
		}
		else
		{
			// Try to read an attribute:
			XMLAttribute* attr = new XMLAttribute();
			if ( !attr )
			{
				if ( doc ) doc->SetError( 1 );
				return 0;
			}

			attr->SetDocument( doc );
			const char* pErr = p;
			p = attr->Parser( p, encoding );

			if ( !p || !*p )
			{
				if ( doc ) doc->SetError( 1 );
				delete attr;
				return 0;
			}

			XMLAttribute* node = attributes.Find( attr->Name() );
			if ( node )
			{
				node->SetValue( attr->Value() );
				delete attr;
				return 0;
			}

			attributes.Add( attr );
		}
	}
	return p;
}

const char* XMLDeclare::Parser( const char* p, OnlyEncoding _encoding )
{
	p = Str_SkipBlank(p);
	XMLDoc *doc = this->GetDocument();
	if(!p||!Str_Equal(p,"<?xml",true,ONLY_ENCODING_UNKNOW)|| !*p)
	{
		doc->SetError(ONLY_RES_NULL);
	}
	p += 5;

	version = "";
	encoding = "";
	standalone = "";

	while ( p && *p )
	{
		if ( *p == '>' )
		{
			++p;
			return p;
		}

		p = Str_SkipBlank( p );
		if ( Str_Equal( p, "version", true, _encoding ) )
		{
			XMLAttribute attri;
			p = attri.Parser( p, _encoding );		
			version = attri.Value();
		}
		else if ( Str_Equal( p, "encoding", true, _encoding ) )
		{
			XMLAttribute attri;
			p = attri.Parser( p, _encoding );		
			encoding = attri.Value();
		}
		else if ( Str_Equal( p, "standalone", true, _encoding ) )
		{
			XMLAttribute attri;
			p = attri.Parser( p,  _encoding );		
			standalone = attri.Value();
		}
		else
		{
			// Read over whatever it is.
			while( p && *p && *p != '>' && !Char_IsBlank( *p ) )
				++p;
		}
	}

	return 0;

}


const char* XMLDoc::Parser(const char *pData,OnlyEncoding encoding )
{
	if ( encoding == ONLY_ENCODING_UNKNOW )
	{
		const unsigned char* pU = (const unsigned char*) pData;
		if (	*(pU+0) && *(pU+0) == 0xefU
			&& *(pU+1) && *(pU+1) == 0xbbU
			&& *(pU+2) && *(pU+2) == 0xbfU )
		{
			encoding = ONLY_ENCODING_UTF8;
		}
	}
	while(pData &&*pData)
	{
		XMLNode* node = GetXMLNode( pData, encoding );
		if ( node )
		{	
			pData = node->Parser(pData, encoding );
			AddNodeChild( node );
		}
		else
		{
			break;
		}

		if (    encoding == ONLY_ENCODING_UNKNOW
			&& node->ToDeclaration() )
		{
			XMLDeclare* decNode = node->ToDeclaration();
			const char* enc = decNode->Encoding();
			assert( decNode );
			if ( *enc == 0 )
				encoding = ONLY_ENCODING_UTF8;
			else if ( Str_Equal( enc, "UTF-8", true, ONLY_ENCODING_UNKNOW ) )
				encoding = ONLY_ENCODING_UTF8;
			else 
				encoding = ONLY_ENCODING_ASCII;
		}

		pData = Str_SkipBlank( pData );
	}

	if ( !firstChild ) {
		SetError( ONLY_RES_PARSER_EMPTY );
		return 0;
	}

	return pData;
}

const char* XMLAttribute::Parser( const char* p, OnlyEncoding _encoding )
{
	p = Str_SkipBlank( p );
	if ( !p || !*p ) return 0;
	int tabsize = 4;
	if ( doc )
		tabsize = doc->TabSize();

	p = GetName(p,&name,_encoding);
	if(!p)
	{
		cout<<"解析属性名称出错！"<<endl;
		return NULL;
	}

	p = Str_SkipBlank(p);
	if(!p||!*p||*p != '=')
	{
		doc->SetError(ONLY_RES_NULL);
		cout<<"非法属性表达式！"<<endl;
		return NULL;
	}
	++p;	 
	p = Str_SkipBlank( p );
	if(!p)
	{
		cout<<"解析属性名称出错！"<<endl;
		return NULL;
	}
	const char *endtag ;
	if(*p=='\'')
	{
		++p;
		endtag = "\'";
		p = GetText( p, &value, false, endtag,false ,_encoding );
	}else if(*p == '\"')
	{
		++p;
		endtag = "\"";
		p = GetText( p, &value, false, endtag,false ,_encoding );
	}else
	{
		value = "";
		while (    p && *p									 
			&& !Char_IsBlank( *p ) && *p != '\n' && *p != '\r' 
			&& *p != '/' && *p != '>' )						 
		{
			value += *p;
			++p;
		}
	}
	return p;
}


const char* XMLText::Parser( const char* p,OnlyEncoding encoding )
{
	value = "";

	bool ignoreWhite = true;

	const char* endTag = "<";
	p = GetText( p, &value, ignoreWhite, endTag, false, encoding );
	if ( p )
		return p-1; 
	return NULL;
}

const char* XMLComment::Parser( const char* p, OnlyEncoding encoding )
{
	XMLDoc* doc = GetDocument();
	value = "";

	p = Str_SkipBlank( p );

	const char* startTag = "<!--";
	const char* endTag   = "-->";

	if ( !Str_Equal( p, startTag, false, encoding ) )
	{
		doc->SetError( 1 );
		return 0;
	}
	p += strlen( startTag );
	p = GetText( p, &value, false, endTag, false, encoding );
	return p;
}

const char* XMLUnknow::Parser(const char* p,OnlyEncoding encoding )
{
	XMLDoc* doc = GetDocument();

	p = Str_SkipBlank( p );


	if ( !p || !*p || *p != '<' )
	{
		if ( doc ) doc->SetError( 1 );
		return 0;
	}
	++p;
	value = "";

	while ( p && *p && *p != '>' )
	{
		value += *p;
		++p;
	}

	if ( !p )
	{
		if ( doc )	doc->SetError( 1 );
	}
	if ( *p == '>' )
		return p+1;
	return p;
}


