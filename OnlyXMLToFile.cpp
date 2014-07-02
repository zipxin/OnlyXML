//////////////////////////////////////////////////////////////////////////
//File: OnlyXMLToFile.cpp
//Project: Only XML (The XML Lib For Only Game Engine) 
//Author:Zipxin
//Desc ���������ƣ�������ʾXML��Ϣ�ͽ�XML�洢���ļ���ȥ
//       ����Ŀ��OnlyGameEngine��Ŀ������Ŀ
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "OnlyXML.h"

//Display() ������ʾ , ��д����Ŀʱ�����õ�
//			�����ĵ�һ�����ܣ�����cout������ʾ
//          �����п���̨�ĳ����ˣ�������ܿ��п���
//			
//ToFile() ����̻����ı���ȥ

void XMLDoc::Display() const 
{
	const XMLNode* node;
	for ( node = FirstChild_Const(); node; node=node->NextSibling_Const() )
	{
		node->Display();
	}
}

void XMLDoc::ToFile(FILE *file,int blankdepth)
{
	XMLNode* nodetemp = NULL;
	for ( nodetemp=FirstChild(); nodetemp; nodetemp=nodetemp->NextSibling() )
	{
		nodetemp->ToFile( file, blankdepth );
		fprintf_s( file, "\n" );
	}
}


void XMLDeclare::Display() const 
{
	cout << "<?xml ";

	if ( !version.empty() )
	{
		cout << "version=\"";
		cout<<version.c_str();
		cout << "\" ";
	}
	if ( !encoding.empty() )
	{
		cout << "encoding=\"";
		cout<<encoding.c_str();
		cout << "\" ";
	}
	if ( !standalone.empty() )
	{
		cout << "standalone=\"";
		cout <<standalone.c_str();
		cout << "\" ";
	}
	cout << "?>";
}


void XMLDeclare::ToFile(FILE *file,int blankdepth)
{
	fprintf_s (file, "<?xml ");

	if ( !version.empty() )
		fprintf_s (file, "version=\"%s\" ", version.c_str ());
	if ( !encoding.empty() )
		fprintf_s (file, "encoding=\"%s\" ", encoding.c_str ());
	if ( !standalone.empty() )
		fprintf_s (file, "standalone=\"%s\" ", standalone.c_str ());
	fprintf_s (file, "?>");
}

void XMLElement::Display() const 
{
	cout << "<" << value;

	const XMLAttribute* attrib = NULL;
	for ( attrib = attributes.First_Const(); attrib; attrib = attrib->Next_Const() )
	{	
		cout << " ";
		attrib->Display();
	}

	XMLNode* node = NULL;
	if ( firstChild )
	{ 		
		cout << ">";

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			node->Display( );
		}
		cout << "</" << value << ">";
	}
	else
	{
		cout << " />";
	}
}

//���ԣ�TinyXML
void XMLElement::ToFile(FILE *file,int blankdepth)
{
	int i;
	for ( i = 0; i < blankdepth; i++ )
	{
		fprintf( file, "    " );
	}

	fprintf_s( file, "<%s", value.c_str() );

	XMLAttribute* attri = NULL;
	for ( attri = attributes.First(); attri; attri = attri->Next() )
	{
		fprintf_s( file, " " );
		attri->ToFile( file, blankdepth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	
	if ( !firstChild )
	{
		fprintf_s( file, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf_s( file, ">" );
		firstChild->ToFile( file, blankdepth + 1 );
		fprintf_s( file, "</%s>", value.c_str() );
	}
	else
	{
		XMLNode* node = NULL;
		fprintf_s( file, ">" );
		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf_s( file, "\n" );
			}
			node->ToFile( file, blankdepth + 1 );
		}
		fprintf_s( file, "\n" );
		for( i=0; i < blankdepth; ++i )
			fprintf_s( file, "    " );
		fprintf_s( file, "</%s>", value.c_str() );
	}
}

void XMLAttribute::Display() const 
{
	if (value.find( '\"' ) != ONLY_STRING::npos)
	{
		cout<<name.c_str();
		cout << "=" << "'";
		cout<<value.c_str();
		cout << "'";
	}
	else
	{
		cout<<name.c_str();
		cout << "=" << "\"";
		cout<<value.c_str();
		cout << "\"";
	}
}
void XMLAttribute::ToFile(FILE *file,  int blankdepth)
{
	ONLY_STRING n, v;

	PutString( name, &n );
	PutString( value, &v );

	if (value.find ('\"') == ONLY_STRING::npos)
		fprintf_s (file, "%s=\"%s\"", n.c_str(), v.c_str() );
	else
		fprintf_s (file, "%s='%s'", n.c_str(), v.c_str() );
}

void XMLUnknow::Display() const 
{
	cout << "<" << value.c_str() << ">";
}
void XMLUnknow::ToFile(FILE *file,int blankdepth)
{
	for ( int i = 0; i < blankdepth; i++ )
		fprintf( file, "    " );
	fprintf( file, "<%s>", value.c_str() );
}

void XMLText::Display() const 
{
	cout<<value.c_str()<<endl;
}

void XMLText::ToFile(FILE *file,int blankdepth)
{
	ONLY_STRING buffer;
	PutString( value, &buffer );
	fprintf_s( file, "%s", buffer.c_str() );
}


void XMLComment::Display() const 
{
	cout << "<!--";
	cout << value;
	cout << "-->";
}
void XMLComment::ToFile(FILE *file,int blankdepth)
{
	for ( int i=0; i<blankdepth; i++ )
	{
		fputs( "    ", file );
	}
	fprintf_s( file, "<!--%s-->", value.c_str() );
}
