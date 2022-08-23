#ifndef UL_H
#define UL_H

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

int wcsindexof(wchar_t* iBuffer, int iBufferLen, wchar_t* iValue, int iValueLen);

typedef struct ULContext       ULContext;

typedef struct ULTokenTypeList ULTokenTypeList;
typedef struct ULTokenTypeList ULTokenTypeStack;

typedef struct ULToken         ULToken;
typedef struct ULTokenList     ULTokenList;
typedef struct ULTokenList     ULTokenStack;
//typedef struct ULTokenRefList ULTokenRefList;
//typedef struct ULTokenRefList ULTokenRefStack;
typedef struct LexerState      LexerState;
typedef struct LexerContext    LexerContext;

typedef struct ULSyntaxNode     ULSyntaxNode;
typedef struct ULSyntaxNodeList ULSyntaxNodeList;

typedef struct ULSyntaxParser   ULSyntaxParser;

#define dULTokenValueMaxLength 8
///#define dULTokenStackMaxSize   256
#define dULTokenListInitialCapacity    1024*8
#define dULTokenStackInitialCapacity   16
#define dULTokenTypeListCapacityMultiplier  1.2f
#define dULTokenListCapacityMultiplier      1.2f
#define dULSyntaxNodeListCapacityMultiplier 1.2f



struct ULContext
{
	LexerContext*   Lexer;
	ULSyntaxParser* Parser;
	
};

ULContext* gfULCtx_Create           (wchar_t* iBuffer, size_t iBufferLength, bool iDoBuildAST);
void       gfULCtx_Destroy          (ULContext* iULC);



typedef enum ULTokenType
{
	UL_TOK_Undefined,
	UL_TOK_SyntaxToken,

	UL_TOK_Whitespace,
	UL_TOK_Space,
	UL_TOK_Tab,
	UL_TOK_NewLine,
	
	UL_TOK_Garbage,
	UL_TOK_Comment,
	UL_TOK_MultilineCommentOpener, ///~~ NI;
	UL_TOK_MultilineCommentCloser, ///~~ NI;
	
	UL_TOK_String,
	UL_TOK_Character,
	/** ... define all your literals here or modify literal token range in the lexer */
	
	//Int32,
	//Float32,
	//Float64,

	//InvalidNumber,
	UL_TOK_Number,

	


	UL_TOK_Expression_Delimiter,      /// "1";"2";
	///UL_TOK_List_Delimiter,            /// "1" "2";
	UL_TOK_ListItem_Delimiter,        /// "1","2";
	UL_TOK_Atom_Delimiter,            /// iItem'Name
	
	
	UL_TOK_Identifiers__Begin, ///~~ "var _IsIdentifier = ... "
		UL_TOK_Instruction,
		UL_TOK_Label,
		UL_TOK_Pointer,
		
		//UL_TOK_Identifier,
		UL_TOK_Reference_Ident,
		UL_TOK_Input_Ident,
		UL_TOK_Output_Ident,
		UL_TOK_Local_Ident,
		UL_TOK_Global_Ident,
		UL_TOK_Member_Ident,
		UL_TOK_Word,
		//UL_TOK_Host_Object,
		//UL_TOK_Packed_Tuple,
		//UL_TOK_Type,
	UL_TOK_Identifiers__End,
		//EndOF

	UL_TOK_All_Syntax_Tokens__Begin,
		UL_TOK_Brace,        UL_TOK_Parenthesis,        UL_TOK_Bracket,
		UL_TOK_Brace_Opener, UL_TOK_Parenthesis_Opener, UL_TOK_Bracket_Opener,
		UL_TOK_Brace_Closer, UL_TOK_Parenthesis_Closer, UL_TOK_Bracket_Closer,
		
		UL_TOK_Special_Syntax_Tokens__Begin,
			
			UL_TOK_Root,        UL_TOK_Block,        UL_TOK_Expression,        UL_TOK_List,        UL_TOK_ListItem,        
			UL_TOK_Root_Opener, UL_TOK_Block_Opener, UL_TOK_Expression_Opener, UL_TOK_List_Opener, UL_TOK_ListItem_Opener, 
			UL_TOK_Root_Closer, UL_TOK_Block_Closer, UL_TOK_Expression_Closer, UL_TOK_List_Closer, UL_TOK_ListItem_Closer, 
			
			UL_TOK_Expect_Expression,           
			UL_TOK_Expect_List,                 
			UL_TOK_Expect_ListItem,             
			UL_TOK_Expect_Next_Atom,            
			
			UL_TOK_Root_Error,
			UL_TOK_Block_Error,
			UL_TOK_Expression_Error,
			UL_TOK_List_Error,
			UL_TOK_ListItem_Error,
			
		UL_TOK_Special_Syntax_Tokens__End,
	UL_TOK_All_Syntax_Tokens__End,
}
ULTokenType;


struct ULTokenTypeList
{
	ULTokenType* Items;
	int          Count, Capacity;
};

struct ULToken
{
	wchar_t Value[dULTokenValueMaxLength];
	ULTokenType Type;
	int Id, Offset, Fragment, Length;
	int PairId;
};

struct ULTokenList
{
	ULToken*  Items;
	int       Count, Capacity;
};
//struct ULTokenRefList
//{
//	ULToken** Items;
//	int       Count, Capacity;
//};


ULTokenTypeList* gfTypeList_Create    (int iCapacity);
void             gfTypeList_Destroy   (ULTokenTypeList* iList);

ULTokenType*     gfTypeList_Allocate  (ULTokenTypeList* iList, int iCount);

void             gfTypeList_Push      (ULTokenTypeList* iList, ULTokenType* iItem);
void             gfTypeList_PushValue (ULTokenTypeList* iList, ULTokenType iItem);
ULTokenType*     gfTypeList_Pop       (ULTokenTypeList* iList);
ULTokenType      gfTypeList_PopValue  (ULTokenTypeList* iList);
ULTokenType*     gfTypeList_Peek      (ULTokenTypeList* iList);
ULTokenType      gfTypeList_PeekValue (ULTokenTypeList* iList);

void             gfTypeList_Reserve   (ULTokenTypeList* iList, int iCapacity);
void             gfTypeList_Clear     (ULTokenTypeList* iList);



ULTokenList* gfTList_Create   (int iCapacity);
void         gfTList_Destroy  (ULTokenList* iList);

ULToken*     gfTList_Allocate (ULTokenList* iList, int iCount);

void         gfTList_Push     (ULTokenList* iList, ULToken* iItem);
ULToken*     gfTList_Pop      (ULTokenList* iList);
ULToken*     gfTList_Peek     (ULTokenList* iList);

void         gfTList_Reserve  (ULTokenList* iList, int iCapacity);
void         gfTList_Clear    (ULTokenList* iList);




ULSyntaxNodeList* gfSNList_Create   (int iCapacity);
void              gfSNList_Destroy  (ULSyntaxNodeList* iList);

ULSyntaxNode*     gfSNList_Allocate (ULSyntaxNodeList* iList, int iCount);

void              gfSNList_Push     (ULSyntaxNodeList* iList, ULSyntaxNode* iItem);
ULSyntaxNode*     gfSNList_Pop      (ULSyntaxNodeList* iList);
ULSyntaxNode*     gfSNList_Peek     (ULSyntaxNodeList* iList);

void              gfSNList_Reserve  (ULSyntaxNodeList* iList, int iCapacity);
void              gfSNList_Clear    (ULSyntaxNodeList* iList);


//
///ULTokenRefList* gfTRList_Create   ();
//void            gfTRList_Destroy  (ULTokenRefList* irList);
//void            gfTRList_Push     (ULTokenList* irList, ULToken* iToken);
//ULToken*        gfTRList_Pop      (ULTokenList* irList);
//ULToken*        gfTRList_Peek     (ULTokenList* irList);

//
//struct LexerState
//{

//};


struct LexerState
{
	int               Position;
	ULTokenTypeStack* TypeStack;
};
struct LexerContext
{
	ULContext*  ULC;
	wchar_t*    Buffer;
	size_t      BufferLength;

	ULTokenList*  Tokens;
	LexerState*   State;
	//ULToken* TokenStack[dULTokenStackMaxSize];
	//int      TokenStackSize;
	
	
};

void gfToken_Init    (ULToken* iToken);
void gfToken_Cleanup (ULToken* iToken);

bool gfToken_IsAligned     (ULToken* iToken);
bool gfToken_IsZeroLength  (ULToken* iToken);
bool gfToken_IsTerminated  (ULToken* iToken);
bool gfToken_IsPaired      (ULTokenType iTokenType);
bool gfToken_IsOpener      (ULTokenType iTokenType);
bool gfToken_IsCloser      (ULTokenType iTokenType);
bool gfToken_IsWhitespace  (ULTokenType iTokenType);
bool gfToken_IsGarbage     (ULTokenType iTokenType);


LexerState*  gfLState_Create      ();
void         gfLState_Destroy     (LexerState* iState);



LexerContext* gfLexCtx_Create           (ULContext* iULCtx, wchar_t* iBuffer, size_t iBufferLength);
void          gfLexCtx_Destroy          (LexerContext* iCtx);

ULToken*      gfLexCtx_AllocateToken    (LexerContext* iCtx);
ULToken*      gfLexCtx_AllocateTokens   (LexerContext* iCtx, int iCount);
///void          gfLexCtx_AddTokens        (LexerContext* iCtx, ULTokenList* iNewTokens, ULTokenList* irTokens);
void          gfLexCtx_AddToken         (LexerContext* iCtx, ULTokenType iType, void* iValue, int iBegOffs, int iEndOffsOrLength);
void          gfLexCtx_AddSyntaxTokens  (LexerContext* iCtx, ULTokenType iTokenType, bool iIsTerminated);
void          gfLexCtx_ProcessPairs     (LexerContext* iCtx);///, ULTokenList* iTokens);
void          gfLexCtx_LinkTokens       (LexerContext* iCtx, ULToken* irOpenerToken, ULToken* irCloserToken);

ULTokenList*  gfLexCtx_ParseBuffer      (LexerContext* iCtx);

void          gfLexCtx_ParseNextTokens  (LexerContext* iCtx);
///void          gfLexCtx_SkipNonTokens    (LexerContext* iCtx);

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

bool _inline  gfLexCtx_IsNewline      (wchar_t iChar);
bool _inline  gfLexCtx_IsWhitespace   (wchar_t iChar);
bool _inline  gfLexCtx_IsDecimalDigit (wchar_t iChar);
bool _inline  gfLexCtx_IsNumberChar   (wchar_t iChar);
bool _inline  gfLexCtx_IsAlpha        (wchar_t iChar);
bool _inline  gfLexCtx_IsUpperCase    (wchar_t iChar);
bool _inline  gfLexCtx_IsLowerCase    (wchar_t iChar);
bool _inline  gfLexCtx_IsIdentChar    (wchar_t iChar);
bool _inline  gfLexCtx_IsPunctuation  (wchar_t iChar);
bool _inline  gfLexCtx_IsBracket      (wchar_t iChar);
bool _inline  gfLexCtx_IsQuote        (wchar_t iChar);
bool _inline  gfLexCtx_IsSpecial      (wchar_t iChar);




typedef enum ULSyntaxNodeType
{
	UL_SYN_Unknown,

	UL_SYN_Root,
	UL_SYN_Expression,
	UL_SYN_List,
	UL_SYN_ListItem,
	
	UL_SYN_ParenthesisBlock,
	UL_SYN_BracketBlock,
	UL_SYN_BraceBlock,
	
	
	UL_SYN_Value__Begin,
		UL_SYN_Number,
		UL_SYN_NumInvalid,
		UL_SYN_NumInt32,
		UL_SYN_NumFloat32,
		UL_SYN_NumFloat64,
		
		UL_SYN_String,
	UL_SYN_Value__End,
	
	UL_SYN_Identifiers__Begin,
		UL_SYN_Identifier,
		UL_SYN_Instruction,
		UL_SYN_Label,
		UL_SYN_Pointer,
		UL_SYN_ReferenceIdentifier,
		UL_SYN_InputIdentifier,
		UL_SYN_OutputIdentifier,
		UL_SYN_LocalIdentifier,
		UL_SYN_GlobalIdentifier,
		UL_SYN_MemberIdentifier,
		UL_SYN_Word,
	
		UL_SYN_HostObject,
		UL_SYN_Type,
		UL_SYN_PackedTuple,
	UL_SYN_Identifiers__End,
}
ULSyntaxNodeType;

typedef enum ULSemanticRole
{
	UL_SEM_Unknown = 0,

		///~~ Expression
		
		UL_SEM_ExpInstruction,
		UL_SEM_ExpInstructionLabelDefinition,
		UL_SEM___ExpInstructionLabelReference,
		UL_SEM_ExpCustom,

		UL_SEM_ExpOneList,

		UL_SEM_ExpReturn,
		UL_SEM_ExpSignature,
			UL_SEM_ExpSignatureInput,
			UL_SEM_ExpSignatureOutput,
		UL_SEM_ExpAssignment,
		

		UL_SEM_ExpSemanticHanULerDefinition,
		UL_SEM_ExpInstructionHanULerDefinition,
		UL_SEM_ExpWordHanULerDefinition,

		//ExpInterpreterInstructionDefinition,
		//ExpInterpreterEventHanULerDefinition,
		//ExpWordDefinition,


		///~~ List
		UL_SEM_ListOneInstruction,
		UL_SEM_ListQueriesOnly,

		UL_SEM_ListLiteralsOnly,
		UL_SEM_ListWordsOnly,
		UL_SEM_ListLocalsOnly,
		UL_SEM_ListGlobalsOnly,
		UL_SEM_ListMembersOnly,

		UL_SEM_ListInputsOnly,
		UL_SEM_ListOutputsOnly,
		UL_SEM_ListBothInputsAndOutputs,

		UL_SEM_ListTypesOnly,

		///ListMiscellaneous,
		UL_SEM_ListStrings,

		UL_SEM_ListMixedItems,
		
		///~~Atom???
		UL_SEM_AtomUnknown,
		UL_SEM_AtomLiteral,
		UL_SEM_AtomIdentifier,
		UL_SEM_AtomInstruction,
		UL_SEM_AtomLabel,
		UL_SEM_AtomPointer,
		UL_SEM_AtomWord,
		UL_SEM_AtomBlock,

		///~~;
		UL_SEM_Query,
}
ULSemanticRole;

///struct UL
struct ULSyntaxNodeList
{
	ULSyntaxNode* Owner;
	ULSyntaxNode* Items;
	int           Count,Capacity;
};
struct ULSyntaxNode
{
	ULSyntaxNodeType Type;
	ULSyntaxNode*    Parent;
//public Token[]        Tokens;
	ULToken*         Token_;
	ULSemanticRole   Role;
	///LabelDictionary  Labels;

	int BegToken, EndToken; /// query uses token range while still remains to be just a syntax item;

	ULSyntaxNodeList* Children;
};

struct ULSyntaxParser
{
	ULContext*    ULC;
	ULTokenList*  Tokens;
	int           Position;
	
	ULSyntaxNode* RootNode;
	ULSyntaxNode* TopNode;
};

ULSyntaxParser* gfParser_Create      (ULContext* iULC);
void            gfParser_Destroy     (ULSyntaxParser* irParser);
void            gfParser_Init        (ULSyntaxParser* irParser, ULTokenList* iTokens);
ULSyntaxNode*   gfParser_ParseTokens (ULSyntaxParser* irParser);

void            gfParser_AddNode     (ULSyntaxParser* irParser, ULSyntaxNodeType iNodeType, ULToken* iToken);
ULSyntaxNode*   gfParser_OpenNode    (ULSyntaxParser* irParser, ULSyntaxNodeType iNodeType, ULToken* iToken);
ULSyntaxNode*   gfParser_CloseNode   (ULSyntaxParser* irParser, ULSyntaxNodeType iNodeType);







__declspec(noinline) void gfULSyntaxNodeType_ToString(char* irStr, ULSyntaxNodeType iValue);





#ifdef __cplusplus
    }
#endif

#endif