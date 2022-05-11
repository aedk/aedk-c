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
#define WTFE(idMsg) STOP


DataNodeStruct gDataNodeStruct_Null;

struct $__DataNodeCollection__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};

void                 gfDNChildren_Init     (DataNodeStruct* irParentNode)
{
	assert(irParentNode->Children == nullptr);
	
	
	gfDNChildren_Reserve(irParentNode, 1);
	///irNode->Children = gfDNChildren_Create(1);
	///irNode->Children->Owner = irNode;
}

void                 gfDNChildren_Reserve  (DataNodeStruct* irParentNode, int iCapacity);
//DataNodeCollection*  gfDNChildren_Create   (DataNodeStruct* irParentNode, int iCapacity)
//{
//	DataNodeCollection* oCollection = malloc(sizeof(DataNodeCollection));
//	{
//		///oCollection->Owner    = nullptr; ///~~ commented for C-'template' compatibility: share the same code with DLItemCollection, DLItemTypeCollection etc;
//		oCollection->Items    = nullptr;
//		oCollection->Count    = 0;
//		oCollection->Capacity = 0;
//		
//		gfDNChildren_Reserve(oCollection,iCapacity);
//	}
//	return oCollection;
//}
//void                 gfDNChildren_Destroy  (DataNodeStruct* irParentNode)
//{
//	if(irCollection->Items != nullptr) free(irCollection->Items);
//	
//	free(irCollection);
//}
DataNodeStruct*      gfDNChildren_Allocate (DataNodeStruct* irParentNode, int iCount)
{
	gfDNChildren_Reserve(irParentNode, irParentNode->ChildCount + iCount);
	
	irParentNode->ChildCount += iCount;
	
	return &irParentNode->Children[irParentNode->ChildCount - 1];
}

///void               gfDNChildren_Push     (DataNodeStruct* irParentNode, DataNodeCollection* iItem)
//{
//	DataNodeCollection* _Item = gfDNChildren_Allocate(irCollection,1);
//	
//	irCollection->Items[irCollection->Count - 1] = *iItem;
//}
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
	irNode->Name [0] = L'\0';
	irNode->Value[0] = L'\0';
	
	irNode->Type      = iNodeType;
	irNode->ValueType = iValueType;
	
	irNode->Parent    = nullptr;
	
	irNode->Children      = nullptr;
	irNode->ChildCount    = 0;
	irNode->ChildCapacity = 0;
}
void gfDataNode_AppendMember(DataNodeStruct* irNode, DLSyntaxNode* iSyntaxNode, DLContext* iDLC)
{
	
}

void gfDataNode_SetNodeValue(DataNodeStruct* irNode, DLSyntaxNode* iSynNode, DLContext* iDLC)
{
	DLToken* _Token = &iDLC->Lexer->Tokens->Items[cSynNode->BegToken];
	
	wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
	size_t _TokenLen = _Token->Length;// + 40;
	DataNodeValueType _ValueType;
	
	assert(_TokenLen >= 1);
		
	_ValueType; switch(cSynNode->Type)
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
			
			_Token = &iDLC->Lexer->Tokens->Items[cSynNode->BegToken];
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

void gfDataNode_FromDLSyntaxNode(DataNodeStruct* irNode, DLSyntaxNode* iSyntaxNode, DLContext* iDLC)
{
	if(iSyntaxNode == nullptr) WTFE("Non-null node expected");
	///if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) WTFE("Empty node specified");
	if(iSyntaxNode->Children == nullptr || iSyntaxNode->Children->Count == 0) return;
	
	
	///if(iSyntaxNode->Type != DL_SYN_Expression) WTFE("Expression syntax node is expected");
	///if(iSyntaxNode->Type != DL_SYN_FunctionBlock) WTFE("Expected function block as node type");
	
	if(iSyntaxNode->Type == DL_SYN_FunctionBlock || iSyntaxNode->Type == DL_SYN_File)
	{
		///~~ 'this' can be NOT initialized!;
		int _EEc, cEi;
	
		///~~ Document?;
		if(irNode->Children == nullptr) gfDNChildren_Init(irNode);
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
			cChildNode = gfDNChildren_Allocate(irNode, 1);
			{
				gfDataNode_Init_2(cChildNode, DN_T_Element);
				cChildNode->Parent = irNode;
				
				gfDataNode_FromDLSyntaxNode(cChildNode, cSynNode, iDLC);
				
				
				///*cChildNode = DataNode(&cSynNode);
			}
		}
	
	}
	else if(iSyntaxNode->Type == DL_SYN_Expression)
	{
		#ifdef PEOFEPFOK
		
			ChB;
			ChB "ChB_1";
			
			ChB (attr1 1; attr2 2.0; attr3 "V3";);
			ChB (attr1 1; attr2 2.0; attr3 "V3";) {ChC;};
			
			ChB "ChB_1" (attr1 1; attr2 2.0; attr3 "V3";);
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
				//DLToken* _Token = &iDLC->Lexer->Tokens->Items[cSynNode->BegToken];
				//
				//wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
				//size_t _TokenLen = _Token->Length;// + 40;
				//DataNodeValueType _ValueType;
				//
				//assert(_TokenLen >= 1);
				//	
				//_ValueType; switch(cSynNode->Type)
				//{
				//	case DL_SYN_Number     : _ValueType = DN_VT_Float64;    break; ///~~ default;
				//	case DL_SYN_NumInvalid : _ValueType = DN_VT_NumInvalid; break;
				//	case DL_SYN_NumInt32   : _ValueType = DN_VT_Int32;      break;
				//	case DL_SYN_NumFloat32 : _ValueType = DN_VT_Float32;    break;
				//	case DL_SYN_NumFloat64 : _ValueType = DN_VT_Float64;    break;
				//	
				//	case DL_SYN_String     :
				//	{
				//		size_t _TokenLenWOQ, _TokenLenWOQZ; bool _IsNeededHeapAlloc;
				//		wchar_t* _TokenStrWOQ;
				//		wchar_t* _StrPtr;
				//		
				//		assert(_Token->Length >= 2); ///~~ quotes included;
				//	
				//		_TokenLen = _Token->Length;// + 40;
				//		_TokenLenWOQ  = _TokenLen - 2; ///~~ without quotes;
				//		_TokenLenWOQZ = _TokenLenWOQ + 1;  ///~~ without quotes but with a null terminator;
				//		_IsNeededHeapAlloc = _TokenLenWOQZ >= dDataNodeValueMaxLength;
				//		
				//		_Token = &iDLC->Lexer->Tokens->Items[cSynNode->BegToken];
				//		_TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
				//		_TokenStrWOQ = _TokenStr + 1; ///~~ skip opening quote;
				//		
				//		
				//		
				//		if(_IsNeededHeapAlloc)
				//		{
				//			_StrPtr = malloc(_TokenLenWOQZ * sizeof(wchar_t));
				//			
				//			memcpy(&irNode->Value, &_StrPtr, sizeof(void*));
				//			
				//			_ValueType = DN_VT_StringPtr;
				//		}
				//		else                   {_StrPtr = (wchar_t*)&irNode->Value;                            _ValueType = DN_VT_String;}
				//		
				//		wcsncpy_s(_StrPtr, _TokenLenWOQZ, _TokenStrWOQ, _TokenLenWOQ);
				//		
				//		//(*(wchar_t*)_StrPtr)[]
				//		
				//		//this->Value[_TokenLen] = '\0';
				//		
				//		break;
				//	}
				//	
				//	default : STOP;
				//}
				//
				//irNode->ValueType = _ValueType;
			}
			else switch(cSynNode->Type)
			{
				case DL_SYN_GroupingBlock:
				{
					if(cSynNode->Children != nullptr)
					{
						int cEi,_EEc = cSynNode->Children->Count; for(cEi = 0; cEi < _EEc; cEi ++)
						{
							DLSyntaxNode *cExpNode,*cNameNode,*cValueNode;
							///wchar_t* _1;
							
							cExpNode = &cSynNode->Children->Items[cEi];
							
							assert(cExpNode->Children->Count == 2);
							
							///~~ List;
							cNameNode  = &cExpNode->Children->Items[0]; assert(cNameNode ->Children->Count == 1 && cNameNode ->Children->Items[0].Children->Count == 1);
							cValueNode = &cExpNode->Children->Items[1]; assert(cValueNode->Children->Count == 1 && cValueNode->Children->Items[0].Children->Count == 1);
							
							///~~ List;
							cNameNode  = &cNameNode->Children->Items[0].Children->Items[0];//  assert(cNameNode ->Children->Count == 1);
							cValueNode = &cValueNode->Children->Items[0].Children->Items[0];// assert(cValueNode->Children->Count == 1);
							
							///wchar_t cPath = L"@";
							///if(cNameNode->Token_->Length
							
							///wchar_t* _1 = &cValueNode->Token_->Value;
							
							switch(cValueNode->Type)
							{
								//case DL_SYN_Number     : break;
								//case DL_SYN_NumInvalid : break;
								///case DL_SYN_NumInt32   : gfDataNode_SetValueI32(irNode, &cNameNode->Token_->Value, *(int*)   &cValueNode->Token_->Value); break;
								///case DL_SYN_NumFloat32 : gfDataNode_SetValueF32(irNode, &cNameNode->Token_->Value, *(float*) &cValueNode->Token_->Value); break;
								case DL_SYN_Number : gfDataNode_SetValueF32(irNode, &cNameNode->Token_->Value, *((float*) &cValueNode->Token_->Value)); break;
								//case DL_SYN_NumFloat64 : break;
								
								case DL_SYN_String : gfDataNode_SetValueWSZ(irNode, &cNameNode->Token_->Value, (wchar_t*)&cValueNode->Token_->Value); break;
								default : STOP;
							}
							
							
							///cNameNode->Children->Items[0].Children->Items[0]
							//irNode. 
							HERE;
						}
						///DLSyntaxNode* cGroupSynNode;
						
						///dGetNode(cSynNode, 0, cGroupSynNode);
						
						HERE;
					}
					HERE;
					break;
				}
				case DL_SYN_FunctionBlock:
				{
					HERE;
					
					
///					gfDataNode_FromDLSyntaxNode(irNode,cSynNode,iDLC);
					
					break;
				}
			}
			HERE;
		}
		
		
		
		
		//if(_ChildCount >= 1)
		//{
		//	DLToken* _Token; wchar_t* _TokenStr; size_t _TokenLen;
		//	
		//	DLSyntaxNode* _Ident; dGetNode(0, _Ident);
		//	
		//	_Token    = &iDLC->Lexer->Tokens->Items[_Ident->BegToken];
		//	_TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
		//	_TokenLen = _Token->Length;// + 40;
		//	
		//	
		//	if(!(_Ident->Type > DL_SYN_Identifiers__Begin && _Ident->Type < DL_SYN_Identifiers__End)) WTFE("?");
		//	
		//	assert(_TokenLen >= 1);
		//	
		//	
		//	if(_TokenLen < dDataNodeNameMaxLength)
		//	{
		//		//memset(&this->Name,1,sizeof(this->Name));
		//		//wcscpy_s((wchar_t*)&this->Name, _TokenLen, _TokenStr); doesn't work;
		//		
		//		//memset(&this->Name,0,sizeof(this->Name));
		//		
		//		///gfDataNode_SetName(&irNode->Name, _TokenStr);
		//		memcpy(&irNode->Name, _TokenStr, _TokenLen * sizeof(wchar_t));
		//		irNode->Name[_TokenLen] = '\0';
		//		HERE;
		//	}
		//	else
		//	{
		//		///~~ pointer;
		//		STOP;
		//	}
		//}
		//
		//if(_ChildCount == 1)
		//{
		//	HERE;
		//}
		//else if(_ChildCount == 2)
		//{
		//	///~~ name or body;
		//	DLSyntaxNode* _Node; dGetNode(1, _Node);
		//	{
		//		if(_Node->Type > DL_SYN_Value__Begin && _Node->Type < DL_SYN_Value__End)
		//		{
		//			///void* _Value; bool _DoFree = gfDLCtx_GetSyntaxNodeValue(_Node, &_Value);
		//			
		//			DLToken* _Token = &iDLC->Lexer->Tokens->Items[_Node->BegToken];
		//			
		//			wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
		//			size_t _TokenLen = _Token->Length;// + 40;
		//			DataNodeValueType _ValueType; 
		//			
		//			assert(_TokenLen >= 1);
		//			
		//			_ValueType; switch(_Node->Type)
		//			{
		//				case DL_SYN_Number     : _ValueType = DN_VT_Float64;    break; ///~~ default;
		//				case DL_SYN_NumInvalid : _ValueType = DN_VT_NumInvalid; break;
		//				case DL_SYN_NumInt32   : _ValueType = DN_VT_Int32;      break;
		//				case DL_SYN_NumFloat32 : _ValueType = DN_VT_Float32;    break;
		//				case DL_SYN_NumFloat64 : _ValueType = DN_VT_Float64;    break;
		//				
		//				case DL_SYN_String     :
		//				{
		//					size_t _TokenLenWOQ, _TokenLenWOQZ; bool _IsNeededHeapAlloc;
		//					wchar_t* _TokenStrWOQ;
		//					wchar_t* _StrPtr;
		//					
		//					assert(_Token->Length >= 2); ///~~ quotes included;
		//				
		//					_TokenLen = _Token->Length;// + 40;
		//					_TokenLenWOQ  = _TokenLen - 2; ///~~ without quotes;
		//					_TokenLenWOQZ = _TokenLenWOQ + 1;  ///~~ without quotes but with a null terminator;
		//					_IsNeededHeapAlloc = _TokenLenWOQZ >= dDataNodeValueMaxLength;
		//					
		//					_Token = &iDLC->Lexer->Tokens->Items[_Node->BegToken];
		//					_TokenStr = &iDLC->Lexer->Buffer[_Token->Offset];
		//					_TokenStrWOQ = _TokenStr + 1; ///~~ skip opening quote;
		//					
		//					
		//					
		//					if(_IsNeededHeapAlloc)
		//					{
		//						_StrPtr = malloc(_TokenLenWOQZ * sizeof(wchar_t));
		//						
		//						memcpy(&irNode->Value, &_StrPtr, sizeof(void*));
		//						
		//						_ValueType = DN_VT_StringPtr;
		//					}
		//					else                   {_StrPtr = (wchar_t*)&irNode->Value;                            _ValueType = DN_VT_String;}
		//					
		//					wcsncpy_s(_StrPtr, _TokenLenWOQZ, _TokenStrWOQ, _TokenLenWOQ);
		//					
		//					//(*(wchar_t*)_StrPtr)[]
		//					
		//					//this->Value[_TokenLen] = '\0';
		//					
		//						
		//					
		//					break;
		//				}
		//				
		//				default : STOP;
		//			}
		//			
		//			irNode->ValueType = _ValueType;
		//			//					
		//			//if(_TokenLen < dDataNodeValueMaxLength)
		//			//{
		//			//	//memset(&this->Name,1,sizeof(this->Name));
		//			//	///wcscpy_s((wchar_t*)&this->Name, _TokenLen, _TokenStr); doesn't work;
		//			//	
		//			//	//memset(&this->Name,0,sizeof(this->Name));
		//			//	///#define 
		//			//	
		//			//	
		//			//	
		//			//	
		//			//	///this->ValueType = 
		//			//	
		//			//	///this->ValueType
		//			//	HERE;
		//			//	
		//			//}
		//			//else
		//			//{
		//			//	///~~ pointer;
		//			//	//if(_Node->Type != DL_SYN_String) WTFE("String expected");
		//			//	//
		//			//	//DLToken& _Token = iDLC->Lexer->Tokens->Items[_Node->BegToken];
		//			//	//wchar_t* _TokenStr = &iDLC->Lexer->Buffer[_Token.Offset];
		//			//	//size_t _TokenLen = _Token.Length;// + 40;
		//			//	//assert(_TokenLen >= 2); ///~~ quotes included;
		//			//	//size_t _TokenLenWithoutQuotes = _TokenLen - 2;
		//			//	//void* _StrPtr = malloc((_TokenLenWithoutQuotes + 1) * sizeof(wchar_t)); ///~~ null-terminator included;
		//			//	//
		//			//	//memcpy(&_StrPtr, _TokenStr + 1, (_TokenLenWithoutQuotesButWithNullTerm - 1) * sizeof(wchar_t)); ///~~ skip opener quote, stop before closer;
		//			//	//(*(wchar_t*)_StrPtr)[]
		//			//	//
		//			//	//this->Value = _StrPtr;
		//			//	//this->ValueType = DN_VT_StringPtr;
		//			//	HERE;
		//			//}
		//			//
		//		
		//			///if(_DoFree) free(_Value);
		//			
		//			HERE;
		//		}
		//		else if(_Node->Type == DL_SYN_FunctionBlock)
		//		{
		//			gfDataNode_FromDLSyntaxNode(irNode,_Node,iDLC);
		//			
		//			HERE;
		//		}
		//		else WTFE("?");
		//	}
		//	
		//	
		//	HERE;
		//}
		//else
		//{
		//	HERE;
		//}
		
		
		
		
		
		
		
		/*
		for(int _EEc = iSyntaxNode->Children->Count, cEi = 0; cEi < _EEc; cEi ++)
		{
			DLSyntaxNode& cSynNode = iSyntaxNode->Children->Items[cEi]; if(cSynNode.Type != DL_SYN_List) continue;
			
			if(this->Children == nullptr) this->InitChildren();
			
			///DataNode* cChildNode = this->Children->Allocate(1);
			DataNode* cChildNode = gfDNChildren_Allocate(this->Children, 1);
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
	int _Len = wcslen(iValue);
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
void gfDataNode_SetValueI32 (DataNodeStruct* iNode, wchar_t* iPath, int      iValue)
{
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(iNode, iPath, true, true);
	
	*((int*)&_TgtNode->Value) = iValue;
	
	_TgtNode->ValueType = DN_VT_Int32;
	
	HERE;
}
void gfDataNode_SetValueF32 (DataNodeStruct* iNode, wchar_t* iPath, float    iValue)
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