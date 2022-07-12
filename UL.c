#include "UL.h"

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


struct $__UL_Context__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};



ULContext* gfULCtx_Create           (wchar_t* iBuffer, size_t iBufferLength, bool iIsComplete)
{
	ULContext* oULC = malloc(sizeof(ULContext));
	{
		oULC->Lexer = gfLexCtx_Create(oULC, iBuffer, iBufferLength);
		oULC->Parser = gfParser_Create(oULC);
	}
	return oULC;
}
void       gfULCtx_Destroy          (ULContext* irULC)
{
	if(irULC->Lexer  != nullptr) gfLexCtx_Destroy(irULC->Lexer);
	if(irULC->Parser != nullptr) gfParser_Destroy(irULC->Parser);
	
	free(irULC);
}


struct $__UL_Token__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};



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

void gfToken_Init          (ULToken* iToken)
{
	iToken->Id       = -1;
	iToken->Offset   = -1;
	iToken->Length   = -1;
	iToken->Fragment = -1;
	
	iToken->Type     = UL_TOK_Undefined;
	iToken->Value[0] = L'\0';
	///iToken->Pair     = nullptr;
	iToken->PairId   = -1;
}
void gfToken_Cleanup       (ULToken* iToken)
{
	if(iToken->Type == UL_TOK_String)
	{
		///if(iToken->Value[0] != 0)
		{
			///free()
		}
	}
}

bool gfToken_IsAligned     (ULToken* iToken){return iToken->Offset != -1;}
bool gfToken_IsZeroLength  (ULToken* iToken){return iToken->Length == 0;}
bool gfToken_IsTerminated  (ULToken* iToken){return iToken->Length != -1;}
bool gfToken_IsPaired      (ULTokenType iTokenType){return gfToken_IsOpener(iTokenType) || gfToken_IsCloser(iTokenType);}

//bool gfToken_IsOpener      (ULTokenType iTokenType){return iTokenType == UL_TOK_Expression_Opener || iTokenType == UL_TOK_List_Opener || iTokenType == UL_TOK_ListItem_Opener || iTokenType == UL_TOK_Parenthesis_Opener || iTokenType == UL_TOK_Bracket_Opener || iTokenType == UL_TOK_Brace_Opener;}
//bool gfToken_IsCloser      (ULTokenType iTokenType){return iTokenType == UL_TOK_Expression_Closer || iTokenType == UL_TOK_List_Closer || iTokenType == UL_TOK_ListItem_Closer || iTokenType == UL_TOK_Parenthesis_Closer || iTokenType == UL_TOK_Bracket_Closer || iTokenType == UL_TOK_Brace_Closer;}

bool gfToken_IsOpener      (ULTokenType iTokenType){return iTokenType == UL_TOK_ListItem_Opener || iTokenType == UL_TOK_List_Opener || iTokenType == UL_TOK_Expression_Opener || iTokenType == UL_TOK_Parenthesis_Opener || iTokenType == UL_TOK_Bracket_Opener || iTokenType == UL_TOK_Brace_Opener || iTokenType == UL_TOK_File_Opener;}
bool gfToken_IsCloser      (ULTokenType iTokenType){return iTokenType == UL_TOK_ListItem_Closer || iTokenType == UL_TOK_List_Closer || iTokenType == UL_TOK_Expression_Closer || iTokenType == UL_TOK_Parenthesis_Closer || iTokenType == UL_TOK_Bracket_Closer || iTokenType == UL_TOK_Brace_Closer || iTokenType == UL_TOK_File_Closer;}

bool gfToken_IsWhitespace  (ULTokenType iTokenType){return iTokenType >= UL_TOK_Whitespace && iTokenType <= UL_TOK_NewLine;}

bool gfToken_IsGarbage     (ULTokenType iTokenType){return iTokenType == UL_TOK_Garbage || iTokenType == UL_TOK_Comment;}
bool gfToken_IsGarbageSTX  (ULTokenType iTokenType){return iTokenType >= UL_TOK_Whitespace && iTokenType <= UL_TOK_Comment;}



struct $__UL_TokenTypeList__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
ULTokenTypeList*  gfTypeList_Create   (int iCapacity)
{
	ULTokenTypeList* oList = malloc(sizeof(ULTokenTypeList));
	{
		oList->Items    = nullptr;
		oList->Count    = 0;
		oList->Capacity = 0;
		
		gfTypeList_Reserve(oList,iCapacity);
	}
	return oList;
}
void              gfTypeList_Destroy  (ULTokenTypeList* irList)
{
	if(irList->Items != nullptr) free(irList->Items);
	
	free(irList);
}
ULTokenType*      gfTypeList_Allocate (ULTokenTypeList* irList, int iCount)
{
	gfTypeList_Reserve(irList, irList->Count + iCount);
	
	irList->Count += iCount;
	
	return &irList->Items[irList->Count - 1];
}


void              gfTypeList_PushValue    (ULTokenTypeList* irList, ULTokenType iItem)
{
	ULTokenType* _Item = gfTypeList_Allocate(irList,1);
	
	*_Item = iItem;
	///irList->Items[irList->Count - 1] = iItem;
}
ULTokenType       gfTypeList_PopValue      (ULTokenTypeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return irList->Items[-- irList->Count];
}
ULTokenType*      gfTypeList_Pop      (ULTokenTypeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[-- irList->Count];
}
ULTokenType       gfTypeList_PeekValue (ULTokenTypeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return irList->Items[irList->Count - 1];
}
ULTokenType*      gfTypeList_Peek     (ULTokenTypeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[irList->Count - 1];
}

void              gfTypeList_Reserve  (ULTokenTypeList* irList, int iCapacity)
{
	if(iCapacity <= irList->Capacity) return;
	
	
	{
		size_t _OldSize, _NewSize;
		ULTokenType *_ItemsOldPtr = irList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dULTokenTypeListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(ULTokenType); //, _Size;
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
void              gfTypeList_Clear    (ULTokenTypeList* irList)
{
	irList->Count = 0;
}



struct $__UL_TokenList__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
ULTokenList*      gfTList_Create   (int iCapacity)
{
	ULTokenList* oTokenList = malloc(sizeof(ULTokenList));
	{
		oTokenList->Items    = nullptr;
		oTokenList->Count    = 0;
		oTokenList->Capacity = 0;
		
		gfTList_Reserve(oTokenList,iCapacity);
	}
	return oTokenList;
}
void              gfTList_Destroy  (ULTokenList* irList)
{
	if(irList->Items != nullptr) free(irList->Items);
	
	free(irList);
}
ULToken*          gfTList_Allocate (ULTokenList* irList, int iCount)
{
	gfTList_Reserve(irList, irList->Count + iCount);
	
	irList->Count += iCount;
	
	return &irList->Items[irList->Count - 1];
}

void              gfTList_Push     (ULTokenList* irList, ULToken* iItem)
{
	ULToken* _Token = gfTList_Allocate(irList,1);
	
	irList->Items[irList->Count - 1] = *iItem;
	//if(irList->Count + 1 >= irList->Capacity)
	//{
	//	gfTList_Reserve(irList, irList->Capacity == 0 ? 1 : (int)(irList->Capacity * dULTokenListCapacityMultiplier));
	//}
	//irList->Items[irList->Count ++] = *iToken;
}
ULToken*          gfTList_Pop      (ULTokenList* irList)
{
	ULToken* oToken;

	if(irList->Count <= 0) return nullptr;
	
	
	return &irList->Items[-- irList->Count];
}
ULToken*          gfTList_Peek     (ULTokenList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[irList->Count - 1];
}

void              gfTList_Reserve  (ULTokenList* irList, int iCapacity)
{
	if(iCapacity <= irList->Capacity) return;
	
	
	{
		size_t _OldSize, _NewSize;
		ULToken *_ItemsOldPtr = irList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dULTokenListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(ULToken); //, _Size;
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
void              gfTList_Clear    (ULTokenList* irList)
{
	irList->Count = 0;
}


struct $__UL_SyntaxNodeList__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
///ULSyntaxNodeList*  gfSNList_Create   (ULSyntaxNode* iOwner, int iCapacity)
ULSyntaxNodeList* gfSNList_Create   (int iCapacity)
{
	ULSyntaxNodeList* oList = malloc(sizeof(ULSyntaxNodeList));
	{
		///oList->Owner    = nullptr; ///~~ commented for C-'template' compatibility: share the same code with ULTokenList, ULTokenTypeList etc;
		oList->Items    = nullptr;
		oList->Count    = 0;
		oList->Capacity = 0;
		
		gfSNList_Reserve(oList,iCapacity);
	}
	return oList;
}
void              gfSNList_Destroy  (ULSyntaxNodeList* irList)
{
	if(irList->Items != nullptr) free(irList->Items);
	
	free(irList);
}
ULSyntaxNode*     gfSNList_Allocate (ULSyntaxNodeList* irList, int iCount)
{
	gfSNList_Reserve(irList, irList->Count + iCount);
	
	/*if(iCount != 1)
	{
		HERE;
	}*/
	irList->Count += iCount;
	
	return &irList->Items[irList->Count - 1];
}

///void               gfSNList_Push     (ULSyntaxNodeList* irList, ULSyntaxNode* iItem)
//{
//	ULSyntaxNode* _Token = gfSNList_Allocate(irList,1);
//	
//	irList->Items[irList->Count - 1] = *iItem;
//}
ULSyntaxNode*     gfSNList_Pop      (ULSyntaxNodeList* irList)
{
	ULSyntaxNode* oToken;

	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[-- irList->Count];
}
ULSyntaxNode*     gfSNList_Peek     (ULSyntaxNodeList* irList)
{
	if(irList->Count <= 0) return nullptr;
	
	return &irList->Items[irList->Count - 1];
}

void              gfSNList_Reserve  (ULSyntaxNodeList* irList, int iCapacity)
{
	if(iCapacity <= irList->Capacity) return;
	
	{
		size_t _OldSize, _NewSize;
		ULSyntaxNode *_ItemsOldPtr = irList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(irList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dULSyntaxNodeListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(ULSyntaxNode); //, _Size;
		
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
				ULSyntaxNode* cParentNode = &irList->Items[cPi];
				int _CCc, cCi; 
				
				cParentNode->Children->Owner = cParentNode;
				
				for(_CCc = cParentNode->Children->Count, cCi = 0; cCi < _CCc; cCi ++)
				{
					ULSyntaxNode* cChildNode = &cParentNode->Children->Items[cCi];
					cChildNode->Parent = cParentNode;
				}
			}
		}
	}
}
void              gfSNList_Clear    (ULSyntaxNodeList* irList)
{
	irList->Count = 0;
}


struct $__UL_LexerState__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
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

struct $__UL_LexerContext__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
LexerContext* gfLexCtx_Create           (ULContext* iULC, wchar_t* iBuffer, size_t iBufferLength)
{
	LexerContext* oCtx = malloc(sizeof(LexerContext));
	{
		///memset(oCtx,0,sizeof(LexerContext));
		oCtx->ULC          = iULC;
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


ULToken*      gfLexCtx_AllocateToken    (LexerContext* iCtx)
{
	ULToken* _Tokens = gfTList_Allocate(iCtx->Tokens, 1);
	{
		///_Tokens[0]->Offset
	}
	return _Tokens;//[0];
}
ULToken*      gfLexCtx_AllocateTokens   (LexerContext* iCtx, int iCount)
{
	ULToken* oTokens = gfTList_Allocate(iCtx->Tokens, iCount);
	return oTokens;
}
///void          gfLexCtx_AddToken         (LexerContext* iCtx, ULTokenType iType, void* iValue, int iLength)


void          gfLexCtx_AddToken         (LexerContext* iCtx, ULTokenType iTokenType, void* iValue, int iBegOffs, int iEndOffsOrLength)
{
	
	
	bool _IsBegOffsSpecified = iBegOffs != -1;
	ULToken* _Token;
	
	///int _TokenLength = _IsBegOffsSpecified ? iEndOffsOrLength - iBegOffs : iEndOffsOrLength;
	
	if(iTokenType == UL_TOK_String && (iBegOffs == -1 || iEndOffsOrLength == -1))
	{
		WTFE("?");
	}
	
	if(iTokenType < UL_TOK_Special_Syntax_Tokens__Begin || iTokenType > UL_TOK_Special_Syntax_Tokens__End)
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
void          gfLexCtx_AddTokens        (LexerContext* iCtx, ULTokenList* iNewTokens, ULTokenList* irTokens)
{
	STOP;
	///gfLexCtx_ProcessSyntax(iCtx, iNewTokens);
}
void          gfLexCtx_AddSyntaxTokens  (LexerContext* iCtx, ULTokenType iTokenType, bool iIsTerminated)///, ULTokenStack irStack)
{
	ULTokenTypeStack* _TypeStack = iCtx->State->TypeStack;

	//if(_TypeStack->Count == 0)
	//{
	//	gfTypeList_PushValue(_TypeStack, UL_TOK_File);

	//	gfLexCtx_AddToken(iCtx, UL_TOK_File_Opener, nullptr, -1, -1);
	//}
	if(gfToken_IsWhitespace(iTokenType) || gfToken_IsGarbage(iTokenType))
	{
		if(_TypeStack->Count == 0 || *gfTypeList_Peek(_TypeStack) != UL_TOK_Whitespace)
		{
			gfTypeList_PushValue(_TypeStack, UL_TOK_Whitespace);
		}
		return;
	}
	else
	{
		bool _IsBlockOpener = iTokenType == UL_TOK_Parenthesis_Opener || iTokenType == UL_TOK_Bracket_Opener || iTokenType == UL_TOK_Brace_Opener;
		bool _IsBlockCloser = iTokenType == UL_TOK_Parenthesis_Closer || iTokenType == UL_TOK_Bracket_Closer || iTokenType == UL_TOK_Brace_Closer;

		bool _IsLiteral = iTokenType >= UL_TOK_String && iTokenType <= UL_TOK_Number;

		bool _IsBlock               = _IsBlockOpener || _IsBlockCloser;
		bool _IsIdentifier          = (iTokenType > UL_TOK_Identifiers__Begin && iTokenType < UL_TOK_Identifiers__End);
		bool _IsAtomDelim           = iTokenType == UL_TOK_Atom_Delimiter;
		bool _IsListItem            = _IsLiteral || _IsIdentifier || _IsBlockOpener || _IsAtomDelim;
		bool _IsListItemDelim       = iTokenType == UL_TOK_ListItem_Delimiter;
		bool _IsList                = _IsListItem || _IsListItemDelim || _IsAtomDelim;
		bool _IsExpressionItem      = _IsList;
		bool _IsExpressionDelimiter = iTokenType == UL_TOK_Expression_Delimiter;

		while(true)
		{
			/**
				Think of the following 'continue' and 'break' statements as they are used to control the 'while' loop only,
				but not the 'switch' (see the 'break' statement after the 'switch' in the end of 'while' block)
				So, breaking this loop also means 'return': no more additional tokens to add and no more changes to syntax stack to apply
			*/
			bool cStackIsEmpty = _TypeStack->Count == 0;
			ULTokenType cStackTop = cStackIsEmpty ? UL_TOK_Undefined : *gfTypeList_Peek(_TypeStack);
		
			switch(cStackTop)
			{
				case UL_TOK_Undefined  :
				{
					LABEL_UL_TOK_Undefined:
					
					gfTypeList_PushValue(_TypeStack, UL_TOK_Expect_Expression);
					continue;
				}
				//case UL_TOK_File  :
				//{
				//	goto LABEL_UL_TOK_Undefined;
				//}
				case UL_TOK_Expect_Expression :
				{
					gfTypeList_Pop(_TypeStack);

					if(_IsExpressionItem)
					{
						gfLexCtx_AddToken(iCtx, UL_TOK_Expression_Opener, nullptr, -1,-1);
						gfTypeList_PushValue(_TypeStack, UL_TOK_Expression);
					
						continue;
					}
					else
					{
						break;
					}
				}
				case UL_TOK_Expression :
				{
					if(_IsList)
					{
						gfTypeList_PushValue(_TypeStack, UL_TOK_Expect_List);
					}
					else/// if(_IsExpressionDelimiter || _IsBlockCloser)
					{
						gfTypeList_Pop(_TypeStack);
						gfLexCtx_AddToken(iCtx, UL_TOK_Expression_Closer, nullptr, -1,-1);

						if(_IsExpressionDelimiter) break;
					}
					continue;
				}
				case UL_TOK_Expect_List      :
				{
					gfTypeList_Pop(_TypeStack);

					if(_IsList)
					{
						gfLexCtx_AddToken(iCtx, UL_TOK_List_Opener, nullptr, -1,-1);
						gfTypeList_PushValue(_TypeStack, UL_TOK_List);

						gfTypeList_PushValue(_TypeStack, UL_TOK_Expect_ListItem);
					}
					continue;
				}
				case UL_TOK_List      :
				{
					if(_IsList)
					{
						if(_IsListItem)
						{
							gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Opener, nullptr, -1,-1);
							gfTypeList_PushValue(_TypeStack, UL_TOK_ListItem);

							gfTypeList_PushValue(_TypeStack, UL_TOK_Expect_Next_Atom);
						}
						else if(_IsListItemDelim)
						{
							gfTypeList_PushValue(_TypeStack, UL_TOK_Expect_ListItem);
							break;
						}
						else if(_IsAtomDelim)
						{
							gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Error, nullptr, -1,-1);
							break;
						}
					}
					else
					{
						gfTypeList_Pop(_TypeStack);
						gfLexCtx_AddToken(iCtx, UL_TOK_List_Closer, nullptr, -1,-1);
					}
					continue;
				}
				case UL_TOK_Expect_ListItem      :
				{
					gfTypeList_Pop(_TypeStack);

					if(_IsList)
					{
						if(_IsListItem || _IsAtomDelim)
						{
							gfTypeList_PushValue(_TypeStack, UL_TOK_ListItem);
							gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Opener, nullptr, -1,-1);

							if(_IsAtomDelim)
							{
								gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Error, nullptr, -1,-1);
							}

							gfTypeList_PushValue(_TypeStack, UL_TOK_Expect_Next_Atom);

							continue;
						}
						else if(_IsListItemDelim)
						{
							gfLexCtx_AddToken(iCtx, UL_TOK_List_Error, nullptr, -1,-1);
						}
					}
					else
					{
						gfLexCtx_AddToken(iCtx, UL_TOK_List_Error, nullptr, -1,-1);
					}
					continue;
				}
				case UL_TOK_Expect_Next_Atom      :
				{
					if(_IsAtomDelim)
					{
						gfTypeList_PushValue(_TypeStack, UL_TOK_Atom_Delimiter);
						break;
					}
					if(_IsListItemDelim)
					{
						gfTypeList_Pop(_TypeStack);
						gfTypeList_Pop(_TypeStack);
						gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Closer, nullptr, -1,-1);

						gfTypeList_PushValue(_TypeStack, UL_TOK_Expect_ListItem);
						break;
					}
					else
					{
						if(_IsListItem)
						{
							if(_IsBlockOpener)
							{
								switch(iTokenType)
								{
									case UL_TOK_Parenthesis_Opener : gfTypeList_PushValue(_TypeStack, UL_TOK_Parenthesis); break;
									case UL_TOK_Bracket_Opener     : gfTypeList_PushValue(_TypeStack, UL_TOK_Bracket);     break;
									case UL_TOK_Brace_Opener       : gfTypeList_PushValue(_TypeStack, UL_TOK_Brace);       break;
								}
							}
							else if(iTokenType == UL_TOK_String && !iIsTerminated)
							{
								gfTypeList_PushValue(_TypeStack, UL_TOK_String);
							}

							break;
						}
						else
						{
							if(_IsBlockOpener)
							{
								gfTypeList_PushValue(_TypeStack, UL_TOK_Block);
								break;
							}
							else if(_IsBlockCloser || _IsExpressionDelimiter)
							{
								gfTypeList_Pop(_TypeStack);
								gfTypeList_Pop(_TypeStack);
								gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Closer, nullptr, -1,-1);

								continue;
							}
						}
					}
					continue;
				}
				case UL_TOK_Atom_Delimiter      :
				{
					if(_IsListItem)
					{
						gfTypeList_Pop(_TypeStack);

						if(_IsAtomDelim)
						{
							gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Error, nullptr, -1,-1);
						}
					}
					else
					{
						if(_IsListItemDelim || _IsExpressionDelimiter || _IsBlockCloser)
						{
							gfTypeList_Pop(_TypeStack);
							gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Error, nullptr, -1,-1);
						}
					}
					continue;
				}
				case UL_TOK_Whitespace :
				{
					gfTypeList_Pop(_TypeStack);

					cStackIsEmpty = _TypeStack->Count == 0;
					cStackTop     = cStackIsEmpty ? UL_TOK_Undefined : *gfTypeList_Peek(_TypeStack);

					if(cStackTop == UL_TOK_Expect_Next_Atom)
					{
						if(_IsAtomDelim)
						{
							gfTypeList_PushValue(_TypeStack, UL_TOK_Atom_Delimiter);
							break;
						}

						gfTypeList_Pop(_TypeStack);

						gfTypeList_Pop(_TypeStack);
						gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Closer, nullptr, -1,-1);

						if(!_IsListItemDelim)
						{
							gfTypeList_Pop(_TypeStack);
							gfLexCtx_AddToken(iCtx, UL_TOK_List_Closer, nullptr, -1,-1);
						}

						continue;
					}
					continue;
				}
				case UL_TOK_Parenthesis : {if(_IsBlockCloser && iTokenType != UL_TOK_Parenthesis_Closer) {gfLexCtx_AddToken(iCtx, UL_TOK_Block_Error, nullptr, -1,-1);} goto LABEL_UL_TOK_Block;}
				case UL_TOK_Bracket     : {if(_IsBlockCloser && iTokenType != UL_TOK_Bracket_Closer)     {gfLexCtx_AddToken(iCtx, UL_TOK_Block_Error, nullptr, -1,-1);} goto LABEL_UL_TOK_Block;}
				case UL_TOK_Brace       : {if(_IsBlockCloser && iTokenType != UL_TOK_Brace_Closer)       {gfLexCtx_AddToken(iCtx, UL_TOK_Block_Error, nullptr, -1,-1);} goto LABEL_UL_TOK_Block;}
				case UL_TOK_Block       :
				{
					LABEL_UL_TOK_Block:
					
					if(_IsExpressionItem)
					{
						goto LABEL_UL_TOK_Undefined;
					}
					else if(_IsExpressionDelimiter)
					{
						break;
					}
					else
					{
						gfTypeList_Pop(_TypeStack);

						break;
					}
				}
				

				case UL_TOK_String     :
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
	ULTokenStack* _SyntaxTree = gfTList_Create(dULTokenStackInitialCapacity);
	
	
	for(_TTc = iCtx->Tokens->Count, cTi = 0; cTi < _TTc; cTi ++)
	{
		ULToken* cToken = &iCtx->Tokens->Items[cTi];
		ULTokenType cTokenType = cToken->Type;
		
		ULToken* _SyntaxTop = _SyntaxTree->Count != 0 ? gfTList_Peek(_SyntaxTree) : nullptr;
		ULToken* _SyntaxTopTargetRef;

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
					(_SyntaxTop->Type == UL_TOK_Parenthesis_Opener && cToken->Type != UL_TOK_Parenthesis_Closer) ||
					(_SyntaxTop->Type == UL_TOK_Bracket_Opener     && cToken->Type != UL_TOK_Bracket_Closer)     ||
					(_SyntaxTop->Type == UL_TOK_Brace_Opener       && cToken->Type != UL_TOK_Brace_Closer)       ||

					(_SyntaxTop->Type == UL_TOK_Expression_Opener  && cToken->Type != UL_TOK_Expression_Closer)  ||
					(_SyntaxTop->Type == UL_TOK_List_Opener        && cToken->Type != UL_TOK_List_Closer)        ||
					(_SyntaxTop->Type == UL_TOK_ListItem_Opener    && cToken->Type != UL_TOK_ListItem_Closer)
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
		/*if(gfTList_Peek(_SyntaxTree)->Type == UL_TOK_List_Opener)
		{
			///ULToken* _ListItem = gfTList_Pop(_SyntaxTree);
			
		}
		else
		*/
		WTFE("Syntax tree incosistent");
	}
	gfTList_Destroy(_SyntaxTree);
}
void          gfLexCtx_LinkTokens       (LexerContext* iCtx, ULToken* irOpenerToken, ULToken* irCloserToken)
{
	///~~ Note that we are re-using ULTokenList as token stack. It contains a value array - not references;
	///~~ ULTokenRefList can be used as reference stack, but it needs additional (duplicated) functions;
	
	//ULToken* _Opener = &iCtx->Tokens->Items[iOpenerToken->Id];
	//ULToken* _Closer = &iCtx->Tokens->Items[iCloserToken->Id];
	//
	irCloserToken->PairId = irOpenerToken->Id;
	irOpenerToken->PairId = irCloserToken->Id;
	
	//_Opener->PairId = _Closer _Closer->Id;
	//_Closer->PairId = _Opener->Id;
}



ULTokenList*  gfLexCtx_ParseBuffer      (LexerContext* iCtx)
{
	if(iCtx->Tokens == nullptr) iCtx->Tokens = gfTList_Create(dULTokenListInitialCapacity);
	if(iCtx->Tokens->Count != 0) WTFE("Empty list expected");
	
	
	while(iCtx->State->Position < iCtx->BufferLength)
	{
		//ULTokenList* cTokenGroup = gfLexCtx_ParseNextTokens(iCtx);
		gfLexCtx_ParseNextTokens(iCtx);
		
		///gfLexCtx_ProcessSyntax(iNewTokens, ioTokens, (iCtx.State as GenericCodeLexerState).TokenStack);
		///ioTokens.AddRange(iNewTokens);
			
		//gfLexCtx_AddToken(iCtx, UL_TOK_Garbage, nullptr, -1,-1);
		
		
		
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
	///ULTokenList* oTokens;
	
	LexerState* _LexerState = iCtx->State;
	
	
	if(_LexerState->Position >= iCtx->BufferLength) WTFE("return null?");
	
	
	///oTokens = gfTList_Create(1);
	
	
	{
		wchar_t cChar = iCtx->Buffer[_LexerState->Position];
		wchar_t nChar = _LexerState->Position < iCtx->BufferLength - 1 ? iCtx->Buffer[_LexerState->Position + 1] : L'\0';///~~ new Char()??;

		
		if(_LexerState->TypeStack->Count > 0 && gfTypeList_PeekValue(_LexerState->TypeStack) == UL_TOK_String)
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
						gfLexCtx_AddToken(iCtx, UL_TOK_Word, nullptr, -1,0);
						//{
							///_Word->Offset = iCtx->
						//}
						///ULToken* _Word = gfTList_Alloc(oTokens, 1);
						//char& _ = iCtx->Buffer[0];
						
						//gfToken_Init();
						//ULToken* _Word = gfToken_Init
						///gfTList_Push(new TokenInfo(TokenType.Word, iCtx.Offset, ++iCtx.Offset));
					}
				}
				else
				{
					///if      (cChar == '#' || AEULLexer.IsDecimalDigit(cChar) || ((cChar == '+' || cChar == '-') && ((iCtx.Buffer.Length - iCtx.Offset > 1) && AEULLexer.IsDecimalDigit(iCtx.Buffer[iCtx.Offset + 1]))))
					if      (cChar == L'#' || gfLexCtx_IsDecimalDigit(cChar) || ((cChar == L'+' || cChar == L'-') && ((iCtx->BufferLength - iCtx->State->Position > 1) && gfLexCtx_IsDecimalDigit(iCtx->Buffer[iCtx->State->Position + 1]))))
					{
						gfLexCtx_ParseNumber(iCtx);
					}
					else if (cChar == L'"')               gfLexCtx_ParseString(iCtx);
					///~~else if (cChar == L"'")          gfLexCtx_ParseChar(iCtx);
					else if (gfLexCtx_IsBracket(cChar))   gfLexCtx_ParseBracket(iCtx);
					
					else if (cChar == L';')               gfLexCtx_AddToken(iCtx, UL_TOK_Expression_Delimiter, nullptr, -1,1);
					//else if (cChar == ":")               oTokens.Add(new Token(TokenType.Colon,     cChar, iCtx.Position, iCtx.Position++));
					else if (cChar == L',')               gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Delimiter,   nullptr, -1,1);
					else if (cChar == L'\'')              gfLexCtx_AddToken(iCtx, UL_TOK_Atom_Delimiter, nullptr, -1,1);

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
		
		if(_LexerState->TypeStack->Count != 0)
		{
			WTFE("Not expected");
		}
		//if(_LexerState->TypeStack->Count > 1)
		//{
		//	gfLexCtx_AddSyntaxTokens(iCtx, UL_TOK_File_Closer, true);
		//	
		//	
		//	///gfLexCtx_AddToken(iCtx, UL_TOK_F, nullptr, -1,0);
		//}
		//
		//if(_LexerState->TypeStack->Count == 1 && *gfTypeList_Peek(_LexerState->TypeStack) == UL_TOK_File)
		//{
		//	gfLexCtx_AddToken(iCtx, UL_TOK_File_Closer, nullptr, -1, -1);
		//	gfTypeList_Pop(_LexerState->TypeStack);
		//	
		//	HERE;
		//}
		//else
		//{
		//	WTFE("Not expected");
		//}
		
		HERE;
	}
	///if(_LexerState->Position >= iCtx->BufferLength) WTFE("?");
}
//void          gfLexCtx_SkipNonTokens    (LexerContext* iCtx)
//{
//	while(iCtx->State->Position < iCtx->BufferLength)
//	{
//		wchar_t cChar = iCtx->Buffer[iCtx->State->Position];
//		if(cChar != L' ' && cChar != L'\t' && cChar != L'\r' && cChar != L'\n') break;
//
//		iCtx->State->Position ++;
//	}
//}


void          gfLexCtx_ParseWhitespaces (LexerContext* iCtx)
{
	ULTokenType _TokenType; 

	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 1;
	wchar_t _FoundWs = iCtx->Buffer[_BegOffs];
	
	while(_EndOffs < iCtx->BufferLength)
	{
		wchar_t cChar = iCtx->Buffer[_EndOffs];

		///if(!AEULLexer.IsWhitespace(cChar)) break;
		if(!gfLexCtx_IsWhitespace(cChar)) break;
		if(cChar != _FoundWs)             break;
		if(gfLexCtx_IsNewline(cChar))     break;

		_EndOffs ++;
	}
	
	iCtx->State->Position = _EndOffs; ///~~ ? 2022.06.11;

	_TokenType; switch(_FoundWs)
	{
		case ' '  : _TokenType = UL_TOK_Space;   break;
		case '\t' : _TokenType = UL_TOK_Tab;     break;
		
		//case "\r" : _TokenType = "RETURN";  break;
		
		///case '\r' :  return;/// nullptr;
		case '\r' : _TokenType = UL_TOK_Space;   break;
		case '\n' : _TokenType = UL_TOK_NewLine; break;

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
	
	ULTokenType _TokenType; switch(cChar)
	{
		case L'(' : _TokenType = UL_TOK_Parenthesis_Opener; break;
		case L')' : _TokenType = UL_TOK_Parenthesis_Closer; break;

		case L'[' : _TokenType = UL_TOK_Bracket_Opener; break;
		case L']' : _TokenType = UL_TOK_Bracket_Closer; break;

		case L'{' : _TokenType = UL_TOK_Brace_Opener; break;
		case L'}' : _TokenType = UL_TOK_Brace_Closer; break;
		
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
	gfLexCtx_AddToken(iCtx, UL_TOK_Number, nullptr, _BegOffs, _EndOffs); 
}
void          gfLexCtx_ParseHostObject  (LexerContext* iCtx)
{
	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 1; while(_EndOffs < iCtx->BufferLength && gfLexCtx_IsIdentChar(iCtx->Buffer[_EndOffs])) _EndOffs ++;

	gfLexCtx_AddToken(iCtx, UL_TOK_Host_Object, nullptr, _BegOffs, _EndOffs); 
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
	gfLexCtx_AddToken(iCtx, UL_TOK_Type, nullptr, _BegOffs, _EndOffs); 
}
void          gfLexCtx_ParseList        (LexerContext* iCtx)
{
	STOP;
}
void          gfLexCtx_ParseIdentifier  (LexerContext* iCtx)
{
	wchar_t* _Str; int _StrLen;
	ULTokenType _Type;
	
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


	_Type  = UL_TOK_Undefined;
	{
		bool _IsMultiChar    = _StrLen > 1;

		wchar_t _FstChar        = _Str[0];
		wchar_t _SndChar        = _IsMultiChar ? _Str[1] : L'X'; //??

		bool _IsLinkedIdent  = _IsMultiChar && (_FstChar >= L'A' && _FstChar <= L'Z');


		if(_IsLinkedIdent)
		{
			///_Type = TokenType.Identifier;
			_Type = UL_TOK_Member_Ident;
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
			bool _IsFstCharPfx = _FstChar == L'_' || _FstChar == L':' || _FstChar == L'^' || _IsFstLowC;///(_FstChar >= L'a' && _FstChar <= L'z');


			if (_IsFstCharPfx && _IsFollowingIdent) switch(_FstChar)
			{
				//case "_" :                       _Type = "LOCV"; break;
				//case "c" : case "p" : case "n" : _Type = "CYCV"; break;
				
				case L'@' : _Type = UL_TOK_Instruction;    break;
				case L':' : _Type = UL_TOK_Label;          break;
				case L'^' : _Type = UL_TOK_Pointer;        break;
				case L'g' : _Type = UL_TOK_Global_Ident;    break;
				///case L'f' : _Type = UL_TOK_FunctionIdent; break;
				case L'r' : _Type = UL_TOK_Reference_Ident; break;
				case L'i' : _Type = UL_TOK_Input_Ident;     break;
				case L'o' : _Type = UL_TOK_Output_Ident;    break;

				default  : _Type = UL_TOK_Local_Ident;     break;
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
				else _Type = UL_TOK_Word;
				
			}
		}
	}

	gfLexCtx_AddToken(iCtx, _Type, nullptr, _BegOffs, _EndOffs);
	///return new TokenInfo(_Type, _BegOffs, _EndOffs){Value = iCtx.Buffer.Substring(_BegOffs, _EndOffs - _BegOffs)};
}
void          gfLexCtx_ParseString      (LexerContext* iCtx)
{
	ULTokenTypeStack* _Stack = iCtx->State->TypeStack;
	
	bool _IsStringOpenBefore = _Stack->Count != 0 && _Stack->Items[_Stack->Count - 1] == UL_TOK_String;

	int _BegOffs = iCtx->State->Position;
	int _EndOffs = iCtx->State->Position;
	
	bool pIsEscapeChar = false, cIsTerminated = false; int _BufferLen = (int)iCtx->BufferLength, cOffs = _BegOffs + (_IsStringOpenBefore ? 0 : 1); while(true)
	{
		if(cOffs < _BufferLen)
		{
			wchar_t cChar = iCtx->Buffer[cOffs];

			if(pIsEscapeChar) pIsEscapeChar = false;
			else
			{
				if(cChar == L'"')  cIsTerminated = true;
				if(cChar == L'\\') pIsEscapeChar = true;
			}
			
			_EndOffs = ++ cOffs;
			
			if(cIsTerminated) break;
		}
		else
		{
			cIsTerminated = false;
			_EndOffs = cOffs;

			break;
		}
	}
	iCtx->State->Position = cIsTerminated ? _EndOffs : _BufferLen;
	
	gfLexCtx_AddToken(iCtx, UL_TOK_String, nullptr, _BegOffs, cIsTerminated ? _EndOffs : _BegOffs - 1);
	
}
///void          gfLexCtx_ParseString      (LexerContext* iCtx)
//{
//	ULTokenTypeStack* _Stack = iCtx->State->TypeStack;
//	
//	bool _IsStringOpen = _Stack->Count != 0 && _Stack->Items[_Stack->Count - 1] == UL_TOK_String;
//
//	int _BegOffs = iCtx->State->Position;
//	int _EndOffs = _BegOffs;
//	
//
//	while(_EndOffs < iCtx->BufferLength)
//	{
//		int cBegSearch = _EndOffs + (_IsStringOpen ? 0 : 1);
//		_EndOffs = cBegSearch + wcsindexof(&iCtx->Buffer[cBegSearch], (int)iCtx->BufferLength - _EndOffs, L"\"",  1);
//		
//		///_EndOffs = iCtx.Buffer.IndexOf('"', _EndOffs + (_IsStringOpen ? 0 : 1));
//
//		if(_EndOffs == -1)
//		{
//			_EndOffs = (int)iCtx->BufferLength;
//			//_EndOffs = _BegOffs - 1; //~~ mark token as unterminated;
//
//			//_Is
//			_IsStringOpen = true;
//
//			//if(!_LexerState.IsStringOpen) _LexerState.IsStringOpen = true;
//
//			///if(!_IsStringOpen) _IsStringOpen = true;
//			//else throw new Exception("???");
//
//			break;
//		}
//		else
//		{
//			bool _IsQuoteCancelled = false;
//			{
//				///for(int cPos = _EndOffs - 1; cPos >= 0; cPos --, _IsQuoteCancelled =! _IsQuoteCancelled)
//				///{
//				///    if(iCtx.Buffer[cPos] != '\\') break;
//					
//				///}
//				int cPos; for(cPos = _EndOffs - 1; cPos >= 0; cPos --)
//				{
//					if(iCtx->Buffer[cPos] == L'\\')
//					{
//						_IsQuoteCancelled =! _IsQuoteCancelled;
//					}
//					else break;
//				}
//			}
//
//			_EndOffs ++;
//
//			if(_IsQuoteCancelled) {/**_LexerState.IsStringOpen = false;*/   continue;}
//			///else                  {if(_LexerState.IsStringOpen == true) _LexerState.IsStringOpen = false; break;}
//			else                  {if(_IsStringOpen) _IsStringOpen = false; break;}
//		}
//	}
//
//	///iCtx.Offset = _EndOffs;
//
//	//return new TokenInfo(TokenType.String, _BegOffs, _IsStringOpen ? _BegOffs - 1 : _EndOffs){Value = iCtx.Buffer.Substring(_BegOffs + 1, _EndOffs - _BegOffs - (_IsStringOpen ? 1 : 2))};
//	///return new TokenInfo(TokenType.String, _BegOffs, _IsStringOpen ? _BegOffs - 1 : _EndOffs){Value = iCtx.Buffer.Substring(_BegOffs + 1, Math.Max(0, _EndOffs - _BegOffs - 2))};
//	
//	gfLexCtx_AddToken(iCtx, UL_TOK_String, nullptr, _BegOffs, (_IsStringOpen ? _BegOffs - 1 : _EndOffs));
//}
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
			gfLexCtx_AddToken(iCtx, UL_TOK_Comment, nullptr, _BegOffs, _EndOffs);
		}
		else
		{
			gfLexCtx_AddToken(iCtx, UL_TOK_Garbage, nullptr, _BegOffs, _EndOffs);
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


struct $__UL_SyntaxNode__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
void          gfSNode_Init              (ULSyntaxNode* irNode, ULSyntaxNodeType iNodeType, ULToken* iToken, int iChildCapacity)
{
	irNode->Type   = iNodeType;
	irNode->Token_ = iToken;
	irNode->Role   = UL_SEM_Unknown;
	
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

struct $__UL_SyntaxParser__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};

ULSyntaxParser* gfParser_Create      (ULContext* iULC)
{
	ULSyntaxNode* _Root;

	ULSyntaxParser* oParser = malloc(sizeof(ULSyntaxParser));
	{
		oParser->ULC      = iULC;
		oParser->Position = -1;
		oParser->Tokens   = nullptr;///iLexerContext->Tokens;
		
		oParser->RootNode  = nullptr;
		oParser->TopNode   = nullptr;
		//_Root = malloc(sizeof(ULSyntaxNode));
		//{
		//	gfSNode_Init(_Root, UL_SYN_FunctionBlock, nullptr, 1);
		///*	_Root->Parent = nullptr;///~~ ???;
		//
		//
		//	_Root->Type  = UL_SYN_FunctionBlock;
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
void            gfParser_Destroy     (ULSyntaxParser* irParser)
{
	///if(irParser->Tokens  != nullptr) gfTList_Destroy(irParser->Tokens);
	if(irParser->TopNode != nullptr)
	{
		gfSNList_Destroy(irParser->TopNode->Children);
		free(irParser->TopNode);
	}
}
void            gfParser_Init        (ULSyntaxParser* irParser, ULTokenList* iTokens)
{
	ULSyntaxNode* _Root;

	irParser->Position = -1;
	irParser->Tokens   = iTokens;
	
	irParser->RootNode = nullptr;
	irParser->TopNode = nullptr;
	
	///_Root = malloc(sizeof(ULSyntaxNode));
	///{
	///	gfSNode_Init(_Root, UL_SYN_FunctionBlock, nullptr, 1);
	///}
	///irParser->RootNode = _Root;
	///irParser->TopNode  = _Root;
}
ULSyntaxNode*   gfParser_ParseTokens (ULSyntaxParser* irParser)
{
	gfParser_Init(irParser, irParser->ULC->Lexer->Tokens);
	
	/*this.Position ++;

			return (this.Position < this.Tokens.Count);
	*/		
	while(irParser->Position ++, irParser->Position < irParser->Tokens->Count)
	{
		ULToken* cToken = &irParser->Tokens->Items[irParser->Position];

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
			ULSyntaxNodeType _NodeType; switch(cToken->Type)
			{
				case UL_TOK_ListItem_Opener    : _NodeType = UL_SYN_ListItem;       break;
				case UL_TOK_List_Opener        : _NodeType = UL_SYN_List;           break;
				case UL_TOK_Expression_Opener  : _NodeType = UL_SYN_Expression;     break;

				case UL_TOK_Parenthesis_Opener : _NodeType = UL_SYN_ParenthesisBlock; break;
				case UL_TOK_Bracket_Opener     : _NodeType = UL_SYN_BracketBlock;     break;
				case UL_TOK_Brace_Opener       : _NodeType = UL_SYN_BraceBlock;       break;

				//case UL_TOK_Root_Opener        : _NodeType = UL_SYN_Root;     break;

				///default : printf("Token (type = %d) is skipped", cToken->Type); break;
				default : WTFE("Opener token does not match any of syntax node type"); break;
			}
			gfParser_OpenNode(irParser, _NodeType, cToken);
		}
		else
		{
			switch(cToken->Type)
			{
				case UL_TOK_ListItem_Closer    : gfParser_CloseNode(irParser, UL_SYN_ListItem);     break;
				case UL_TOK_List_Closer        : gfParser_CloseNode(irParser, UL_SYN_List);         break;
				case UL_TOK_Expression_Closer  : gfParser_CloseNode(irParser, UL_SYN_Expression);   break;
				
				case UL_TOK_Parenthesis_Closer : gfParser_CloseNode(irParser, UL_SYN_ParenthesisBlock); break;
				case UL_TOK_Bracket_Closer     : gfParser_CloseNode(irParser, UL_SYN_BracketBlock);     break;
				case UL_TOK_Brace_Closer       : gfParser_CloseNode(irParser, UL_SYN_BraceBlock);       break;

				///case UL_TOK_Root_Closer        : gfParser_CloseNode(irParser, UL_SYN_Root);          break;
				
				//case TokenType.ListItemCloser :
				//{

				//    break;
				//}

				case UL_TOK_Instruction   : gfParser_AddNode(irParser, UL_SYN_Instruction, cToken); break;
				case UL_TOK_Label         : gfParser_AddNode(irParser, UL_SYN_Label,       cToken); break;
				case UL_TOK_Pointer       : gfParser_AddNode(irParser, UL_SYN_Pointer,     cToken); break;
				case UL_TOK_Word          : gfParser_AddNode(irParser, UL_SYN_Word,        cToken); break;
				case UL_TOK_Number        : gfParser_AddNode(irParser, UL_SYN_Number,      cToken); break;
				///case UL_TOK_InvalidNumber : gfParser_AddNode(irParser, UL_SYN_NumInvalid, cToken); break;

				///case UL_TOK_Int32   : gfParser_AddNode(irParser, UL_SYN_NumInt32,   cToken); break;
				///case UL_TOK_Float32 : gfParser_AddNode(irParser, UL_SYN_NumFloat32, cToken); break;
				///case UL_TOK_Float64 : gfParser_AddNode(irParser, UL_SYN_NumFloat64, cToken); break;
				


				case UL_TOK_String           : gfParser_AddNode(irParser, UL_SYN_String, cToken); break;

				
				case UL_TOK_Host_Object       : gfParser_AddNode(irParser, UL_SYN_HostObject,            cToken); break;
				
				case UL_TOK_Identifier        : gfParser_AddNode(irParser, UL_SYN_Identifier,            cToken); break;
				case UL_TOK_Local_Ident       : gfParser_AddNode(irParser, UL_SYN_LocalIdentifier,       cToken); break;
				case UL_TOK_Global_Ident      : gfParser_AddNode(irParser, UL_SYN_GlobalIdentifier,      cToken); break;
				case UL_TOK_Reference_Ident   : gfParser_AddNode(irParser, UL_SYN_ReferenceIdentifier,   cToken); break;
				case UL_TOK_Input_Ident       : gfParser_AddNode(irParser, UL_SYN_InputIdentifier,       cToken); break;
				case UL_TOK_Output_Ident      : gfParser_AddNode(irParser, UL_SYN_OutputIdentifier,      cToken); break;
				case UL_TOK_Member_Ident      : gfParser_AddNode(irParser, UL_SYN_MemberIdentifier,      cToken); break;
				///case UL_TOK_Function_Ident : gfParser_AddNode(irParser, UL_SYN_FunctionIdentifier,    cToken); break;

				case UL_TOK_Type              : gfParser_AddNode(irParser, UL_SYN_Type,                  cToken); break;
				case UL_TOK_Packed_Tuple      : gfParser_AddNode(irParser, UL_SYN_PackedTuple,           cToken); break;

				//case TokenType.Space:


				///default : throw new Exception("WTFE");
				///default : Console.WriteLine("Token '" + cToken.Type.ToString() + "' was not handled"); break;
			}
		}
	}
	return irParser->TopNode;
}

void            gfParser_AddNode     (ULSyntaxParser* irParser, ULSyntaxNodeType iNodeType, ULToken* iToken)
{
	ULSyntaxNode* _Node;

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
ULSyntaxNode*   gfParser_OpenNode    (ULSyntaxParser* irParser, ULSyntaxNodeType iNodeType, ULToken* iToken)
{
	ULSyntaxNode* oNode;
	
	irParser->TopNode->Children;
	
	if(iNodeType == UL_SYN_List)
	{
		HERE;
	}

	if(irParser->RootNode == nullptr)
	{
		assert(irParser->TopNode == nullptr);
		
		oNode = malloc(sizeof(ULSyntaxNode));
		
		irParser->RootNode = oNode;
		irParser->TopNode  = oNode;
		
		gfSNode_Init(oNode, UL_SYN_Root, iToken, 1);
		{
			
		}
		
		HERE;
	}
	/*else
	{*/
		if(irParser->TopNode->Children == nullptr)
		{
			irParser->TopNode->Children        = gfSNList_Create(1);
			irParser->TopNode->Children->Owner = irParser->TopNode;
		}
		
		oNode = gfSNList_Allocate(irParser->TopNode->Children, 1);
		{
			gfSNode_Init(oNode, iNodeType, iToken, 0);
			oNode->Parent = irParser->TopNode;
		}
		
		irParser->TopNode = oNode;
	//}
	return oNode;
}
ULSyntaxNode*   gfParser_CloseNode   (ULSyntaxParser* irParser, ULSyntaxNodeType iNodeType)
{
	if(irParser->TopNode == nullptr) WTFE("?");
	if(irParser->TopNode->Type != iNodeType) WTFE("Unmatched pair token");
	
	/*if(iNodeType == UL_SYN_List)
	{
		HERE;
	}*/
	irParser->TopNode = irParser->TopNode->Parent;
	
	return irParser->TopNode;
}


struct $__UL_002__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_003__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_004__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_005__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_006__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_007__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};

void gfULSyntaxNodeType_ToString(char* irStr, ULSyntaxNodeType iValue)
{
	switch (iValue)
	{
		///#define STR(r) case r: memcpy(irErrStr, L#r, sizeof(wchar_t) * 64); return
		#define STR(r) case r: memcpy(irStr, #r, sizeof(char) * 64); return

		STR(UL_SYN_Unknown);

		STR(UL_SYN_Root);
		STR(UL_SYN_Expression);
		STR(UL_SYN_List);
		STR(UL_SYN_ListItem);
		
		STR(UL_SYN_ParenthesisBlock);
		STR(UL_SYN_BracketBlock);
		STR(UL_SYN_BraceBlock);
		
		STR(UL_SYN_Value__Begin);
			STR(UL_SYN_Number);
			STR(UL_SYN_NumInvalid);
			STR(UL_SYN_NumInt32);
			STR(UL_SYN_NumFloat32);
			STR(UL_SYN_NumFloat64);
			
			STR(UL_SYN_String);
		STR(UL_SYN_Value__End);

		STR(UL_SYN_Identifier);
			STR(UL_SYN_Instruction);
			STR(UL_SYN_Label);
			STR(UL_SYN_Pointer);
			STR(UL_SYN_ReferenceIdentifier);
			STR(UL_SYN_InputIdentifier);
			STR(UL_SYN_OutputIdentifier);
			STR(UL_SYN_LocalIdentifier);
			STR(UL_SYN_GlobalIdentifier);
			STR(UL_SYN_MemberIdentifier);
			//UL_SYN_FunctionIdentifier);
			STR(UL_SYN_Word);

			STR(UL_SYN_HostObject);
			STR(UL_SYN_Type);
			STR(UL_SYN_PackedTuple);
		
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