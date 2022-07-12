#include "DataNode.h"
#include "UL.h"
#include "AE.h"

#include <stdlib.h>
#include <string.h>

#include <wchar.h>
#include <assert.h>
#include <memory.h>

#define nullptr 0
size_t _msize(void* iPtr);

///DataNode DataNode::Null = DataNode(DN_T_Null);

//int strindexof(char* dataset, int datasetLength, char* target, int targetLen);
//int wcsindexof(wchar_t* dataset, int datasetLength, wchar_t* target, int targetLen);


DataNode gDataNode_Null;


DataNode::DataNode()
{
	///this->Init(DN_T_Undefined, DN_VT_Null);
	
	this->Ref = nullptr;
	//this->Ref = gfDataNode_Create();
	//gfDataNode_Init_1(this->Ref);
}

//DataNode::DataNode(DLSyntaxNode* iSyntaxNode)
//{
//	this->Name [0] = L'\0';
//	this->Value[0] = L'\0';
//	
//	this->Type      = DN_T_Undefined;
//	this->ValueType = DN_VT_Undefined; ///~~ not a null;
//	
//	this->Parent   = nullptr;
//	this->Children = nullptr;
//	
//	this->FromDLSyntaxNode(iSyntaxNode);
//};

DataNode::~DataNode()
{
	if(this->Ref != nullptr && this->Ref->Parent == nullptr)
	{
		gfDataNode_Destroy(this->Ref);
		this->Ref = nullptr;
	}
	///throw "NI";
	HERE;
};
//DataNode::DataNode(DataNodeType iNodeType)
//{
//	this->Init(iNodeType);
//};
//DataNode::DataNode(DataNodeStruct iValue)
//{
//	this->Ref = iValue;
//	///throw "NI";
//};
DataNode::DataNode(DataNodeStruct* iValue)
{
	this->Ref = iValue;
	///throw "NI";
};
DataNode::DataNode(DataNodeStruct& iValue)
{
	this->Ref = &iValue;
	///throw "NI";
};
DataNode::DataNode(char*    iValue)
{
	throw "NI";
};
DataNode::DataNode(wchar_t* iValue)
{
	throw "NI";
};
DataNode::DataNode(int      iValue)
{
	throw "NI";
};
DataNode::DataNode(float    iValue)
{
	throw "NI";
};
DataNode::DataNode(double   iValue)
{
	throw "NI";
};

DataNode::operator DataNodeStruct*   ()
{
	throw "NI";
};
DataNode::operator char*    ()
{
	throw "NI";
};
DataNode::operator wchar_t* ()
{
	throw "NI";
};
DataNode::operator int      ()
{
	throw "NI";
};
DataNode::operator float    ()
{
	throw "NI";
};
DataNode::operator double   ()
{
	throw "NI";
};



bool             DataNode::operator!() 
{
	throw "NI";
}

//DataNode&        DataNode::operator[](int iChildIndex) 
//{
//	throw "NI";
//}
//const DataNode&  DataNode::operator[](int iChildIndex) const
//{
//	throw "NI";
//}
//DataNode&        DataNode::operator[](wchar_t* iPath) 
//{
//	throw "NI";
//}
//const DataNode&  DataNode::operator[](wchar_t* iPath) const
//{
//	throw "NI";
//}



//DataNodeStruct&   DataNode::operator[](wchar_t* iPath)
//{
//	return *gfDataNode_GetNodeByPath(this->Ref, iPath, true, false);
//	///~~ Valid X-path expressions: "ChA", "ChA[2]", "ChA/ChB", "ChA[2]/ChB[1]", "ChA/@attr1","ChA[2]/ChB[3]/@attr1", ;
//	/////~~ ;
//	//
//	//
//	/////int _PathLen = int(strlen(iPath));
//	//int _PathLen = int(wcslen(iPath));
//	//
//	//int _BracketIndex = wcsindexof(iPath, _PathLen, L"[", 1);
//	//int _DelimiterIndex = wcsindexof(iPath, _PathLen, L"/", 1);
//	//
//	//bool _IsDelimiterFound = _DelimiterIndex != -1;
//	//bool _IsBracketFound = _BracketIndex != -1;
//	//bool _IsBracketBeforeDelimiter = _IsBracketFound && (_IsDelimiterFound ? _BracketIndex < _DelimiterIndex : true);
//	//
//	//
//	//if(true)
//	//{
//	//	if(_IsBracketFound) throw "NI";
//	//	
//	//	///~~ simple path without indices;
//	//	
//	//	int _NameEndsAt = _IsDelimiterFound ? _DelimiterIndex : _PathLen;
//	//	
//	//	///DataNode& _FoundChild = this->GetChildByNameAndIndex(iPath, 0);
//	//	DataNodeStruct* _FoundChild = gfDataNode_GetChildByNameAndIndex_2(this->Ref, iPath, 0);
//	//	bool _IsChildFound = _FoundChild->Type != DN_VT_Null; ///assert(_IsChildFound);
//	//	
//	//	
//	//	/*if(!_IsChildFound)
//	//	{
//	//		return DataNode::Null;
//	//	}
//	//	*/
//	//	///_FoundChild
//	//	return _FoundChild;
//	//	///HERE;
//	//}
//	//else if(_IsBracketBeforeDelimiter)
//	//{
//	//	int _ChildNameEndsAt = _BracketIndex;
//	//	
//	//	throw "NI";
//	//}
//	//else
//	//{
//	//	///ch
//	//}
//	//
//	//throw "NI";
//}
      DataNodeStruct&  DataNode::operator[](char* iPath)
{
	throw "NI";
}


const DataNodeStruct&  DataNode::operator[](char* iPath) const
{
	throw "NI";
}

      DataNodeStruct&  DataNode::operator[](wchar_t* iPath)
{
	if(this->Ref == nullptr) this->Ref = gfDataNode_Create();
	
	DataNodeStruct* _DNPtr = gfDataNode_GetNodeByPath(this->Ref, iPath, true, true);
	
	if(_DNPtr != nullptr)
	{
		return *_DNPtr;
		
	}
	else
	{
		return gDataNodeStruct_Null;
	}
}


const DataNodeStruct&  DataNode::operator[](wchar_t* iPath) const
{
	throw "NI";
}
//
//void DataNode::Init()
//{
//	this->Init(DN_T_Undefined, DN_VT_Null);
//}
//void DataNode::Init(DataNodeType iNodeType)
//{
//	this->Init(iNodeType, DN_VT_Null);
//}
//void DataNode::Init(DataNodeType iNodeType, DataNodeValueType iValueType)
//{
//	this->Name [0] = L'\0';
//	this->Value[0] = L'\0';
//	
//	this->Type      = iNodeType;
//	this->ValueType = iValueType;
//	
//	this->Parent   = nullptr;
//	this->Children = nullptr;
//}
//void DataNode::InitChildren()
//{
//	this->Children = gfDNChildren_Create(1);
//	this->Children->Owner = this;
//}
//void DataNode::FromDLSyntaxNode(DLSyntaxNode* iSyntaxNode, DLContext* iDLC)
//{
//	if(iSyntaxNode == nullptr) WTFE("Non-null node expected");
//	///if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) WTFE("Empty node specified");
//	if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) return;
//	
//	
//	///if(iSyntaxNode->Type != DL_SYN_Expression) WTFE("Expression syntax node is expected");
//	///if(iSyntaxNode->Type != DL_SYN_FunctionBlock) WTFE("Expected function block as node type");
//	
//	if(iSyntaxNode->Type == DL_SYN_File || iSyntaxNode->Type == DL_SYN_FunctionBlock)
//	{
//		///~~ 'this' can be NOT initialized!;
//	
//		///~~ Document?;
//		if(this->Children == nullptr) this->InitChildren();
//		if(iSyntaxNode->Children == nullptr) throw "WTFE";
//		
//		for(int _EEc = iSyntaxNode->Children->Count, cEi = 0; cEi < _EEc; cEi ++)
//		{
//			DLSyntaxNode& cSynNode = iSyntaxNode->Children->Items[cEi]; if(cSynNode.Type != DL_SYN_Expression) continue;
//			
//			if(wcscmp((wchar_t*)&this->Name,L"ChA4") == 0)/// && this->Children->Count >= 2)
//			{
//				HERE;
//			}
//			///DataNode* cChildNode = this->Children->Allocate(1);
//			DataNode* cChildNode = gfDNChildren_Allocate(this->Children, 1);
//			{
//				cChildNode->Init(DN_T_Element);
//				cChildNode->Parent = this;
//				
//				cChildNode->FromDLSyntaxNode(&cSynNode, iDLC);
//				
//				
//				///*cChildNode = DataNode(&cSynNode);
//			}
//		}
//	}
//	else if(iSyntaxNode->Type == DL_SYN_Expression)
//	{
//		if(iSyntaxNode->Children == nullptr) throw "WTFE";
//		
//		#define dIsTypeAndSingleton(idNode,idType) (idNode.Type == idType && idNode.Children != nullptr && idNode.Children->Count == 1)
//		
//		#define dGetNode(idChildIndex,irdNode) \
//		{\
//			DLSyntaxNode& _List1     = iSyntaxNode->Children->Items[idChildIndex]; if(!dIsTypeAndSingleton(_List1,     DL_SYN_List))     WTFE("1?");\
//			DLSyntaxNode& _ListItem1 = _List1      .Children->Items[0];            if(!dIsTypeAndSingleton(_ListItem1, DL_SYN_ListItem)) WTFE("2?");\
//			irdNode = &_ListItem1.Children->Items[0];\
//			\
//		}
//			
//		
//		int _ChildCount = iSyntaxNode->Children->Count;
//		
//		
//		if(_ChildCount >= 1)
//		{
//			/*DLSyntaxNode& _List1     = iSyntaxNode->Children->Items[0]; if(!dIsTypeAndSingleton(_List1,     DL_SYN_List))     WTFE("?");
//			DLSyntaxNode& _ListItem1 = _List1      .Children->Items[0]; if(!dIsTypeAndSingleton(_ListItem1, DL_SYN_ListItem)) WTFE("?");
//			*/
//			DLSyntaxNode* _Ident; dGetNode(0, _Ident); if(!(_Ident->Type > DL_SYN_Identifiers__Begin && _Ident->Type < DL_SYN_Identifiers__End)) WTFE("?");
//			
//			DLToken& _Token = iDLC->Lexer->Tokens->Items[_Ident->BegToken];
//			
//			wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token.Offset];
//			size_t _TokenLen = _Token.Length;// + 40;
//			
//			assert(_TokenLen >= 1);
//			
//			
//			if(_TokenLen < dDataNodeNameMaxLength)
//			{
//				//memset(&this->Name,1,sizeof(this->Name));
//				///wcscpy_s((wchar_t*)&this->Name, _TokenLen, _TokenStr); doesn't work;
//				
//				//memset(&this->Name,0,sizeof(this->Name));
//				memcpy(&this->Name, _TokenStr, _TokenLen * sizeof(wchar_t));
//				this->Name[_TokenLen] = '\0';
//				HERE;
//			}
//			else
//			{
//				///~~ pointer;
//				STOP;
//			}
//		}
//		
//		if(_ChildCount == 1)
//		{
//			HERE;
//		}
//		else if(_ChildCount == 2)
//		{
//			///~~ name or body;
//			DLSyntaxNode* _Node; dGetNode(1, _Node);
//			{
//				if(_Node->Type > DL_SYN_Value__Begin && _Node->Type < DL_SYN_Value__End)
//				{
//					///void* _Value; bool _DoFree = gfDLCtx_GetSyntaxNodeValue(_Node, &_Value);
//					
//					DLToken& _Token = iDLC->Lexer->Tokens->Items[_Node->BegToken];
//					
//					wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token.Offset];
//					size_t _TokenLen = _Token.Length;// + 40;
//					
//					assert(_TokenLen >= 1);
//					
//					DataNodeValueType _ValueType; switch(_Node->Type)
//					{
//						case DL_SYN_Number     : _ValueType = DN_VT_Float64;    break; ///~~ default;
//						case DL_SYN_NumInvalid : _ValueType = DN_VT_NumInvalid; break;
//						case DL_SYN_NumInt32   : _ValueType = DN_VT_Int32;      break;
//						case DL_SYN_NumFloat32 : _ValueType = DN_VT_Float32;    break;
//						case DL_SYN_NumFloat64 : _ValueType = DN_VT_Float64;    break;
//						
//						case DL_SYN_String     :
//						{
//							assert(_Token.Length >= 2); ///~~ quotes included;
//						
//							size_t _TokenLen = _Token.Length;// + 40;
//							size_t _TokenLenWOQ  = _TokenLen - 2; ///~~ without quotes;
//							size_t _TokenLenWOQZ = _TokenLenWOQ + 1;  ///~~ without quotes but with a null terminator;
//							bool _IsNeededHeapAlloc = _TokenLenWOQZ >= dDataNodeValueMaxLength;
//							
//							DLToken& _Token = iDLC->Lexer->Tokens->Items[_Node->BegToken];
//							wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token.Offset];
//							wchar_t* _TokenStrWOQ = _TokenStr + 1; ///~~ skip opening quote;
//							
//							wchar_t* _StrPtr;
//							
//							if(_IsNeededHeapAlloc)
//							{
//							///_StrPtr = (wchar_t*)malloc(_TokenLenWOQZ * sizeof(wchar_t));
//							_ValueType = DN_VT_StringPtr;
//							}
//							else                   {_StrPtr = (wchar_t*)&this->Value;                            _ValueType = DN_VT_String;}
//							
//								
//							///wcsncpy_s(_StrPtr, _TokenLenWOQZ, _TokenStrWOQ, _TokenLenWOQ);
//							
//							//(*(wchar_t*)_StrPtr)[]
//							
//							//this->Value[_TokenLen] = '\0';
//							
//								
//							
//							break;
//						}
//						
//						default : STOP;
//					}
//					
//					
//					//					
//					//if(_TokenLen < dDataNodeValueMaxLength)
//					//{
//					//	//memset(&this->Name,1,sizeof(this->Name));
//					//	///wcscpy_s((wchar_t*)&this->Name, _TokenLen, _TokenStr); doesn't work;
//					//	
//					//	//memset(&this->Name,0,sizeof(this->Name));
//					//	///#define 
//					//	
//					//	
//					//	
//					//	
//					//	///this->ValueType = 
//					//	
//					//	///this->ValueType
//					//	HERE;
//					//	
//					//}
//					//else
//					//{
//					//	///~~ pointer;
//					//	//if(_Node->Type != DL_SYN_String) WTFE("String expected");
//					//	//
//					//	//DLToken& _Token = iDLC->Lexer->Tokens->Items[_Node->BegToken];
//					//	//wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token.Offset];
//					//	//size_t _TokenLen = _Token.Length;// + 40;
//					//	//assert(_TokenLen >= 2); ///~~ quotes included;
//					//	//size_t _TokenLenWithoutQuotes = _TokenLen - 2;
//					//	//void* _StrPtr = malloc((_TokenLenWithoutQuotes + 1) * sizeof(wchar_t)); ///~~ null-terminator included;
//					//	//
//					//	//memcpy(&_StrPtr, _TokenStr + 1, (_TokenLenWithoutQuotesButWithNullTerm - 1) * sizeof(wchar_t)); ///~~ skip opener quote, stop before closer;
//					//	//(*(wchar_t*)_StrPtr)[]
//					//	//
//					//	//this->Value = _StrPtr;
//					//	//this->ValueType = DN_VT_StringPtr;
//					//	HERE;
//					//}
//					//
//				
//					///if(_DoFree) free(_Value);
//					
//					HERE;
//				}
//				else if(_Node->Type == DL_SYN_FunctionBlock)
//				{
//					this->FromDLSyntaxNode(_Node,iDLC);
//					
//					HERE;
//				}
//				else WTFE("?");
//			}
//			
//			
//			HERE;
//		}
//		else
//		{
//			
//		}
//		
//		/*
//		for(int _EEc = iSyntaxNode->Children->Count, cEi = 0; cEi < _EEc; cEi ++)
//		{
//			DLSyntaxNode& cSynNode = iSyntaxNode->Children->Items[cEi]; if(cSynNode.Type != DL_SYN_List) continue;
//			
//			if(this->Children == nullptr) this->InitChildren();
//			
//			///DataNode* cChildNode = this->Children->Allocate(1);
//			DataNode* cChildNode = gfDNChildren_Allocate(this->Children, 1);
//			{
//				cChildNode->Init(DN_T_Element);
//				cChildNode->FromDLSyntaxNode(&cSynNode, iDLC);
//				///*cChildNode = DataNode(&cSynNode);
//			}
//		}*/
//		
//		HERE;
//	}
//	else
//	{
//		WTFE("Expected file, function block or expression as node type");
//	}
//	
//	///iSyntaxNode->
//	
//	HERE;
//	///iSyntaXNode
//}
//
//DataNode& DataNode::GetChildByNameAndIndex(char* iName, int iIndex)
//{
//	return *this;
//}
//DataNode& DataNode::GetChildByNameAndIndex(wchar_t* iName, int iIndex)
//{
//	if(this->Children != nullptr && this->Children->Count != 0)
//	{
//		for(int _CCc = this->Children->Count, cCi = 0; cCi < _CCc; cCi ++)
//		{
//			DataNode& cNode = this->Children->Items[cCi];
//			
//			if(wcscmp(cNode.Name, iName) == 0)
//			{
//				return cNode;
//			}
//		}
//	}
//	return DataNode::Null;
//}
//
//void DataNode::InitStatic()
//{
//	///memset(&DataNode::Null,0,sizeof(DataNode));
//	
//	
//}

DataNodeStruct::DataNodeStruct(char*    iValue)
{
	throw "NI";
};
DataNodeStruct::DataNodeStruct(wchar_t* iValue)
{
	///gfDataNode_SetValueWSZ(this, nullptr, iValue);
	///gfDataNode_Create();
	throw "NI";
};
DataNodeStruct::DataNodeStruct(int      iValue)
{
	gfDataNode_Init_1(this);
	gfDataNode_SetValueI32(this, nullptr, iValue);
};
DataNodeStruct::DataNodeStruct(float    iValue)
{
	gfDataNode_Init_1(this);
	gfDataNode_SetValueF32(this, nullptr, iValue);
};
DataNodeStruct::DataNodeStruct(double   iValue)
{
	throw "NI";
};

//DataNodeStruct::operator DataNodeStruct*   ()
//{
//	throw "NI";
//};
DataNodeStruct::operator char*    ()
{
	throw "NI";
};
DataNodeStruct::operator wchar_t* ()
{
	throw "NI";
};
DataNodeStruct::operator int      ()
{
	throw "NI";
};
DataNodeStruct::operator float    ()
{
	throw "NI";
};
DataNodeStruct::operator double   ()
{
	throw "NI";
};

DataNodeStruct& DataNodeStruct::operator= (const DataNodeStruct& iNode)
{
	
	this->Value.Type = iNode.Value.Type;
	


	memcpy(&this->Value,&iNode.Value, sizeof(this->Value) * sizeof(wchar_t)); ///~~ need to copy 'pointed' contents;
	///memcpy(&this->Value,iNode.Value, sizeof(this->Value) * sizeof(wchar_t));
	
	//*((wchar_t*)&this->Value) = *(wchar_t*)iNode.Value;
	//this->Value = *(wchar_t*)&iNode.Value;
	
	return *this;
}




DataNodeStruct&  DataNode::Create(wchar_t* iPath)
{
	return (*this)[iPath];
}
void DataNode::Save(wchar_t* iPath)
{
	throw "NI";
}
void DataNode::Load(wchar_t* iPath)
{
	throw "NI";
}


int DataNode::ToStringPreview(wchar_t* irBuffer, int iBufferSize)
{
	WString* _WStr = gfWString_Create(iBufferSize);
	gfDataNode_ToString(this->Ref, _WStr, 0);
	
	if(irBuffer     == nullptr)       throw "pre-allocated buffer only";
	if(iBufferSize  <= _WStr->Length) throw "buffer size";
	
	wcsncpy_s(irBuffer, _WStr->Length + 1, _WStr->Data, _WStr->Length);
	
	gfWString_Destroy(_WStr);
	
	return _WStr->Length;
}

int DataNode::ToStringPreview(wchar_t** irBuffer)
{
	assert(*irBuffer == nullptr);
	
	WString* _WStr = gfWString_Create(0);
	gfDataNode_ToString(this->Ref, _WStr, 0);
	
	int oBufferLength = _WStr->Length + 1;
	*irBuffer = new wchar_t[oBufferLength];
	wcsncpy_s(*irBuffer, oBufferLength, _WStr->Data, _WStr->Length);
	
	HERE;
	return oBufferLength;
}
DataNode* DataNode::FromString(wchar_t* iString)
{
	DataNodeStruct* _Struct = gfDataNode_FromString(iString);
	return new DataNode(_Struct);
}
DataNode* DataNode::FromFile(wchar_t* iPath)
{
	throw "NI";
}