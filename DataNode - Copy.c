#include "DataNode.h"
#include "AEDL.h"
//#include "AEDL.h"
//#include "AE.h"

#include <stdlib.h>
#include <string.h>

#include <wchar.h>
#include <assert.h>
#include <memory.h>

#define nullptr 0
size_t _msize(void* iPtr);

static int gf_BreakpointFunction(int iNum){return iNum;}
#define HERE  gf_BreakpointFunction(0)
#define STOP  gf_BreakpointFunction(1 / gf_BreakpointFunction(0))
#define WTFE(idMsg) STOP


DataNodeStruct gDataNodeStruct_Null;

struct $__DataNodeCollection__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
void                 gfDNCollection_Reserve  (DataNodeCollection* irCollection, int iCapacity);
DataNodeCollection*  gfDNCollection_Create   (int iCapacity)
{
	DataNodeCollection* oCollection = malloc(sizeof(DataNodeCollection));
	{
		///oCollection->Owner    = nullptr; ///~~ commented for C-'template' compatibility: share the same code with DLItemCollection, DLItemTypeCollection etc;
		oCollection->Items    = nullptr;
		oCollection->Count    = 0;
		oCollection->Capacity = 0;
		
		gfDNCollection_Reserve(oCollection,iCapacity);
	}
	return oCollection;
}
void                 gfDNCollection_Destroy  (DataNodeCollection* irCollection)
{
	if(irCollection->Items != nullptr) free(irCollection->Items);
	
	free(irCollection);
}
DataNodeStruct*      gfDNCollection_Allocate (DataNodeCollection* irCollection, int iCount)
{
	gfDNCollection_Reserve(irCollection, irCollection->Count + iCount);
	
	irCollection->Count += iCount;
	
	return &irCollection->Items[irCollection->Count - 1];
}

///void               gfDNCollection_Push     (DataNodeCollection* irCollection, DataNodeCollection* iItem)
//{
//	DataNodeCollection* _Item = gfDNCollection_Allocate(irCollection,1);
//	
//	irCollection->Items[irCollection->Count - 1] = *iItem;
//}
DataNodeStruct*      gfDNCollection_Pop      (DataNodeCollection* irCollection)
{
	DataNodeStruct* oItem;

	if(irCollection->Count <= 0) return nullptr;
	
	return &irCollection->Items[-- irCollection->Count];
}
DataNodeStruct*      gfDNCollection_Peek     (DataNodeCollection* irCollection)
{
	if(irCollection->Count <= 0) return nullptr;
	
	return &irCollection->Items[irCollection->Count - 1];
}



void                 gfDNCollection_Reserve  (DataNodeCollection* irCollection, int iCapacity)
{
	if(iCapacity <= irCollection->Capacity) return;
	
	{
		size_t _OldSize, _NewSize;
		DataNodeStruct *_ItemsOldPtr = irCollection->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irCollection->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dDataNodeCollectionCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(DataNodeStruct); //, _Size;
		
		//_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		 
		
		_ItemsNewPtr = (irCollection->Items == nullptr ? malloc(_NeedSize) : realloc(irCollection->Items, _NeedSize));
		
		//_NewSize = _msize(_ItemsNewPtr);
			
		assert(_ItemsNewPtr != nullptr);
		
		
		irCollection->Items    = _ItemsNewPtr;
		irCollection->Capacity = _NeedCapacity;
		
		
		
		if(true)
		{
			int _PPc, cPi; for(_PPc = irCollection->Count, cPi = 0; cPi < _PPc; cPi ++)
			{
				DataNodeStruct* cParentNode = &irCollection->Items[cPi];
				
				if(cParentNode->Children != nullptr)
				{
					int _CCc = cParentNode->Children->Count, cCi = 0; for(; cCi < _CCc; cCi ++)
					{
						DataNodeStruct* cChildNode = &cParentNode->Children->Items[cCi];
						cChildNode->Parent = cParentNode;
					}
				}
			}
		}
	}
}
void                 gfDNCollection_Clear    (DataNodeCollection* irCollection)
{
	irCollection->Count = 0;
}


struct $__DataNodeStruct__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
DataNodeStruct* gfDN_Create()
{
	DataNodeStruct* oNode = malloc(sizeof(DataNodeStruct));
	
	gfDN_Init_1(oNode);
	
	return oNode;
}

void gfDN_Init_1(DataNodeStruct* irNode)
{
	gfDN_Init_3(irNode, DN_T_Undefined, DN_VT_Null);
}
void gfDN_Init_2(DataNodeStruct* irNode, DataNodeType iNodeType)
{
	gfDN_Init_3(irNode, iNodeType, DN_VT_Null);
}
void gfDN_Init_3(DataNodeStruct* irNode, DataNodeType iNodeType, DataNodeValueType iValueType)
{
	irNode->Name [0] = L'\0';
	irNode->Value[0] = L'\0';
	
	irNode->Type      = iNodeType;
	irNode->ValueType = iValueType;
	
	irNode->Parent   = nullptr;
	irNode->Children = nullptr;
}
void gfDN_InitChildren(DataNodeStruct* irNode)
{
	irNode->Children = gfDNCollection_Create(1);
	irNode->Children->Owner = irNode;
}
void gfDN_FromDLSyntaxNode(DataNodeStruct* irNode, DLSyntaxNode* iSyntaxNode, DLContext* iDLC)
{
	if(iSyntaxNode == nullptr) WTFE("Non-null node expected");
	///if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) WTFE("Empty node specified");
	if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) return;
	
	
	///if(iSyntaxNode->Type != DL_SYN_Expression) WTFE("Expression syntax node is expected");
	///if(iSyntaxNode->Type != DL_SYN_FunctionBlock) WTFE("Expected function block as node type");
	
	if(iSyntaxNode->Type == DL_SYN_File || iSyntaxNode->Type == DL_SYN_FunctionBlock)
	{
		///~~ 'this' can be NOT initialized!;
		int _EEc, cEi;
	
		///~~ Document?;
		if(irNode->Children == nullptr) gfDN_InitChildren(irNode);
		if(iSyntaxNode->Children == nullptr) WTFE("?");
		
		for(_EEc = iSyntaxNode->Children->Count, cEi = 0; cEi < _EEc; cEi ++)
		{
			DLSyntaxNode* cSynNode;
			DataNodeStruct* cChildNode;
			
			cSynNode = &iSyntaxNode->Children->Items[cEi]; if(cSynNode->Type != DL_SYN_Expression) continue;
			
			if(wcscmp((wchar_t*)&irNode->Name,L"ChA4") == 0)/// && this->Children->Count >= 2)
			{
				HERE;
			}
			///DataNode* cChildNode = this->Children->Allocate(1);
			cChildNode = gfDNCollection_Allocate(irNode->Children, 1);
			{
				gfDN_Init_2(cChildNode, DN_T_Element);
				cChildNode->Parent = irNode;
				
				gfDN_FromDLSyntaxNode(cChildNode, cSynNode, iDLC);
				
				
				///*cChildNode = DataNode(&cSynNode);
			}
		}
	
	}
	else if(iSyntaxNode->Type == DL_SYN_Expression)
	{
		int _ChildCount; if(iSyntaxNode->Children == nullptr) WTFE("?");
		_ChildCount = iSyntaxNode->Children->Count;
		
		#define dIsTypeAndSingleton(idNode,idType) (idNode->Type == idType && idNode->Children != nullptr && idNode->Children->Count == 1)
		#define dGetNode(idChildIndex,irdNode) \
		{\
			DLSyntaxNode *_List1,*_ListItem1; \
			\
			_List1     = &iSyntaxNode->Children->Items[idChildIndex]; if(!dIsTypeAndSingleton(_List1,     DL_SYN_List))     WTFE("1?");\
			_ListItem1 = &_List1     ->Children->Items[0];            if(!dIsTypeAndSingleton(_ListItem1, DL_SYN_ListItem)) WTFE("2?");\
			irdNode    = &_ListItem1 ->Children->Items[0];\
		}
			
		
		
		
		
		if(_ChildCount >= 1)
		{
			DLToken* _Token; wchar_t* _TokenStr; size_t _TokenLen;
			
			DLSyntaxNode* _Ident; dGetNode(0, _Ident);
			
			_Token    = &iDLC->Lexer->Tokens->Items[_Ident->BegToken];
			_TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
			_TokenLen = _Token->Length;// + 40;
			
			
			if(!(_Ident->Type > DL_SYN_Identifiers__Begin && _Ident->Type < DL_SYN_Identifiers__End)) WTFE("?");
			
			assert(_TokenLen >= 1);
			
			
			if(_TokenLen < dDataNodeNameMaxLength)
			{
				//memset(&this->Name,1,sizeof(this->Name));
				///wcscpy_s((wchar_t*)&this->Name, _TokenLen, _TokenStr); doesn't work;
				
				//memset(&this->Name,0,sizeof(this->Name));
				memcpy(&irNode->Name, _TokenStr, _TokenLen * sizeof(wchar_t));
				irNode->Name[_TokenLen] = '\0';
				HERE;
			}
			else
			{
				///~~ pointer;
				STOP;
			}
		}
		
		if(_ChildCount == 1)
		{
			HERE;
		}
		else if(_ChildCount == 2)
		{
			///~~ name or body;
			DLSyntaxNode* _Node; dGetNode(1, _Node);
			{
				if(_Node->Type > DL_SYN_Value__Begin && _Node->Type < DL_SYN_Value__End)
				{
					///void* _Value; bool _DoFree = gfDLCtx_GetSyntaxNodeValue(_Node, &_Value);
					
					DLToken* _Token = &iDLC->Lexer->Tokens->Items[_Node->BegToken];
					
					wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
					size_t _TokenLen = _Token->Length;// + 40;
					DataNodeValueType _ValueType; 
					
					assert(_TokenLen >= 1);
					
					_ValueType; switch(_Node->Type)
					{
						case DL_SYN_Number     : _ValueType = DN_VT_Float64;    break; ///~~ default;
						case DL_SYN_NumInvalid : _ValueType = DN_VT_NumInvalid; break;
						case DL_SYN_NumInt32   : _ValueType = DN_VT_Int32;      break;
						case DL_SYN_NumFloat32 : _ValueType = DN_VT_Float32;    break;
						case DL_SYN_NumFloat64 : _ValueType = DN_VT_Float64;    break;
						
						case DL_SYN_String     :
						{
							size_t _TokenLenWOQ, _TokenLenWOQZ; bool _IsNeededHeapAlloc;
							wchar_t* _TokenStrWOQ;
							wchar_t* _StrPtr;
							
							assert(_Token->Length >= 2); ///~~ quotes included;
						
							_TokenLen = _Token->Length;// + 40;
							_TokenLenWOQ  = _TokenLen - 2; ///~~ without quotes;
							_TokenLenWOQZ = _TokenLenWOQ + 1;  ///~~ without quotes but with a null terminator;
							_IsNeededHeapAlloc = _TokenLenWOQZ >= dDataNodeValueMaxLength;
							
							_Token = &iDLC->Lexer->Tokens->Items[_Node->BegToken];
							_TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
							_TokenStrWOQ = _TokenStr + 1; ///~~ skip opening quote;
							
							
							
							if(_IsNeededHeapAlloc)
							{
								_StrPtr = malloc(_TokenLenWOQZ * sizeof(wchar_t));
								
								memcpy(&irNode->Value, &_StrPtr, sizeof(void*));
								
								_ValueType = DN_VT_StringPtr;
							}
							else                   {_StrPtr = (wchar_t*)&irNode->Value;                            _ValueType = DN_VT_String;}
							
							wcsncpy_s(_StrPtr, _TokenLenWOQZ, _TokenStrWOQ, _TokenLenWOQ);
							
							//(*(wchar_t*)_StrPtr)[]
							
							//this->Value[_TokenLen] = '\0';
							
								
							
							break;
						}
						
						default : STOP;
					}
					
					irNode->ValueType = _ValueType;
					//					
					//if(_TokenLen < dDataNodeValueMaxLength)
					//{
					//	//memset(&this->Name,1,sizeof(this->Name));
					//	///wcscpy_s((wchar_t*)&this->Name, _TokenLen, _TokenStr); doesn't work;
					//	
					//	//memset(&this->Name,0,sizeof(this->Name));
					//	///#define 
					//	
					//	
					//	
					//	
					//	///this->ValueType = 
					//	
					//	///this->ValueType
					//	HERE;
					//	
					//}
					//else
					//{
					//	///~~ pointer;
					//	//if(_Node->Type != DL_SYN_String) WTFE("String expected");
					//	//
					//	//DLToken& _Token = iDLC->Lexer->Tokens->Items[_Node->BegToken];
					//	//wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token.Offset];
					//	//size_t _TokenLen = _Token.Length;// + 40;
					//	//assert(_TokenLen >= 2); ///~~ quotes included;
					//	//size_t _TokenLenWithoutQuotes = _TokenLen - 2;
					//	//void* _StrPtr = malloc((_TokenLenWithoutQuotes + 1) * sizeof(wchar_t)); ///~~ null-terminator included;
					//	//
					//	//memcpy(&_StrPtr, _TokenStr + 1, (_TokenLenWithoutQuotesButWithNullTerm - 1) * sizeof(wchar_t)); ///~~ skip opener quote, stop before closer;
					//	//(*(wchar_t*)_StrPtr)[]
					//	//
					//	//this->Value = _StrPtr;
					//	//this->ValueType = DN_VT_StringPtr;
					//	HERE;
					//}
					//
				
					///if(_DoFree) free(_Value);
					
					HERE;
				}
				else if(_Node->Type == DL_SYN_FunctionBlock)
				{
					gfDN_FromDLSyntaxNode(irNode,_Node,iDLC);
					
					HERE;
				}
				else WTFE("?");
			}
			
			
			HERE;
		}
		else
		{
			
		}
		
		/*
		for(int _EEc = iSyntaxNode->Children->Count, cEi = 0; cEi < _EEc; cEi ++)
		{
			DLSyntaxNode& cSynNode = iSyntaxNode->Children->Items[cEi]; if(cSynNode.Type != DL_SYN_List) continue;
			
			if(this->Children == nullptr) this->InitChildren();
			
			///DataNode* cChildNode = this->Children->Allocate(1);
			DataNode* cChildNode = gfDNCollection_Allocate(this->Children, 1);
			{
				cChildNode->Init(DN_T_Element);
				cChildNode->FromDLSyntaxNode(&cSynNode, iDLC);
				///*cChildNode = DataNode(&cSynNode);
			}
		}*/
		
		HERE;
	}
	else
	{
		WTFE("Expected file, function block or expression as node type");
	}
	
	///iSyntaxNode->
	
	HERE;
	///iSyntaXNode
}

DataNodeStruct* gfDN_GetChildByNameAndIndex(DataNodeStruct* iNode, char* iName, int iIndex)
{
	return iNode;
}
DataNodeStruct* gfDN_GetChildByNameAndIndex_2(DataNodeStruct* iNode, wchar_t* iName, int iIndex)
{
	if(iNode->Children != nullptr && iNode->Children->Count != 0)
	{
		int _CCc, cCi; for(_CCc = iNode->Children->Count, cCi = 0; cCi < _CCc; cCi ++)
		{
			DataNodeStruct* cNode = &iNode->Children->Items[cCi];
			
			if(wcscmp(cNode->Name, iName) == 0)
			{
				return cNode;
			}
		}
	}
	return &gDataNodeStruct_Null;
}



///bool gfSplitPath(wchar_t* iName, size_t iNameSize, wchar_t iPath)
#define dfSplitPath() \
\
	wchar_t _ChildName[dDataNodeNameMaxLength];\
	wchar_t* _SubPath = nullptr;\
	{\
		int _PathLen = (int)wcslen(iPath), _DelimIndex = wcsindexof(iPath, (int)wcslen(iPath), L"/", 1);\
		\
		wcsncpy_s((wchar_t*)&_ChildName, sizeof(_ChildName) / sizeof(wchar_t), iPath, _DelimIndex != -1 ? _DelimIndex : _PathLen);\
		\
		if(_DelimIndex != -1)\
		{\
			if(!iDoAllowSubPath) STOP;\
			_SubPath = &iPath[_DelimIndex + 1];\
		}\
	}



DataNodeStruct* gfDN_GetNodeByPath(DataNodeStruct* irNode, wchar_t* iPath, bool iDoAllowSubPath, bool iDoCreateMissing)
{
	DataNodeStruct* oNode = nullptr;
	
	dfSplitPath();
	
	
	if(irNode->Children != nullptr)
	{
		///bool _IsChildFound = false;
		
		///int _CCc = iNode->Children->Count, cCi = 0; for(; cCi < _CCc; cCi ++)
		int cCi = 0; for(; cCi < irNode->Children->Count; cCi ++)
		{
			DataNodeStruct* cChild = &irNode->Children->Items[cCi];
			
			if(wcscmp((wchar_t*)&cChild->Name, (wchar_t*)&_ChildName) == 0)
			{
				oNode = _SubPath == nullptr ? cChild : gfDN_GetNodeByPath(cChild, _SubPath, iDoAllowSubPath, iDoCreateMissing);
				
				//if(_SubPath != nullptr)
				//{
				//	oNode = gfDN_GetNodeByPath(cChild, _SubPath, iDoCreateMissing);
				//}
				//else
				//{
				//	oNode = cChild;
				//}
				break;
			}
			///if(cChild->Name 
		}
	}
	
	if(oNode == nullptr && iDoCreateMissing)
	{
		DataNodeStruct* _Child;
		
		HERE;
		
		if(irNode->Children == nullptr) gfDN_InitChildren(irNode);
		
		_Child = gfDNCollection_Allocate(irNode->Children, 1);
		{
			gfDN_Init_1(_Child);
			gfDataNode_SetName(_Child, (wchar_t*)&_ChildName);
			
			_Child->Parent = irNode;
		}
		
		oNode = _SubPath != nullptr ? gfDN_GetNodeByPath(irNode, _SubPath, iDoAllowSubPath, iDoCreateMissing) : _Child;
		/*
		if(_SubPath != nullptr)
		{
			///gfDN_GetNodeByPath(*irNode, _SubPath != nullptr ? iPath, ,
		}
		*/
		HERE;
	}

	return oNode;
	
	
	
	/// WORKS: if(iNode->Children != nullptr)
	//{
	//	///int _CCc = iNode->Children->Count, cCi = 0; for(; cCi < _CCc; cCi ++)
	//	int cCi = 0; for(; cCi < iNode->Children->Count; cCi ++)
	//	{
	//		DataNodeStruct* cChild = &iNode->Children->Items[cCi];
	//		
	//		if(wcscmp((wchar_t*)&cChild->Name, (wchar_t*)&_ChildName) == 0)
	//		{
	//			if(_SubPath != nullptr)
	//			{
	//				return gfDN_GetNodeByPath(cChild, _SubPath);
	//			}
	//			else return cChild;
	//		}
	//		///if(cChild->Name 
	//	}
	//}
	//return nullptr;
}
DataNodeStruct* gfDN_SetNodeByPath(DataNodeStruct* irNode, wchar_t* iPath,  DataNodeStruct* iNewNode, bool iDoAllowSubPath, bool iDoCreateMissing)
{
	STOP;
	return nullptr;
}


void gfDataNode_Begin(DataNodeStruct** irNode, wchar_t* iPath)
{
	DataNodeStruct* _Node = gfDN_GetNodeByPath(*irNode, iPath, false, true);
	
	*irNode = _Node;
	HERE;
	
	///STOP;
	//DataNodeStruct* _Node = *irNode;
	//DataNodeStruct* _TgtNode = nullptr;
	//dfSplitPath();
	//
	//if(_Node->Children == nullptr) gfDN_InitChildren(_Node);
	//
	//
	//if(true)
	//{
	//	int cCi = 0; for(; cCi < _Node->Children->Count; cCi ++)
	//	{
	//		DataNodeStruct* cChild = &_Node->Children->Items[cCi];
	//		
	//		if(wcscmp((wchar_t*)&cChild->Name, (wchar_t*)&_ChildName) == 0)
	//		{
	//			_TgtNode = cChild;
	//			
	//			
	//			if(_SubPath != nullptr)
	//			{
	//				DataNodeStruct* _SubNode = gfDN_GetNodeByPath(cChild, _SubPath);
	//				
	//				if(_SubNode == nullptr)
	//				{
	//					WFOWEIFHOWEI
	//					
	//					gfDataNode_Begin(&_TgtNode, _SubPath);
	//				}
	//				else
	//				{
	//					
	//				}
	//			}
	//		}
	//		///if(cChild->Name 
	//	}
	//}
	//
	//
	//HERE;
	//*irNode = _TgtNode;
	//
	//
	////_ChildName
	/////gfDNCollection_Allocate
	/////gfDN
	/////STOP;
}
void gfDataNode_End(DataNodeStruct** iNode)
{
	DataNodeStruct* _Parent = (*iNode)->Parent;
	assert(_Parent != nullptr);
	
	*iNode = _Parent;
	
	HERE;
}

void gfDataNode_SetName     (DataNodeStruct* irNode, wchar_t* iName)
{
	wcsncpy_s((wchar_t*)&irNode->Name, _countof(irNode->Name), iName, _TRUNCATE);
	
	HERE;
	///gfDN_
	///STOP;
}
void gfDataNode_SetValueSZ  (DataNodeStruct* iNode, wchar_t* iPath, char*    iValue){STOP;}
void gfDataNode_SetValueWSZ (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iValue){STOP;}
void gfDataNode_SetValueI32 (DataNodeStruct* iNode, wchar_t* iPath, int      iValue)
{
	DataNodeStruct* _TgtNode = gfDN_GetNodeByPath(iNode, iPath, true, true);
	
	*((int*)&_TgtNode->Value) = iValue;
	
	_TgtNode->ValueType = DN_VT_Int32;
	
	HERE;
}
void gfDataNode_SetValueF32 (DataNodeStruct* iNode, wchar_t* iPath, float    iValue)
{
	DataNodeStruct* _TgtNode = gfDN_GetNodeByPath(iNode, iPath, true, true);
	
	*((float*)&_TgtNode->Value) = iValue;
	
	_TgtNode->ValueType = DN_VT_Float32;
	
	HERE;
}

wchar_t* gfDataNode_GetName     (DataNodeStruct* iNode){STOP; return nullptr;}
char*    gfDataNode_GetValueSZ  (DataNodeStruct* iNode, wchar_t* iPath){STOP; return nullptr;}
wchar_t* gfDataNode_GetValueWSZ (DataNodeStruct* iNode, wchar_t* iPath){STOP; return nullptr;}
int      gfDataNode_GetValueI32 (DataNodeStruct* iNode, wchar_t* iPath)
{
	int oValue;
	DataNodeStruct* _TgtNode = gfDN_GetNodeByPath(iNode, iPath, true, false);
	
	assert(_TgtNode->ValueType == DN_VT_Int32);
	
	oValue = *((int*)&_TgtNode->Value);
	
	return oValue;
}
float    gfDataNode_GetValueF32 (DataNodeStruct* iNode, wchar_t* iPath)
{
	float oValue;
	DataNodeStruct* _TgtNode = gfDN_GetNodeByPath(iNode, iPath, true, false);
	
	assert(_TgtNode->ValueType == DN_VT_Float32);
	
	oValue = *((float*)&_TgtNode->Value);
	
	return oValue;
}

wchar_t* gfDataNode_GetNameOr     (DataNodeStruct* iNode, wchar_t* iName, wchar_t* iOrName){STOP; return nullptr;}
char*    gfDataNode_GetValueSZOr  (DataNodeStruct* iNode, wchar_t* iPath, char*    iOrValue){STOP; return nullptr;}
wchar_t* gfDataNode_GetValueWSZOr (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iOrValue){STOP; return nullptr;}
int      gfDataNode_GetValueI32Or (DataNodeStruct* iNode, wchar_t* iPath, int      iOrValue){STOP; return -1;}
float    gfDataNode_GetValueF32Or (DataNodeStruct* iNode, wchar_t* iPath, float    iOrValue){STOP; return -1.0f;}


void gfDN_InitStatic()
{
	///memset(&DataNode::Null,0,sizeof(DataNode));
	
	
}