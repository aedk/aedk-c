#include "DataNode.h"
#include "AEDL.h"
//#include "AEDL.h"
//#include "AE.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <wchar.h>
#include <assert.h>
#include <memory.h>
//#include <string.h>

#define nullptr 0
size_t _msize(void* iPtr);

static int gf_BreakpointFunction(int iNum){return iNum;}
#define HERE  gf_BreakpointFunction(0)
#define STOP  gf_BreakpointFunction(1 / gf_BreakpointFunction(0))
#define WTFE(idMsg) printf(idMsg); STOP;


DataNodeStruct gDataNodeStruct_Null;

struct $__DataNodeChildren__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
void                 gfDNChildren_Init     (DataNodeStruct* irParentNode)
{
	assert(irParentNode->Children == nullptr);
	
	gfDNChildren_Reserve(irParentNode, 1);
}

DataNodeStruct*      gfDNChildren_Allocate (DataNodeStruct* irParentNode, int iCount)
{
	gfDNChildren_Reserve(irParentNode, irParentNode->ChildCount + iCount);
	
	irParentNode->ChildCount += iCount;
	
	return &irParentNode->Children[irParentNode->ChildCount - 1];
}

DataNodeStruct*      gfDNChildren_Pop      (DataNodeStruct* irParentNode)
{
	DataNodeStruct* oItem;

	if(irParentNode->ChildCount <= 0) return nullptr;
	
	return &irParentNode->Children[-- irParentNode->ChildCount];
}
DataNodeStruct*      gfDNChildren_Peek     (DataNodeStruct* irParentNode)
{
	if(irParentNode->ChildCount <= 0) return nullptr;
	
	return &irParentNode->Children[irParentNode->ChildCount - 1];
}



void                 gfDNChildren_Reserve  (DataNodeStruct* irParentNode, int iCapacity)
{
	if(iCapacity <= irParentNode->ChildCapacity) return;
	
	{
		size_t _OldSize, _NewSize;
		DataNodeStruct *_ItemsOldPtr = irParentNode->Children, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irCollection->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dDataNodeCollectionCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(DataNodeStruct); //, _Size;
		
		//_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		 
		
		_ItemsNewPtr = (irParentNode->Children == nullptr ? malloc(_NeedSize) : realloc(irParentNode->Children, _NeedSize));
		
		//_NewSize = _msize(_ItemsNewPtr);
			
		assert(_ItemsNewPtr != nullptr);
		
		
		irParentNode->Children      = _ItemsNewPtr;
		irParentNode->ChildCapacity = _NeedCapacity;
		
		
		
		if(true)
		{
			int _PPc, cPi; for(_PPc = irParentNode->ChildCount, cPi = 0; cPi < _PPc; cPi ++)
			{
				DataNodeStruct* cParentNode = &irParentNode->Children[cPi];
				
				if(cParentNode->Children != nullptr)
				{
					int _CCc = cParentNode->ChildCount, cCi = 0; for(; cCi < _CCc; cCi ++)
					{
						DataNodeStruct* cChildNode = &cParentNode->Children[cCi];
						cChildNode->Parent = cParentNode;
					}
				}
			}
		}
	}
}
void                 gfDNChildren_Clear    (DataNodeStruct* irParentNode)
{
	irParentNode->ChildCount = 0;
	WTFE("NI: not freed resources");
}


struct $__DataNodeAttributes__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
void                 gfDNAttributes_Init     (DataNodeStruct* irParentNode)
{
	assert(irParentNode->Attributes == nullptr);
	
	gfDNAttributes_Reserve(irParentNode, 1);
}
DataNodeStruct*      gfDNAttributes_Allocate (DataNodeStruct* irParentNode, int iCount)
{
	gfDNAttributes_Reserve(irParentNode, irParentNode->AttrCount + iCount);
	
	irParentNode->AttrCount += iCount;
	
	return &irParentNode->Attributes[irParentNode->AttrCount - 1];
}

DataNodeStruct*      gfDNAttributes_Pop      (DataNodeStruct* irParentNode)
{
	DataNodeStruct* oItem;

	if(irParentNode->AttrCount <= 0) return nullptr;
	
	return &irParentNode->Attributes[-- irParentNode->AttrCount];
}
DataNodeStruct*      gfDNAttributes_Peek     (DataNodeStruct* irParentNode)
{
	if(irParentNode->AttrCount <= 0) return nullptr;
	
	return &irParentNode->Attributes[irParentNode->AttrCount - 1];
}



void                 gfDNAttributes_Reserve  (DataNodeStruct* irParentNode, int iCapacity)
{
	if(iCapacity <= irParentNode->AttrCapacity) return;
	
	{
		size_t _OldSize, _NewSize;
		DataNodeStruct *_ItemsOldPtr = irParentNode->Attributes, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irCollection->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dDataNodeCollectionCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(DataNodeStruct); //, _Size;
		
		//_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		 
		
		_ItemsNewPtr = (irParentNode->Attributes == nullptr ? malloc(_NeedSize) : realloc(irParentNode->Attributes, _NeedSize));
		
		//_NewSize = _msize(_ItemsNewPtr);
			
		assert(_ItemsNewPtr != nullptr);
		
		
		irParentNode->Attributes      = _ItemsNewPtr;
		irParentNode->AttrCapacity = _NeedCapacity;
		
		
		
		if(true)
		{
			int _PPc, cPi; for(_PPc = irParentNode->AttrCount, cPi = 0; cPi < _PPc; cPi ++)
			{
				DataNodeStruct* cParentNode = &irParentNode->Attributes[cPi];
				
				if(cParentNode->Attributes != nullptr)
				{
					int _CCc = cParentNode->AttrCount, cCi = 0; for(; cCi < _CCc; cCi ++)
					{
						DataNodeStruct* cDataNode = &cParentNode->Attributes[cCi];
						cDataNode->Parent = cParentNode;
					}
				}
			}
		}
	}
}
void                 gfDNAttributes_Clear    (DataNodeStruct* irParentNode)
{
	irParentNode->AttrCount = 0;
	WTFE("NI: not freed resources");
}


struct $__DataNodeStruct__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
DataNodeStruct* gfDataNode_Create()
{
	DataNodeStruct* oNode = malloc(sizeof(DataNodeStruct));
	
	gfDataNode_Init_1(oNode);
	
	return oNode;
}
void gfDataNode_Destroy(DataNodeStruct* irNode)
{
	
	HERE;
}

void gfDataNode_Init_1(DataNodeStruct* irNode)
{
	gfDataNode_Init_3(irNode, DN_T_Undefined, DN_VT_Null);
}
void gfDataNode_Init_2(DataNodeStruct* irNode, DataNodeType iNodeType)
{
	gfDataNode_Init_3(irNode, iNodeType, DN_VT_Null);
}
void gfDataNode_Init_3(DataNodeStruct* irNode, DataNodeType iNodeType, DataNodeValueType iValueType)
{
	irNode->Name [0] = L'\0'; irNode->NameType  = DN_VT_Null;
	irNode->Value[0] = L'\0'; irNode->ValueType = iValueType;
	
	irNode->Type      = iNodeType;
	
	irNode->Parent    = nullptr;
	
	irNode->Attributes    = nullptr;
	irNode->AttrCount     = 0;
	irNode->AttrCapacity  = 0;
	
	irNode->Children      = nullptr;
	irNode->ChildCount    = 0;
	irNode->ChildCapacity = 0;
}
void gfDataNode_AppendMember(DataNodeStruct* irNode, DLSyntaxNode* iSyntaxNode, DLContext* iDLC)
{
	
}

void gfDataNode_SetNodeValue(DataNodeStruct* irNode, DLSyntaxNode* iSynNode, DLContext* iDLC)
{
	DLToken* _Token = &iDLC->Lexer->Tokens->Items[iSynNode->BegToken];
	
	wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
	size_t _TokenLen = _Token->Length;// + 40;
	DataNodeValueType _ValueType;
	
	assert(_TokenLen >= 1);
		
	_ValueType; switch(iSynNode->Type)
	{
		case DL_SYN_Number     : 
		case DL_SYN_NumFloat64 : {double _Value; int _ReadCount = swscanf_s(_TokenStr, L"%lf", &_Value); *((double*)&irNode->Value) = _Value; _ValueType = DN_VT_Float64; if(_ReadCount < 1 || _ReadCount > 20) STOP; break;}
		case DL_SYN_NumFloat32 : {float  _Value; int _ReadCount = swscanf_s(_TokenStr, L"%f",  &_Value); *((float*) &irNode->Value) = _Value; _ValueType = DN_VT_Float32; if(_ReadCount < 1 || _ReadCount > 20) STOP; break;}
		
		
		case DL_SYN_NumInvalid : _ValueType = DN_VT_NumInvalid; STOP; break;
		case DL_SYN_NumInt32   : {int    _Value; int _ReadCount = swscanf_s(_TokenStr, L"%d",  &_Value); *((int*) &irNode->Value) = _Value; _ValueType = DN_VT_Int32; if(_ReadCount < 1 || _ReadCount > 20) STOP; break;}
		
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
			
			_Token = &iDLC->Lexer->Tokens->Items[iSynNode->BegToken];
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
}

void gfDataNode_FromDLSyntaxNode(DataNodeStruct* irNode, DLSyntaxNode* iSyntaxNode, DLContext* iDLC)///, DataNodeType iNodeType)
{
	if(iSyntaxNode == nullptr) WTFE("Non-null node expected");
	///if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) WTFE("Empty node specified");
	if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) return;
	
	
	///if(iSyntaxNode->Type != DL_SYN_Expression) WTFE("Expression syntax node is expected");
	///if(iSyntaxNode->Type != DL_SYN_FunctionBlock) WTFE("Expected function block as node type");
	
	
	if(iSyntaxNode->Type == DL_SYN_Expression)
	{
		#if 0
		
			ChB;
			ChB "ChB_1";
			
			ChB (attr1 1; attr2 2.0; attr3 "V3";);
			ChB (attr1 1; attr2 2.0; attr3 "V3";) {ChC;};
			
			ChB "ChB_1" (attr1 1; attr2 2.0; attr3 "V3";);
			ChB "ChB_1" (attr1 1; attr2 2.0; attr3 "V3";) {ChC;};
			--
			ChB "ChB_1" (attr1 1; attr2 2.0; attr3 "V3";) {ChC;};
			
		#endif
		
		int _ChildCount, cCi; if(iSyntaxNode->Children == nullptr) WTFE("?");
		_ChildCount = iSyntaxNode->Children->Count;
		
		#define dIsTypeAndSingleton(idNode,idType) (idNode->Type == idType && idNode->Children != nullptr && idNode->Children->Count == 1)
		#define dGetNode(idSyntaxNode,idChildIndex,irdNode) \
		{\
			DLSyntaxNode *_List1,*_ListItem1; \
			\
			_List1     = &idSyntaxNode->Children->Items[idChildIndex]; if(!dIsTypeAndSingleton(_List1,     DL_SYN_List))     WTFE("1?");\
			_ListItem1 = &_List1      ->Children->Items[0];            if(!dIsTypeAndSingleton(_ListItem1, DL_SYN_ListItem)) WTFE("2?");\
			irdNode    = &_ListItem1  ->Children->Items[0];\
		}
			
		for(cCi = 0; cCi < _ChildCount; cCi ++)
		{
			DLSyntaxNode* cSynNode; dGetNode(iSyntaxNode, cCi, cSynNode);
			
			if(cSynNode->Type > DL_SYN_Identifiers__Begin && cSynNode->Type < DL_SYN_Identifiers__End)
			{
				DLToken* _Token; wchar_t* _TokenStr; size_t _TokenLen;
			
				_Token    = &iDLC->Lexer->Tokens->Items[cSynNode->BegToken];
				_TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
				_TokenLen = _Token->Length;// + 40;
				
				if(_TokenLen < dDataNodeNameMaxLength)
				{
					//memset(&this->Name,1,sizeof(this->Name));
					//wcscpy_s((wchar_t*)&this->Name, _TokenLen, _TokenStr); doesn't work;
					
					//memset(&this->Name,0,sizeof(this->Name));
					
					///gfDataNode_SetName(&irNode->Name, _TokenStr);
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
			else if(cSynNode->Type > DL_SYN_Value__Begin && cSynNode->Type < DL_SYN_Value__End)
			{
				gfDataNode_SetNodeValue(irNode, cSynNode, iDLC);
			}
			else
			{
				gfDataNode_FromDLSyntaxNode(irNode, cSynNode, iDLC);
			}
			//
			//switch(cSynNode->Type)
			//{
			//	case DL_SYN_GroupingBlock:
			//	{
			//		HERE;
			//		gfDataNode_FromDLSyntaxNode(irNode, cSynNode, iDLC);//, DN_T_Attribute);
			//		HERE;
			//		break;
			//	}
			//	case DL_SYN_FunctionBlock:
			//	{
			//		HERE;
			//		gfDataNode_FromDLSyntaxNode(irNode, cSynNode, iDLC);//, DN_T_Element);
			//		
			//		break;
			//	}
			//	
			//	default : STOP; break;
			//}
			HERE;
		}
		
		HERE;
	}
	else if(iSyntaxNode->Type == DL_SYN_GroupingBlock || iSyntaxNode->Type == DL_SYN_FunctionBlock || iSyntaxNode->Type == DL_SYN_File)
	{
		///~~ 'this' can be NOT initialized!;
		int _EEc, cEi;
	
		///~~ Document?;
		if(irNode->Children == nullptr) gfDNChildren_Init(irNode);
		if(iSyntaxNode->Children == nullptr) WTFE("?");
		
		for(_EEc = iSyntaxNode->Children->Count, cEi = 0; cEi < _EEc; cEi ++)
		{
			DLSyntaxNode* cExpNode;
			DataNodeStruct* cDataNode;
			bool cIsAttr = iSyntaxNode->Type == DL_SYN_GroupingBlock;
			
			cExpNode = &iSyntaxNode->Children->Items[cEi]; if(cExpNode->Type != DL_SYN_Expression) continue;
			
			if(wcscmp((wchar_t*)&irNode->Name,L"ChA4") == 0)/// && this->Children->Count >= 2)
			{
				HERE;
			}
			
			if(cIsAttr) {cDataNode = gfDNAttributes_Allocate (irNode, 1); gfDataNode_Init_2(cDataNode, DN_T_Attribute); cDataNode->Parent = irNode; gfDataNode_FromDLSyntaxNode(cDataNode, cExpNode, iDLC);}
			else        {cDataNode = gfDNChildren_Allocate   (irNode, 1); gfDataNode_Init_2(cDataNode, DN_T_Element);   cDataNode->Parent = irNode; gfDataNode_FromDLSyntaxNode(cDataNode, cExpNode, iDLC);}
		}
	}
	/*else if(iSyntaxNode->Type == DL_SYN_GroupingBlock)
	{
		
		HERE;
	}*/
	else
	{
		WTFE("Expected file, function block or expression as node type");
	}
	
	///iSyntaxNode->
	
	HERE;
	///iSyntaXNode
}
DataNodeStruct* gfDataNode_FromString(wchar_t* iBuffer)
{
	DataNodeStruct* oNode;
	DLContext* _DLC = gfDLCtx_Create(iBuffer, wcslen(iBuffer),true);
	{
		gfLexCtx_ParseBuffer(_DLC->Lexer);
		gfLexCtx_ProcessPairs(_DLC->Lexer);
		
		gfParser_ParseTokens(_DLC->Parser);
	}

	oNode = gfDataNode_Create();
	{
		gfDataNode_FromDLSyntaxNode(oNode, _DLC->Parser->RootNode, _DLC);//, DN_T_Test);
		///gfDLCtx_Destroy(_DLC);
	}
	
	gfDLCtx_Destroy(_DLC);
	
	return oNode;
}

DataNodeStruct* gfDataNode_GetChildByNameAndIndex(DataNodeStruct* iNode, char* iName, int iIndex)
{
	return iNode;
}
DataNodeStruct* gfDataNode_GetChildByNameAndIndex_2(DataNodeStruct* iNode, wchar_t* iName, int iIndex)
{
	if(iNode->Children != nullptr && iNode->ChildCount != 0)
	{
		int _CCc, cCi; for(_CCc = iNode->ChildCount, cCi = 0; cCi < _CCc; cCi ++)
		{
			DataNodeStruct* cNode = &iNode->Children[cCi];
			
			if(wcscmp(cNode->Name, iName) == 0)
			{
				return cNode;
			}
		}
	}
	return &gDataNodeStruct_Null;
}



///bool gfSplitPath(wchar_t* iName, size_t iNameSize, wchar_t iPath)
//#define dfSplitPath() \
//\
//	wchar_t _ChildName[dDataNodeNameMaxLength];\
//	wchar_t* _SubPath = nullptr;\
//	{\
//		int _PathLen = (int)wcslen(iPath), _DelimIndex = wcsindexof(iPath, (int)wcslen(iPath), L"/", 1);\
//		\
//		wcsncpy_s((wchar_t*)&_ChildName, sizeof(_ChildName) / sizeof(wchar_t), iPath, _DelimIndex != -1 ? _DelimIndex : _PathLen);\
//		\
//		if(_DelimIndex != -1)\
//		{\
//			if(!iDoAllowSubPath) STOP;\
//			_SubPath = &iPath[_DelimIndex + 1];\
//		}\
//	}
//
//

DataNodeStruct* gfDataNode_GetNodeByPath(DataNodeStruct* irNode, wchar_t* iPath, bool iDoAllowSubPath, bool iDoCreateMissing)
{
	if   (iPath == nullptr) return irNode;
	else
	{
		DataNodeStruct* oNode = nullptr;
		wchar_t _ChildName[dDataNodeNameMaxLength];
		wchar_t* _SubPath = nullptr;
		{
			int _PathLen = (int)wcslen(iPath), _DelimIndex = wcsindexof(iPath, (int)wcslen(iPath), L"/", 1);
			
			wcsncpy_s((wchar_t*)&_ChildName, sizeof(_ChildName) / sizeof(wchar_t), iPath, _DelimIndex != -1 ? _DelimIndex : _PathLen);
			
			if(_DelimIndex != -1)
			{
				if(!iDoAllowSubPath) WTFE("Subpath query is disallowed in this mode");
				_SubPath = &iPath[_DelimIndex + 1];
			}
		}
		
		///dfSplitPath();

		
		if(irNode->Children != nullptr)
		{
			///bool _IsChildFound = false;
			
			///int _CCc = iNode->Children->Count, cCi = 0; for(; cCi < _CCc; cCi ++)
			int cCi = 0; for(; cCi < irNode->ChildCount; cCi ++)
			{
				DataNodeStruct* cChild = &irNode->Children[cCi];
				
				if(wcscmp((wchar_t*)&cChild->Name, (wchar_t*)&_ChildName) == 0)
				{
					oNode = _SubPath == nullptr ? cChild : gfDataNode_GetNodeByPath(cChild, _SubPath, iDoAllowSubPath, iDoCreateMissing);
					
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
			
			if(irNode->Children == nullptr) gfDNChildren_Init(irNode);
			
			///_Child = gfDNChildren_Allocate(irNode->Children, 1);
			_Child = gfDNChildren_Allocate(irNode, 1);
			{
				gfDataNode_Init_1(_Child);
				gfDataNode_SetName(_Child, (wchar_t*)&_ChildName);
				
				_Child->Parent = irNode;
			}
			
			///oNode = _SubPath != nullptr ? gfDN_GetNodeByPath(irNode, _SubPath, iDoAllowSubPath, iDoCreateMissing) : _Child;
			oNode = _SubPath != nullptr ? gfDataNode_GetNodeByPath(_Child, _SubPath, iDoAllowSubPath, iDoCreateMissing) : _Child;
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
}
DataNodeStruct* gfDataNode_SetNodeByPath(DataNodeStruct* irNode, wchar_t* iPath,  DataNodeStruct* iNewNode, bool iDoAllowSubPath, bool iDoCreateMissing)
{
	STOP;
	return nullptr;
}


void gfDataNode_Begin(DataNodeStruct** irNode, wchar_t* iPath)
{
	DataNodeStruct* _Node = gfDataNode_GetNodeByPath(*irNode, iPath, false, true);
	
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
	/////gfDNChildren_Allocate
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
void gfDataNode_SetValueWSZ (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iValue)
{
	size_t _Len = wcslen(iValue);
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, true);
	
	if(_Len + 1 > dDataNodeValueMaxLength)
	{
		//wchar_t* _Str =  (wchar_t*)malloc(_Len + 1);
		
		//wcsncpy_s((wchar_t*)&irNode->Name, _countof(irNode->Name), iName, _TRUNCATE);
		
		STOP;
		_TgtNode->ValueType = DN_VT_StringPtr;
	}
	else
	{
		_TgtNode->ValueType = DN_VT_String;
	}
	HERE;
	
	///*((wchar_t*)&_TgtNode->Value) = iValue;
	
	
	
	STOP;
}
void     gfDataNode_SetValueI32 (DataNodeStruct* iNode, wchar_t* iPath, int      iValue)
{
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, true);
	
	*((int*)&_TgtNode->Value) = iValue;
	
	_TgtNode->ValueType = DN_VT_Int32;
	
	HERE;
}
void     gfDataNode_SetValueF32 (DataNodeStruct* iNode, wchar_t* iPath, float    iValue)
{
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, true);
	
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
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, false);
	
	assert(_TgtNode->ValueType == DN_VT_Int32);
	
	oValue = *((int*)&_TgtNode->Value);
	
	return oValue;
}
float    gfDataNode_GetValueF32 (DataNodeStruct* iNode, wchar_t* iPath)
{
	float oValue;
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, false);
	
	assert(_TgtNode->ValueType == DN_VT_Float32);
	
	oValue = *((float*)&_TgtNode->Value);
	
	return oValue;
}

wchar_t* gfDataNode_GetNameOr     (DataNodeStruct* iNode, wchar_t* iName, wchar_t* iOrName) {STOP; return nullptr;}
char*    gfDataNode_GetValueSZOr  (DataNodeStruct* iNode, wchar_t* iPath, char*    iOrValue){STOP; return nullptr;}
wchar_t* gfDataNode_GetValueWSZOr (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iOrValue){STOP; return nullptr;}
int      gfDataNode_GetValueI32Or (DataNodeStruct* iNode, wchar_t* iPath, int      iOrValue){STOP; return -1;}
float    gfDataNode_GetValueF32Or (DataNodeStruct* iNode, wchar_t* iPath, float    iOrValue){STOP; return -1.0f;}

struct $__WString__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
WString* gfWString_Create(int iInitialCapacity)
{
	WString* oStr = malloc(sizeof(WString));
	{
		oStr->Data     = nullptr;
		oStr->Capacity = 0;
		oStr->Length   = 0;
		
		if(iInitialCapacity != 0)
		{
			gfWString_Reserve(oStr, iInitialCapacity);
		}
	}
	return oStr;
}
void     gfWString_Destroy(WString* irString)
{
	if(irString->Data != nullptr)
	{
		free(irString->Data);
	}
	free(irString);
}

void gfWString_Reserve(WString* irString, int iNewCapacity)
{
	int _NewSize = iNewCapacity * sizeof(wchar_t);
	
	wchar_t* _DataNewPtr;
	
	if(_NewSize <= irString->Capacity) return;
	
	_DataNewPtr = realloc(irString->Data, _NewSize);
	assert(_DataNewPtr != nullptr);
	
	irString->Data = _DataNewPtr;
	irString->Capacity = _NewSize / sizeof(wchar_t);
}

//
//int gfStringReserve(wchar_t** irBuffer, int iCurrentCapacity, int iNewCapacity)
//{
//	///if(*irBuffer == nullptr)
//	///int _NeedCapacity = (int)(iCapacity * 1);
//	int _NewSize = iNewCapacity * sizeof(wchar_t);
//	
//	
//	wchar_t* _BufferNewPtr = realloc(*irBuffer, _NewSize);
//	
//	assert(_BufferNewPtr != nullptr);
//		
//	*irBuffer = _BufferNewPtr;
//	
//	return _NewSize / sizeof(wchar_t);
//}
int gfWString_Write(WString* irString, wchar_t* iSrcBuffer, int iSrcLength)
{
	
	int _LenStep = iSrcLength == -1 ? (int)wcslen(iSrcBuffer) + 1 : iSrcLength;/// - 1;
	
	if(iSrcLength == -1)
	{
		HERE;
	}
	/*if(iSrcLength == _TRUNCATE)
	{
		int _Length = ;
		HERE;
	}
	*/
	HERE;
	gfWString_Reserve(irString, irString->Length + _LenStep);///_LenStep); ///~~ SZ?;
	///irString->
	///HERE;
	
	
	///wcsncpy_s(_StrPtr, _TokenLenWOQZ, _TokenStrWOQ, _TokenLenWOQ);
	wcsncpy_s(irString->Data + irString->Length, iSrcLength, iSrcBuffer, _LenStep);
	
	irString->Length += _LenStep - 1;
	
	return irString->Length;
}
int gfWString_WriteLine(WString* irString, wchar_t* iSrcBuffer, int iSrcLength, int iIndent)
{
	HERE;
	
	return 0;
}
///int gfDataNode_ToString(DataNodeStruct* iNode, wchar_t** irBuffer, int iBufferCapacity, int iBufferSize, int iIndent)
int gfDataNode_ToString(DataNodeStruct* iNode, WString* irString, int iIndent)
{
	///gfStringWriteLine(irBuffer, iBufferCapacity, iBufferSize, L"", );
	//if(iBufferCapacity == 0) iBufferCapacity = 64;
	//if(iBufferSize >= iBufferCapacity - 1)
	//{
	//	iBufferCapacity = 64;
	//	
	//	gfReserveString(irBuffer, iBufferCapacity * 2);
	//}
	int _NameLenZ;
	wchar_t _Tab[] = L"\t";
	wchar_t _ExpDelim[] = L";\r\n";
	wchar_t _BlockOpener[] = L"\r\n{\r\n";
	wchar_t _BlockCloser[] = L"}";
	
	
	int cIi = 0; for(; cIi < iIndent; cIi ++)
	{
		///gfWString_Write(irString, (wchar_t*)L"\t", sizeof(wchar_t));
		gfWString_Write(irString, (wchar_t*)&_Tab, sizeof(_Tab) / sizeof(wchar_t));
	}
	
	_NameLenZ = (int)wcslen((wchar_t*)&iNode->Name) + 1;
	
	if(_NameLenZ == 0) WTFE("Null-terminated string expected");
	if(_NameLenZ != 1)
	{
		gfWString_Write(irString, (wchar_t*)&iNode->Name, _NameLenZ);///_NameLenZ);
	}
	
	if(iNode->ValueType != DN_VT_Null)
	{
		wchar_t _Buffer[256]; int _CharCount;
		if(iNode->ChildCount != 0) WTFE("Not expected");

		///_CharCount = _vsnwprintf_s((wchar_t*)&_Buffer, sizeof(_Buffer) / sizeof(_Buffer[0]), _TRUNCATE, iStr, _ArgPtr);
		
		switch(iNode->ValueType)
		{
			case DN_VT_Int32   : _CharCount = swprintf_s((wchar_t*)&_Buffer, sizeof(_Buffer) / sizeof(_Buffer[0]), L"%d", *(int    *)iNode->Value); break;
			case DN_VT_Float32 : _CharCount = swprintf_s((wchar_t*)&_Buffer, sizeof(_Buffer) / sizeof(_Buffer[0]), L"%f", *(float  *)iNode->Value); break;
			case DN_VT_Float64 : _CharCount = swprintf_s((wchar_t*)&_Buffer, sizeof(_Buffer) / sizeof(_Buffer[0]), L"%f", *(double *)iNode->Value); break;
			
			default : STOP;
		}
		
		gfWString_Write(irString, L" ", -1);
		gfWString_Write(irString, (wchar_t*)&_Buffer, _CharCount + 1);
		
		HERE;
	}
	if(iNode->ChildCount != 0)
	{
		int cCi; 
		///gfWString_Write(irString, (wchar_t*)L"{", sizeof(wchar_t));
		gfWString_Write(irString, (wchar_t*)&_BlockOpener, sizeof(_BlockOpener) / sizeof(wchar_t));
		
		for(cCi = 0; cCi < iNode->ChildCount; cCi ++)
		{
			DataNodeStruct* cChild = &iNode->Children[cCi];
			
			//int cIi = 0; for(; cIi < iIndent; cIi ++)
			//{
			//	///gfWString_Write(irString, (wchar_t*)L"\t", sizeof(wchar_t));
			//	gfWString_Write(irString, (wchar_t*)&_Tab, sizeof(_Tab) / sizeof(wchar_t));
			//}
			
			gfDataNode_ToString(cChild, irString, iIndent + 1);
			
			gfWString_Write(irString, (wchar_t*)&_ExpDelim, sizeof(_ExpDelim) / sizeof(wchar_t));
		}
		
		gfWString_Write(irString, (wchar_t*)&_BlockCloser, sizeof(_BlockCloser) / sizeof(wchar_t));
	}
	
	
	return 0;
}

void gfDataNode_InitStatic()
{
	///memset(&DataNode::Null,0,sizeof(DataNode));
	
	
}