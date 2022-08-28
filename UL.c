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



ULContext* gfULCtx_Create           (wchar_t* iBuffer, size_t iBufferLength, bool iDoBuildAST)
{
	ULContext* oULC = malloc(sizeof(ULContext));
	{
		oULC->Lexer = gfLexCtx_Create(oULC, iBuffer, iBufferLength);
		oULC->Parser = gfParser_Create(oULC);
		
		if(iDoBuildAST)
		{
			gfLexCtx_ParseBuffer(oULC->Lexer);
			gfLexCtx_ProcessPairs(oULC->Lexer);
			
			gfParser_ParseTokens(oULC->Parser);
		}
	}
	return oULC;
}
void       gfULCtx_Destroy          (ULContext* iULC)
{
	if(iULC->Lexer  != nullptr) gfLexCtx_Destroy(iULC->Lexer);
	if(iULC->Parser != nullptr) gfParser_Destroy(iULC->Parser);
	
	free(iULC);
}




struct $__UL_Token__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};



int wcsindexof(wchar_t* iBuffer, int iBufferLen, wchar_t* iValue, int iValueLen)
{
	int cBufCi;
	for(cBufCi = 0; cBufCi < iBufferLen; cBufCi ++)
	{
		if(iBuffer[cBufCi] == iValue[0])
		{
			bool cIsFound = true; int cValCi; for(cValCi = 0; cValCi < iValueLen; cValCi ++)
			{
				int cBufOffs = cBufCi + cValCi;
				
				if(cBufOffs >= iBufferLen || iValue[cValCi] != iBuffer[cBufOffs])
				{
					cIsFound = false;
					break;
				}
			}
			if(cIsFound) return cBufCi;
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

///bool gfToken_IsOpener      (ULTokenType iTokenType){return iTokenType == UL_TOK_ListItem_Opener || iTokenType == UL_TOK_List_Opener || iTokenType == UL_TOK_Expression_Opener || iTokenType == UL_TOK_Parenthesis_Opener || iTokenType == UL_TOK_Bracket_Opener || iTokenType == UL_TOK_Brace_Opener || iTokenType == UL_TOK_File_Opener;}
///bool gfToken_IsCloser      (ULTokenType iTokenType){return iTokenType == UL_TOK_ListItem_Closer || iTokenType == UL_TOK_List_Closer || iTokenType == UL_TOK_Expression_Closer || iTokenType == UL_TOK_Parenthesis_Closer || iTokenType == UL_TOK_Bracket_Closer || iTokenType == UL_TOK_Brace_Closer || iTokenType == UL_TOK_File_Closer;}

bool gfToken_IsOpener      (ULTokenType iTokenType){return (iTokenType >= UL_TOK_Root_Opener && iTokenType <= UL_TOK_ListItem_Opener) || iTokenType == UL_TOK_Brace_Opener || iTokenType == UL_TOK_Parenthesis_Opener || iTokenType == UL_TOK_Bracket_Opener;}
bool gfToken_IsCloser      (ULTokenType iTokenType){return (iTokenType >= UL_TOK_Root_Closer && iTokenType <= UL_TOK_ListItem_Closer) || iTokenType == UL_TOK_Brace_Closer || iTokenType == UL_TOK_Parenthesis_Closer || iTokenType == UL_TOK_Bracket_Closer;}


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
void              gfTypeList_Destroy  (ULTokenTypeList* iList)
{
	if(iList->Items != nullptr) free(iList->Items);
	
	free(iList);
}
ULTokenType*      gfTypeList_Allocate (ULTokenTypeList* iList, int iCount)
{
	gfTypeList_Reserve(iList, iList->Count + iCount);
	
	iList->Count += iCount;
	
	return &iList->Items[iList->Count - 1];
}


void              gfTypeList_PushValue    (ULTokenTypeList* iList, ULTokenType iItem)
{
	ULTokenType* _Item = gfTypeList_Allocate(iList,1);
	
	*_Item = iItem;
	///iList->Items[iList->Count - 1] = iItem;
}
ULTokenType       gfTypeList_PopValue      (ULTokenTypeList* iList)
{
	if(iList->Count <= 0) return nullptr;
	
	return iList->Items[-- iList->Count];
}
ULTokenType*      gfTypeList_Pop      (ULTokenTypeList* iList)
{
	if(iList->Count <= 0) return nullptr;
	
	return &iList->Items[-- iList->Count];
}
ULTokenType       gfTypeList_PeekValue (ULTokenTypeList* iList)
{
	if(iList->Count <= 0) return nullptr;
	
	return iList->Items[iList->Count - 1];
}
ULTokenType*      gfTypeList_Peek     (ULTokenTypeList* iList)
{
	if(iList->Count <= 0) return nullptr;
	
	return &iList->Items[iList->Count - 1];
}

void              gfTypeList_Reserve  (ULTokenTypeList* iList, int iCapacity)
{
	if(iCapacity <= iList->Capacity) return;
	
	
	{
		size_t _OldSize, _NewSize;
		ULTokenType *_ItemsOldPtr = iList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(iList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dULTokenTypeListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(ULTokenType); //, _Size;
		{
			
			
			///iList->Capacity
			/*if(_NeedSize < iList->Capacity * 2)
			{
				
			}*/
		}
		
		///_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		
		_ItemsNewPtr = iList->Items == nullptr ? malloc(_NeedSize) : realloc(iList->Items, _NeedSize);
		
		///_NewSize = _msize(_ItemsNewPtr);
		
		
		
		assert(_ItemsNewPtr != nullptr);
		
		
		iList->Items    = _ItemsNewPtr;
		iList->Capacity = _NeedCapacity;
	}
}
void              gfTypeList_Clear    (ULTokenTypeList* iList)
{
	iList->Count = 0;
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
void              gfTList_Destroy  (ULTokenList* iList)
{
	if(iList->Items != nullptr) free(iList->Items);
	
	free(iList);
}
ULToken*          gfTList_Allocate (ULTokenList* iList, int iCount)
{
	gfTList_Reserve(iList, iList->Count + iCount);
	
	iList->Count += iCount;
	
	return &iList->Items[iList->Count - 1];
}

void              gfTList_Push     (ULTokenList* iList, ULToken* iItem)
{
	ULToken* _Token = gfTList_Allocate(iList,1);
	
	iList->Items[iList->Count - 1] = *iItem;
	//if(iList->Count + 1 >= iList->Capacity)
	//{
	//	gfTList_Reserve(iList, iList->Capacity == 0 ? 1 : (int)(iList->Capacity * dULTokenListCapacityMultiplier));
	//}
	//iList->Items[iList->Count ++] = *iToken;
}
ULToken*          gfTList_Pop      (ULTokenList* iList)
{
	ULToken* oToken;

	if(iList->Count <= 0) return nullptr;
	
	
	return &iList->Items[-- iList->Count];
}
ULToken*          gfTList_Peek     (ULTokenList* iList)
{
	if(iList->Count <= 0) return nullptr;
	
	return &iList->Items[iList->Count - 1];
}

void              gfTList_Reserve  (ULTokenList* iList, int iCapacity)
{
	if(iCapacity <= iList->Capacity) return;
	
	
	{
		size_t _OldSize, _NewSize;
		ULToken *_ItemsOldPtr = iList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(iList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dULTokenListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(ULToken); //, _Size;
		{
			
			
			///iList->Capacity
			/*if(_NeedSize < iList->Capacity * 2)
			{
				
			}*/
		}
		
		///_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		
		_ItemsNewPtr = iList->Items == nullptr ? malloc(_NeedSize) : realloc(iList->Items, _NeedSize);
		
		///_NewSize = _msize(_ItemsNewPtr);
		
		
		
		assert(_ItemsNewPtr != nullptr);
		
		
		iList->Items    = _ItemsNewPtr;
		iList->Capacity = _NeedCapacity;
	
		
	}
	///iList->Items = malloc(iCapacity);
	
	//if(_ItemsOldPtr != nullptr)
	//{
	//	memcpy(iList->Items, _ItemsOldPtr, iList->Count);
	//	free(_ItemsOldPtr);
	//}
}
void              gfTList_Clear    (ULTokenList* iList)
{
	iList->Count = 0;
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
void              gfSNList_Destroy  (ULSyntaxNodeList* iList)
{
	if(iList->Items != nullptr) free(iList->Items);
	
	free(iList);
}
ULSyntaxNode*     gfSNList_Allocate (ULSyntaxNodeList* iList, int iCount)
{
	gfSNList_Reserve(iList, iList->Count + iCount);
	
	/*if(iCount != 1)
	{
		HERE;
	}*/
	iList->Count += iCount;
	
	return &iList->Items[iList->Count - 1];
}

///void               gfSNList_Push     (ULSyntaxNodeList* iList, ULSyntaxNode* iItem)
//{
//	ULSyntaxNode* _Token = gfSNList_Allocate(iList,1);
//	
//	iList->Items[iList->Count - 1] = *iItem;
//}
ULSyntaxNode*     gfSNList_Pop      (ULSyntaxNodeList* iList)
{
	ULSyntaxNode* oToken;

	if(iList->Count <= 0) return nullptr;
	
	return &iList->Items[-- iList->Count];
}
ULSyntaxNode*     gfSNList_Peek     (ULSyntaxNodeList* iList)
{
	if(iList->Count <= 0) return nullptr;
	
	return &iList->Items[iList->Count - 1];
}

void              gfSNList_Reserve  (ULSyntaxNodeList* iList, int iCapacity)
{
	if(iCapacity <= iList->Capacity) return;
	
	{
		size_t _OldSize, _NewSize;
		ULSyntaxNode *_ItemsOldPtr = iList->Items, *_ItemsNewPtr;//, _ItemsNewPtr;
		///_NewPtr = realloc((void*)(iList->Items), iCapacity);
		
		int _NeedCapacity = (int)(iCapacity * dULSyntaxNodeListCapacityMultiplier);
		size_t _NeedSize = _NeedCapacity * sizeof(ULSyntaxNode); //, _Size;
		
		///_OldSize = _ItemsOldPtr != nullptr ? _msize(_ItemsOldPtr) : 0;
		
		_ItemsNewPtr = iList->Items == nullptr ? malloc(_NeedSize) : realloc(iList->Items, _NeedSize);
		
		///_NewSize = _msize(_ItemsNewPtr);
			
		assert(_ItemsNewPtr != nullptr);
		
		
		iList->Items    = _ItemsNewPtr;
		iList->Capacity = _NeedCapacity;
		
		
		
		if(true)
		{
			int _PPc = iList->Count, cPi = 0; for(; cPi < _PPc; cPi ++)
			{
				ULSyntaxNode* cParentNode = &iList->Items[cPi];
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
void              gfSNList_Clear    (ULSyntaxNodeList* iList)
{
	iList->Count = 0;
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
		
		_Token->Id       = iCtx->Tokens->Count - 1;
		_Token->Offset   = _IsBegOffsSpecified ? iBegOffs : iCtx->State->Position;
		_Token->Length   = iEndOffsOrLength != -1 ?  (_IsBegOffsSpecified ? iEndOffsOrLength - iBegOffs : iEndOffsOrLength) : 0;/// iEndOffsOrLength;
		
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
	
	if(_Token->Length != 0)
	{
		iCtx->State->Position = _Token->Offset + _Token->Length;
	}
}
void          gfLexCtx_AddSyntaxTokens  (LexerContext* iCtx, ULTokenType iTokenType, bool iIsTerminated)
{
	ULTokenTypeStack* _TypeStack = iCtx->State->TypeStack;

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
void          gfLexCtx_LinkTokens       (LexerContext* iCtx, ULToken* iOpenerToken, ULToken* iCloserToken)
{
	///~~ Note that we are re-using ULTokenList as token stack. It contains a value array - not references;
	
	iCloserToken->PairId = iOpenerToken->Id;
	iOpenerToken->PairId = iCloserToken->Id;
}



ULTokenList*  gfLexCtx_ParseBuffer      (LexerContext* iCtx)
{
	if(iCtx->Tokens == nullptr) iCtx->Tokens = gfTList_Create(dULTokenListInitialCapacity);
	if(iCtx->Tokens->Count != 0) WTFE("Empty list expected");
	
	
	while(iCtx->State->Position < iCtx->BufferLength)
	{
		gfLexCtx_ParseNextTokens(iCtx);
	}
	
	HERE;
	return iCtx->Tokens;
}

void          gfLexCtx_ParseNextTokens  (LexerContext* iCtx)
{
	LexerState* _LexerState = iCtx->State;

	if(_LexerState->Position >= iCtx->BufferLength) WTFE("return null?");
	{
		wchar_t cChar = iCtx->Buffer[_LexerState->Position];
		wchar_t nChar = _LexerState->Position < iCtx->BufferLength - 1 ? iCtx->Buffer[_LexerState->Position + 1] : L'\0';///~~ new Char()??;

		if(_LexerState->TypeStack->Count > 0 && gfTypeList_PeekValue(_LexerState->TypeStack) == UL_TOK_String)
		{
			gfLexCtx_ParseString(iCtx);
		}
		else
		{
			if(gfLexCtx_IsWhitespace(cChar)) gfLexCtx_ParseWhitespaces(iCtx);
			else
			{
				if(cChar == L'/' && (nChar == L'/' || nChar == L'*'))
				{
					if   (nChar == L'/') gfLexCtx_ParseGarbage(iCtx);
					else
					{
						gfLexCtx_AddToken(iCtx, UL_TOK_Word, nullptr, -1,0);
					}
				}
				else
				{
					if      (cChar == L'#' || gfLexCtx_IsDecimalDigit(cChar) || ((cChar == L'+' || cChar == L'-') && ((iCtx->BufferLength - iCtx->State->Position > 1) && gfLexCtx_IsDecimalDigit(iCtx->Buffer[iCtx->State->Position + 1]))))
					{
						gfLexCtx_ParseNumber(iCtx);
					}
					else if (cChar == L'"')               gfLexCtx_ParseString(iCtx);
					else if (gfLexCtx_IsBracket(cChar))   gfLexCtx_ParseBracket(iCtx);
					
					else if (cChar == L';')               gfLexCtx_AddToken(iCtx, UL_TOK_Expression_Delimiter, nullptr, -1,1);
					else if (cChar == L',')               gfLexCtx_AddToken(iCtx, UL_TOK_ListItem_Delimiter,   nullptr, -1,1);
					else if (cChar == L'\'')              gfLexCtx_AddToken(iCtx, UL_TOK_Atom_Delimiter, nullptr, -1,1);

					else
					{
						gfLexCtx_ParseIdentifier(iCtx);
					}
				}
			}
		}
	}
	
	if(iCtx->Buffer[_LexerState->Position] == '\0') ///~~ note that position has already been incremented;
	{
		int cTi; for(cTi = 0; cTi < _LexerState->TypeStack->Count; cTi ++)
		{
			ULTokenType cType = _LexerState->TypeStack->Items[cTi];
			
			if(cType == UL_TOK_Brace || cType == UL_TOK_Parenthesis || cType == UL_TOK_Bracket)
			{
				WTFE("Syntax error: block not closed");
			}
		}
		gfLexCtx_AddSyntaxTokens(iCtx, UL_TOK_Root_Closer, true);
	}
}

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
//void          gfLexCtx_ParseHostObject  (LexerContext* iCtx)
//{
//	int _BegOffs = iCtx->State->Position;
//	int _EndOffs = _BegOffs + 1; while(_EndOffs < iCtx->BufferLength && gfLexCtx_IsIdentChar(iCtx->Buffer[_EndOffs])) _EndOffs ++;
//
//	gfLexCtx_AddToken(iCtx, UL_TOK_Host_Object, nullptr, _BegOffs, _EndOffs); 
//}
//void          gfLexCtx_ParseType        (LexerContext* iCtx)
//{
//	int _BegOffs = iCtx->State->Position;
//	int _EndOffs = _BegOffs + 1;
//
//
//
//	while(_EndOffs < iCtx->BufferLength)
//	{
//		wchar_t cChar = iCtx->Buffer[_EndOffs];
//		//    if(
//		
//		if((iswalpha(cChar) && iswlower(cChar)) || gfLexCtx_IsNumberChar(cChar)) _EndOffs ++;
//		else if(iswupper(cChar)) WTFE("?");
//		else break;
//	}
//	gfLexCtx_AddToken(iCtx, UL_TOK_Type, nullptr, _BegOffs, _EndOffs); 
//}
void          gfLexCtx_ParseList        (LexerContext* iCtx)
{
	STOP;
}
void          gfLexCtx_ParseIdentifier  (LexerContext* iCtx)
{
	wchar_t* _Str; int _StrLen; int cCi;
	ULTokenType _Type;
	
	
	int _BegOffs = iCtx->State->Position;
	int _EndOffs = _BegOffs + 1;
	
	
	int _IsConvIdent = 0; for(cCi = 1 ;; cCi ++)
	{
		wchar_t cChar = iCtx->Buffer[_EndOffs];

		if(_EndOffs >= iCtx->BufferLength || !gfLexCtx_IsIdentChar(cChar)) break;

		if(_IsConvIdent == 0)
		{
			if((gfLexCtx_IsUpperCase(cChar) || gfLexCtx_IsDecimalDigit(cChar)) && cCi <= 2)
			{
				_IsConvIdent = +1;
			}
			else if(!gfLexCtx_IsLowerCase(cChar))
			{
				_IsConvIdent = -1;
			}
		}
		_EndOffs ++;
	}

	_Str = &iCtx->Buffer[_BegOffs];///, _EndOffs - _BegOffs);
	{
		int _1 = wcsindexof(_Str, _EndOffs - _BegOffs, L"//", 2);
		int _2 = wcsindexof(_Str, _EndOffs - _BegOffs, L"/*", 2);

		if      (_1 != -1) _EndOffs = _BegOffs + _1;
		else if (_2 != -1) _EndOffs = _BegOffs + _2;

		_StrLen = _EndOffs - _BegOffs;
	}
	
	iCtx->State->Position = _EndOffs;

	_Type  = UL_TOK_Undefined;
	{
		bool _IsMultiChar    = _StrLen > 1;

		wchar_t _FstChar        = _Str[0];
		wchar_t _SndChar        = _IsMultiChar ? _Str[1] : L'X'; //??

		bool _IsLinkedIdent  = _IsMultiChar && gfLexCtx_IsUpperCase(_FstChar);


		if(_IsLinkedIdent)
		{
			_Type = UL_TOK_Member_Ident;
		}
		else
		{
			bool _IsFstLowC    = _FstChar >= L'a' && _FstChar <= L'z';
			///var _IsFstPrefix = _FstChar == '_' || _FstChar == '@'|| _FstChar == '^';
			
			//bool _IsSndUppC    = _IsMultiChar && gfLexCtx_IsUpperCase(_SndChar >= L'A' && _SndChar <= L'Z');
			//bool _IsSndDigit   = _IsMultiChar && (_SndChar >= L'0' && _SndChar <= L'9');

			/*bool _IsFollowingIdent = _IsSndUppC || _IsSndDigit;*/
			bool _IsFollowingIdent = _IsConvIdent == +1;
			
			bool _IsFstCharPfx = _FstChar == L'_' || _FstChar == L':' || _FstChar == L'^' || gfLexCtx_IsLowerCase(_FstChar);///(_FstChar >= L'a' && _FstChar <= L'z');


			if (_IsFstCharPfx && _IsFollowingIdent) switch(_FstChar)
			{
				case L'@' : _Type = UL_TOK_Instruction;     break;
				case L':' : _Type = UL_TOK_Label;           break;
				case L'^' : _Type = UL_TOK_Pointer;         break;
				case L'g' : _Type = UL_TOK_Global_Ident;    break;
				case L'i' : _Type = _SndChar == 'r'
				                  ? UL_TOK_Reference_Ident
				                  : UL_TOK_Input_Ident;
				                                            break;
				case L'o' : _Type = UL_TOK_Output_Ident;    break;

				default   : _Type = UL_TOK_Local_Ident;     break;
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
		///~~ single-line garbage;

		_BegOffs = iCtx->State->Position;/// + 2;
		_EndOffs = _BegOffs; while(_EndOffs < iCtx->BufferLength && !gfLexCtx_IsNewline(iCtx->Buffer[_EndOffs])) _EndOffs ++;

		if(_FwdS[0] == L'/' && _FwdS[1] == L'/' && _FwdS[2] == L'~' && _FwdS[3] == L'~')
		{
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
bool          gfLexCtx_IsAlpha          (wchar_t iChar){return gfLexCtx_IsUpperCase(iChar) || gfLexCtx_IsLowerCase(iChar) || iChar == L'_';}
bool          gfLexCtx_IsUpperCase      (wchar_t iChar){return iChar >= L'A' && iChar <= L'Z';}
bool          gfLexCtx_IsLowerCase      (wchar_t iChar){return iChar >= L'a' && iChar <= L'z';}
bool          gfLexCtx_IsIdentChar      (wchar_t iChar){return gfLexCtx_IsAlpha(iChar) || gfLexCtx_IsDecimalDigit(iChar) || gfLexCtx_IsSpecial(iChar);}
bool          gfLexCtx_IsPunctuation    (wchar_t iChar){return iChar == L'\'' || iChar == L',' || iChar == L';';}
bool          gfLexCtx_IsBracket        (wchar_t iChar){return iChar == L'(' || iChar == L')' || iChar == L'[' || iChar == L']' || iChar == L'{' || iChar == L'}';}
bool          gfLexCtx_IsQuote          (wchar_t iChar){return iChar == L'"';}// || iChar == '\''},
bool          gfLexCtx_IsSpecial        (wchar_t iChar){return !gfLexCtx_IsBracket(iChar) && !gfLexCtx_IsQuote(iChar) && !gfLexCtx_IsWhitespace(iChar) && !gfLexCtx_IsPunctuation(iChar);}


struct $__UL_SyntaxNode__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
void          gfSNode_Init              (ULSyntaxNode* iNode, ULSyntaxNodeType iNodeType, ULToken* iToken, int iChildCapacity)
{
	iNode->Type   = iNodeType;
	iNode->Token_ = iToken;
	iNode->Role   = UL_SEM_Unknown;
	
	iNode->Parent = nullptr;
	
	if(iToken != nullptr)
	{
		iNode->BegToken = iToken->Id;
		iNode->EndToken = iToken->PairId;
	}
	else
	{
		iNode->BegToken = -1;
		iNode->EndToken = -1;
	}
	
	if(iChildCapacity != 0) {iNode->Children = gfSNList_Create(iChildCapacity); iNode->Children->Owner = iNode;}
	else                     iNode->Children = nullptr;
}

struct $__UL_SyntaxParser__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};

ULSyntaxParser* gfParser_Create      (ULContext* iULC)
{
	ULSyntaxNode* _Root;

	ULSyntaxParser* oParser = malloc(sizeof(ULSyntaxParser));
	{
		oParser->ULC       = iULC;
		oParser->Position  = -1;
		oParser->Tokens    = nullptr;
		
		oParser->RootNode  = nullptr;
		oParser->TopNode   = nullptr;
	}
	return oParser;
}
void            gfParser_Destroy     (ULSyntaxParser* iParser)
{
	if(iParser->TopNode != nullptr)
	{
		gfSNList_Destroy(iParser->TopNode->Children);
		free(iParser->TopNode);
	}
}
void            gfParser_Init        (ULSyntaxParser* iParser, ULTokenList* iTokens)
{
	ULSyntaxNode* _Root;

	iParser->Position = -1;
	iParser->Tokens   = iTokens;
	
	iParser->RootNode = nullptr;
	iParser->TopNode  = nullptr;
}
ULSyntaxNode*   gfParser_ParseTokens (ULSyntaxParser* iParser)
{
	gfParser_Init(iParser, iParser->ULC->Lexer->Tokens);
	
	while(iParser->Position ++, iParser->Position < iParser->Tokens->Count)
	{
		ULToken* cToken = &iParser->Tokens->Items[iParser->Position];

		bool cIsTrash  = gfToken_IsGarbageSTX (cToken->Type);
		bool cIsOpener = gfToken_IsOpener     (cToken->Type);
		
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
			gfParser_OpenNode(iParser, _NodeType, cToken);
		}
		else
		{
			switch(cToken->Type)
			{
				case UL_TOK_ListItem_Closer    : gfParser_CloseNode(iParser, UL_SYN_ListItem);     break;
				case UL_TOK_List_Closer        : gfParser_CloseNode(iParser, UL_SYN_List);         break;
				case UL_TOK_Expression_Closer  : gfParser_CloseNode(iParser, UL_SYN_Expression);   break;
				
				case UL_TOK_Parenthesis_Closer : gfParser_CloseNode(iParser, UL_SYN_ParenthesisBlock); break;
				case UL_TOK_Bracket_Closer     : gfParser_CloseNode(iParser, UL_SYN_BracketBlock);     break;
				case UL_TOK_Brace_Closer       : gfParser_CloseNode(iParser, UL_SYN_BraceBlock);       break;

				///case UL_TOK_Root_Closer        : gfParser_CloseNode(iParser, UL_SYN_Root);          break;

				case UL_TOK_Instruction   : gfParser_AddNode(iParser, UL_SYN_Instruction, cToken); break;
				case UL_TOK_Label         : gfParser_AddNode(iParser, UL_SYN_Label,       cToken); break;
				case UL_TOK_Pointer       : gfParser_AddNode(iParser, UL_SYN_Pointer,     cToken); break;
				case UL_TOK_Word          : gfParser_AddNode(iParser, UL_SYN_Word,        cToken); break;
				case UL_TOK_Number        : gfParser_AddNode(iParser, UL_SYN_Number,      cToken); break;
				///case UL_TOK_InvalidNumber : gfParser_AddNode(iParser, UL_SYN_NumInvalid, cToken); break;

				///case UL_TOK_Int32   : gfParser_AddNode(iParser, UL_SYN_NumInt32,   cToken); break;
				///case UL_TOK_Float32 : gfParser_AddNode(iParser, UL_SYN_NumFloat32, cToken); break;
				///case UL_TOK_Float64 : gfParser_AddNode(iParser, UL_SYN_NumFloat64, cToken); break;

				case UL_TOK_String           : gfParser_AddNode(iParser, UL_SYN_String, cToken); break;
				case UL_TOK_Local_Ident       : gfParser_AddNode(iParser, UL_SYN_LocalIdentifier,       cToken); break;
				case UL_TOK_Global_Ident      : gfParser_AddNode(iParser, UL_SYN_GlobalIdentifier,      cToken); break;
				case UL_TOK_Reference_Ident   : gfParser_AddNode(iParser, UL_SYN_ReferenceIdentifier,   cToken); break;
				case UL_TOK_Input_Ident       : gfParser_AddNode(iParser, UL_SYN_InputIdentifier,       cToken); break;
				case UL_TOK_Output_Ident      : gfParser_AddNode(iParser, UL_SYN_OutputIdentifier,      cToken); break;
				case UL_TOK_Member_Ident      : gfParser_AddNode(iParser, UL_SYN_MemberIdentifier,      cToken); break;

				default : STOP;
			}
		}
	}
	return iParser->TopNode;
}

void            gfParser_AddNode     (ULSyntaxParser* iParser, ULSyntaxNodeType iNodeType, ULToken* iToken)
{
	ULSyntaxNode* _Node;

	if(iParser->TopNode->Children == nullptr)
	{
		iParser->TopNode->Children        = gfSNList_Create(1);
		iParser->TopNode->Children->Owner = iParser->TopNode;
	}
	
	_Node = gfSNList_Allocate(iParser->TopNode->Children, 1);
	{
		gfSNode_Init(_Node, iNodeType, iToken, 0);
		_Node->Parent = iParser->TopNode;
	}
}
ULSyntaxNode*   gfParser_OpenNode    (ULSyntaxParser* iParser, ULSyntaxNodeType iNodeType, ULToken* iToken)
{
	ULSyntaxNode* oNode;
	
	iParser->TopNode->Children;
	
	if(iNodeType == UL_SYN_List)
	{
		HERE;
	}

	if(iParser->RootNode == nullptr)
	{
		assert(iParser->TopNode == nullptr);
		
		oNode = malloc(sizeof(ULSyntaxNode));
		
		iParser->RootNode = oNode;
		iParser->TopNode  = oNode;
		
		gfSNode_Init(oNode, UL_SYN_Root, iToken, 1);
		{
			
		}
		
		HERE;
	}
	/*else
	{*/
		if(iParser->TopNode->Children == nullptr)
		{
			iParser->TopNode->Children        = gfSNList_Create(1);
			iParser->TopNode->Children->Owner = iParser->TopNode;
		}
		
		oNode = gfSNList_Allocate(iParser->TopNode->Children, 1);
		{
			gfSNode_Init(oNode, iNodeType, iToken, 0);
			oNode->Parent = iParser->TopNode;
		}
		
		iParser->TopNode = oNode;
	//}
	return oNode;
}
ULSyntaxNode*   gfParser_CloseNode   (ULSyntaxParser* iParser, ULSyntaxNodeType iNodeType)
{
	if(iParser->TopNode == nullptr) WTFE("?");
	if(iParser->TopNode->Type != iNodeType) WTFE("Unmatched pair token");
	
	/*if(iNodeType == UL_SYN_List)
	{
		HERE;
	}*/
	iParser->TopNode = iParser->TopNode->Parent;
	
	return iParser->TopNode;
}


struct $__UL_002__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_003__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_004__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_005__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_006__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};
struct $__UL_007__ooooooooooooooooooooooooooooooooooooooooooooooooooooooo{int i;};

void gfULSyntaxNodeType_ToString(char* iStr, ULSyntaxNodeType iValue)
{
	switch (iValue)
	{
		#define STR(diR) case diR: memcpy(iStr, #diR, sizeof(char) * 64); return

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