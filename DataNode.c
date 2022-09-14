#include "DataNode.h"
#include "UL.h"
//#include "AE.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <wchar.h>
#include <assert.h>
#include <memory.h>
//#include <string.h>
#include <stdarg.h>


#define nullptr 0
size_t _msize(void* iPtr);

static int gf_BreakpointFunction(int iNum){return iNum;}
#define HERE  gf_BreakpointFunction(0)
#define STOP  gf_BreakpointFunction(1 / gf_BreakpointFunction(0))
#define WTFE(idMsg) STOP;

#define dfCheckMissing(idNode) assert(idNode != nullptr && idNode->Type != DN_T_Attribute_Missing && idNode->Type != DN_T_Element_Missing);
#define dTypeListInitialCapacity   4

DataNodeStruct gDataNodeStruct_Null;

#define dfCastNumberValue(idValue, idType) switch(idValue->Type)\
{\
	case DN_VT_Float32 : oValue = (idType) *( (float*)&(idValue->Data)); break;\
	case DN_VT_Float64 : oValue = (idType) *((double*)&(idValue->Data)); break;\
	case DN_VT_Int32   : oValue = (idType) *(   (int*)&(idValue->Data)); break;\
	\
	default : STOP;\
}

	
WString* gfWString_Create    (int iInitialCapacity)
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
void     gfWString_Destroy   (WString* iString)
{
	if(iString->Data != nullptr)
	{
		free(iString->Data);
	}
	free(iString);
}

void     gfWString_Reserve   (WString* iString, int iNewCapacity)
{
	int _NewSize = iNewCapacity * sizeof(wchar_t);
	
	wchar_t* _DataNewPtr;
	
	if(_NewSize <= iString->Capacity) return;
	
	_DataNewPtr = realloc(iString->Data, _NewSize);
	assert(_DataNewPtr != nullptr);
	
	iString->Data     = _DataNewPtr;
	iString->Capacity = _NewSize / sizeof(wchar_t);
}

int      gfWString_Write     (WString* iString, wchar_t* iSrcBuffer, int iSrcLength)
{
	int _LenStep = iSrcLength == -1 ? (int)wcslen(iSrcBuffer) + 1 : iSrcLength;/// - 1;
	
	gfWString_Reserve(iString, iString->Length + _LenStep);
	
	wcsncpy_s(iString->Data + iString->Length, iSrcLength, iSrcBuffer, _LenStep);
	
	iString->Length += _LenStep - 1;
	
	return iString->Length;
}
int      gfWString_WriteLine (WString* iString, wchar_t* iSrcBuffer, int iSrcLength, int iIndent)
{
	STOP;
	
	return 0;
}

	
DataNodeValue* gfDNValue_AllocNext(DataNodeValue* iValue)
{
	DataNodeValue* oNextValue = (DataNodeValue*)malloc(sizeof(DataNodeValue));
	{
		oNextValue->NextPtr = nullptr;
	}
	
	iValue->NextPtr = oNextValue;
	
	return oNextValue;
}
void           gfDNValue_ClearAll(DataNodeValue* iValue)
{
	DataNodeValue* cValue = iValue; while(cValue != nullptr)
	{
		if(cValue->Type == DN_VT_StringPtr || cValue->Type == DN_VT_WStringPtr)
		{
			free(cValue->Data);
		}
		cValue = cValue->NextPtr;
	}
}
DataNodeValue* gfDNValue_GetNext(DataNodeValue* iValue)
{
	return iValue->NextPtr;
}
int      gfDNValue_GetI32(DataNodeValue* iValue)
{
	///int oValue = *((int*)iValue->Data);
	int oValue; dfCastNumberValue(iValue, int);
	return oValue;
}
float    gfDNValue_GetF32(DataNodeValue* iValue)
{
	///float oValue = *((float*)iValue->Data);
	float oValue; dfCastNumberValue(iValue, float);
	return oValue;
}
double   gfDNValue_GetF64(DataNodeValue* iValue)
{
	///double oValue = *((double*)iValue->Data);
	double oValue; dfCastNumberValue(iValue, double);
	return oValue;
}
void     gfDNValue_GetStr  (DataNodeValue* iValue, char*    iDstBuf, size_t iMaxLen)
{
	//bool _IsWCS = iValue->Type == DN_VT_WString   || iValue->Type == DN_VT_WStringPtr;
	//bool _IsPtr = iValue->Type == DN_VT_StringPtr || iValue->Type == DN_VT_WStringPtr;
	//
	//
	STOP;
}
void     gfDNValue_GetWStr (DataNodeValue* iValue, wchar_t* iDstBuf, size_t iMaxLen)
{
	bool _IsWCS = iValue->Type == DN_VT_WString   || iValue->Type == DN_VT_WStringPtr;
	bool _IsPtr = iValue->Type == DN_VT_StringPtr || iValue->Type == DN_VT_WStringPtr;

	wchar_t** _SrcPtr; size_t _Len = min(iMaxLen, dDataNodeValueMaxLength);
	{
		if(!_IsWCS)
		{
			STOP;
		}
		
		_SrcPtr = _IsPtr ? (wchar_t**)iValue->Data : (wchar_t**)&iValue->Data;
	}
	
	///wcsncpy_s(iBuffer, min(iMaxLen, dDataNodeValueMaxLength), *_SrcPtr, iMaxLen - 1);
	wcsncpy_s(iDstBuf, min(iMaxLen, dDataNodeValueMaxLength), *_SrcPtr, _TRUNCATE);
}
char*    gfDNValue_GetStrPtr  (DataNodeValue* iValue)
{
	STOP;
	
	return nullptr;
}
wchar_t* gfDNValue_GetWStrPtr (DataNodeValue* iValue)
{
	bool _IsWCS = iValue->Type == DN_VT_WString   || iValue->Type == DN_VT_WStringPtr;
	bool _IsPtr = iValue->Type == DN_VT_StringPtr || iValue->Type == DN_VT_WStringPtr;
	
	wchar_t* oStrPtr;
	{
		if(!_IsWCS)
		{
			STOP;
		}
		oStrPtr = _IsPtr ? *(wchar_t**)iValue->Data : iValue->Data;
	}
	
	return oStrPtr;
}

	
DataNodeTypeList*    gfDNTypeList_Create(wchar_t* iTypeListString)
{
	wchar_t* cTypeStr; size_t cStrLenReminder;
	
	DataNodeTypeList* oList = malloc(sizeof(DataNodeTypeList));
	{
		oList->Items    = malloc(sizeof(DataNodeTypeList) * dTypeListInitialCapacity);
		oList->Count    = 0;
		oList->Capacity = dTypeListInitialCapacity;

		cStrLenReminder = wcslen(iTypeListString);
		
		cTypeStr = iTypeListString; while(cTypeStr[0] != L'\0')
		{
			DataNodeValueType cType = DN_VT_Undefined;
			
			int cLen = wcsindexof(cTypeStr, (int)cStrLenReminder, L",", 1);
			
			if      (wcsncmp(cTypeStr, L"i32", cLen) == 0) cType = DN_VT_Int32;
			else if (wcsncmp(cTypeStr, L"f32", cLen) == 0) cType = DN_VT_Float32;
			else if (wcsncmp(cTypeStr, L"f64", cLen) == 0) cType = DN_VT_Float64;
			else if (wcsncmp(cTypeStr, L"str", cLen) == 0)
			{
				cType = DN_VT_String;
			}
			else if (wcsncmp(cTypeStr, L"wcs", cLen) == 0)
			{
				cType = DN_VT_WString;
			}
			else
			{
				STOP;
			}
			
			assert(oList->Count < oList->Capacity);
			
			oList->Items[oList->Count ++] = cType;
			
			
			
			HERE;
			if(cLen != -1)
			{
				cTypeStr += cLen + 1;
				continue;
			}
			else break;
		}
	}
	//gfDataNode_Init_1(oNode);
	
	return oList;
}
void                 gfDNTypeList_Destroy(DataNodeTypeList* iTypeList)
{
	free(iTypeList->Items);
	free(iTypeList);
}
	
void                 gfDNChildren_Init     (DataNodeStruct* iParentNode)
{
	assert(iParentNode->Children == nullptr);
	
	gfDNChildren_Reserve(iParentNode, 0);
}

DataNodeStruct*      gfDNChildren_Allocate (DataNodeStruct* iParentNode, int iCount)
{
	gfDNChildren_Reserve(iParentNode, iParentNode->ChildCount + iCount);
	
	iParentNode->ChildCount += iCount;
	
	return &iParentNode->Children[iParentNode->ChildCount - 1];
}

DataNodeStruct*      gfDNChildren_Pop      (DataNodeStruct* iParentNode)
{
	DataNodeStruct* oItem;

	if(iParentNode->ChildCount <= 0) return nullptr;
	
	return &iParentNode->Children[-- iParentNode->ChildCount];
}
DataNodeStruct*      gfDNChildren_Peek     (DataNodeStruct* iParentNode)
{
	if(iParentNode->ChildCount <= 0) return nullptr;
	
	return &iParentNode->Children[iParentNode->ChildCount - 1];
}



void                 gfDNChildren_Reserve  (DataNodeStruct* iParentNode, int iCapacity)
{
	if(iCapacity <= iParentNode->ChildCapacity) return;
	
	
	{
		size_t _OldSize, _NewSize;
		DataNodeStruct *_ItemsOldPtr = iParentNode->Children, *_ItemsNewPtr;
		
		int _NeedCapacity = (int)(iCapacity * dDataNodeCollectionCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(DataNodeStruct);
		
		_ItemsNewPtr = (iParentNode->Children == nullptr ? malloc(_NeedSize) : realloc(iParentNode->Children, _NeedSize));
		
		assert(_ItemsNewPtr != nullptr);
		
		
		iParentNode->Children      = _ItemsNewPtr;
		iParentNode->ChildCapacity = _NeedCapacity;
		
		if(true)
		{
			int _PPc, cPi; for(_PPc = iParentNode->ChildCount, cPi = 0; cPi < _PPc; cPi ++)
			{
				DataNodeStruct* cParentNode = &iParentNode->Children[cPi];
				
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
void                 gfDNChildren_Clear    (DataNodeStruct* iParentNode)
{
	iParentNode->ChildCount = 0;
	WTFE("NI: not freed resources");
}


	
void                 gfDNAttributes_Init     (DataNodeStruct* iParentNode)
{
	assert(iParentNode->Attributes == nullptr);
	
	gfDNAttributes_Reserve(iParentNode, 0);
}
DataNodeStruct*      gfDNAttributes_Allocate (DataNodeStruct* iParentNode, int iCount)
{
	gfDNAttributes_Reserve(iParentNode, iParentNode->AttrCount + iCount);
	
	iParentNode->AttrCount += iCount;
	
	return &iParentNode->Attributes[iParentNode->AttrCount - 1];
}

DataNodeStruct*      gfDNAttributes_Pop      (DataNodeStruct* iParentNode)
{
	DataNodeStruct* oItem;

	if(iParentNode->AttrCount <= 0) return nullptr;
	
	return &iParentNode->Attributes[-- iParentNode->AttrCount];
}
DataNodeStruct*      gfDNAttributes_Peek     (DataNodeStruct* iParentNode)
{
	if(iParentNode->AttrCount <= 0) return nullptr;
	
	return &iParentNode->Attributes[iParentNode->AttrCount - 1];
}



void                 gfDNAttributes_Reserve  (DataNodeStruct* iParentNode, int iCapacity)
{
	if(iCapacity <= iParentNode->AttrCapacity) return;
	
	{
		size_t _OldSize, _NewSize;
		DataNodeStruct *_ItemsOldPtr = iParentNode->Attributes, *_ItemsNewPtr;
		
		int _NeedCapacity = (int)(iCapacity * dDataNodeCollectionCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(DataNodeStruct); //, _Size;
		
		_ItemsNewPtr = (iParentNode->Attributes == nullptr ? malloc(_NeedSize) : realloc(iParentNode->Attributes, _NeedSize));
		
		assert(_ItemsNewPtr != nullptr);
		
		iParentNode->Attributes      = _ItemsNewPtr;
		iParentNode->AttrCapacity = _NeedCapacity;
		
		if(true)
		{
			int _PPc, cPi; for(_PPc = iParentNode->AttrCount, cPi = 0; cPi < _PPc; cPi ++)
			{
				DataNodeStruct* cParentNode = &iParentNode->Attributes[cPi];
				
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
void                 gfDNAttributes_Clear    (DataNodeStruct* iParentNode)
{
	iParentNode->AttrCount = 0;
	WTFE("NI: not freed resources");
}


	
DataNodeStruct* gfDataNode_Create()
{
	DataNodeStruct* oNode = malloc(sizeof(DataNodeStruct));
	
	gfDataNode_Init_1(oNode);
	
	return oNode;
}
void            gfDataNode_Destroy(DataNodeStruct* iNode)
{
	int cAi,cCi;
	
	if(iNode->Value.Type == DN_VT_StringPtr || iNode->Value.Type == DN_VT_WStringPtr)
	{
		gfDNValue_ClearAll(&iNode->Value);
	}
	if(iNode->Attributes != nullptr) for(cAi = 0; cAi < iNode->AttrCount; cAi ++)
	{
		gfDataNode_Destroy(&iNode->Attributes[cAi]); 
	}
	if(iNode->Children != nullptr) for(cCi = 0; cCi < iNode->ChildCount; cCi ++)
	{
		gfDataNode_Destroy(&iNode->Children[cCi]);
	}
	HERE;
}

	
void gfDataNode_Init_1(DataNodeStruct* iNode)
{
	gfDataNode_Init_3(iNode, DN_T_Undefined, DN_VT_Null);
}
void gfDataNode_Init_2(DataNodeStruct* iNode, DataNodeType iNodeType)
{
	gfDataNode_Init_3(iNode, iNodeType, DN_VT_Null);
}
void gfDataNode_Init_3(DataNodeStruct* iNode, DataNodeType iNodeType, DataNodeValueType iValueType)
{
	iNode->Name       [0] = L'\0'; iNode->NameType   = DN_VT_Null;
	iNode->Value.Data [0] = L'\0'; iNode->Value.Type = iValueType;
	iNode->Value.NextPtr = nullptr;
	
	iNode->Type      = iNodeType;
	
	iNode->Parent    = nullptr;
	
	iNode->Attributes    = nullptr;
	iNode->AttrCount     = 0;
	iNode->AttrCapacity  = 0;
	
	iNode->Children      = nullptr;
	iNode->ChildCount    = 0;
	iNode->ChildCapacity = 0;
}
void gfDataNode_UpdateState(DataNodeStruct* iNode, bool iIsMissing)
{
	if(iIsMissing)
	{
		if     (iNode->Type == DN_T_Attribute) iNode->Type = DN_T_Attribute_Missing;
		else if(iNode->Type == DN_T_Element)   iNode->Type = DN_T_Element_Missing;
	}
	else
	{
		if     (iNode->Type == DN_T_Attribute_Missing) iNode->Type = DN_T_Attribute;
		else if(iNode->Type == DN_T_Element_Missing)   iNode->Type = DN_T_Element;
	}
}

void gfDataNode_AppendMember(DataNodeStruct* iNode, ULSyntaxNode* iSyntaxNode, ULContext* iULC)
{
	
}

void gfDataNode_SetNodeValue(DataNodeValue* iNodeValue, ULSyntaxNode* iSynNode, ULContext* iULC)
{
	ULToken* _Token = &iULC->Lexer->Tokens->Items[iSynNode->BegToken];
	
	wchar_t* _TokenStr = &iULC->Lexer->Buffer[_Token->Offset];
	size_t _TokenLen = _Token->Length;// + 40;
	DataNodeValueType _ValueType;
	
	assert(_TokenLen >= 1);
		
	_ValueType; switch(iSynNode->Type)
	{
		case UL_SYN_NumInvalid : _ValueType = DN_VT_NumInvalid; STOP; break;
		
		case UL_SYN_Number     : 
		case UL_SYN_NumFloat64 : {double _Value; int _ReadCount = swscanf_s(_TokenStr, L"%lf", &_Value); *((double*)&iNodeValue->Data) = _Value; _ValueType = DN_VT_Float64; if(_ReadCount < 1 || _ReadCount > 20) STOP; break;}
		case UL_SYN_NumFloat32 : {float  _Value; int _ReadCount = swscanf_s(_TokenStr, L"%f",  &_Value); *((float *)&iNodeValue->Data) = _Value; _ValueType = DN_VT_Float32; if(_ReadCount < 1 || _ReadCount > 20) STOP; break;}
		case UL_SYN_NumInt32   : {int    _Value; int _ReadCount = swscanf_s(_TokenStr, L"%d",  &_Value); *((int*)   &iNodeValue->Data) = _Value; _ValueType = DN_VT_Int32; if(_ReadCount < 1 || _ReadCount > 20) STOP; break;}
		
		case UL_SYN_String     :
		{
			size_t _TokenLenWOQ, _TokenLenWOQZ; bool _IsNeededHeapAlloc;
			wchar_t* _TokenStrWOQ;
			wchar_t* _StrPtr;
			
			assert(_Token->Length >= 2); ///~~ quotes included;
		
			//_TokenLen = _Token->Length;// + 40;
			_TokenLenWOQ  = _TokenLen - 2; ///~~ without quotes;
			_TokenLenWOQZ = _TokenLenWOQ + 1;  ///~~ without quotes but with a null terminator;
			_IsNeededHeapAlloc = _TokenLenWOQZ >= dDataNodeValueMaxLength;
			
			//_Token = &iULC->Lexer->Tokens->Items[iSynNode->BegToken];
			//_TokenStr = &iULC->Lexer->Buffer[_Token->Offset];
			_TokenStrWOQ = _TokenStr + 1; ///~~ skip opening quote;
			
			
			
			if(_IsNeededHeapAlloc){_StrPtr = malloc(_TokenLenWOQZ * sizeof(wchar_t)); _ValueType = DN_VT_WStringPtr; memcpy(&iNodeValue->Data, &_StrPtr, sizeof(void*));}
			else                  {_StrPtr = (wchar_t*)&iNodeValue->Data;             _ValueType = DN_VT_WString;}
			
			wcsncpy_s(_StrPtr, _TokenLenWOQZ, _TokenStrWOQ, _TokenLenWOQ);
			
			//(*(wchar_t*)_StrPtr)[]
			
			//this->Value[_TokenLen] = '\0';
			
			break;
		}
		case UL_SYN_ReferenceIdentifier :
		case UL_SYN_InputIdentifier     :
		case UL_SYN_OutputIdentifier    :
		case UL_SYN_LocalIdentifier     :
		case UL_SYN_GlobalIdentifier    :
		case UL_SYN_MemberIdentifier    :
		case UL_SYN_Word                :
		{
			size_t _TokenLenZ; bool _IsNeededHeapAlloc;
			wchar_t* _StrPtr;
		
			_TokenLenZ = _Token->Length + 1;
			_IsNeededHeapAlloc = _TokenLenZ >= dDataNodeValueMaxLength;
			
			//_Token = &iULC->Lexer->Tokens->Items[iSynNode->BegToken];
			//_TokenStr = &iULC->Lexer->Buffer[_Token->Offset];
			
			if(_IsNeededHeapAlloc){_StrPtr = malloc(_TokenLenZ * sizeof(wchar_t)); _ValueType = DN_VT_WStringPtr; memcpy(&iNodeValue->Data, &_StrPtr, sizeof(void*));}
			else                  {_StrPtr = (wchar_t*)&iNodeValue->Data;          _ValueType = DN_VT_WString;}
			
			wcsncpy_s(_StrPtr, _TokenLenZ, _TokenStr, _TokenLen);
			
			
			HERE;
			break;
		}
		
		default : STOP;
	}
	
	iNodeValue->Type = _ValueType;
}
void gfDataNode_SetNodeValues(DataNodeStruct* iNode, ULSyntaxNode* iListNode, ULContext* iULC)
{
	int cIi; DataNodeValue* cValue = &iNode->Value; for(cIi = 0; cIi < iListNode->Children->Count; cIi ++)
	{
		ULSyntaxNode* cListItem = &iListNode->Children->Items[cIi];
		ULSyntaxNode* cAoBNode  = &cListItem->Children->Items[0];
		
		if(cIi > 0) cValue = gfDNValue_AllocNext(cValue);
		
		gfDataNode_SetNodeValue(cValue, cAoBNode, iULC);
		
		if(cIi > 0)
		{
			HERE;
		}
	}
	HERE;
}

	
void            gfDataNode_FromULSyntaxNode(DataNodeStruct* iNode, ULSyntaxNode* iSyntaxNode, ULContext* iULC)///, DataNodeType iNodeType)
{
	if(iSyntaxNode == nullptr) WTFE("Non-null node expected");
	///if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) WTFE("Empty node specified");
	if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) return;
		
	if(iSyntaxNode->Type == UL_SYN_Expression)
	{
		int _ListCount, cLi; wchar_t* _NodeName = nullptr;
		
		if(iSyntaxNode->Children == nullptr) WTFE("?");
		_ListCount = iSyntaxNode->Children->Count;
		
		#define dfIsType(idNode,idType) (idNode->Type == idType)
		#define dfIsSingleton(idNode)   (idNode->Children != nullptr && idNode->Children->Count == 1)
		 
		#define dfGetList(idSyntaxNode,idListIndex,irdListNode) \
		{\
			ULSyntaxNode *_List;\
			\
			_List       = &idSyntaxNode->Children->Items[idListIndex]; if(!dfIsType(_List, UL_SYN_List)) WTFE("List?");\
			irdListNode = _List;\
		}
		#define dfGetAoB(idListNode,idListItemIndex,irdAoBNode) \
		{\
			ULSyntaxNode *_ListItem;\
			\
			_ListItem  = &idListNode->Children->Items[idListItemIndex]; if(!dfIsType(_ListItem, UL_SYN_ListItem)) WTFE("ListItem?");\
			irdAoBNode = &_ListItem  ->Children->Items[0];\
		}
		
		_NodeName = iSyntaxNode->Children->Items[0].Children->Items[0].Children->Items[0].Token_->Value;
		
		
		for(cLi = 0; cLi < _ListCount; cLi ++)
		{
			ULSyntaxNode *cListNode, *cAoBNode;
			{
				dfGetList (iSyntaxNode, cLi, cListNode);
				dfGetAoB  (cListNode,     0, cAoBNode);
			}
			
			if(cLi == 0)
			{
				ULToken* _Token; wchar_t* _TokenStr; size_t _TokenLen;
				
				assert(cAoBNode->Type > UL_SYN_Identifiers__Begin && cAoBNode->Type < UL_SYN_Identifiers__End);
			
				_Token    = &iULC->Lexer->Tokens->Items[cAoBNode->BegToken];
				_TokenStr = &iULC->Lexer->Buffer[_Token->Offset];
				_TokenLen = _Token->Length;// + 40;
				
				if(_TokenLen < dDataNodeNameMaxLength)
				{
					memcpy(&iNode->Name, _TokenStr, _TokenLen * sizeof(wchar_t));
					iNode->Name[_TokenLen] = '\0';
					HERE;
				}
				else
				{
					///~~ pointer;
					STOP;
				}
			}
			else if(cAoBNode->Type > UL_SYN_Value__Begin && cAoBNode->Type < UL_SYN_Value__End)
			{
				gfDataNode_SetNodeValues(iNode, cListNode, iULC);
			}
			else
			{
				gfDataNode_FromULSyntaxNode(iNode, cAoBNode, iULC);
			}
			
			HERE;
		}
		
		HERE;
	}
	else if(iSyntaxNode->Type == UL_SYN_ParenthesisBlock || iSyntaxNode->Type == UL_SYN_BraceBlock || iSyntaxNode->Type == UL_SYN_Root)
	{
		int _EEc, cEi;
		
		if(iNode->Children == nullptr) gfDNChildren_Init(iNode);
		if(iSyntaxNode->Children == nullptr) WTFE("?");
		
		for(_EEc = iSyntaxNode->Children->Count, cEi = 0; cEi < _EEc; cEi ++)
		{
			ULSyntaxNode* cExpNode;
			DataNodeStruct* cDataNode;
			bool cIsAttr = iSyntaxNode->Type == UL_SYN_ParenthesisBlock;
			
			cExpNode = &iSyntaxNode->Children->Items[cEi]; if(cExpNode->Type != UL_SYN_Expression) continue;
			
			if(cIsAttr) {cDataNode = gfDNAttributes_Allocate (iNode, 1); gfDataNode_Init_2(cDataNode, DN_T_Attribute); cDataNode->Parent = iNode; gfDataNode_FromULSyntaxNode(cDataNode, cExpNode, iULC);}
			else        {cDataNode = gfDNChildren_Allocate   (iNode, 1); gfDataNode_Init_2(cDataNode, DN_T_Element);   cDataNode->Parent = iNode; gfDataNode_FromULSyntaxNode(cDataNode, cExpNode, iULC);}
		}
	}
	else
	{
		WTFE("Expected file, function block or expression as node type");
	}
}

DataNodeStruct* gfDataNode_FromString(wchar_t* iBuffer)
{
	DataNodeStruct* oNode;
	ULContext* _ULC = gfULCtx_Create(iBuffer, wcslen(iBuffer),true);
	
	oNode = gfDataNode_Create();
	{
		gfDataNode_FromULSyntaxNode(oNode, _ULC->Parser->RootNode, _ULC);
	}
	
	gfULCtx_Destroy(_ULC);
	
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

DataNodeStruct* gfDataNode_GetNodeByPath(DataNodeStruct* iNode, wchar_t* iPath, bool iDoAllowSubPath, bool iDoCreateMissing)
{
	if   (iPath == nullptr) return iNode;
	else
	{
		DataNodeStruct* oNode = nullptr;
		wchar_t _ChildOrAttrName[dDataNodeNameMaxLength];
		wchar_t* _SubPath = nullptr;
		bool _IsChildQuery = false;
		{
			int _PathLen = (int)wcslen(iPath), _DelimIndex = wcsindexof(iPath, (int)wcslen(iPath), L"/", 1);
			
			wcsncpy_s((wchar_t*)&_ChildOrAttrName, sizeof(_ChildOrAttrName) / sizeof(wchar_t), iPath, _DelimIndex != -1 ? _DelimIndex : _PathLen);
			
			if(_DelimIndex != -1)
			{
				if(!iDoAllowSubPath) WTFE("Subpath query is disallowed in this mode");
				_SubPath = &iPath[_DelimIndex + 1];
			}
			_IsChildQuery = _ChildOrAttrName[0] != '@';
		}

		if(_IsChildQuery)
		{
			if(iNode->Children != nullptr)
			{
				int cCi = 0; for(; cCi < iNode->ChildCount; cCi ++)
				{
					DataNodeStruct* cChild = &iNode->Children[cCi];
					
					if(wcscmp((wchar_t*)&cChild->Name, (wchar_t*)&_ChildOrAttrName) == 0)
					{
						oNode = _SubPath == nullptr ? cChild : gfDataNode_GetNodeByPath(cChild, _SubPath, iDoAllowSubPath, iDoCreateMissing);

						break;
					}
				}
			}
			else
			{
				HERE;
			}
		}
		else
		{
			wchar_t* _AttrName = &_ChildOrAttrName[1];
			
			int cAi = 0; for(; cAi < iNode->AttrCount; cAi ++)
			{
				DataNodeStruct* cAttribute = &iNode->Attributes[cAi];
				
				if(wcscmp((wchar_t*)&cAttribute->Name, _AttrName) == 0)
				{
					oNode = cAttribute;
					break;
				}
			}
			if(oNode == nullptr)
			{
				HERE;
				///oNode = gfDataNode_GetNodeByPath(cChild, _SubPath, false, true);
			}
		}
		
		if(oNode == nullptr && iDoCreateMissing)
		{
			DataNodeStruct* _ChildOrAttr;
		
			if(_IsChildQuery)
			{
				if(iNode->Children == nullptr) gfDNChildren_Init(iNode);
				_ChildOrAttr = gfDNChildren_Allocate(iNode, 1);
			}
			else
			{
				if(iNode->Attributes == nullptr) gfDNAttributes_Init(iNode);
				_ChildOrAttr = gfDNAttributes_Allocate(iNode, 1);
			}
			
			gfDataNode_Init_2(_ChildOrAttr, _IsChildQuery ? DN_T_Element_Missing : DN_T_Attribute_Missing);
			gfDataNode_SetName(_ChildOrAttr, (wchar_t*)(&_ChildOrAttrName[_IsChildQuery ? 0 : 1]));
			_ChildOrAttr->Parent = iNode;
			
			oNode = _SubPath != nullptr ? gfDataNode_GetNodeByPath(_ChildOrAttr, _SubPath, iDoAllowSubPath, iDoCreateMissing) : _ChildOrAttr;
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
DataNodeStruct* gfDataNode_SetNodeByPath(DataNodeStruct* iNode, wchar_t* iPath,  DataNodeStruct* iNewNode, bool iDoAllowSubPath, bool iDoCreateMissing)
{
	STOP;
	return nullptr;
}


	
void     gfDataNode_Begin         (DataNodeStruct** irNode, wchar_t* iPath)
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
void     gfDataNode_End           (DataNodeStruct** irNode)
{
	DataNodeStruct* _Parent = (*irNode)->Parent;
	assert(_Parent != nullptr);
	
	*irNode = _Parent;
	
	HERE;
}

void     gfDataNode_SetName       (DataNodeStruct* iNode, wchar_t* iName)
{
	wcsncpy_s((wchar_t*)&iNode->Name, _countof(iNode->Name), iName, _TRUNCATE);
}
void     gfDataNode_SetValueSZ    (DataNodeStruct* iNode, wchar_t* iPath, char*    iValue){STOP;}
void     gfDataNode_SetValueWSZ   (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iValue)
{
	size_t _Len = wcslen(iValue);
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, true);
	
	if(_Len + 1 > dDataNodeValueMaxLength)
	{
		//wchar_t* _Str =  (wchar_t*)malloc(_Len + 1);
		
		//wcsncpy_s((wchar_t*)&iNode->Name, _countof(iNode->Name), iName, _TRUNCATE);
		
		STOP;
		_TgtNode->Value.Type = DN_VT_WStringPtr;
	}
	else
	{
		_TgtNode->Value.Type = DN_VT_WString;
	}
	HERE;
	
	///*((wchar_t*)&_TgtNode->Value) = iValue;
	
	STOP;
}
void     gfDataNode_SetValueI32   (DataNodeStruct* iNode, wchar_t* iPath, int      iValue)
{
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, true);
	
	*((int*)&_TgtNode->Value.Data) = iValue;
	
	_TgtNode->Value.Type = DN_VT_Int32;
}
void     gfDataNode_SetValueF32   (DataNodeStruct* iNode, wchar_t* iPath, float    iValue)
{
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, true);
	
	*((float*)&_TgtNode->Value.Data) = iValue;
	
	_TgtNode->Value.Type = DN_VT_Float32;
}
void     gfDataNode_SetValueF64   (DataNodeStruct* iNode, wchar_t* iPath, double    iValue)
{
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, true);
	
	*((double*)&_TgtNode->Value.Data) = iValue;
	
	_TgtNode->Value.Type = DN_VT_Float64;
}

wchar_t* gfDataNode_GetName       (DataNodeStruct* iNode){STOP; return nullptr;}


void     gfDataNode_GetValuesVA   (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iFormat, void* iVArgs)
{
	va_list _VArgs = (va_list)iVArgs;
	DataNodeTypeList* _Types; int cVi;
	void* _StrPtr;
		 
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, false);
	
	dfCheckMissing(_TgtNode);
	
	_Types = gfDNTypeList_Create(iFormat);
	{
		DataNodeValue* cValue = &_TgtNode->Value; for(cVi = 0; cVi < _Types->Count; cVi ++)
		{
			DataNodeValueType cType = _Types->Items[cVi]; switch(cType)
			{
				case DN_VT_Int32   : *va_arg(_VArgs,    int*) = gfDNValue_GetI32(cValue); break;
				case DN_VT_Float32 : *va_arg(_VArgs,  float*) = gfDNValue_GetF32(cValue); break;
				case DN_VT_Float64 : *va_arg(_VArgs, double*) = gfDNValue_GetF64(cValue); break;
				
				case DN_VT_String  : {char*    cBuf = va_arg(_VArgs, char*);    size_t cLen = va_arg(_VArgs, size_t); gfDNValue_GetStr (cValue, cBuf, cLen); break;}
				case DN_VT_WString : {wchar_t* cBuf = va_arg(_VArgs, wchar_t*); size_t cLen = va_arg(_VArgs, size_t); gfDNValue_GetWStr(cValue, cBuf, cLen); break;}
			}
			
			cValue = cValue->NextPtr;
		}
	}
	gfDNTypeList_Destroy(_Types);
}
void     gfDataNode_GetValues     (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iFormat, ...)
{
	va_list _VArgs;
	va_start(_VArgs, iFormat);
	{
		gfDataNode_GetValuesVA(iNode, iPath, iFormat, _VArgs);
	}
	va_end(_VArgs);
}


char*    gfDataNode_GetValueSZ    (DataNodeStruct* iNode, wchar_t* iPath){STOP; return nullptr;}
wchar_t* gfDataNode_GetValueWSZ   (DataNodeStruct* iNode, wchar_t* iPath){STOP; return nullptr;}
int      gfDataNode_GetValueI32   (DataNodeStruct* iNode, wchar_t* iPath)
{
	int oValue;
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, false);
	
	dfCheckMissing(_TgtNode);
	///dfCastNumberValue((&_TgtNode->Value), int);
	oValue = gfDNValue_GetI32(&_TgtNode->Value);
	
	return oValue;
}
float    gfDataNode_GetValueF32   (DataNodeStruct* iNode, wchar_t* iPath)
{
	float oValue;
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, false);
	
	dfCheckMissing(_TgtNode);
	///dfCastNumberValue((&_TgtNode->Value), float);
	oValue = gfDNValue_GetF32(&_TgtNode->Value);
	
	return oValue;
}

double   gfDataNode_GetValueF64   (DataNodeStruct* iNode, wchar_t* iPath)
{
	double oValue;
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, false);
	
	dfCheckMissing(_TgtNode);
	///dfCastNumberValue((&_TgtNode->Value), double);
	oValue = gfDNValue_GetF64(&_TgtNode->Value);
	
	return oValue;
}

wchar_t* gfDataNode_GetNameOr     (DataNodeStruct* iNode, wchar_t* iName, wchar_t* iOrName) {STOP; return nullptr;}
char*    gfDataNode_GetValueSZOr  (DataNodeStruct* iNode, wchar_t* iPath, char*    iOrValue){STOP; return nullptr;}
wchar_t* gfDataNode_GetValueWSZOr (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iOrValue){STOP; return nullptr;}
int      gfDataNode_GetValueI32Or (DataNodeStruct* iNode, wchar_t* iPath, int      iOrValue){STOP; return -1;}
float    gfDataNode_GetValueF32Or (DataNodeStruct* iNode, wchar_t* iPath, float    iOrValue){STOP; return -1.0f;}
double   gfDataNode_GetValueF64Or (DataNodeStruct* iNode, wchar_t* iPath, double   iOrValue){STOP; return -1.0;}

int      gfDataNode_ToString      (DataNodeStruct* iNode, WString* iString, int iIndent)
{
	//if(iBufferCapacity == 0) iBufferCapacity = 64;
	//if(iBufferSize >= iBufferCapacity - 1)
	//{
	//	iBufferCapacity = 64;
	//	
	//	gfReserveString(iBuffer, iBufferCapacity * 2);
	//}
	int _NameLenZ;
	wchar_t _Tab         [] = L"\t";
	wchar_t _ExpDelim    [] = L";\r\n";
	wchar_t _BlockOpener [] = L"\r\n{\r\n";
	wchar_t _BlockCloser [] = L"}";
	
	
	int cIi = 0; for(; cIi < iIndent; cIi ++)
	{
		///gfWString_Write(iString, (wchar_t*)L"\t", sizeof(wchar_t));
		gfWString_Write(iString, (wchar_t*)&_Tab, sizeof(_Tab) / sizeof(wchar_t));
	}
	
	_NameLenZ = (int)wcslen((wchar_t*)&iNode->Name) + 1;
	
	if(_NameLenZ == 0) WTFE("Null-terminated string expected");
	if(_NameLenZ != 1)
	{
		gfWString_Write(iString, (wchar_t*)&iNode->Name, _NameLenZ);///_NameLenZ);
	}
	
	if(iNode->Value.Type != DN_VT_Null)
	{
		wchar_t _Buffer[256]; int _CharCount;
		if(iNode->ChildCount != 0) WTFE("Not expected");

		///_CharCount = _vsnwprintf_s((wchar_t*)&_Buffer, sizeof(_Buffer) / sizeof(_Buffer[0]), _TRUNCATE, iStr, _ArgPtr);
		
		switch(iNode->Value.Type)
		{
			case DN_VT_Int32   : _CharCount = swprintf_s((wchar_t*)&_Buffer, sizeof(_Buffer) / sizeof(_Buffer[0]), L"%d", *(int    *)iNode->Value.Data); break;
			case DN_VT_Float32 : _CharCount = swprintf_s((wchar_t*)&_Buffer, sizeof(_Buffer) / sizeof(_Buffer[0]), L"%f", *(float  *)iNode->Value.Data); break;
			case DN_VT_Float64 : _CharCount = swprintf_s((wchar_t*)&_Buffer, sizeof(_Buffer) / sizeof(_Buffer[0]), L"%f", *(double *)iNode->Value.Data); break;
			
			default : STOP;
		}
		
		gfWString_Write(iString, L" ", -1);
		gfWString_Write(iString, (wchar_t*)&_Buffer, _CharCount + 1);
		
		HERE;
	}
	if(iNode->ChildCount != 0)
	{
		int cCi; 
		///gfWString_Write(iString, (wchar_t*)L"{", sizeof(wchar_t));
		gfWString_Write(iString, (wchar_t*)&_BlockOpener, sizeof(_BlockOpener) / sizeof(wchar_t));
		
		for(cCi = 0; cCi < iNode->ChildCount; cCi ++)
		{
			DataNodeStruct* cChild = &iNode->Children[cCi];
			
			//int cIi = 0; for(; cIi < iIndent; cIi ++)
			//{
			//	///gfWString_Write(iString, (wchar_t*)L"\t", sizeof(wchar_t));
			//	gfWString_Write(iString, (wchar_t*)&_Tab, sizeof(_Tab) / sizeof(wchar_t));
			//}
			
			gfDataNode_ToString(cChild, iString, iIndent + 1);
			
			gfWString_Write(iString, (wchar_t*)&_ExpDelim, sizeof(_ExpDelim) / sizeof(wchar_t));
		}
		
		gfWString_Write(iString, (wchar_t*)&_BlockCloser, sizeof(_BlockCloser) / sizeof(wchar_t));
	}
	
	
	return 0;
}

void     gfDataNode_InitStatic    ()
{
	///memset(&DataNode::Null,0,sizeof(DataNode));
	
	
}
	
