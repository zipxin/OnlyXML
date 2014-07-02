//////////////////////////////////////////////////////////////////////////
//File: OnlyXML.cpp
//Project: Only XML (The XML Lib For Only Game Engine) 
//Author:Zipxin
//Desc ：XML节点函数的基本实现
//       本项目是OnlyGameEngine项目的子项目
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "OnlyXML.h"

//////////////////////////////////////////////////////////////////////////
//Global Var
bool     XMLBase::isIgnoreBlank = true;

//////////////////////////////////////////////////////////////////////////
//ErrorDesc
const char* ErrorDesc[ ONLY_ERROR_COUNT ]=
{
	"执行成功!",      //0
	"未知错误!",      //1
	"打开文件失败!",  //2
	"空内容或者未发现EOF在输入流中！"    ,   //3
	"内存分配失败！", //4
	"解析元素节点错误！", //5
	"不能获取元素节点的值", //6
	"读取属性节点错误！", //7
	"无法找到结束节点.", //8
	"解析XML声明节点出错！", //9
	"XML文档为空.",  //10
	"无法解析未知节点.", //11
	"解析注释出错.",
	"解析文本节点出错.",
	"读取属性值错误.",
};

//////////////////////////////////////////////////////////////////////////
//Entity
XMLEntity entity[ ONLYXML_ENTITY_NUM ] =
{
	{ "&amp;",  5, '&'  },
	{ "&lt;",   4, '<'  },
	{ "&gt;",   4, '>'  },
	{ "&quot;", 6, '\"' },
	{ "&apos;", 6, '\'' }
};

//////////////////////////////////////////////////////////////////////////
//XMLNode Implements
const char* XMLHeader        = { "<?xml" };
const char* XMLCommentHeader = { "<!--" };
const char* XMLDTDHeader     = { "<!" };
XMLNode * XMLNode::GetXMLNode(const char *p, OnlyEncoding encoding)
{
	XMLNode *node;
	//跳过空白
	p = Str_SkipBlank(p);
	
	if( !p || !*p || *p != '<' )  return NULL;
	
	//获取父节点——文档节点
	XMLDoc *doc = GetDocument();

	if(Str_Equal(p,XMLHeader,true,encoding))
	{
		node = new XMLDeclare();
	}else if(Str_Equal(p,XMLCommentHeader,true,encoding))
	{
		node = new XMLComment();
	}else if(Str_Equal(p,XMLDTDHeader,true,encoding))
	{
		node = new XMLUnknow();
	}else if(Char_IsAlpha(*(p+1),encoding) || *(p+1) == '-')
	{
		node = new XMLElement();
	}else
	{
		node = new XMLUnknow();
	}

	if( node )
		node->parent = this;
	else
		doc->SetError(ONLY_RES_PARSER_UNKNOW);

	return node;
}


//XMLNode Operator.................................................
void XMLNode::CopyTo(XMLNode *node) const
{
	node->SetValue(value);
}

bool XMLNode::AddNodeChild(XMLNode *node)
{
	if(!node) return false;
	node->parent = this;
	node->prev = lastChild;
	node->next = NULL;
	
	if(lastChild) lastChild->next = node;
	else firstChild = node;

	lastChild = node;
	return true;
}

bool XMLNode::InsertBeforeChild( XMLNode* beforeOne, const XMLNode& _addOne )
{
	if(!beforeOne && beforeOne->parent != this) return false;
	
	XMLNode *node = _addOne.Clone();

	if(!node) return false;

	node->parent = this;
	node->next = beforeOne;
	node->prev = beforeOne->prev;
	if( !beforeOne->prev )
	{
		assert(beforeOne == firstChild);
		firstChild = node;
	}
	else
	{
		beforeOne->prev->next = node;
	}
	beforeOne->prev = node;

	return true;
}


bool XMLNode::InsertAfterChild(  XMLNode* afterOne, const XMLNode& _addOne )
{
	if(!afterOne || afterOne->parent != this) return false;

	XMLNode *node = _addOne.Clone();
	if(!node) return false;  
	node->parent = this;
	node->prev = afterOne;

	if (afterOne->next)
	{
		node->next = afterOne->next;
		afterOne->next->prev = node;
	}else
	{
		assert(lastChild = afterOne);
		lastChild = node;
		node->next = NULL;
	}
	afterOne->next = node;
	
	return true;
}

bool XMLNode::ReplaceChild( XMLNode* replaceOne, const XMLNode& _newOne )
{
	if (!replaceOne || replaceOne->parent != this)
		return false;

	XMLNode *node = _newOne.Clone();

	node->next = replaceOne->next;
	node->prev = replaceOne->prev;

	if(replaceOne->next)
		replaceOne->next->prev = node;
	else
	{
		assert(lastChild == replaceOne);
		node->next = NULL;
		lastChild  = node;
	}

	if(replaceOne->prev)
		replaceOne->prev->next = node;
	else
	{
		assert(firstChild == replaceOne);
		node->prev = NULL;
		firstChild = node;
	}
	delete replaceOne;
	node->parent = this;

	return true;
}

bool XMLNode::RemoveChild( XMLNode* removeOne )
{
	if(!removeOne || removeOne->parent != this)
		return false;
	

	if(removeOne != lastChild)
	{
		removeOne->next->prev = removeOne->prev;
	}
	else
	{
		lastChild =  removeOne->prev;
		removeOne->next->prev = NULL;
	}
	if(removeOne != firstChild)
	{
		removeOne->prev->next = removeOne->next;
	}else
	{
		firstChild = removeOne->next;
		removeOne->prev->next = NULL;
	}
	delete removeOne;
	return true;
}

const XMLElement*  XMLNode::NextSiblingElement() const
{
	const XMLNode *node = NextSibling_Const();
	while(node)
	{
		if(node->ToElement_Const())
			return node->ToElement_Const();
		node = node->NextSibling_Const();
	}
	return NULL;
}
XMLElement* XMLNode::NextSiblingElement()
{
	XMLNode *node = NextSibling();
	while(node)
	{
		if(node->ToElement())
			return node->ToElement();
		node = node->NextSibling();
	}
	return NULL;
}

const XMLDoc* XMLNode::GetDocument_Const() const
{
	const XMLNode *node = this;
	while(node)
	{
		if(node->ToDocument_Const()) return node->ToDocument_Const();
		node = node->parent;
	}
	return NULL;
}

XMLDoc* XMLNode::GetDocument()
{
	XMLNode *node = this;
	while(node)
	{
		if(node->ToDocument()) return node->ToDocument();
		node = node->parent;
	}
	return NULL;
}

const XMLNode* XMLNode::FirstFindChild_Const( const char * value ) const
{
	const XMLNode *node = firstChild;
	while(node)
	{
		if(node->Value() == value) return node;
		node = node->next;
	}
	return NULL;
}


XMLNode* XMLNode::FirstFindChild( const char * value )
{
	XMLNode *node = firstChild;
	while(node)
	{
		if(node->Value() == value) return node;
		node = node->next;
	}
	return NULL;
}


const XMLNode* XMLNode::LastFindChild_Const( const char * value ) const
{
	const XMLNode *node = lastChild;
	while(node)
	{
		if(node->Value() == value) return node;
		node = node->prev;
	}
	return NULL;
}
XMLNode* XMLNode::LastFindChild( const char * value )
{
	XMLNode *node = lastChild;
	while(node)
	{
		if(node->Value() == value) return node;
		node = node->prev;
	}
	return NULL;
}


const XMLNode* XMLNode::IterateChildren( const XMLNode* previous ) const
{
	if(!previous ) return FirstChild_Const();
	else
	{
		assert(previous->parent == this);
		return previous->NextSibling_Const();
	}
}

XMLNode* XMLNode::IterateChildren( XMLNode* previous )
{
	if(!previous ) return FirstChild();
	else
	{
		assert(previous->parent == this);
		return previous->NextSibling();
	}
}


XMLNode*       XMLNode::FindChildrenNext( const char * val )
{
	XMLNode *node = next;
	while(next)
	{
		if(node->Value() == val) return node;
		node = node->next;
	}
	return NULL;
}

const XMLNode* XMLNode::FindChildrenNext_Const( const char * val ) const
{
	const XMLNode *node = next;
	while(next)
	{
		if(node->Value() == val) return node;
		node = node->next;
	}
	return NULL;
}


XMLNode*       XMLNode::FindChildrenPrev( const char * val )
{
	XMLNode *node = prev;
	while(next)
	{
		if(node->Value() == val) return node;
		node = node->prev;
	}
	return NULL;
}

const XMLNode* XMLNode::FindChildrenPrev_Const( const char * val ) const
{
	const XMLNode *node = prev;
	while(next)
	{
		if(node->Value() == val) return node;
		node = node->prev;
	}
	return NULL;
}

XMLNode::~XMLNode()
{
	XMLNode* node = firstChild;
	XMLNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	
	firstChild = 0;
	lastChild = 0;
}


void XMLNode::Clear()
{
	XMLNode* node = firstChild;
	XMLNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	

	firstChild = 0;
	lastChild = 0;
}



//XMLAttribute .........................................................
const XMLAttribute* XMLAttribute::Next_Const() const
{
	if (!next) return 0;
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}
XMLAttribute* XMLAttribute::Next()
{
	if (!next) return 0;
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

const XMLAttribute* XMLAttribute::Previous_Const() const
{
	if (!prev) return 0;
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}
XMLAttribute* XMLAttribute::Previous()
{
	if (!prev) return 0;
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

void XMLAttribute::SetIntValue( int _value )
{
	char buf [64];
	sprintf_s (buf, 64, "%d", _value);
	SetValue (buf);
}

void XMLAttribute::SetDoubleValue( double _value )
{
	char buf [256];
	sprintf_s (buf, 256, "%lf", _value);
	SetValue (buf);
}

const int XMLAttribute::IntValue_Const() const
{
	return atoi (value.c_str());
}

const double  XMLAttribute::DoubleValue_Const() const
{
	return atof (value.c_str());
}

const XMLAttribute*	XMLAttributeOp::Find_Const( const char * name ) const
{
	if(!name||!head) return NULL;

	const XMLAttribute *attr = head;
	
	while(attr)
	{
		if(attr->name == name)
			return attr;
		attr = attr->next;
	}
	return NULL;
}
XMLAttribute*	XMLAttributeOp::Find( const char * name )
{
	if(!name||!head) return NULL;

	XMLAttribute *attr = head;

	while(attr)
	{
		if(attr->name == name)
			return attr;
		attr = attr->next;
	}
	return NULL;
}

void XMLAttributeOp::Add( XMLAttribute* attribute )
{
	if(Find(attribute->name.c_str()))
	{
		assert(!"重名属性！");
		return ;
	}
	if(!attribute||attribute->Empty()) return ;
	
	if(!head)
	{
		attribute->prev = NULL;
		attribute->next = NULL;
		head = attribute;
	}else
	{
		attribute->next = head;
		if(head->prev)
			head->prev->next = attribute;
		attribute->prev = head->prev;
		head->prev = attribute;
	}
}
void XMLAttributeOp::Remove( XMLAttribute* attribute )
{
	XMLAttribute *attr = head;
	while(attr)
	{
		if(attr == attribute) 
		{
			attr->prev->next = attr->next;
			attr->next->prev = attr->prev;
			attr->next = NULL;
			attr->prev = NULL;
			return;
		}
		attr = attr->prev;
	}
	assert(!"head==NULL或者移除的属性不存在！");
}

void XMLAttributeOp::Clear()
{
	if(!head) return ;
	XMLAttribute *prev = head;
	XMLAttribute *next = head->prev;
	while(next)
	{
		delete prev;
		prev = next;
		next = prev->prev;
	}
	delete prev;
	delete next;
	head = NULL;
}


bool XMLDoc::LoadFile(const char *FileName,OnlyEncoding encoding )
{
	//重置
	Clear();

	value = FileName;
	//ERROR: file open fail
	if(!FileName) 
	{
		SetError(ONLY_RES_NULL);
		return false;
	}
	FILE *file = fopen( FileName , "r" );

	if(!file) 
	{
		SetError(ONLY_RES_NULL);
		return false;
	}

	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );//查看文件大小

	if(length == 0)
	{
		fclose(file);
		SetError(ONLY_RES_FILEOPENFAIL);
		return false;
	}

	ONLY_STRING FileData;
	FileData.reserve(length);

	const int BUF_SIZE = 2048;
	char buffer[BUF_SIZE];

	while(fgets(buffer, BUF_SIZE, file ))
	{
		FileData += buffer;
	}

	fclose( file );
	Parser(FileData.c_str(), ONLY_ENCODING_UNKNOW);
	if (  HasError() )
		return false;

	return true;
}

bool XMLDoc::LoadFile( OnlyEncoding encoding )
{
	if ( !value.empty() && LoadFile( value.c_str(), encoding ) )
		return true;

	return false;
}
bool XMLDoc::SaveFile() 
{
	if ( !value.empty() && SaveFile( value.c_str() ) )
		return true;

	return false;
}

bool XMLDoc::SaveFile(const char * filename)
{
	FILE* file = fopen( filename, "w" );
	if ( file )
	{
		ToFile( file,0 );
		fclose( file );
		return true;
	}
	return false;
}

//XMLElememt .............................................
void XMLElement::ClearElemet()
{
	Clear();
	attributes.Clear();
}

XMLElement::~XMLElement()
{
	ClearElemet();
}



const char* XMLElement::GetAttributeValue( const char* name ) const
{
	const XMLAttribute *attr = attributes.Find_Const(name);
	if(attr) return attr->Value();
	return NULL;
}
const char* XMLElement::GetIntAttributeValue( const char* name, int* i ) const
{
	const char *value = GetAttributeValue(name);
	if(i)
	{
		if(value)
			*i = atoi(value);
		else 
			*i = 0;
	}
	return value;
}
const char* XMLElement::GetDoubleAttributeValue( const char* name, double* d ) const
{
	const char *value = GetAttributeValue(name);
	if(d)
	{
		if(value)
			*d = atof(value);
		else 
			*d = 0;
	}
	return value;
}

void  XMLElement::SetAttribute( const char* name, const char * value )
{
	XMLAttribute* node = attributes.Find( name );
	if ( node )
	{
		node->SetValue( value );
		return;
	}

	XMLAttribute *attr = new XMLAttribute(name,value);

	if(attr)
	{
		attributes.Add(attr);

	}else
	{
		XMLDoc* document = GetDocument();
		if ( document ) document->SetError( ONLY_RES_OUTOFMEMORY );
	}
}

void  XMLElement::SetAttribute( const char * name, int val )
{
	char Buf[32];
	printf(Buf,"%d",val);
	SetAttribute( name, Buf );
}

void  XMLElement::SetAttribute( const char * name, double val )
{
	char buf[64];
	sprintf_s( buf, 64, "%f", val );
	SetAttribute( name, buf );
}
void XMLElement::RemoveAttribute( const char * name )
{
	XMLAttribute *attr = attributes.Find(name);
	if(!attr) return;
	
	attributes.Remove(attr);
	delete attr;
}

const XMLElement* XMLNode::FirstChildElement_Const()	const
{
	const XMLNode *node = firstChild;
	while(node)
	{
		if(node->ToElement_Const())
			return node->ToElement_Const();
		node = node->NextSibling_Const();
	}
	return NULL;
}
XMLElement* XMLNode::FirstChildElement()
{
	XMLNode *node = firstChild;
	while(node)
	{
		if(node->ToElement())
			return node->ToElement();
		node = node->NextSibling();
	}
	return NULL;
}

bool XMLText::IsBlank()
{
	for ( unsigned i=0; i<value.length(); i++ )
		if ( !Char_IsBlank( value[i] ) )
			return false;
	return true;
}


const char* XMLElement::GetValue( const char* p, XMLDoc *_doc,OnlyEncoding encoding )
{
	XMLDoc *doc = _doc;
	if(!doc) return 0;

	const char* MateSource = p;
	
	p = Str_SkipBlank( p );
	if(!p && !*p) return NULL;
	while ( p && *p )
	{
		if( *p != '<')
		{
			XMLText *textNode = new XMLText("");
			if ( !textNode )
			{
				if ( doc ) doc->SetError( 1 );
				return 0;
			}
			if( XMLBase::IgnoreBlank() )
			{
				p = textNode->Parser(p,encoding);
			}else
			{
				p = textNode->Parser(MateSource,encoding);
			}
			if ( !textNode->IsBlank())
				AddNodeChild( textNode );
			else
				delete textNode;

		}// *p != '<'
		else// *p == '<'
		{
			if ( Str_Equal( p, "</", false, encoding ) )
			{
				return p;
			}
			else
			{
				XMLNode* node = GetXMLNode( p, encoding );
				if ( node )
				{
					p = node->Parser( p,  encoding );
					AddNodeChild( node );
				}				
				else
				{
					return 0;
				}
			}
		}
		p = Str_SkipBlank( p );
	}
	if ( !p )
	{
		if ( doc ) doc->SetError( 1 );
	}	
	return p;
}

