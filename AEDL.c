#include "AEDL.h"

#include <wchar.h>
///#include "AE.h"
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
//#include <locale>


///#include <malloc.h>
//#include <memory.h>
//#include <stdio.h>


static int gf_BreakpointFunction(int iNum){return iNum;}
#define HERE  gf_BreakpointFunction(0)
#define STOP  gf_BreakpointFunction(1 / gf_BreakpointFunction(0))
#define WTFE(idMsg) STOP

///size_t _msize(void* iPtr);


struct $__DL_Context__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};



DLContext* gfDLCtx_Create           (wchar_t* iBuffer, size_t iBufferLength, bool iIsComplete)
{
	DLContext* oDLC = malloc(sizeof(DLContext));
	{
		oDLC->Lexer = gfLexCtx_Create(oDLC, iBuffer, iBufferLength);
		oDLC->Parser = gfParser_Create(oDLC);
	}
	return oDLC;
}
void       gfDLCtx_Destroy          (DLContext* irDLC)
{
	if(irDLC->Lexer  != nullptr) gfLexCtx_Destroy(irDLC->Lexer);
	if(irDLC->Parser != nullptr) gfParser_Destroy(irDLC->Parser);
	
	free(irDLC);
}


struct $__DL_Token__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};



int wcsindexof(wchar_t* dataset, int datasetLength, wchar_t* target, int targetLen)
{
	int i,j;

	for(i = 0; i < datasetLength; i ++)
	{
		if(dataset[i] == target[0])
		{
			int found = 1;
			for(j = 0; j < targetLen; j ++)
			{
				int k = i + j;
				if(k >= datasetLength || target[j] != dataset[k])
				{
					found = 0;
					break;
				}
			}
			if(found) return i;
		}
	}
	return -1;
}
///int memsearch(char* dataset, int datasetLength, char* target, int targetLen)
//{
//	for(int i = 0; i < datasetLength; i++)
//	{
//		if(dataset[i] == target[0])
//		{
//			int found = 1;
//			for(int j = 0; j < targetLen; j++)
//			{
//				int k = i + j;
//				if(k >= datasetLength || target[j] != dataset[k])
//				{
//					found = 0;
//					break;
//				}
//			}
//			if(found) return i;
//		}
//	}
//	return -1;
//}

void gfToken_Init          (DLToken* iToken)
{
	iToken->Id       = -1;
	iToken->Offset   = -1;
	iToken->Length   = -1;
	iToken->Fragment = -1;
	
	iToken->Type     = DL_TOK_Undefined;
	iToken->Value[0] = L'\0';
	///iToken->Pair     = nullptr;
	iToken->PairId   = -1;
}
void gfToken_Cleanup       (DLToken* iToken)
{
	if(iToken->Type == DL_TOK_String)
	{
		///if(iToken->Value[0] != 0)
		{
			///free()
		}
	}
}

bool gfToken_IsAligned     (DLToken* iToken){return iToken->Offset != -1;}
bool gfToken_IsZeroLength  (DLToken* iToken){return iToken->Length == 0;}
bool gfToken_IsTerminated  (DLToken* iToken){return iToken->Length != -1;}
bool gfToken_IsPaired      (DLTokenType iTokenType){return gfToken_IsOpener(iTokenType) || gfToken_IsCloser(iTokenType);}

//bool gfToken_IsOpener      (DLTokenType iTokenType){return iTokenType == DL_TOK_Expression_Opener || iTokenType == DL_TOK_List_Opener || iTokenType == DL_TOK_ListItem_Opener || iTokenType == DL_TOK_Parenthesis_Opener || iTokenType == DL_TOK_Bracket_Opener || iTokenType == DL_TOK_Brace_Opener;}
//bool gfToken_IsCloser      (DLTokenType iTokenType){return iTokenType == DL_TOK_Expression_Closer || iTokenType == DL_TOK_List_Closer || iTokenType == DL_TOK_ListItem_Closer || iTokenType == DL_TOK_Parenthesis_Closer || iTokenType == DL_TOK_Bracket_Closer || iTokenType == DL_TOK_Brace_Closer;}

bool gfToken_IsOpener      (DLTokenType iTokenType){return iTokenType == DL_TOK_ListItem_Opener || iTokenType == DL_TOK_List_Opener || iTokenType == DL_TOK_Expression_Opener || iTokenType == DL_TOK_Parenthesis_Opener || iTokenType == DL_TOK_Bracket_Opener || iTokenType == DL_TOK_Brace_Opener || iTokenType == DL_TOK_File_Opener;}
bool gfToken_IsCloser      (DLTokenType iTokenType){return iTokenType == DL_TOK_ListItem_Closer || iTokenType == DL_TOK_List_Closer || iTokenType == DL_TOK_Expression_Closer || iTokenType == DL_TOK_Parenthesis_Closer || iTokenType == DL_TOK_Bracket_Closer || iTokenType == DL_TOK_Brace_Closer || iTokenType == DL_TOK_File_Closer;}

bool gfToken_IsWhitespace  (DLTokenType iTokenType){return iTokenType >= DL_TOK_Whitespace && iTokenType <= DL_TOK_NewLine;}

bool gfToken_IsGarbage     (DLTokenType iTokenType){return iTokenType == DL_TOK_Garbage || iTokenType == DL_TOK_Comment;}
bool gfToken_IsGarbageSTX  (DLTokenType iTokenType){return iTokenType >= DL_TOK_Whitespace && iTokenType <= DL_TOK_Comment;}



struct $__DL_TokenTypeList__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
DLTokenTypeList*  gfTypeList_Create   (int iCapacity)
{
	DLTokenTypeList* oList = malloc(sizeof(DLTokenTypeList));
	{
		oList->Items    = nullptr;
		oList->Count    = 0;
		oList->Capacity = 0;
		
		gfTypeList_Reserve(oList,iCapacity);
	}
	return oList;
}
void              gfTypeList_Destroy  (DLTokenTypeList* irList)
{
	if(irList->Items != nullptr) free(irList->Items);
	
	free(irList);
}
DLTokenType*      gfTypeList_Allocate (DLTokenTypeList* irList, int iCount)
{
	gfTypeList_Reserve(irList, irList->Count + iCount);
	
	irList->Count += iCount;
	
	return &irList->Items[irList->Count - 1];
}


void              gfTypeList_PushValue    (DLTokenTypeList* irList, DLTokenType iItem)
{
	DLTokenType* _Item = gfTypeList_Allocate(irList,1);
	
	*_Item = iItem;
	///irList->Items[irList->Count - 1] = iItem;
}
DLTokenType       gfTypeList_PopValue      (DLTokenTypeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return irList->Items[-- irList->Count];
}
DLTokenType*      gfTypeList_Pop      (DLTokenTypeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[-- irList->Count];
}
DLTokenType       gfTypeList_PeekValue (DLTokenTypeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return irList->Items[irList->Count - 1];
}
DLTokenType*      gfTypeList_Peek     (DLTokenTypeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[irList->Count - 1];
}

void              gfTypeList_Reserve  (DLTokenTypeList* irList, int iCapacity)
{
	if(iCapacity <= irList->Capacity) return;
	
	
	{
		size_t _OldSize, _NewSize;
		DLTokenType *_ItemsOldPtr = irList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dDLTokenTypeListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(DLTokenType); //, _Size;
		{
			
			
			///irList->Capacity
			/*if(_NeedSize < irList->Capacity * 2)
			{
				
			}*/
		}
		
		///_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		
		_ItemsNewPtr = irList->Items == nullptr ? malloc(_NeedSize) : realloc(irList->Items, _NeedSize);
		
		///_NewSize = _msize(_ItemsNewPtr);
		
		
		
		assert(_ItemsNewPtr != nullptr);
		
		
		irList->Items    = _ItemsNewPtr;
		irList->Capacity = _NeedCapacity;
	}
}
void              gfTypeList_Clear    (DLTokenTypeList* irList)
{
	irList->Count = 0;
}



struct $__DL_TokenList__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
DLTokenList*      gfTList_Create   (int iCapacity)
{
	DLTokenList* oTokenList = malloc(sizeof(DLTokenList));
	{
		oTokenList->Items    = nullptr;
		oTokenList->Count    = 0;
		oTokenList->Capacity = 0;
		
		gfTList_Reserve(oTokenList,iCapacity);
	}
	return oTokenList;
}
void              gfTList_Destroy  (DLTokenList* irList)
{
	if(irList->Items != nullptr) free(irList->Items);
	
	free(irList);
}
DLToken*          gfTList_Allocate (DLTokenList* irList, int iCount)
{
	gfTList_Reserve(irList, irList->Count + iCount);
	
	irList->Count += iCount;
	
	return &irList->Items[irList->Count - 1];
}

void              gfTList_Push     (DLTokenList* irList, DLToken* iItem)
{
	DLToken* _Token = gfTList_Allocate(irList,1);
	
	irList->Items[irList->Count - 1] = *iItem;
	//if(irList->Count + 1 >= irList->Capacity)
	//{
	//	gfTList_Reserve(irList, irList->Capacity == 0 ? 1 : (int)(irList->Capacity * dDLTokenListCapacityMultiplier));
	//}
	//irList->Items[irList->Count ++] = *iToken;
}
DLToken*          gfTList_Pop      (DLTokenList* irList)
{
	DLToken* oToken;

	if(irList->Count <= 0) return nullptr;
	
	
	return &irList->Items[-- irList->Count];
}
DLToken*          gfTList_Peek     (DLTokenList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[irList->Count - 1];
}

void              gfTList_Reserve  (DLTokenList* irList, int iCapacity)
{
	if(iCapacity <= irList->Capacity) return;
	
	
	{
		size_t _OldSize, _NewSize;
		DLToken *_ItemsOldPtr = irList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dDLTokenListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(DLToken); //, _Size;
		{
			
			
			///irList->Capacity
			/*if(_NeedSize < irList->Capacity * 2)
			{
				
			}*/
		}
		
		///_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		
		_ItemsNewPtr = irList->Items == nullptr ? malloc(_NeedSize) : realloc(irList->Items, _NeedSize);
		
		///_NewSize = _msize(_ItemsNewPtr);
		
		
		
		assert(_ItemsNewPtr != nullptr);
		
		
		irList->Items    = _ItemsNewPtr;
		irList->Capacity = _NeedCapacity;
	
		
	}
	///irList->Items = malloc(iCapacity);
	
	//if(_ItemsOldPtr != nullptr)
	//{
	//	memcpy(irList->Items, _ItemsOldPtr, irList->Count);
	//	free(_ItemsOldPtr);
	//}
}
void              gfTList_Clear    (DLTokenList* irList)
{
	irList->Count = 0;
}


struct $__DL_SyntaxNodeList__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
///DLSyntaxNodeList*  gfSNList_Create   (DLSyntaxNode* iOwner, int iCapacity)
DLSyntaxNodeList* gfSNList_Create   (int iCapacity)
{
	DLSyntaxNodeList* oList = malloc(sizeof(DLSyntaxNodeList));
	{
		///oList->Owner    = nullptr; ///~~ commented for C-'template' compatibility: share the same code with DLTokenList, DLTokenTypeList etc;
		oList->Items    = nullptr;
		oList->Count    = 0;
		oList->Capacity = 0;
		
		gfSNList_Reserve(oList,iCapacity);
	}
	return oList;
}
void              gfSNList_Destroy  (DLSyntaxNodeList* irList)
{
	if(irList->Items != nullptr) free(irList->Items);
	
	free(irList);
}
DLSyntaxNode*     gfSNList_Allocate (DLSyntaxNodeList* irList, int iCount)
{
	gfSNList_Reserve(irList, irList->Count + iCount);
	
	/*if(iCount != 1)
	{
		HERE;
	}*/
	irList->Count += iCount;
	
	return &irList->Items[irList->Count - 1];
}

///void               gfSNList_Push     (DLSyntaxNodeList* irList, DLSyntaxNode* iItem)
//{
//	DLSyntaxNode* _Token = gfSNList_Allocate(irList,1);
//	
//	irList->Items[irList->Count - 1] = *iItem;
//}
DLSyntaxNode*     gfSNList_Pop      (DLSyntaxNodeList* irList)
{
	DLSyntaxNode* oToken;

	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[-- irList->Count];
}
DLSyntaxNode*     gfSNList_Peek     (DLSyntaxNodeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[irList->Count - 1];
}

void              gfSNList_Reserve  (DLSyntaxNodeList* irList, int iCapacity)
{
	if(iCapacity <= irList->Capacity) return;
	
	{
		size_t _OldSize, _NewSize;
		DLSyntaxNode *_ItemsOldPtr = irList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dDLSyntaxNodeListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(DLSyntaxNode); //, _Size;
		
		///_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		
		_ItemsNewPtr = irList->Items == nullptr ? malloc(_NeedSize) : realloc(irList->Items, _NeedSize);
		
		///_NewSize = _msize(_ItemsNewPtr);
			
		assert(_ItemsNewPtr != nullptr);
		
		
		irList->Items    = _ItemsNewPtr;
		irList->Capacity = _NeedCapacity;
		
		
		
		if(true)
		{
			int _PPc = irList->Count, cPi = 0; for(; cPi < _PPc; cPi ++)
			{
				DLSyntaxNode* cParentNode = &irList->Items[cPi];
				int _CCc, cCi; 
				
				cParentNode->Children->Owner = cParentNode;
				
				for(_CCc = cParentNode->Children->Count, cCi = 0; cCi < _CCc; cCi ++)
				{
					DLSyntaxNode* cChildNode = &cParentNode->Children->Items[cCi];
					cChildNode->Parent = cParentNode;
				}
			}
		}
	}
}
void              gfSNList_Clear    (DLSyntaxNodeList* irList)
{
	irList->Count = 0;
}


struct $__DL_LexerState__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
LexerState*   gfLState_Create  ()
{
	LexerState* oState = malloc(sizeof(LexerState));
	{
		oState->Position   = 0;
		oState->TypeStack = gfTypeList_Create(0);
	}
	return oState;
}
void          gfLState_Destroy (LexerState* iState)
{
	gfTypeList_Destroy(iState->TypeStack);
	free(iState);
}

struct $__DL_LexerContext__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
LexerContext* gfLexCtx_Create           (DLContext* iDLC, wchar_t* iBuffer, size_t iBufferLength)
{
	LexerContext* oCtx = malloc(sizeof(LexerContext));
	{
		///memset(oCtx,0,sizeof(LexerContext));
		oCtx->DLC          = iDLC;
		oCtx->Buffer       = iBuffer;
		oCtx->BufferLength = iBufferLength;
		oCtx->State        = gfLState_Create();
		oCtx->Tokens       = nullptr;
		
		///if(oCtx->Buffer[0] == 0xfeff) oCtx->State->Position ++; ///~~ UTF-8 BOM;
	}
	return oCtx;
}
void          gfLexCtx_Destroy          (LexerContext* iCtx)
{
	gfLState_Destroy(iCtx->State);
	//gfTList_Destroy(iCtx->TokenStack);
	free(iCtx);
}


DLToken*      gfLexCtx_AllocateToken    (LexerContext* iCtx)
{
	DLToken* _Tokens = gfTList_Allocate(iCtx->Tokens, 1);
	{
		///_Tokens[0]->Offset
	}
	return _Tokens;//[0];
}
DLToken*      gfLexCtx_AllocateTokens   (LexerContext* iCtx, int iCount)
{
	DLToken* oTokens = gfTList_Allocate(iCtx->Tokens, iCount);
	return oTokens;
}
///void          gfLexCtx_AddToken         (LexerContext* iCtx, DLTokenType iType, void* iValue, int iLength)


void          gfLexCtx_AddToken         (LexerContext* iCtx, DLTokenType iTokenType, void* iValue, int iBegOffs, int iEndOffsOrLength)
{
	bool _IsBegOffsSpecified = iBegOffs != -1;
	///int _TokenId = iCtx->Tokens->Count; ///~~ needs to be initialized after syntax tokens have been added (or do not use at all);
	DLToken* _Token;
	
	///int _TokenLength = _IsBegOffsSpecified ? iEndOffsOrLength - iBegOffs : iEndOffsOrLength;
	
	if(iTokenType < DL_TOK_Special_Syntax_Tokens__Begin || iTokenType > DL_TOK_Special_Syntax_Tokens__End)
	{
		gfLexCtx_AddSyntaxTokens(iCtx, iTokenType, iEndOffsOrLength != -1);
	}
	
	
	_Token = gfLexCtx_AllocateToken(iCtx);
	{
		gfToken_Init(_Token);
		
		_Token->Type     = iTokenType;
		///_Token->Value[0] = 0;///iValue;///~~ not used?;
		
		_Token->Id       = iCtx->Tokens->Count - 1;
		_Token->Offset   = _IsBegOffsSpecified ? iBegOffs : iCtx->State->Position;
		///_Token->Length   = _IsBegOffsSpecified ? (iEndOffsOrLength != -1 ? iEndOffsOrLength - iBegOffs : 0) : iEndOffsOrLength;
		_Token->Length   = iEndOffsOrLength != -1 ?  (_IsBegOffsSpecified ? iEndOffsOrLength - iBegOffs : iEndOffsOrLength) : 0;/// iEndOffsOrLength;
		///_Token->Length   = _TokenLength;
		
		
		///_Token->Fragment = -1;
		///_Token->Pair     = nullptr;
		///_Token->Pair     = -1;
		
		if(_Token->Length != 0)
		{
			size_t _SizeLimit = min(_Token->Length * sizeof(wchar_t), sizeof(_Token->Value) - sizeof(wchar_t));
			
			if(_Token->Length == 3)
			{
				HERE;
			}
			
			if(iValue != nullptr)
			{
				memcpy(&_Token->Value, iValue, _SizeLimit);
			}
			else
			{
				memcpy(&_Token->Value, &iCtx->Buffer[_Token->Offset], _SizeLimit);
			}
			_Token->Value[(_SizeLimit / sizeof(wchar_t))] = 0;///~~ SZ;
		}
	}
	/*if(_IsBegOffsSpecified)
	{
		if(iEndOffsOrLength != -1)
		{
			iCtx->State->Position = iEndOffsOrLength;
		}
		else
		{
			STOP;///~~ to check. Multiple tokens for single position;
		}
	}
	else
	{
		
	}*/
	//irList->Count
	if(_Token->Length != 0)
	{
		iCtx->State->Position = _Token->Offset + _Token->Length;
	}
}
void          gfLexCtx_AddTokens        (LexerContext* iCtx, DLTokenList* iNewTokens, DLTokenList* irTokens)
{
	STOP;
	///gfLexCtx_ProcessSyntax(iCtx, iNewTokens);
}
void          gfLexCtx_AddSyntaxTokens  (LexerContext* iCtx, DLTokenType iTokenType, bool iIsTerminated)///, DLTokenStack irStack)
{
	DLTokenTypeStack* _TypeStack = iCtx->State->TypeStack;

	if(_TypeStack->Count == 0)
	{
		gfTypeList_PushValue(_TypeStack, DL_TOK_File);

		gfLexCtx_AddToken(iCtx, DL_TOK_File_Opener, nullptr, -1, -1);
	}
	///DLTokenType iTokenType = iToken->Type;
	///bool _IsSingleToken = iNewTokens.Count == 1; if(!_IsSingleToken){}
	
	if      (gfToken_IsWhitespace(iTokenType)) return;
	else if (gfToken_IsGarbage(iTokenType))    return;
	else
	{
		bool _IsBlockOpener = iTokenType == DL_TOK_Parenthesis_Opener || iTokenType == DL_TOK_Bracket_Opener || iTokenType == DL_TOK_Brace_Opener;
		bool _IsBlockCloser = iTokenType == DL_TOK_Parenthesis_Closer || iTokenType == DL_TOK_Bracket_Closer || iTokenType == DL_TOK_Brace_Closer;
		//var _IsBlockOpener = iTokenType == DL_TOK_ParenthesisOpener;


		bool _IsLiteral = iTokenType >= DL_TOK_String && iTokenType <= DL_TOK_Number;
		//var _IsContinuation = 
		//var _IsBlockCloser = iTokenType == DL_TOK_BlockCloser;

		///var _IsWord        = iTokenType == DL_TOK_Word;
		

		bool _IsBlock          = _IsBlockOpener || _IsBlockCloser;
		bool _IsIdentifier     = (iTokenType > DL_TOK_Identifiers__Begin && iTokenType < DL_TOK_Identifiers__End);
		bool _IsListItem      = _IsLiteral || _IsIdentifier || _IsBlockOpener;/// /**/ || _IsWord; /**/
		bool _IsListItemDelim = iTokenType == DL_TOK_ListItem_Delimiter;
		bool _IsListItemContinuation = iTokenType == DL_TOK_Identifier_Delimiter || iTokenType == DL_TOK_Bracket_Opener;
		bool _IsList          = _IsListItem || _IsListItemContinuation || _IsListItemDelim;
		//var _IsListItemDelimiter
		//_
		
		bool _IsExpressionItem  = _IsList;/// || _IsWord;
		bool _IsExpressionDelimiter = iTokenType == DL_TOK_Expression_Delimiter;

		//var _IsExpression = _StackTop == DL_TOK_Expression;
		//var _IsList      = _StackTop == DL_TOK_List;
		//var _IsBlock      = _StackTop == DL_TOK_Block;
		//var _IsString     = _StackTop == DL_TOK_String;

		if(iTokenType == DL_TOK_Identifier_Delimiter)
		{
		
		}
		if(_IsExpressionDelimiter)
		{
		
		}
		//if(iTokenType == DL_TOK_ExpressionDelimiter)
		//{
		//    if(ioStack.Count > 0 && ioStack.Peek() == DL_TOK_ExpressionDelimiter)
		//    {
			
		//    }
		//}
		while(true)
		{
			
			/**
				Think of the following 'continue' and 'break' statements as they are used to control the 'while' loop only,
				but not the 'switch' (see the 'break' statement after the 'switch' in the end of 'while' block)
				So, breaking this loop also means 'return': no more additional tokens to add and no more changes to syntax stack to apply
			*/
			bool _IsStackEmpty = _TypeStack->Count == 0;
			DLTokenType _StackTop = _IsStackEmpty ? DL_TOK_Undefined : *gfTypeList_Peek(_TypeStack);
		
			//if(_IsWord){}
			if(_IsBlockCloser){}


			switch(_StackTop)
			{
				case DL_TOK_Undefined  :
				{
					LABEL_DL_TOK_UNDEFINED:
					
					if(_IsExpressionItem)
					{
						gfTypeList_PushValue(_TypeStack, DL_TOK_Expression);
						
						gfLexCtx_AddToken(iCtx, DL_TOK_Expression_Opener, nullptr, -1,-1);

						continue;
					}
					else break;
				}
				case DL_TOK_File     :
				{
					goto LABEL_DL_TOK_UNDEFINED;
					break;
				}
				case DL_TOK_Expression :
				{
					if(_IsExpressionItem)
					{
						if(_IsList)
						{
							if(_IsListItemContinuation || _IsListItemDelim)
							{
								gfLexCtx_AddToken(iCtx, DL_TOK_ListError, nullptr, -1, -1);
								
								break;
							}
							else
							{
								gfTypeList_PushValue(_TypeStack, DL_TOK_List);
								///ioStack.Push(DL_TOK_ExpectListItem);

								gfLexCtx_AddToken(iCtx, DL_TOK_List_Opener, nullptr, -1, -1);

								gfTypeList_PushValue(_TypeStack, DL_TOK_Expect_ListItem);
								//ioStack.Push(DL_TOK_ListItem);
								//ioTokens.Add(new TokenInfo(DL_TOK_ListItemOpener));

								continue;
							}
						}
						//else if(_IsWord)
						//{
						//    break;
						//}
						else WTFE("?");
						
					}
					///if        (_IsWord)      {/** ??? */}
					
					else/// if   (_IsExpressionDelimiter)
					{
						gfTypeList_Pop(_TypeStack);
						gfLexCtx_AddToken(iCtx, DL_TOK_Expression_Closer, nullptr, -1, -1);
					//}
					//else
					//{
						//ioStack.Pop();
						//ioTokens.Add(new TokenInfo(DL_TOK_ExpressionCloser));
						if(_IsBlockCloser)
						{
							continue;
						}
						else break;
					}
					///else             {ioStack.Pop(); goto End;}

					continue;
				}
				case DL_TOK_List      :
				{
					gfTypeList_Pop(_TypeStack);
					gfLexCtx_AddToken(iCtx, DL_TOK_List_Closer, nullptr, -1, -1);

					continue;


					//if(_IsListItem)
					//{
					//    ///ioTokens.Add(new TokenInfo(DL_TOK_ListOpener));
					//    ioStack.Push(DL_TOK_ExpectListItem);
						
					//    //continue;
					//}


					//else if(_IsListItemDelim)
					//{
					//    throw new Exception("???");
					//    //ioTokens.Add(new TokenInfo(DL_TOK_ListItemCloser));
					//    //ioStack.Push(DL_TOK_ExpectListItem);
					//}
					//else
					//{
					//    ioStack.Pop();
					//    ioTokens.Add(new TokenInfo(DL_TOK_ListCloser));
					//}

					/////goto End;

					//continue;
				}
				case DL_TOK_Expect_ListItem      :
				{
					gfTypeList_Pop(_TypeStack);

					if   (_IsListItem)
					{
						gfTypeList_PushValue(_TypeStack, DL_TOK_ListItem);
						gfLexCtx_AddToken(iCtx, DL_TOK_ListItem_Opener, nullptr, -1, -1);

						continue;
						//ioTokens.Add(new TokenInfo(DL_TOK_ListItemOpener));
						

						
						///break;
					}
					else if(_IsListItemDelim)
					{
						///ioStack.Pop();


						gfLexCtx_AddToken(iCtx, DL_TOK_ListError, nullptr, -1, -1);

						//throw new Exception("???");
						///break;
					}
					else if(_IsExpressionDelimiter)
					{
						//throw new Exception("???");
						///ioStack.Pop();


						gfLexCtx_AddToken(iCtx, DL_TOK_ListError, nullptr, -1, -1);
						///break;
					}
					else if(_IsBlockCloser)
					{
						gfLexCtx_AddToken(iCtx, DL_TOK_ListError, nullptr, -1, -1);
						//throw new Exception();
					}
					
					continue;
				}
				//case DL_TOK_ExpectNextListItem:
				//{
				//    break;
				//}

				case DL_TOK_ListItem            :
				{
					if(_IsBlock)
					{
						if(_IsBlockOpener)
						{
							///ioStack.Pop();
							gfTypeList_PushValue(_TypeStack, DL_TOK_Block);
							break;
						}
						else
						{
							//ioTokens.Add(new TokenInfo(DL_TOK_ListItemCloser));
							//ioStack.Pop();
							gfTypeList_PushValue(_TypeStack, DL_TOK_Expect_ListItem_Continuation);
							continue;
							///ioTokens.Add(new TokenInfo(DL_TOK_ListItemCloser));
							//throw new Exception("WTFE");
						}
						//else if(_IsBlockCloser)
						//{
						//    throw new Exception();

						//    ioStack.Pop();
						//    continue;
						//}
						//else
						//{
						//    throw new Exception();
						//}
					}
					else
					{
						//ioStack.Pop();
						gfTypeList_PushValue(_TypeStack, DL_TOK_Expect_ListItem_Continuation);
						///break;
					}
					
					if(iTokenType == DL_TOK_String && !iIsTerminated)
					{
						gfTypeList_PushValue(_TypeStack, DL_TOK_String);
					}
					//else
					//{
					//    ioStack.Pop();
					//    ioStack.Push(DL_TOK_ExpectListItemContinuation);
					//}

					break;
				}
				
				case DL_TOK_Expect_ListItem_Continuation      :
				{
					gfTypeList_Pop(_TypeStack);

					//if(ioStack.Peek() == DL_TOK_ExpressionDelimiter)
					//{
					
					//}

					if(_IsListItemDelim)
					{
						gfTypeList_Pop(_TypeStack);
						gfLexCtx_AddToken(iCtx, DL_TOK_ListItem_Closer, nullptr, -1, -1);
						
						gfTypeList_PushValue(_TypeStack, DL_TOK_Expect_ListItem);
						break;
					}
					else if(_IsListItemContinuation)
					{
						///ioStack.Pop();

						if(iTokenType == DL_TOK_Identifier_Delimiter)
						{
							//ioStack.Push(DL_TOK_ExpectListItem);
						}
						else if(iTokenType == DL_TOK_Bracket_Opener)
						{
							///ioStack.Pop();
							gfTypeList_PushValue(_TypeStack, DL_TOK_Block);
						}
						

						break;
					}
					else
					{
						gfTypeList_Pop(_TypeStack);
						gfLexCtx_AddToken(iCtx, DL_TOK_ListItem_Closer, nullptr, -1, -1);
					}
					
					continue;
				}
				case DL_TOK_Block      :
				{
					if(_IsExpressionItem)
					{
						goto LABEL_DL_TOK_UNDEFINED;
					}
					else if(_IsExpressionDelimiter)
					{
						//continue;
						break;
					}
					else
					{
						gfTypeList_Pop(_TypeStack);

						gfTypeList_PushValue(_TypeStack, DL_TOK_Expect_ListItem_Continuation);

						break;
					}
				}
				

				case DL_TOK_String     :
				{
					if(iIsTerminated)
					{
						gfTypeList_Pop(_TypeStack);
					}

					break;
				}

				default : WTFE("?");
			}
			break; ///~~!!!;
		}
	}
}
void          gfLexCtx_ProcessPairs     (LexerContext* iCtx)
{
	int _TTc,cTi, __MaxUsedCapacity = 0;
	DLTokenStack* _SyntaxTree = gfTList_Create(dDLTokenStackInitialCapacity);
	
	
	for(_TTc = iCtx->Tokens->Count, cTi = 0; cTi < _TTc; cTi ++)
	{
		DLToken* cToken = &iCtx->Tokens->Items[cTi];
		DLTokenType cTokenType = cToken->Type;
		
		DLToken* _SyntaxTop = _SyntaxTree->Count != 0 ? gfTList_Peek(_SyntaxTree) : nullptr;
		DLToken* _SyntaxTopTargetRef;

		if(gfToken_IsGarbage(cTokenType) || !gfToken_IsPaired(cTokenType)) continue;
		
		if(gfToken_IsOpener(cTokenType))
		{
			gfTList_Push(_SyntaxTree, cToken);
			///__MaxUsedCapacity = max(_SyntaxTree->Count,__MaxUsedCapacity);
		}
		else if(gfToken_IsCloser(cTokenType))
		{
			if(_SyntaxTop != nullptr)
			{
				if
				(
					(_SyntaxTop->Type == DL_TOK_Parenthesis_Opener && cToken->Type != DL_TOK_Parenthesis_Closer) ||
					(_SyntaxTop->Type == DL_TOK_Bracket_Opener     && cToken->Type != DL_TOK_Bracket_Closer)     ||
					(_SyntaxTop->Type == DL_TOK_Brace_Opener       && cToken->Type != DL_TOK_Brace_Closer)       ||

					(_SyntaxTop->Type == DL_TOK_Expression_Opener  && cToken->Type != DL_TOK_Expression_Closer)  ||
					(_SyntaxTop->Type == DL_TOK_List_Opener        && cToken->Type != DL_TOK_List_Closer)        ||
					(_SyntaxTop->Type == DL_TOK_ListItem_Opener    && cToken->Type != DL_TOK_ListItem_Closer)
				)
				WTFE("Syntax error");
			}
			else WTFE("?");

			_SyntaxTopTargetRef = &iCtx->Tokens->Items[_SyntaxTop->Id];
			
			gfLexCtx_LinkTokens(iCtx, _SyntaxTopTargetRef, cToken);
			
			gfTList_Pop(_SyntaxTree);
		}
		else WTFE("?");
	}
	
	if(_SyntaxTree->Count != 0)
	{
		/*if(gfTList_Peek(_SyntaxTree)->Type == DL_TOK_List_Opener)
		{
			///DLToken* _ListItem = gfTList_Pop(_SyntaxTree);
			
		}
		else
		*/
		WTFE("Syntax tree incosistent");
	
		
		/**
			2017.08.02 - FIXED?
			BUG: trouble with the first line on modification, when additional
			(pseudo-)tokens are not added around the block-closer brace at the end of program block.
			
			To get here: modify in editor the line just after the 'program' opener brace
			(catched at line #1) and try to start execution, when AST will need to be re-parsed.

			see UpdateLineLexerStates(int iToLine)
			there is specified line index as >1 or >0 (fix)
		*/
		
	}
	gfTList_Destroy(_SyntaxTree);
}
void          gfLexCtx_LinkTokens       (LexerContext* iCtx, DLToken* irOpenerToken, DLToken* irCloserToken)
{
	///~~ Note that we are re-using DLTokenList as token stack. It contains a value array - not references;
	///~~ DLTokenRefList can be used as reference stack, but it needs additional (duplicated) functions;
	
	//DLToken* _Opener = &iCtx->Tokens->Items[iOpenerToken->Id];
	//DLToken* _Closer = &iCtx->Tokens->Items[iCloserToken->Id];
	//
	irCloserToken->PairId = irOpenerToken->Id;
	irOpenerToken->PairId = irCloserToken->Id;
	
	//_Opener->PairId = _Closer _Closer->Id;
	//_Closer->PairId = _Opener->Id;
}



DLTokenList*  gfLexCtx_ParseBuffer      (LexerContext* iCtx)
{
	if(iCtx->Tokens == nullptr) iCtx->Tokens = gfTList_Create(dDLTokenListInitialCapacity);
	if(iCtx->Tokens->Count != 0) WTFE("Empty list expected");
	
	
	while(iCtx->State->Position < iCtx->BufferLength)
	{
		//DLTokenList* cTokenGroup = gfLexCtx_ParseNextTokens(iCtx);
		gfLexCtx_ParseNextTokens(iCtx);
		
		///gfLexCtx_ProcessSyntax(iNewTokens, ioTokens, (iCtx.State as GenericCodeLexerState).TokenStack);
		///ioTokens.AddRange(iNewTokens);
			
		//gfLexCtx_AddToken(iCtx, DL_TOK_Garbage, nullptr, -1,-1);
		
		
		
		//if(cTokenGroup->Count == 1 && cTokenGroup->Items[0] == nullptr) WTFE("It must be 'continue'?");
		
		//gfLexCtx_AddTokens(iCtx, cTokenGroup, oTokens);
		//gfLexCtx_AddTokens(iCtx, cTokenGroup);
	}
	HERE;

	///return oTokens;
	return iCtx->Tokens;
}

void          gfLexCtx_ParseNextTokens  (LexerContext* iCtx)
{
	///DLTokenList* oTokens;
	
	LexerState* _LexerState = iCtx->State;
	
	
	if(_LexerState->Position >= iCtx->BufferLength) WTFE("return null?");
	
	
	///oTokens = gfTList_Create(1);
	
	
	{
		wchar_t cChar = iCtx->Buffer[_LexerState->Position];
		wchar_t nChar = _LexerState->Position < iCtx->BufferLength - 1 ? iCtx->Buffer[_LexerState->Position + 1] : L'\0';///~~ new Char()??;

		
		if(_LexerState->TypeStack->Count > 0 && gfTypeList_PeekValue(_LexerState->TypeStack) == DL_TOK_String)
		{
			gfLexCtx_ParseString(iCtx);
			///gfTList_Push(oTokens, gfLexCtx_ParseString(iCtx));
		}
		else
		{
			//if(gfLexCtx_IsWhitespace(cChar)) gfTList_Push(oTokens, gfLexCtx_ParseWhitespaces(iCtx));
			if(gfLexCtx_IsWhitespace(cChar)) gfLexCtx_ParseWhitespaces(iCtx);
			else
			{
				if(cChar == L'/' && (nChar == L'/' || nChar == L'*'))
				{
					///if   (nChar == L'/') gfTList_Push(oTokens, gfLexCtx_ParseGarbage(iCtx));
					if   (nChar == L'/') gfLexCtx_ParseGarbage(iCtx);
					else
					{
						gfLexCtx_AddToken(iCtx, DL_TOK_Word, nullptr, -1,0);
						//{
							///_Word->Offset = iCtx->
						//}
						///DLToken* _Word = gfTList_Alloc(oTokens, 1);
						//char& _ = iCtx->Buffer[0];
						
						//gfToken_Init();
						//DLToken* _Word = gfToken_Init
						///gfTList_Push(new TokenInfo(TokenType.Word, iCtx.Offset, ++iCtx.Offset));
					}
				}
				else
				{
					///if      (cChar == '#' || AEDLLexer.IsDecimalDigit(cChar) || ((cChar == '+' || cChar == '-') && ((iCtx.Buffer.Length - iCtx.Offset > 1) && AEDLLexer.IsDecimalDigit(iCtx.Buffer[iCtx.Offset + 1]))))
					if      (cChar == L'#' || gfLexCtx_IsDecimalDigit(cChar) || ((cChar == L'+' || cChar == L'-') && ((iCtx->BufferLength - iCtx->State->Position > 1) && gfLexCtx_IsDecimalDigit(iCtx->Buffer[iCtx->State->Position + 1]))))
					{
						gfLexCtx_ParseNumber(iCtx);
					}
					else if (cChar == L'"')               gfLexCtx_ParseString(iCtx);
					///~~else if (cChar == L"'")          gfLexCtx_ParseChar(iCtx);
					else if (gfLexCtx_IsBracket(cChar))   gfLexCtx_ParseBracket(iCtx);
					
					else if (cChar == L';')               gfLexCtx_AddToken(iCtx, DL_TOK_Expression_Delimiter, nullptr, -1,1);
					//else if (cChar == ":")               oTokens.Add(new Token(TokenType.Colon,     cChar, iCtx.Position, iCtx.Position++));
					else if (cChar == L',')               gfLexCtx_AddToken(iCtx, DL_TOK_ListItem_Delimiter,   nullptr, -1,1);
					else if (cChar == L'\'')              gfLexCtx_AddToken(iCtx, DL_TOK_Identifier_Delimiter, nullptr, -1,1);

					else if (cChar == L'$')
					{
						if(nChar == L'-' || nChar == L'+') /// $-1, $+2 etc
						{
							gfLexCtx_ParseList(iCtx);
						}
						//else if(gfLexCtx_IsLetter(nChar))
						else if(iswalpha(nChar))
						{
							iswupper(nChar) ? gfLexCtx_ParseHostObject(iCtx) : gfLexCtx_ParseType(iCtx);
						}
						//else if(nChar == '-' || nChar == '+')
						
						else gfLexCtx_ParseHostObject(iCtx);
					}
					else
					{
						if(iCtx->State->Position >= 295)
						{
							HERE;
						}
						
						gfLexCtx_ParseIdentifier(iCtx);

						///if(cToken.Type != TokenType.Word) oTokens.Add(new TokenInfo{Type = TokenType.ListOpener, Offset = cToken.Offset, Length = 0});
						//oTokens.Add(cToken);
						///if(cToken.Type != TokenType.Word) oTokens.Add(new TokenInfo{Type = TokenType.ListCloser, Offset = cToken.Offset + cToken.Length, Length = 0});
					}
					
					//else                           throw new Exception("Token error at " + iCtx.Position);

				}
				//return this.ParseWord();
			}
		}
	}
	//else
	///if(_LexerState->Position == iCtx->BufferLength)
	
	if(iCtx->Buffer[_LexerState->Position] == '\0')
	{
		///~~ note that position has already been incremented;
		
		if(_LexerState->TypeStack->Count > 1)
		{
			gfLexCtx_AddSyntaxTokens(iCtx, DL_TOK_File_Closer, true);
			
			
			///gfLexCtx_AddToken(iCtx, DL_TOK_F, nullptr, -1,0);
		}
		if(_LexerState->TypeStack->Count == 1 && *gfTypeList_Peek(_LexerState->TypeStack) == DL_TOK_File)
		{
			gfLexCtx_AddToken(iCtx, DL_TOK_File_Closer, nullptr, -1, -1);
			gfTypeList_Pop(_LexerState->TypeStack);
			
			HERE;
		}
		else
		{
			WTFE("Not expected");
		}
		
		HERE;
	}
	///if(_LexerState->Position >= iCtx->BufferLength) WTFE("?");
	
	
	//return oTokens;
	//return iCtx->Tokens;
}
void          gfLexCtx_SkipNonTokens    (LexerContext* iCtx)
{
	while(iCtx->State->Position < iCtx->BufferLength)
	{
		wchar_t cChar = iCtx->Buffer[iCtx->State->Position];
		if(cChar != L' ' && cChar != L'\t' && cChar != L'\r' && cChar != L'\n') break;

		iCtx->State->Position ++;
	}
}


void          gfLexCtx_ParseWhitespaces (LexerContext* iCtx)
{
	DLTokenType _TokenType; 

	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 1;
	wchar_t _FoundWs = iCtx->Buffer[_BegOffs];
	
	while(_EndOffs < iCtx->BufferLength)
	{
		wchar_t cChar = iCtx->Buffer[_EndOffs];

		///if(!AEDLLexer.IsWhitespace(cChar)) break;
		if(!gfLexCtx_IsWhitespace(cChar)) break;
		if(cChar != _FoundWs)             break;
		if(gfLexCtx_IsNewline(cChar))     break;

		_EndOffs ++;
	}
	
	///iCtx->State->Position = _EndOffs;

	_TokenType; switch(_FoundWs)
	{
		case ' '  : _TokenType = DL_TOK_Space;   break;
		case '\t' : _TokenType = DL_TOK_Tab;     break;
		
		//case "\r" : _TokenType = "RETURN";  break;
		case '\r' :  return;/// nullptr;
		case '\n' : _TokenType = DL_TOK_NewLine; break;

		default    : WTFE("?"); break;
	}
	///return new TokenInfo(_TokenType, iCtx.Buffer.Substring(_BegOffs, _EndOffs - _BegOffs), _BegOffs, _EndOffs, true);
	
	///return new TokenInfo(_TokenType, _BegOffs, _EndOffs);
	gfLexCtx_AddToken(iCtx, _TokenType, nullptr, _BegOffs, _EndOffs); 
}
void          gfLexCtx_ParseBracket     (LexerContext* iCtx)
{
	int _Position = iCtx->State->Position;
	wchar_t cChar = iCtx->Buffer[_Position];
	
	DLTokenType _TokenType; switch(cChar)
	{
		case L'(' : _TokenType = DL_TOK_Parenthesis_Opener; break;
		case L')' : _TokenType = DL_TOK_Parenthesis_Closer; break;

		case L'[' : _TokenType = DL_TOK_Bracket_Opener; break;
		case L']' : _TokenType = DL_TOK_Bracket_Closer; break;

		case L'{' : _TokenType = DL_TOK_Brace_Opener; break;
		case L'}' : _TokenType = DL_TOK_Brace_Closer; break;
		
		default  : WTFE("bracket not supported"); break;
	}
	///return new TokenInfo(_TokenType, cChar.ToString(), iCtx.Position, ++iCtx.Position, false, true, _IsOpener);
	gfLexCtx_AddToken(iCtx, _TokenType, nullptr, _Position, _Position + 1); 
	
	///iCtx->State->Position = _Position;
}
void          gfLexCtx_ParseNumber      (LexerContext* iCtx)
{
	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 1;

	while(_EndOffs < iCtx->BufferLength && gfLexCtx_IsNumberChar(iCtx->Buffer[_EndOffs])) _EndOffs ++;

	//iCtx.Offset = _EndOffs;// + 1;

	//return new TokenInfo(TokenType.Number, iCtx.Buffer.Substring(_BegOffs, _EndOffs - _BegOffs), _BegOffs, _EndOffs);
	///string _ValueAsString = iCtx.Buffer.Substring(_BegOffs, _EndOffs - _BegOffs).ToLower();
	gfLexCtx_AddToken(iCtx, DL_TOK_Number, nullptr, _BegOffs, _EndOffs); 
}
void          gfLexCtx_ParseHostObject  (LexerContext* iCtx)
{
	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 1; while(_EndOffs < iCtx->BufferLength && gfLexCtx_IsIdentChar(iCtx->Buffer[_EndOffs])) _EndOffs ++;

	gfLexCtx_AddToken(iCtx, DL_TOK_Host_Object, nullptr, _BegOffs, _EndOffs); 
}
void          gfLexCtx_ParseType        (LexerContext* iCtx)
{
	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 1;



	while(_EndOffs < iCtx->BufferLength)
	{
		wchar_t cChar = iCtx->Buffer[_EndOffs];
		//    if(
		
		if((iswalpha(cChar) && iswlower(cChar)) || gfLexCtx_IsNumberChar(cChar)) _EndOffs ++;
		else if(iswupper(cChar)) WTFE("?");
		else break;
	}
	gfLexCtx_AddToken(iCtx, DL_TOK_Type, nullptr, _BegOffs, _EndOffs); 
}
void          gfLexCtx_ParseList        (LexerContext* iCtx)
{
	STOP;
}
void          gfLexCtx_ParseIdentifier  (LexerContext* iCtx)
{
	wchar_t* _Str; int _StrLen;
	DLTokenType _Type;
	
	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 1;
	


	while(_EndOffs < iCtx->BufferLength && gfLexCtx_IsIdentChar(iCtx->Buffer[_EndOffs])) _EndOffs ++;
	
	_Str = &iCtx->Buffer[_BegOffs];///, _EndOffs - _BegOffs);
	{
		//wchar_t*  _1 = wcsstr(_Str,L"//");
		//wchar_t*  _2 = wcsstr(_Str,L"/*");
		int _1 = wcsindexof(_Str, _EndOffs - _BegOffs, L"//", 2);
		int _2 = wcsindexof(_Str, _EndOffs - _BegOffs, L"/*", 2);

		if      (_1 != -1) _EndOffs = _BegOffs + _1;
		else if (_2 != -1) _EndOffs = _BegOffs + _2;


		_StrLen = _EndOffs - _BegOffs;
		///if(_1 != -1 || _2 != -1)
		//{
		///	_Str = iCtx.Buffer[_BegOffs];///, _EndOffs - _BegOffs);
		//}
	}

	//iCtx.Offset = _EndOffs;
	
	//debugger;

	//var _Str = iCtx.Buffer.Substring(_BegOffs, _EndOffs - _BegOffs);


	_Type  = DL_TOK_Undefined;
	{
		bool _IsMultiChar    = _StrLen > 1;

		wchar_t _FstChar        = _Str[0];
		wchar_t _SndChar        = _IsMultiChar ? _Str[1] : L'X'; //??

		bool _IsLinkedIdent  = _IsMultiChar && (_FstChar >= L'A' && _FstChar <= L'Z');


		if(_IsLinkedIdent)
		{
			///_Type = TokenType.Identifier;
			_Type = DL_TOK_Member_Ident;
		}
		else
		{
		/*	if(_FstChar == L'^')
			{
			
			}*/
			bool _IsFstLowC    = _FstChar >= L'a' && _FstChar <= L'z';
			///var _IsFstPrefix = _FstChar == '_' || _FstChar == '@'|| _FstChar == '^';
			
			bool _IsSndUppC    = _IsMultiChar && (_SndChar >= L'A' && _SndChar <= L'Z');
			bool _IsSndDigit   = _IsMultiChar && (_SndChar >= L'0' && _SndChar <= L'9');

			bool _IsFollowingIdent = _IsSndUppC || _IsSndDigit;

			//if(_FstChar == 'i' && _SndChar == 'o')
			//{
			
			//}
			bool _IsFstLetterPfx = _FstChar == L'_' || _FstChar == L':' || _FstChar == L'^' || _IsFstLowC;///(_FstChar >= L'a' && _FstChar <= L'z');


			if (_IsFstLetterPfx && _IsFollowingIdent) switch(_FstChar)
			{
				//case "_" :                       _Type = "LOCV"; break;
				//case "c" : case "p" : case "n" : _Type = "CYCV"; break;
				
				case L'@' : _Type = DL_TOK_Instruction;    break;
				case L':' : _Type = DL_TOK_Label;          break;
				case L'^' : _Type = DL_TOK_Pointer;        break;
				case L'g' : _Type = DL_TOK_Global_Ident;    break;
				///case L'f' : _Type = DL_TOK_FunctionIdent; break;
				case L'r' : _Type = DL_TOK_Reference_Ident; break;
				case L'i' : _Type = DL_TOK_Input_Ident;     break;
				case L'o' : _Type = DL_TOK_Output_Ident;    break;

				default  : _Type = DL_TOK_Local_Ident;     break;
			}
			else
			{
				if(iCtx->State->Position >= 295)
				{
					HERE;
				}
				//if       (BwdOperandRegex.IsMatch(_Str)) _Type = TokenType.BwdOpd;
				//else if  (FwdOperandRegex.IsMatch(_Str)) _Type = TokenType.FwdOpd;

				///else if  (TypeRegex.IsMatch(_Str))        _Type = TokenType.Type;


				///else if  (_FstChar == '@')                _Type = TokenType.Instruction;
				
				if(false){}
				else _Type = DL_TOK_Word;
				
			}
		}
	}

	gfLexCtx_AddToken(iCtx, _Type, nullptr, _BegOffs, _EndOffs);
	///return new TokenInfo(_Type, _BegOffs, _EndOffs){Value = iCtx.Buffer.Substring(_BegOffs, _EndOffs - _BegOffs)};
}
void          gfLexCtx_ParseString      (LexerContext* iCtx)
{
	DLTokenTypeStack* _Stack = iCtx->State->TypeStack;
	
	bool _IsStringOpen = _Stack->Count != 0 && _Stack->Items[_Stack->Count - 1] == DL_TOK_String;

	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs;
	

	while(_EndOffs < iCtx->BufferLength)
	{
		int cBegSearch = _EndOffs + (_IsStringOpen ? 0 : 1);
		_EndOffs = cBegSearch + wcsindexof(&iCtx->Buffer[cBegSearch], (int)iCtx->BufferLength - _EndOffs, L"\"",  1);
		
		///_EndOffs = iCtx.Buffer.IndexOf('"', _EndOffs + (_IsStringOpen ? 0 : 1));

		if(_EndOffs == -1)
		{
			_EndOffs = (int)iCtx->BufferLength;
			//_EndOffs = _BegOffs - 1; //~~ mark token as unterminated;

			//_Is
			_IsStringOpen = true;

			//if(!_LexerState.IsStringOpen) _LexerState.IsStringOpen = true;

			///if(!_IsStringOpen) _IsStringOpen = true;
			//else throw new Exception("???");

			break;
		}
		else
		{
			bool _IsQuoteCancelled = false;
			{
				///for(int cPos = _EndOffs - 1; cPos >= 0; cPos --, _IsQuoteCancelled =! _IsQuoteCancelled)
				///{
				///    if(iCtx.Buffer[cPos] != '\\') break;
					
				///}
				int cPos; for(cPos = _EndOffs - 1; cPos >= 0; cPos --)
				{
					if(iCtx->Buffer[cPos] == L'\\')
					{
						_IsQuoteCancelled =! _IsQuoteCancelled;
					}
					else break;
				}
			}

			_EndOffs ++;

			if(_IsQuoteCancelled) {/**_LexerState.IsStringOpen = false;*/   continue;}
			///else                  {if(_LexerState.IsStringOpen == true) _LexerState.IsStringOpen = false; break;}
			else                  {if(_IsStringOpen) _IsStringOpen = false; break;}
		}
	}

	///iCtx.Offset = _EndOffs;

	//return new TokenInfo(TokenType.String, _BegOffs, _IsStringOpen ? _BegOffs - 1 : _EndOffs){Value = iCtx.Buffer.Substring(_BegOffs + 1, _EndOffs - _BegOffs - (_IsStringOpen ? 1 : 2))};
	///return new TokenInfo(TokenType.String, _BegOffs, _IsStringOpen ? _BegOffs - 1 : _EndOffs){Value = iCtx.Buffer.Substring(_BegOffs + 1, Math.Max(0, _EndOffs - _BegOffs - 2))};
	
	gfLexCtx_AddToken(iCtx, DL_TOK_String, nullptr, _BegOffs, (_IsStringOpen ? _BegOffs - 1 : _EndOffs));
}
void          gfLexCtx_ParseChar        (LexerContext* iCtx)
{
	STOP;
}
void          gfLexCtx_ParseGarbage     (LexerContext* iCtx)
{
	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 2;
	
	///var _FwdS = iCtx.Buffer.Substring(_BegOffs, Math.Min(iCtx.Buffer.Length - _BegOffs, 4));
	wchar_t* _FwdS = &iCtx->Buffer[_BegOffs];

	if(_FwdS[1] == '/')
	{
		//~~ single-line garbage;

		_BegOffs = iCtx->State->Position;/// + 2;
		_EndOffs = _BegOffs; while(_EndOffs < iCtx->BufferLength && !gfLexCtx_IsNewline(iCtx->Buffer[_EndOffs])) _EndOffs ++;

		///iCtx->State->Position = _EndOffs;


		///if(_FwdS == "//~~")
		if(_FwdS[0] == L'/' && _FwdS[1] == L'/' && _FwdS[2] == L'~' && _FwdS[3] == L'~')
		{
			//oTokens.Add(new Token(TokenType.Garbage, iCtx.Buffer.Substring(_BegOffs, 2), _BegOffs, _BegOffs + 2, true));
			//oTokens.Add(new Token(TokenType.Comment, iCtx.Buffer.Substring(_BegOffs + 2, _EndOffs - (_BegOffs + 2)), _BegOffs + 2, _EndOffs, true));

			//oTokens.Add(new TokenInfo(TokenType.Comment, iCtx.Buffer.Substring(_BegOffs, _EndOffs - _BegOffs), _BegOffs, _EndOffs, true));
			
			///oTokens.Add(new TokenInfo(TokenType.Comment, _BegOffs, _EndOffs));
			gfLexCtx_AddToken(iCtx, DL_TOK_Comment, nullptr, _BegOffs, _EndOffs);
		}
		else
		{
			gfLexCtx_AddToken(iCtx, DL_TOK_Garbage, nullptr, _BegOffs, _EndOffs);
		}
	}
	/*else if(_FwdS[0] == L'/' && _FwdS[1] == L'*' && _FwdS[2] == L'*')
	{
		WTFE("NI: need multiline comment mode continuation ");
	}*/
	else WTFE("NI: another comment pattern?");
}


bool          gfLexCtx_IsNewline        (wchar_t iChar){return iChar == L'\r' || iChar == L'\n';}
bool          gfLexCtx_IsWhitespace     (wchar_t iChar){return iChar == L' ' || iChar == L'\t' || gfLexCtx_IsNewline(iChar);}
bool          gfLexCtx_IsDecimalDigit   (wchar_t iChar){return iChar >= L'0' && iChar <= L'9';}
bool          gfLexCtx_IsNumberChar     (wchar_t iChar){return !(gfLexCtx_IsWhitespace(iChar) || gfLexCtx_IsPunctuation(iChar) || gfLexCtx_IsBracket(iChar) || gfLexCtx_IsQuote(iChar));}
bool          gfLexCtx_IsAlpha          (wchar_t iChar){return (iChar >= L'a' && iChar <= L'z') || (iChar >= L'A' && iChar <= L'Z') || iChar == L'_';}
bool          gfLexCtx_IsIdentChar      (wchar_t iChar){return gfLexCtx_IsAlpha(iChar) || gfLexCtx_IsDecimalDigit(iChar) || gfLexCtx_IsSpecial(iChar);}
bool          gfLexCtx_IsPunctuation    (wchar_t iChar){return iChar == L'\'' || iChar == L',' || iChar == L';';}
bool          gfLexCtx_IsBracket        (wchar_t iChar){return iChar == L'(' || iChar == L')' || iChar == L'[' || iChar == L']' || iChar == L'{' || iChar == L'}';}
bool          gfLexCtx_IsQuote          (wchar_t iChar){return iChar == L'"';}// || iChar == '\''},
bool          gfLexCtx_IsSpecial        (wchar_t iChar){return !gfLexCtx_IsBracket(iChar) && !gfLexCtx_IsQuote(iChar) && !gfLexCtx_IsWhitespace(iChar) && !gfLexCtx_IsPunctuation(iChar);}


struct $__DL_SyntaxNode__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
void          gfSNode_Init              (DLSyntaxNode* irNode, DLSyntaxNodeType iNodeType, DLToken* iToken, int iChildCapacity)
{
	irNode->Type   = iNodeType;
	irNode->Token_ = iToken;
	irNode->Role   = DL_SEM_Unknown;
	
	irNode->Parent = nullptr;
	
	if(iToken != nullptr)
	{
		irNode->BegToken = iToken->Id;
		///irNode->EndToken = iToken->PairId != -1 ? iToken->PairId : -1;
		irNode->EndToken = iToken->PairId;
	}
	else
	{
		irNode->BegToken = -1;
		irNode->EndToken = -1;
	}
	
	if(iChildCapacity != 0) {irNode->Children = gfSNList_Create(iChildCapacity); irNode->Children->Owner = irNode;}
	else                     irNode->Children = nullptr;
}

struct $__DL_SyntaxParser__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};

DLSyntaxParser* gfParser_Create      (DLContext* iDLC)
{
	DLSyntaxNode* _Root;

	DLSyntaxParser* oParser = malloc(sizeof(DLSyntaxParser));
	{
		oParser->DLC      = iDLC;
		oParser->Position = -1;
		oParser->Tokens   = nullptr;///iLexerContext->Tokens;
		
		oParser->RootNode  = nullptr;
		oParser->TopNode   = nullptr;
		//_Root = malloc(sizeof(DLSyntaxNode));
		//{
		//	gfSNode_Init(_Root, DL_SYN_FunctionBlock, nullptr, 1);
		///*	_Root->Parent = nullptr;///~~ ???;
		//
		//
		//	_Root->Type  = DL_SYN_FunctionBlock;
		//	_Root->Token = nullptr;
		//	
		//	_Root->BegToken = -1;
		//	_Root->EndToken = -1;
		//	
		//	_Root->Children = gfSNList_Create(1);*/
		//}
		//oParser->TopNode = _Root;
	}
	return oParser;
}
void            gfParser_Destroy     (DLSyntaxParser* irParser)
{
	///if(irParser->Tokens  != nullptr) gfTList_Destroy(irParser->Tokens);
	if(irParser->TopNode != nullptr)
	{
		gfSNList_Destroy(irParser->TopNode->Children);
		free(irParser->TopNode);
	}
}
void            gfParser_Init        (DLSyntaxParser* irParser, DLTokenList* iTokens)
{
	DLSyntaxNode* _Root;

	irParser->Position = -1;
	irParser->Tokens   = iTokens;
	
	irParser->RootNode = nullptr;
	irParser->TopNode = nullptr;
	
	///_Root = malloc(sizeof(DLSyntaxNode));
	///{
	///	gfSNode_Init(_Root, DL_SYN_FunctionBlock, nullptr, 1);
	///}
	///irParser->RootNode = _Root;
	///irParser->TopNode  = _Root;
}
DLSyntaxNode*   gfParser_ParseTokens (DLSyntaxParser* irParser)
{
	gfParser_Init(irParser, irParser->DLC->Lexer->Tokens);
	
	/*this.Position ++;

			return (this.Position < this.Tokens.Count);
	*/		
	while(irParser->Position ++, irParser->Position < irParser->Tokens->Count)
	{
		DLToken* cToken = &irParser->Tokens->Items[irParser->Position];

		bool cIsTrash  = gfToken_IsGarbageSTX (cToken->Type);///cIsTrash = cToken.Type >= TokenType.Whitespace && cToken.Type <= TokenType.Comment;
		bool cIsOpener = gfToken_IsOpener     (cToken->Type); ///bool cIsOpener = cToken.Type == TokenType.ExpressionOpener || cToken.Type == TokenType.ListOpener || cToken.Type == TokenType.ListItemOpener || cToken.Type == TokenType.ParenthesisOpener || cToken.Type == TokenType.BracketOpener || cToken.Type == TokenType.BraceOpener;
		
		if(cToken->Id >= 450)
		{
			HERE;
		}


		//ParenthesisOpener,
		//BracketOpener,
		//BraceOpener,

		//FileOpener,
		//BlockOpener,
		//ExpressionOpener,
		//ListOpener,
		

		if(cIsTrash) continue;

		if(cIsOpener)
		{
			DLSyntaxNodeType _NodeType; switch(cToken->Type)
			{
				case DL_TOK_ListItem_Opener    : _NodeType = DL_SYN_ListItem;       break;
				case DL_TOK_List_Opener        : _NodeType = DL_SYN_List;           break;
				case DL_TOK_Expression_Opener  : _NodeType = DL_SYN_Expression;     break;

				case DL_TOK_Parenthesis_Opener : _NodeType = DL_SYN_GroupingBlock;  break;
				case DL_TOK_Bracket_Opener     : _NodeType = DL_SYN_ArgumentBlock;  break;
				case DL_TOK_Brace_Opener       : _NodeType = DL_SYN_FunctionBlock;  break;

				case DL_TOK_File_Opener        : _NodeType = DL_SYN_File;     break;

				///default : printf("Token (type = %d) is skipped", cToken->Type); break;
				default : WTFE("Opener token does not match any of syntax node type"); break;
			}
			gfParser_OpenNode(irParser, _NodeType, cToken);
		}
		else
		{
			switch(cToken->Type)
			{
				case DL_TOK_ListItem_Closer    : gfParser_CloseNode(irParser, DL_SYN_ListItem);     break;
				case DL_TOK_List_Closer        : gfParser_CloseNode(irParser, DL_SYN_List);         break;
				case DL_TOK_Expression_Closer  : gfParser_CloseNode(irParser, DL_SYN_Expression);   break;
				
				case DL_TOK_Parenthesis_Closer : gfParser_CloseNode(irParser, DL_SYN_GroupingBlock); break;
				case DL_TOK_Bracket_Closer     : gfParser_CloseNode(irParser, DL_SYN_ArgumentBlock); break;
				case DL_TOK_Brace_Closer       : gfParser_CloseNode(irParser, DL_SYN_FunctionBlock); break;

				case DL_TOK_File_Closer        : gfParser_CloseNode(irParser, DL_SYN_File);          break;
				
				//case TokenType.ListItemCloser :
				//{

				//    break;
				//}

				case DL_TOK_Instruction   : gfParser_AddNode(irParser, DL_SYN_Instruction, cToken); break;
				case DL_TOK_Label         : gfParser_AddNode(irParser, DL_SYN_Label,       cToken); break;
				case DL_TOK_Pointer       : gfParser_AddNode(irParser, DL_SYN_Pointer,     cToken); break;
				case DL_TOK_Word          : gfParser_AddNode(irParser, DL_SYN_Word,        cToken); break;
				case DL_TOK_Number        : gfParser_AddNode(irParser, DL_SYN_Number,      cToken); break;
				///case DL_TOK_InvalidNumber : gfParser_AddNode(irParser, DL_SYN_NumInvalid, cToken); break;

				///case DL_TOK_Int32   : gfParser_AddNode(irParser, DL_SYN_NumInt32,   cToken); break;
				///case DL_TOK_Float32 : gfParser_AddNode(irParser, DL_SYN_NumFloat32, cToken); break;
				///case DL_TOK_Float64 : gfParser_AddNode(irParser, DL_SYN_NumFloat64, cToken); break;
				


				case DL_TOK_String           : gfParser_AddNode(irParser, DL_SYN_String, cToken); break;

				
				case DL_TOK_Host_Object       : gfParser_AddNode(irParser, DL_SYN_HostObject,            cToken); break;
				
				case DL_TOK_Identifier        : gfParser_AddNode(irParser, DL_SYN_Identifier,            cToken); break;
				case DL_TOK_Local_Ident       : gfParser_AddNode(irParser, DL_SYN_LocalIdentifier,       cToken); break;
				case DL_TOK_Global_Ident      : gfParser_AddNode(irParser, DL_SYN_GlobalIdentifier,      cToken); break;
				case DL_TOK_Reference_Ident   : gfParser_AddNode(irParser, DL_SYN_ReferenceIdentifier,   cToken); break;
				case DL_TOK_Input_Ident       : gfParser_AddNode(irParser, DL_SYN_InputIdentifier,       cToken); break;
				case DL_TOK_Output_Ident      : gfParser_AddNode(irParser, DL_SYN_OutputIdentifier,      cToken); break;
				case DL_TOK_Member_Ident      : gfParser_AddNode(irParser, DL_SYN_MemberIdentifier,      cToken); break;
				///case DL_TOK_Function_Ident : gfParser_AddNode(irParser, DL_SYN_FunctionIdentifier,    cToken); break;

				case DL_TOK_Type              : gfParser_AddNode(irParser, DL_SYN_Type,                  cToken); break;
				case DL_TOK_Packed_Tuple      : gfParser_AddNode(irParser, DL_SYN_PackedTuple,           cToken); break;

				//case TokenType.Space:


				///default : throw new Exception("WTFE");
				///default : Console.WriteLine("Token '" + cToken.Type.ToString() + "' was not handled"); break;
			}
		}
	}
	return irParser->TopNode;
}

void            gfParser_AddNode     (DLSyntaxParser* irParser, DLSyntaxNodeType iNodeType, DLToken* iToken)
{
	DLSyntaxNode* _Node;

	if(irParser->TopNode->Children == nullptr)
	{
		irParser->TopNode->Children        = gfSNList_Create(1);
		irParser->TopNode->Children->Owner = irParser->TopNode;
	}
	
	_Node = gfSNList_Allocate(irParser->TopNode->Children, 1);
	{
		gfSNode_Init(_Node, iNodeType, iToken, 0);
		_Node->Parent = irParser->TopNode;
		
			
		///oNode->Parent = nullptr;///~~ ???;
		//
		//oNode->Type  = iNodeType;
		//oNode->Token = iToken;
		//
		//if(iToken != nullptr)
		//{
		//	oNode->BegToken = iToken->Id;
		//	oNode->EndToken = iToken->Pair != nullptr ? iToken->Pair->Id : -1;
		//}
		//else
		//{
		//	oNode->BegToken = -1;
		//	oNode->EndToken = -1;
		//}
		//oNode->Children = gfSNList_Create(0);
	}
	///return oNode;
}
DLSyntaxNode*   gfParser_OpenNode    (DLSyntaxParser* irParser, DLSyntaxNodeType iNodeType, DLToken* iToken)
{
	DLSyntaxNode* oNode;
	
	irParser->TopNode->Children;
	
	if(iNodeType == DL_SYN_List)
	{
		HERE;
	}

	if(iNodeType == DL_SYN_File)
	{
		assert(irParser->RootNode == nullptr);
		assert(irParser->TopNode == nullptr);
		
		oNode = malloc(sizeof(DLSyntaxNode));
		
		irParser->RootNode = oNode;
		irParser->TopNode  = oNode;
		
		gfSNode_Init(oNode, DL_SYN_File, iToken, 1);
		{
		
		}
		
		HERE;
	}
	else
	{
		if(irParser->TopNode->Children == nullptr)
		{
			irParser->TopNode->Children        = gfSNList_Create(1);
			irParser->TopNode->Children->Owner = irParser->TopNode;
		}
		
		oNode = gfSNList_Allocate(irParser->TopNode->Children, 1);
		{
			gfSNode_Init(oNode, iNodeType, iToken, 0);
			oNode->Parent = irParser->TopNode;
			
			//if(oNode->Type == DL_SYN_List)
			//{
			//	HERE;
			//}
		}
		
		irParser->TopNode = oNode;
	}

	return oNode;
}
DLSyntaxNode*   gfParser_CloseNode   (DLSyntaxParser* irParser, DLSyntaxNodeType iNodeType)
{
	if(irParser->TopNode == nullptr) WTFE("?");
	if(irParser->TopNode->Type != iNodeType) WTFE("Unmatched pair token");
	
	/*if(iNodeType == DL_SYN_List)
	{
		HERE;
	}*/
	irParser->TopNode = irParser->TopNode->Parent;
	
	return irParser->TopNode;
}


struct $__DL_002__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__DL_003__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__DL_004__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__DL_005__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__DL_006__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__DL_007__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};

void gfDLSyntaxNodeType_ToString(char* irStr, DLSyntaxNodeType iValue)
{
	switch (iValue)
	{
		///#define STR(r) case r: memcpy(irErrStr, L#r, sizeof(wchar_t) * 64); return
		#define STR(r) case r: memcpy(irStr, #r, sizeof(char) * 64); return

		STR(DL_SYN_Unknown);

		STR(DL_SYN_Expression);
		
		STR(DL_SYN_List);
		STR(DL_SYN_ListItem);
		
		STR(DL_SYN_GroupingBlock);
		STR(DL_SYN_ArgumentBlock);
		STR(DL_SYN_FunctionBlock);
		
		STR(DL_SYN_Value__Begin);
			STR(DL_SYN_Number);
			STR(DL_SYN_NumInvalid);
			STR(DL_SYN_NumInt32);
			STR(DL_SYN_NumFloat32);
			STR(DL_SYN_NumFloat64);
			
			STR(DL_SYN_String);
		STR(DL_SYN_Value__End);

		STR(DL_SYN_Identifier);
			STR(DL_SYN_Instruction);
			STR(DL_SYN_Label);
			STR(DL_SYN_Pointer);
			STR(DL_SYN_ReferenceIdentifier);
			STR(DL_SYN_InputIdentifier);
			STR(DL_SYN_OutputIdentifier);
			STR(DL_SYN_LocalIdentifier);
			STR(DL_SYN_GlobalIdentifier);
			STR(DL_SYN_MemberIdentifier);
			//DL_SYN_FunctionIdentifier);
			STR(DL_SYN_Word);

			STR(DL_SYN_HostObject);
			STR(DL_SYN_Type);
			STR(DL_SYN_PackedTuple);
		
		#undef STR

		//default:
		//	return "UNKNOWN_ERROR";
	}
}
//
//
//bool IsUpper(wchar_t iChar)
//{
//	return isupper(iChar);
//}
//bool IsLower(wchar_t iChar)
//{
//	return isupper(iChar);
//}