//////////////////////////////////////////////////////////////////////////
//File: OnlyXML.h
//Project: Only XML (The XML Lib For Only Game Engine) 
//Author:Zipxin
//Desc ：原来本项目是用Java写的，之后，Java版本源码丢失
//       差不多，那时我全面转战C++，于是我用C++进行改写
//		 改写过程中，参考了其他XML的解释器源码，并对本代码
//		 做了一定的简化，使其只主要作用与项目的配置文件
//		 以及简单的数据存储。
//       本项目是OnlyGameEngine项目的子项目
//Attention：本项目与之前Java项目有很大区别，主要原因还是在之后
//		  我阅读了TinyXML 框架的源码后，受其影响比较大。感觉TinyXML
//		  是一款非常优秀的xml解释器框架，所以本项目不少代码都来自于该
//        框架。但是tinyxml 解释器运行的速度并不是很快，所以在这里我只
//		  针对游戏（OnlyGameEngine）对其进行了简化和做了一定的优化
//        使其更好的为游戏服务
//<!--	专为游戏打造 【OnlyGameEngine】  -->
//<!--	本类库并没有使用OnlyGameEngine相关文件，是一个独立的项目  -->
//////////////////////////////////////////////////////////////////////////
#ifndef __ONLY_XML_H__
#define __ONLY_XML_H__

#ifdef _MSC_VER
#	pragma once
#endif

#if defined( _DEBUG ) && !defined( DEBUG )
#define DEBUG
#endif

//16个标识位
#define ONLY_ERROR_COUNT 15

//注意在ONLYXML中 0代表真确返回
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
#define ONLYXML_DESC    "OnlyXML For OnlyGameEngine  Author：zipxin"

#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <vector>
using namespace std;

// 处理字符以及字符串的Util函数集合
#include "StrUtil.h"

//////////////////////////////////////////////////////////////////////////
//OnlyXML的继承关系链
class XMLBase;
	class XMLNode;
		class XMLDoc;
		 class XMLDeclare;
		 class XMLElement;
		 class XMLAttribute;
		 class XMLText;
		 class XMLComment;
		 class XMLUnknow; //不支持节点类型
//特殊字符的实体
struct XMLEntity;
//辅助结构体
struct XMLError;

//////////////////////////////////////////////////////////////////////////
#define ONLYXML_ENTITY_NUM 5
struct XMLEntity
{
	const char*     str;        //实体的字符串
	unsigned int	strLength;  //实体字符串的长度
	char		    chr;        //指代的实体字符

};
extern XMLEntity entity[ ONLYXML_ENTITY_NUM ];

//////////////////////////////////////////////////////////////////////////
//错误定位和错误检查需要消耗很多性能
//在这个到处是虚函数的结构里，性能已经不能再继续消耗了
//我假设你能正确的写出xml文件
//这里的错误将是非常笼统的错误
//一旦检测出错误，我们将停止解析，你可以获取错误的集合
//但是别指望得到什么有用的信息

//XML中的结构将在OnlyGameEngine中广泛使用
//你已经看出了 他们为了是协调
struct XMLError
{
	int level;             //错误等级 [在这里很多余的设定]
	int id;                //错误标识
	const char *desc;      //错误描述
	bool    isIgnore;      //是否可以被忽略
};

//对错误的描述字符串
const extern char* ErrorDesc[ ONLY_ERROR_COUNT ];

#define ONLYXML_ERROR_LEVEL_DEBUG 0
#define ONLYXML_ERROR_LEVEL_ERROR 1

typedef std::vector<XMLError> ErrorPool;

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//解析常用函数
//////////////////////////////////////////////////////////////////////////
//Userful Function
//获取有效的标识
//返回获取标识后的字符串
//无法判断取得名称的合法性
//传入的pstr前面不能有空格
//必须是Char_IsAlpha(*pstr, encoding) || *pstr == '_'
//才能正常解析
const char* GetName(const char * pstr, ONLY_STRING *name, OnlyEncoding encoding);

//获取文本节点
const char *GetText(const char *pstr, ONLY_STRING *name, bool isIgnoreBlank, const char *endtag,bool IgnoreCase, OnlyEncoding encoding );

//获取实体字符
const char* GetEntity( const char* pstr, char* &value, int* length, OnlyEncoding encoding );

void PutString( const ONLY_STRING& str, ONLY_STRING* outString );

//////////////////////////////////////////////////////////////////////////
//基类：一切XML节点的父类
class XMLBase
{
	friend class XMLNode;
	friend class XMLELement;
	friend class XMLDoc;
private:     //不允许进行拷贝
	XMLBase( const XMLBase& );				 
	void operator=( const XMLBase& base );	 
protected :
	static bool    isIgnoreBlank;
public: 
	XMLBase(){}
	virtual ~XMLBase(){}
	
	//负责显示
	virtual void Display() const = 0;
	//负责固化到文本上
	//File传入的文本指针
	virtual void ToFile(FILE *file, int blankdepth) = 0;

	//是否忽略空白
	inline static bool IgnoreBlank()                { return isIgnoreBlank; }
	inline static void SetIgnoreBlank(bool isIgnore){ isIgnoreBlank = isIgnore; }
	
	//由子类负责实现每个节点的解析方式
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

	//目前很少有节点实现它
	//我实在不容易想，复制一个节点在游戏里的意义
	virtual XMLNode* Clone() const = 0;

	//将值复制给目标节点
	void CopyTo( XMLNode* target ) const;

	//清空所有的子节点
	void Clear();

	//根据传入的指针p 得到需要解析的节点的类型
	//如果非法p 或者 不以< 开头直接返回NULL
	XMLNode * GetXMLNode(const char *p, OnlyEncoding encoding);

	int  Type() const { return type; }
	bool IsNoChildren() const { return !firstChild; }

//////////////////////////////////////////////////////////////////////////
	//Node Operator
	//适应不同需求
	//在我最初的版本里，对节点操作少之又少,只有最基本的
	//但是 看了Tiny之后，嗯，往下看就知道了
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


	//父类到子类的所有转换
	//来自TinyXML的思路
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

	//获取文档节点，每个节点都不应该是独立存在的，其父节点肯定是文档节点
	//只有文档节点才能创建和加载XML文件
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

	
	//遍历子节点
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
//文档节点：
//所有XML文档元素的父节点
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

	//看到没 不实现，用的话...
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
		//防止Push 过多的错误信息
		//其实一条信息就差不多了
		//并且能定位信息的错误位置
		//这一点不如 TinyXML
		//考虑到性能，我假设你们的xml文件写的都相当正确
		if(ErrorIndex > ONLYXML_MAX_ERRORS)
			return;
		m_ErrorPool.push_back(err);
	}
};

//////////////////////////////////////////////////////////////////////////
//属性节点
//属性节点比较特殊，他不像其他节点，节点下有子节点
//他就是一个只有兄弟节点的特殊的家伙，存放一个键值对
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
	//只比较名称
	bool operator==( const XMLAttribute& rhs ) const { return rhs.name == name; }
	bool operator< ( const XMLAttribute& rhs )	 const { return name < rhs.name; }
	bool operator> ( const XMLAttribute& rhs )  const { return name > rhs.name; }
};

//每个节点都有一个属性链
//本类就是对属性链进行增删改查的操作类
//是每个元素节点必须包含的类
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
//元素节点
class XMLElement : public XMLNode
{
private:
	XMLAttributeOp attributes;
public:
	XMLElement():XMLNode(ELEMENT){  SetValue(value); }
	XMLElement(const char _value):XMLNode(ELEMENT){SetValue(value);}
	virtual ~XMLElement();
	void ClearElemet();

	//相信我这几个函数会经常使用
	//返回的value的字符串形式
	const char* GetAttributeValue( const char* name ) const;
	const char* GetIntAttributeValue( const char* name, int* i ) const;
	const char* GetDoubleAttributeValue( const char* name, double* d ) const;

	//与上面的函数对称
	void SetAttribute( const char* name, const char * value );
	void SetAttribute( const char * name, int val );
	void SetAttribute( const char * name, double val );
	
	void RemoveAttribute( const char * name );

	//这个函数的分量不亚于 Parser（）
	//当解释器发现元素节点的结尾是<element>时，会调用这个函数
	//获取其子节点
	//需要DOC 记录错误，GetDocument（）有性能消耗，再次优化了
	const char* GetValue( const char* p, XMLDoc *doc, OnlyEncoding encoding );

	virtual XMLNode* Clone() const {return NULL;}


	virtual void Display() const;
	virtual void ToFile(FILE *file, int blankdepth);

	virtual const char* Parser(	const char* p,  OnlyEncoding encoding  );
};

//////////////////////////////////////////////////////////////////////////
//注释节点，没什么特别的
//我相信你不会特别关注这个节点
//我也不关注
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
//声明节点
//没事，我不要求xml一定要有这个节点
class XMLDeclare : public XMLNode
{
private:
	ONLY_STRING version;
	ONLY_STRING encoding;
	ONLY_STRING standalone;
public:
	//属性存取
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
//未知节点
//我只能说, OnlyXML会尽力解析，但别期待有什么惊喜
//如果你用本解释器解释这样的节点，我真心为你的系统担心
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
//文本节点
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