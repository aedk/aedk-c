#ifndef AEDL_H
#define AEDL_H

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef __cplusplus
#define nullptr 0

#define true 1
#define false 0

typedef        char           bool;
typedef        unsigned short wchar_t;
#endif

int wcsindexof(wchar_t* dataset, int datasetLength, wchar_t* target, int targetLen);

typedef struct DLContext       DLContext;

typedef struct DLTokenTypeList DLTokenTypeList;
typedef struct DLTokenTypeList DLTokenTypeStack;

typedef struct DLToken         DLToken;
typedef struct DLTokenList     DLTokenList;
typedef struct DLTokenList     DLTokenStack;
//typedef struct DLTokenRefList DLTokenRefList;
//typedef struct DLTokenRefList DLTokenRefStack;
typedef struct LexerState      LexerState;
typedef struct LexerContext    LexerContext;

typedef struct DLSyntaxNode     DLSyntaxNode;
typedef struct DLSyntaxNodeList DLSyntaxNodeList;

typedef struct DLSyntaxParser   DLSyntaxParser;

#define dDLTokenValueMaxLength 8
///#define dDLTokenStackMaxSize   256
#define dDLTokenListInitialCapacity    1024*8
#define dDLTokenStackInitialCapacity   16
#define dDLTokenTypeListCapacityMultiplier  1.2f
#define dDLTokenListCapacityMultiplier      1.2f
#define dDLSyntaxNodeListCapacityMultiplier 1.2f



struct DLContext
{
	LexerContext*   Lexer;
	DLSyntaxParser* Parser;
	
};

DLContext* gfDLCtx_Create           (wchar_t* iBuffer, int iBufferLength, bool iIsComplete);
void       gfDLCtx_Destroy          (DLContext* iDLC);



typedef enum DLTokenType
{
	DL_TOK_Undefined,
	DL_TOK_Pseudotoken,
	DL_TOK_ListError,

	DL_TOK_Whitespace,
	DL_TOK_Space,
	DL_TOK_Tab,
	DL_TOK_NewLine,
	
	DL_TOK_Garbage,
	DL_TOK_Comment,
	DL_TOK_MultilineCommentOpener, ///~~ NI;
	DL_TOK_MultilineCommentCloser, ///~~ NI;
	
	DL_TOK_String,
	DL_TOK_Character,
	/** ... define all your literals here or modify literal token range in the lexer */
	
	//Int32,
	//Float32,
	//Float64,

	//InvalidNumber,
	DL_TOK_Number,

	


	DL_TOK_Expression_Delimiter,      /// "1";"2";
	DL_TOK_Expression_Item_Delimiter,  /// "1" "2";
	DL_TOK_ListItem_Delimiter,        /// "1","2";
	DL_TOK_Identifier_Delimiter,      /// iItem.Name
	
	
	DL_TOK_Identifiers__Begin, ///~~ "var _IsIdentifier = ... "
		DL_TOK_Instruction,
		DL_TOK_Label,
		DL_TOK_Pointer,
		
		DL_TOK_Identifier,
		DL_TOK_Reference_Ident,
		DL_TOK_Input_Ident,
		DL_TOK_Output_Ident,
		DL_TOK_Local_Ident,
		DL_TOK_Global_Ident,
		DL_TOK_Member_Ident,
		DL_TOK_Word,
		DL_TOK_Host_Object,
		DL_TOK_Packed_Tuple,
		DL_TOK_Type,
	DL_TOK_Identifiers__End,
		//EndOF

	DL_TOK_BwdOpd,
	DL_TOK_FwdOpd,

	//DL_TOK_Parenthesis, DL_TOK_ParenthesisOpener, DL_TOK_ParenthesisCloser,
	//DL_TOK_Bracket,     DL_TOK_BracketOpener,     DL_TOK_BracketCloser,
	//DL_TOK_Brace,       DL_TOK_BraceOpener,       DL_TOK_BraceCloser,

	//DL_TOK_File,        DL_TOK_FileOpener,        DL_TOK_FileCloser,
	//DL_TOK_Block,       DL_TOK_BlockOpener,       DL_TOK_BlockCloser,
	//DL_TOK_Expression,  DL_TOK_ExpressionOpener,  DL_TOK_ExpressionCloser,
	//DL_TOK_List,        DL_TOK_ListOpener,        DL_TOK_ListCloser,
	//DL_TOK_ListItem,    DL_TOK_ListItemOpener,    DL_TOK_ListItemCloser,
	//
	
	///~~ new, buggy;
	//DL_TOK_Syntax_Tokens__Begin,
	//	DL_TOK_File,        DL_TOK_Block,        DL_TOK_Expression,        DL_TOK_List,        DL_TOK_ListItem,        DL_TOK_Brace,        DL_TOK_Parenthesis,        DL_TOK_Bracket,
	//	DL_TOK_File_Opener, DL_TOK_Block_Opener, DL_TOK_Expression_Opener, DL_TOK_List_Opener, DL_TOK_ListItem_Opener, DL_TOK_Brace_Opener, DL_TOK_Parenthesis_Opener, DL_TOK_Bracket_Opener,
	//	DL_TOK_File_Closer, DL_TOK_Block_Closer, DL_TOK_Expression_Closer, DL_TOK_List_Closer, DL_TOK_ListItem_Closer, DL_TOK_Brace_Closer, DL_TOK_Parenthesis_Closer, DL_TOK_Bracket_Closer,

	//	DL_TOK_Expect_ListItem,              /// List opened
	//	DL_TOK_Expect_Next_ListItem,         /// List opened, delimiter found?
	//	DL_TOK_Expect_ListItem_Continuation, /// List item added
	//DL_TOK_Syntax_Tokens__End,
	
	DL_TOK_All_Syntax_Tokens__Begin,
		DL_TOK_Brace,        DL_TOK_Parenthesis,        DL_TOK_Bracket,
		DL_TOK_Brace_Opener, DL_TOK_Parenthesis_Opener, DL_TOK_Bracket_Opener,
		DL_TOK_Brace_Closer, DL_TOK_Parenthesis_Closer, DL_TOK_Bracket_Closer,
		
		DL_TOK_Special_Syntax_Tokens__Begin,
			DL_TOK_File,        DL_TOK_Block,        DL_TOK_Expression,        DL_TOK_List,        DL_TOK_ListItem,        
			DL_TOK_File_Opener, DL_TOK_Block_Opener, DL_TOK_Expression_Opener, DL_TOK_List_Opener, DL_TOK_ListItem_Opener, 
			DL_TOK_File_Closer, DL_TOK_Block_Closer, DL_TOK_Expression_Closer, DL_TOK_List_Closer, DL_TOK_ListItem_Closer, 
			
			DL_TOK_Expect_ListItem,              /// List opened
			DL_TOK_Expect_Next_ListItem,         /// List opened, delimiter found?
			DL_TOK_Expect_ListItem_Continuation, /// List item added
			
		DL_TOK_Special_Syntax_Tokens__End,
	DL_TOK_All_Syntax_Tokens__End,
	
	
	
}
DLTokenType;


struct DLTokenTypeList
{
	DLTokenType* Items;
	int          Count, Capacity;
};

struct DLToken
{
	wchar_t Value[dDLTokenValueMaxLength];
	DLTokenType Type;
	
	int Id,Offset,Length;
	int Fragment;
	///DLToken* Pair;
	int PairId;
	
/*	bool IsPaired      ();
	bool IsAligned     ();
	bool IsZeroLength  ();
	bool IsTerminated  ();
	bool IsOpener      ();
	bool IsCloser      ();
	bool IsPseudotoken ();
	bool IsWhitespace  ();
	bool IsGarbage     ();*/
};

struct DLTokenList
{
	DLToken*  Items;
	int       Count, Capacity;
};
//struct DLTokenRefList
//{
//	DLToken** Items;
//	int       Count, Capacity;
//};


DLTokenTypeList* gfTypeList_Create    (int iCapacity);
void             gfTypeList_Destroy   (DLTokenTypeList* irList);

DLTokenType*     gfTypeList_Allocate  (DLTokenTypeList* irList, int iCount);

void             gfTypeList_Push      (DLTokenTypeList* irList, DLTokenType* iItem);
void             gfTypeList_PushValue (DLTokenTypeList* irList, DLTokenType iItem);
DLTokenType*     gfTypeList_Pop       (DLTokenTypeList* irList);
DLTokenType      gfTypeList_PopValue  (DLTokenTypeList* irList);
DLTokenType*     gfTypeList_Peek      (DLTokenTypeList* irList);
DLTokenType      gfTypeList_PeekValue (DLTokenTypeList* irList);

void             gfTypeList_Reserve   (DLTokenTypeList* irList, int iCapacity);
void             gfTypeList_Clear     (DLTokenTypeList* irList);



DLTokenList* gfTList_Create   (int iCapacity);
void         gfTList_Destroy  (DLTokenList* irList);

DLToken*     gfTList_Allocate (DLTokenList* irList, int iCount);

void         gfTList_Push     (DLTokenList* irList, DLToken* iItem);
DLToken*     gfTList_Pop      (DLTokenList* irList);
DLToken*     gfTList_Peek     (DLTokenList* irList);

void         gfTList_Reserve  (DLTokenList* irList, int iCapacity);
void         gfTList_Clear    (DLTokenList* irList);




DLSyntaxNodeList* gfSNList_Create   (int iCapacity);
void              gfSNList_Destroy  (DLSyntaxNodeList* irList);

DLSyntaxNode*     gfSNList_Allocate (DLSyntaxNodeList* irList, int iCount);

void              gfSNList_Push     (DLSyntaxNodeList* irList, DLSyntaxNode* iItem);
DLSyntaxNode*     gfSNList_Pop      (DLSyntaxNodeList* irList);
DLSyntaxNode*     gfSNList_Peek     (DLSyntaxNodeList* irList);

void              gfSNList_Reserve  (DLSyntaxNodeList* irList, int iCapacity);
void              gfSNList_Clear    (DLSyntaxNodeList* irList);


//
///DLTokenRefList* gfTRList_Create   ();
//void            gfTRList_Destroy  (DLTokenRefList* irList);
//void            gfTRList_Push     (DLTokenList* irList, DLToken* iToken);
//DLToken*        gfTRList_Pop      (DLTokenList* irList);
//DLToken*        gfTRList_Peek     (DLTokenList* irList);

//
//struct LexerState
//{

//};


struct LexerState
{
	int               Position;
	DLTokenTypeStack* TypeStack;
};
struct LexerContext
{
	DLContext*  DLC;
	wchar_t*    Buffer;
	int         BufferLength;

	DLTokenList*  Tokens;
	LexerState*   State;
	//DLToken* TokenStack[dDLTokenStackMaxSize];
	//int      TokenStackSize;
	
	
};

void gfToken_Init    (DLToken* iToken);
void gfToken_Cleanup (DLToken* iToken);

bool gfToken_IsAligned     (DLToken* iToken);
bool gfToken_IsZeroLength  (DLToken* iToken);
bool gfToken_IsTerminated  (DLToken* iToken);
bool gfToken_IsPaired      (DLTokenType iTokenType);
bool gfToken_IsOpener      (DLTokenType iTokenType);
bool gfToken_IsCloser      (DLTokenType iTokenType);
bool gfToken_IsWhitespace  (DLTokenType iTokenType);
bool gfToken_IsGarbage     (DLTokenType iTokenType);


LexerState*  gfLState_Create      ();
void         gfLState_Destroy     (LexerState* iState);



LexerContext* gfLexCtx_Create           (DLContext* iDLCtx, wchar_t* iBuffer, int iBufferLength);
void          gfLexCtx_Destroy          (LexerContext* iCtx);

DLToken*      gfLexCtx_AllocateToken    (LexerContext* iCtx);
DLToken*      gfLexCtx_AllocateTokens   (LexerContext* iCtx, int iCount);
///void          gfLexCtx_AddTokens        (LexerContext* iCtx, DLTokenList* iNewTokens, DLTokenList* irTokens);
void          gfLexCtx_AddToken         (LexerContext* iCtx, DLTokenType iType, void* iValue, int iBegOffs, int iEndOffsOrLength);
void          gfLexCtx_AddSyntaxTokens  (LexerContext* iCtx, DLTokenType iTokenType, bool iIsTerminated);
void          gfLexCtx_ProcessPairs     (LexerContext* iCtx);///, DLTokenList* iTokens);
void          gfLexCtx_LinkTokens       (LexerContext* iCtx, DLToken* irOpenerToken, DLToken* irCloserToken);

DLTokenList*  gfLexCtx_ParseBuffer      (LexerContext* iCtx);

void          gfLexCtx_ParseNextTokens  (LexerContext* iCtx);
void          gfLexCtx_SkipNonTokens    (LexerContext* iCtx);

void          gfLexCtx_ParseWhitespaces (LexerContext* iCtx);
void          gfLexCtx_ParseBracket     (LexerContext* iCtx);
void          gfLexCtx_ParseNumber      (LexerContext* iCtx);
void          gfLexCtx_ParseHostObject  (LexerContext* iCtx);
void          gfLexCtx_ParseType        (LexerContext* iCtx);
void          gfLexCtx_ParseList        (LexerContext* iCtx);
void          gfLexCtx_ParseIdentifier  (LexerContext* iCtx);
void          gfLexCtx_ParseString      (LexerContext* iCtx);
void          gfLexCtx_ParseChar        (LexerContext* iCtx);
void          gfLexCtx_ParseGarbage     (LexerContext* iCtx);

bool          gfLexCtx_IsNewline      (wchar_t iChar);
bool          gfLexCtx_IsWhitespace   (wchar_t iChar);
bool          gfLexCtx_IsDecimalDigit (wchar_t iChar);
bool          gfLexCtx_IsNumberChar   (wchar_t iChar);
bool          gfLexCtx_IsAlpha        (wchar_t iChar);
bool          gfLexCtx_IsIdentChar    (wchar_t iChar);
bool          gfLexCtx_IsPunctuation  (wchar_t iChar);
bool          gfLexCtx_IsBracket      (wchar_t iChar);
bool          gfLexCtx_IsQuote        (wchar_t iChar);
bool          gfLexCtx_IsSpecial      (wchar_t iChar);




typedef enum DLSyntaxNodeType
{
	DL_SYN_Unknown,

		DL_SYN_Expression,
		
		DL_SYN_List,
		DL_SYN_ListItem,
		
		DL_SYN_GroupingBlock,
		DL_SYN_ArgumentBlock,
		DL_SYN_FunctionBlock,///CallableBlock(!), Subroutine, ProcedureBlock
		DL_SYN_File,
		
		DL_SYN_Value__Begin,
			DL_SYN_Number,
			DL_SYN_NumInvalid,
			DL_SYN_NumInt32,
			DL_SYN_NumFloat32,
			DL_SYN_NumFloat64,
			
			DL_SYN_String,
		DL_SYN_Value__End,
		
		DL_SYN_Identifiers__Begin,
			DL_SYN_Identifier,
			DL_SYN_Instruction,
			DL_SYN_Label,
			DL_SYN_Pointer,
			DL_SYN_ReferenceIdentifier,
			DL_SYN_InputIdentifier,
			DL_SYN_OutputIdentifier,
			DL_SYN_LocalIdentifier,
			DL_SYN_GlobalIdentifier,
			DL_SYN_MemberIdentifier,
			//DL_SYN_FunctionIdentifier,
			DL_SYN_Word,
		
			DL_SYN_HostObject,
			DL_SYN_Type,
			DL_SYN_PackedTuple,
		DL_SYN_Identifiers__End,
}
DLSyntaxNodeType;

typedef enum DLSemanticRole
{
	DL_SEM_Unknown = 0,

		///~~ Expression
		
		DL_SEM_ExpInstruction,
		DL_SEM_ExpInstructionLabelDefinition,
		DL_SEM___ExpInstructionLabelReference,
		DL_SEM_ExpCustom,

		DL_SEM_ExpOneList,

		DL_SEM_ExpReturn,
		DL_SEM_ExpSignature,
			DL_SEM_ExpSignatureInput,
			DL_SEM_ExpSignatureOutput,
		DL_SEM_ExpAssignment,
		

		DL_SEM_ExpSemanticHandlerDefinition,
		DL_SEM_ExpInstructionHandlerDefinition,
		DL_SEM_ExpWordHandlerDefinition,

		//ExpInterpreterInstructionDefinition,
		//ExpInterpreterEventHandlerDefinition,
		//ExpWordDefinition,


		///~~ List
		DL_SEM_ListOneInstruction,
		DL_SEM_ListQueriesOnly,

		DL_SEM_ListLiteralsOnly,
		DL_SEM_ListWordsOnly,
		DL_SEM_ListLocalsOnly,
		DL_SEM_ListGlobalsOnly,
		DL_SEM_ListMembersOnly,

		DL_SEM_ListInputsOnly,
		DL_SEM_ListOutputsOnly,
		DL_SEM_ListBothInputsAndOutputs,

		DL_SEM_ListTypesOnly,

		///ListMiscellaneous,
		DL_SEM_ListStrings,

		DL_SEM_ListMixedItems,
		
		///~~Atom???
		DL_SEM_AtomUnknown,
		DL_SEM_AtomLiteral,
		DL_SEM_AtomIdentifier,
		DL_SEM_AtomInstruction,
		DL_SEM_AtomLabel,
		DL_SEM_AtomPointer,
		DL_SEM_AtomWord,
		DL_SEM_AtomBlock,

		///~~;
		DL_SEM_Query,
}
DLSemanticRole;

///struct DL
struct DLSyntaxNodeList
{
	DLSyntaxNode* Owner;
	DLSyntaxNode* Items;
	int           Count,Capacity;
};
struct DLSyntaxNode
{
	DLSyntaxNodeType Type;
	DLSyntaxNode*    Parent;
//public Token[]        Tokens;
	DLToken*         Token_;
	DLSemanticRole   Role;
	///LabelDictionary  Labels;

	int BegToken, EndToken; /// query uses token range while still remains to be just a syntax item;

	DLSyntaxNodeList* Children;
};

struct DLSyntaxParser
{
	DLContext*    DLC;
	DLTokenList*  Tokens;
	int           Position;
	
	DLSyntaxNode* RootNode;
	DLSyntaxNode* TopNode;
};

DLSyntaxParser* gfParser_Create      (DLContext* iDLC);
void            gfParser_Destroy     (DLSyntaxParser* irParser);
void            gfParser_Init        (DLSyntaxParser* irParser, DLTokenList* iTokens);
DLSyntaxNode*   gfParser_ParseTokens (DLSyntaxParser* irParser);

void            gfParser_AddNode     (DLSyntaxParser* irParser, DLSyntaxNodeType iNodeType, DLToken* iToken);
DLSyntaxNode*   gfParser_OpenNode    (DLSyntaxParser* irParser, DLSyntaxNodeType iNodeType, DLToken* iToken);
DLSyntaxNode*   gfParser_CloseNode   (DLSyntaxParser* irParser, DLSyntaxNodeType iNodeType);







__declspec(noinline) void gfDLSyntaxNodeType_ToString(char* irStr, DLSyntaxNodeType iValue);





#ifdef __cplusplus
    }
#endif

#endif