//////////////////////////////////////////////////////////////////////////
//File: OnlyXML.h
//Project: Only XML (The XML Lib For Only Game Engine) 
//Author:Zipxin
//Desc ��ԭ������Ŀ����Javaд�ģ�֮��Java�汾Դ�붪ʧ
//       ��࣬��ʱ��ȫ��תսC++����������C++���и�д
//		 ��д�����У��ο�������XML�Ľ�����Դ�룬���Ա�����
//		 ����һ���ļ򻯣�ʹ��ֻ��Ҫ��������Ŀ�������ļ�
//		 �Լ��򵥵����ݴ洢��
//       ����Ŀ��OnlyGameEngine��Ŀ������Ŀ
//Attention������Ŀ��֮ǰJava��Ŀ�кܴ�������Ҫԭ������֮��
//		  ���Ķ���TinyXML ��ܵ�Դ�������Ӱ��Ƚϴ󡣸о�TinyXML
//		  ��һ��ǳ������xml��������ܣ����Ա���Ŀ���ٴ��붼�����ڸ�
//        ��ܡ�����tinyxml ���������е��ٶȲ����Ǻܿ죬������������ֻ
//		  �����Ϸ��OnlyGameEngine����������˼򻯺�����һ�����Ż�
//        ʹ����õ�Ϊ��Ϸ����
//<!--	רΪ��Ϸ���� ��OnlyGameEngine��  -->
//<!--	����Ⲣû��ʹ��OnlyGameEngine����ļ�����һ����������Ŀ  -->
//////////////////////////////////////////////////////////////////////////
#ifndef __ONLY_XML_H__
#define __ONLY_XML_H__

#ifdef _MSC_VER
#	pragma once
#endif

#if defined( _DEBUG ) && !defined( DEBUG )
#define DEBUG
#endif

//16����ʶλ
#define ONLY_ERROR_COUNT 15

//ע����ONLYXML�� 0������ȷ����
#define ONLY_RES_SUCCESS                    0// 0x0000
#define ONLY_RES_UNKNOWERROR                1// 0x0001
#define ONLY_RES_FILEOPENFAIL               2// 0x0002
#define ONLY_RES_NULL                       3// 0x0004
#define ONLY_RES_OUTOFMEMORY                4// 0x0008
#define ONLY_RES_PARSER_EMPTY               5// 0x0010 
#define ONLY_RES_READING_ELEMENTVALUE       6// 0x0020
#define ONLY_RES_READING_ATTRIBUTES         7// 0x0040
#define ONLY_RES_READING_END_TAG            8// 0x0080
#define ONLY_RES_PARSER_DECLARATION         9// 0x0100
#define ONLY_RES_EMPTY_DOCUMENT             10// 0x0200            
#define ONLY_RES_PARSER_UNKNOW              11// 0x0400
#define ONLY_RES_PARSER_COMMENT             12
#define ONLY_RES_PARSER_TEXT_ERROR          13
#define ONLY_RES_READ_ATTRIBUTE_VALUE_FAIL  14

/*
const int ONLY_MAJOR_VERSION = 0;
const int ONLY_MINOR_VERSION = 0;
const int ONLY_PATCH_VERSION = 1;
*/

#define ONLYXML_VERSION 0001
#define ONLYXML_NAME    "OnlyXML"
#define ONLYXML_DESC    "OnlyXML For OnlyGameEngine  Author��zipxin"

#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <vector>
using namespace std;

// �����ַ��Լ��ַ�����Util��������
#include "StrUtil.h"

//////////////////////////////////////////////////////////////////////////
//OnlyXML�ļ̳й�ϵ��
class XMLBase;
	class XMLNode;
		class XMLDoc;
		 class XMLDeclare;
		 class XMLElement;
		 class XMLAttribute;
		 class XMLText;
		 class XMLComment;
		 class XMLUnknow; //��֧�ֽڵ�����
//�����ַ���ʵ��
struct XMLEntity;
//�����ṹ��
struct XMLError;

//////////////////////////////////////////////////////////////////////////
#define ONLYXML_ENTITY_NUM 5
struct XMLEntity
{
	const char*     str;        //ʵ����ַ���
	unsigned int	strLength;  //ʵ���ַ����ĳ���
	char		    chr;        //ָ����ʵ���ַ�

};
extern XMLEntity entity[ ONLYXML_ENTITY_NUM ];

//////////////////////////////////////////////////////////////////////////
//����λ�ʹ�������Ҫ���ĺܶ�����
//������������麯���Ľṹ������Ѿ������ټ���������
//�Ҽ���������ȷ��д��xml�ļ�
//����Ĵ����Ƿǳ���ͳ�Ĵ���
//һ�������������ǽ�ֹͣ����������Ի�ȡ����ļ���
//���Ǳ�ָ���õ�ʲô���õ���Ϣ

//XML�еĽṹ����OnlyGameEngine�й㷺ʹ��
//���Ѿ������� ����Ϊ����Э��
struct XMLError
{
	int level;             //����ȼ� [������ܶ�����趨]
	int id;                //�����ʶ
	const char *desc;      //��������
	bool    isIgnore;      //�Ƿ���Ա�����
};

//�Դ���������ַ���
const extern char* ErrorDesc[ ONLY_ERROR_COUNT ];

#define ONLYXML_ERROR_LEVEL_DEBUG 0
#define ONLYXML_ERROR_LEVEL_ERROR 1

typedef std::vector<XMLError> ErrorPool;

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�������ú���
//////////////////////////////////////////////////////////////////////////
//Userful Function
//��ȡ��Ч�ı�ʶ
//���ػ�ȡ��ʶ����ַ���
//�޷��ж�ȡ�����ƵĺϷ���
//�����pstrǰ�治���пո�
//������Char_IsAlpha(*pstr, encoding) || *pstr == '_'
//������������
const char* GetName(const char * pstr, ONLY_STRING *name, OnlyEncoding encoding);

//��ȡ�ı��ڵ�
const char *GetText(const char *pstr, ONLY_STRING *name, bool isIgnoreBlank, const char *endtag,bool IgnoreCase, OnlyEncoding encoding );

//��ȡʵ���ַ�
const char* GetEntity( const char* pstr, char* &value, int* length, OnlyEncoding encoding );

void PutString( const ONLY_STRING& str, ONLY_STRING* outString );

//////////////////////////////////////////////////////////////////////////
//���ࣺһ��XML�ڵ�ĸ���
class XMLBase
{
	friend class XMLNode;
	friend class XMLELement;
	friend class XMLDoc;
private:     //��������п���
	XMLBase( const XMLBase& );				 
	void operator=( const XMLBase& base );	 
protected :
	static bool    isIgnoreBlank;
public: 
	XMLBase(){}
	virtual ~XMLBase(){}
	
	//������ʾ
	virtual void Display() const = 0;
	//����̻����ı���
	//File������ı�ָ��
	virtual void ToFile(FILE *file, int blankdepth) = 0;

	//�Ƿ���Կհ�
	inline static bool IgnoreBlank()                { return isIgnoreBlank; }
	inline static void SetIgnoreBlank(bool isIgnore){ isIgnoreBlank = isIgnore; }
	
	//�����ฺ��ʵ��ÿ���ڵ�Ľ�����ʽ
	virtual const char* Parser(	const char* p,  OnlyEncoding encoding  ) = 0;
};


class XMLNode : public XMLBase
{
public:
	enum NodeType
	{
		DOCUMENT,
		ELEMENT,
		COMMENT,
		UNKNOWN,
		TEXT,
		DECLARATION,
	};

	const ONLY_STRING& Value()       const { return value ; }
	const char        *Value_C_Str() const { return value.c_str (); }
	void SetValue(const char * _value) { value = _value;}
	void SetValue( const ONLY_STRING& _value )    
	{	  
		SetValue( _value.c_str() );
	}
	virtual void Display() const {}
	virtual void ToFile(FILE *file , int blankdepth){}
protected:
	XMLNode*		parent;
	NodeType		type;

	XMLNode*		firstChild;
	XMLNode*		lastChild;

	ONLY_STRING	    value;

	XMLNode*		prev;
	XMLNode*		next;

private:
	XMLNode( const XMLNode& );				 
	void operator=( const XMLNode& base ); 
public:
	XMLNode( NodeType _type ):XMLBase(),parent(NULL),type(_type),firstChild(NULL),lastChild(NULL),prev(NULL),next(NULL){}	
	virtual ~XMLNode();

	//Ŀǰ�����нڵ�ʵ����
	//��ʵ�ڲ������룬����һ���ڵ�����Ϸ�������
	virtual XMLNode* Clone() const = 0;

	//��ֵ���Ƹ�Ŀ��ڵ�
	void CopyTo( XMLNode* target ) const;

	//������е��ӽڵ�
	void Clear();

	//���ݴ����ָ��p �õ���Ҫ�����Ľڵ������
	//����Ƿ�p ���� ����< ��ͷֱ�ӷ���NULL
	XMLNode * GetXMLNode(const char *p, OnlyEncoding encoding);

	int  Type() const { return type; }
	bool IsNoChildren() const { return !firstChild; }

//////////////////////////////////////////////////////////////////////////
	//Node Operator
	//��Ӧ��ͬ����
	//��������İ汾��Խڵ������֮����,ֻ���������
	//���� ����Tiny֮���ţ����¿���֪����
	XMLNode* Parent()							{ return parent; }
	const XMLNode* Parent_Const() const			{ return parent; }

	const XMLNode* FirstChild_Const()	const	{ return firstChild; }		 
	XMLNode* FirstChild()					    { return firstChild; }
	
	const XMLNode* FirstFindChild_Const( const char * value ) const;			 
	XMLNode* FirstFindChild( const char * value );						

	const XMLNode* LastChild_Const() const	{ return lastChild; }		 
	XMLNode* LastChild()	                { return lastChild; }
	
	const XMLNode* LastFindChild_Const( const char * value ) const;			 
	XMLNode* LastFindChild( const char * value );	


	const XMLNode* FirstChild_Const( const ONLY_STRING& _value ) const	{	return FirstChild_Const (_value.c_str ());	}	 
	XMLNode*       FirstChild( const ONLY_STRING& _value )				{	return FirstChild (_value.c_str ());	} 
	const XMLNode* LastChild_Const( const ONLY_STRING& _value ) const	{	return LastChild_Const (_value.c_str ());	} 
	XMLNode*       LastChild( const ONLY_STRING& _value )				{	return LastChild (_value.c_str ());	} 

	const XMLNode* IterateChildren( const XMLNode* previous ) const;
	XMLNode* IterateChildren( XMLNode* previous );


	bool AddNodeChild(XMLNode *_addOne);

	bool InsertBeforeChild( XMLNode* beforeOne, const XMLNode& _addOne );

	bool InsertAfterChild(  XMLNode* afterOne, const XMLNode& _addOne );

	bool ReplaceChild( XMLNode* replaceOne, const XMLNode& _newOne );

	bool RemoveChild( XMLNode* removeOne );


	//���ൽ���������ת��
	//����TinyXML��˼·
	const XMLDoc*         ToDocument_Const()	const		{ return ( this && type == DOCUMENT )    ? (const XMLDoc*) this : 0; } 
	const XMLElement*     ToElement_Const()     const		{ return ( this && type == ELEMENT  )    ? (const XMLElement*)  this : 0; }  
	const XMLComment*     ToComment_Const()     const		{ return ( this && type == COMMENT  )    ? (const XMLComment*)  this : 0; } 
	const XMLUnknow*      ToUnknown_Const()     const		{ return ( this && type == UNKNOWN  )    ? (const XMLUnknow*)  this : 0; }  
	const XMLText*	      ToText_Const()        const		{ return ( this && type == TEXT     )    ? (const XMLText*)     this : 0; } 
	const XMLDeclare*     ToDeclaration_Const() const	    { return ( this && type == DECLARATION ) ? (const XMLDeclare*) this : 0; }  

	XMLDoc*      ToDocument()			{ return ( this && type == DOCUMENT ) ? (XMLDoc*) this : 0; }  
	XMLElement*  ToElement()			{ return ( this && type == ELEMENT  ) ? (XMLElement*)  this : 0; }  
	XMLComment*  ToComment()			{ return ( this && type == COMMENT  ) ? (XMLComment*)  this : 0; }  
	XMLUnknow*   ToUnknown()			{ return ( this && type == UNKNOWN  ) ? (XMLUnknow*)  this : 0; }  
	XMLText*	 ToText()   			{ return ( this && type == TEXT     ) ? (XMLText*)     this : 0; }  
	XMLDeclare*  ToDeclaration()	    { return ( this && type == DECLARATION ) ? (XMLDeclare*) this : 0; }  

	//��ȡ�ĵ��ڵ㣬ÿ���ڵ㶼��Ӧ���Ƕ������ڵģ��丸�ڵ�϶����ĵ��ڵ�
	//ֻ���ĵ��ڵ���ܴ����ͼ���XML�ļ�
	const XMLDoc* GetDocument_Const() const;
	XMLDoc* GetDocument();

	const XMLNode* PreviousNode_Const() const		        { return prev; }
	XMLNode*       PreviousSibling()						{ return prev; }

	const XMLNode* NextSibling_Const() const				{ return next; }
	XMLNode*       NextSibling()							{ return next; }

	XMLNode*       FindChildrenNext( const char * val );
	const XMLNode* FindChildrenNext_Const( const char * val ) const;

	XMLNode*       FindChildrenPrev( const char * val );
	const XMLNode* FindChildrenPrev_Const( const char * val ) const;

	
	//�����ӽڵ�
	const XMLElement* FirstChildElement_Const()	const;
	XMLElement* FirstChildElement();

	const XMLElement* FirstChildElement_Const( const char * value ) const;
	XMLElement* FirstChildElement( const char * value );
	const XMLElement* FirstChildElement_Const( const std::string& _value ) const	{	return FirstChildElement_Const (_value.c_str ());	}	 
	XMLElement* FirstChildElement( const std::string& _value )				{	return FirstChildElement (_value.c_str ());	}	 

	const XMLElement* NextSiblingElement() const;
	XMLElement* NextSiblingElement();
};

//////////////////////////////////////////////////////////////////////////
//�ĵ��ڵ㣺
//����XML�ĵ�Ԫ�صĸ��ڵ�
// XMLDoc
//	 XMLDeclare
//   XMLElement
//		XMLAttribute
//      XMLText
//      XMLComment
class XMLDoc : public XMLNode
{
private:
	bool        isError;
	ErrorPool   m_ErrorPool;
	int         tabSize;
public:

	XMLDoc():XMLNode(DOCUMENT),tabSize(4){ ClearError(); }
	XMLDoc(const char *_filename):XMLNode( DOCUMENT ),tabSize(4){SetValue(_filename); ClearError(); }
	virtual ~XMLDoc() {}

	bool LoadFile(const char * filename, OnlyEncoding encoding = ONLY_ENCODING_UNKNOW);
	bool SaveFile(const char * filename);
	bool LoadFile( OnlyEncoding encoding = ONLY_ENCODING_UNKNOW );
	bool SaveFile();

	//����û ��ʵ�֣��õĻ�...
	virtual XMLNode* Clone() const { return NULL;};
	
	virtual void Display() const ;
	virtual void ToFile(FILE *file, int blankdepth);
	
	void ClearError()						
	{	
		isError = false; 
		m_ErrorPool.clear();
	}

	void SetTabSize( int _tabsize )		{ tabSize = _tabsize; }
	int TabSize() const	{ return tabSize; }

	bool    HasError() const						{ return isError; }
	
	virtual const char*  Parser(const char *pData, OnlyEncoding encoding = ONLY_ENCODING_UNKNOW);

#define ONLYXML_MAX_ERRORS 16
	inline void SetError( int id,  bool  isIgnore = false, int _level = ONLYXML_ERROR_LEVEL_ERROR)
	{
		assert( id > 0 && id < ONLY_ERROR_COUNT );
		static int ErrorIndex = 0;

		XMLError err;
		err.id = id;
		err.desc = ErrorDesc[id];
		err.level = _level;
		err.isIgnore = isIgnore;
		
		isError = true;
		++ErrorIndex;
		//��ֹPush ����Ĵ�����Ϣ
		//��ʵһ����Ϣ�Ͳ����
		//�����ܶ�λ��Ϣ�Ĵ���λ��
		//��һ�㲻�� TinyXML
		//���ǵ����ܣ��Ҽ������ǵ�xml�ļ�д�Ķ��൱��ȷ
		if(ErrorIndex > ONLYXML_MAX_ERRORS)
			return;
		m_ErrorPool.push_back(err);
	}
};

//////////////////////////////////////////////////////////////////////////
//���Խڵ�
//���Խڵ�Ƚ����⣬�����������ڵ㣬�ڵ������ӽڵ�
//������һ��ֻ���ֵܽڵ������ļһ���һ����ֵ��
class XMLAttribute : public XMLBase
{
	friend class XMLAttributeOp;
private :
	XMLDoc *doc;
	XMLAttribute *prev;
	XMLAttribute *next;

	ONLY_STRING name;
	ONLY_STRING value;

	XMLAttribute  ( const XMLAttribute& );			 
	void operator=( const XMLAttribute& base ); 
public:
	XMLAttribute() : doc(0),prev(0),next(0){}
	XMLAttribute(const ONLY_STRING &_name,const ONLY_STRING &_value ):doc(0),prev(0),next(0),name(_name),value(_value){}
	XMLAttribute( const char * _name, const char * _value ):doc(0),prev(0),next(0),name(_name),value(_value){}

	const char*		Name()  const		{ return name.c_str (); }	 
	const char*		Value() const		{ return value.c_str (); }	 
	void SetName( const char* _name )	{ name = _name; }			
	void SetValue( const char* _value )	{ value = _value; }		
	
	void SetIntValue( int _value );
	void SetDoubleValue( double _value );

	const int IntValue_Const() const;
	const double  DoubleValue_Const() const;
	
	void SetDocument( XMLDoc* _doc )	{ doc = _doc; }

	const XMLAttribute* Next_Const() const;
	XMLAttribute* Next();

	const XMLAttribute* Previous_Const() const;
	XMLAttribute* Previous();

	void Clear()
	{
		name.clear();
		value.clear();
		doc = NULL;
		prev = NULL;
		next = NULL;
	}

	bool Empty()
	{
		if(name.empty() && value.empty()) return true;
		return false;
	}

	const char*  Parser( const char* p, OnlyEncoding encoding );
	
	virtual void Display() const ;
	virtual void ToFile(FILE *file, int blankdepth);

	//Operator 
	//ֻ�Ƚ�����
	bool operator==( const XMLAttribute& rhs ) const { return rhs.name == name; }
	bool operator< ( const XMLAttribute& rhs )	 const { return name < rhs.name; }
	bool operator> ( const XMLAttribute& rhs )  const { return name > rhs.name; }
};

//ÿ���ڵ㶼��һ��������
//������Ƕ�������������ɾ�Ĳ�Ĳ�����
//��ÿ��Ԫ�ؽڵ�����������
class XMLAttributeOp
{
public:
	XMLAttributeOp():head(NULL){}
	~XMLAttributeOp()
	{
		assert(!head);
	}
	const XMLAttribute* Iterator_Const() const		     { return ( !head ) ? 0 : head->prev; }
	XMLAttribute* Iterator()					         { return ( !head ) ? 0 : head->prev; }

	const XMLAttribute*	Find_Const( const char * name ) const;
	XMLAttribute*	Find( const char * name );
	void Add( XMLAttribute* attribute );
	void Remove( XMLAttribute* attribute );
	void Clear();

	XMLAttribute *First() { return head; }
	const XMLAttribute *First_Const() const { return head; }
private:
	XMLAttributeOp( const XMLAttributeOp& );	 
	void operator=( const XMLAttributeOp& );	 
	XMLAttribute *head;
};

//////////////////////////////////////////////////////////////////////////
//Ԫ�ؽڵ�
class XMLElement : public XMLNode
{
private:
	XMLAttributeOp attributes;
public:
	XMLElement():XMLNode(ELEMENT){  SetValue(value); }
	XMLElement(const char _value):XMLNode(ELEMENT){SetValue(value);}
	virtual ~XMLElement();
	void ClearElemet();

	//�������⼸�������ᾭ��ʹ��
	//���ص�value���ַ�����ʽ
	const char* GetAttributeValue( const char* name ) const;
	const char* GetIntAttributeValue( const char* name, int* i ) const;
	const char* GetDoubleAttributeValue( const char* name, double* d ) const;

	//������ĺ����Գ�
	void SetAttribute( const char* name, const char * value );
	void SetAttribute( const char * name, int val );
	void SetAttribute( const char * name, double val );
	
	void RemoveAttribute( const char * name );

	//��������ķ��������� Parser����
	//������������Ԫ�ؽڵ�Ľ�β��<element>ʱ��������������
	//��ȡ���ӽڵ�
	//��ҪDOC ��¼����GetDocument�������������ģ��ٴ��Ż���
	const char* GetValue( const char* p, XMLDoc *doc, OnlyEncoding encoding );

	virtual XMLNode* Clone() const {return NULL;}


	virtual void Display() const;
	virtual void ToFile(FILE *file, int blankdepth);

	virtual const char* Parser(	const char* p,  OnlyEncoding encoding  );
};

//////////////////////////////////////////////////////////////////////////
//ע�ͽڵ㣬ûʲô�ر��
//�������㲻���ر��ע����ڵ�
//��Ҳ����ע
class XMLComment : public XMLNode
{
public:
	XMLComment() : XMLNode( COMMENT ) {}
	XMLComment( const XMLComment& );
	void operator=( const XMLComment& base );
	virtual ~XMLComment()	{}
	virtual XMLNode* Clone() const {return NULL;};
	virtual void Display() const;
	virtual void ToFile(FILE *file, int blankdepth);
	const char*  Parser( const char* p, OnlyEncoding encoding );
};

//////////////////////////////////////////////////////////////////////////
//�����ڵ�
//û�£��Ҳ�Ҫ��xmlһ��Ҫ������ڵ�
class XMLDeclare : public XMLNode
{
private:
	ONLY_STRING version;
	ONLY_STRING encoding;
	ONLY_STRING standalone;
public:
	//���Դ�ȡ
	const char *Version() const	   { return version.c_str (); }
	const char *Encoding() const   { return encoding.c_str (); }
	const char *Standalone() const { return standalone.c_str (); }

	XMLDeclare():XMLNode( DECLARATION ) {}

	XMLDeclare(	const char* _version,
		const char* _encoding,
		const char* _standalone ):XMLNode( DECLARATION ),
		version(_version),
		encoding(_encoding),
		standalone(_standalone){}
	virtual ~XMLDeclare()	{}

	virtual XMLNode* Clone() const {return NULL;};
	
	virtual void Display() const ;
	virtual void ToFile(FILE *file, int blankdepth);
	
	virtual const char* Parser(	const char* p,   OnlyEncoding encoding  );
};

//////////////////////////////////////////////////////////////////////////
//δ֪�ڵ�
//��ֻ��˵, OnlyXML�ᾡ�������������ڴ���ʲô��ϲ
//������ñ����������������Ľڵ㣬������Ϊ���ϵͳ����
class XMLUnknow : public XMLNode
{
public:
	XMLUnknow() : XMLNode( XMLNode::UNKNOWN )	{}
	virtual ~XMLUnknow() {}

	virtual XMLNode* Clone() const {return NULL;};

	virtual void Display() const ;
	virtual void ToFile(FILE *file, int blankdepth);

	XMLUnknow( const XMLUnknow& copy ) : XMLNode( XMLNode::UNKNOWN )		{ copy.CopyTo( this ); }
	void operator=( const XMLUnknow& copy )									{ copy.CopyTo( this ); }
	const char* Parser( const char* p,OnlyEncoding encoding );
	void CopyTo( XMLUnknow* target ) const;
};

//////////////////////////////////////////////////////////////////////////
//�ı��ڵ�
class XMLText : public XMLNode
{
public:
	XMLText(const char * _value) : XMLNode( TEXT ) { SetValue(_value);}

	XMLText( const XMLText& );
	void operator=( const XMLText& base );

	virtual ~XMLText()	{}
	bool IsBlank();

	virtual XMLNode* Clone() const {return NULL;};

	virtual void Display() const ;
	virtual void ToFile(FILE *file, int blankdepth);

	virtual const char*  Parser( const char* p,OnlyEncoding encoding );
};

#endif /*File Is Over*/