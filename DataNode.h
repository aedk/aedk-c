#ifndef DATANODE_H
#define DATANODE_H

#ifdef __cplusplus
    extern "C" {
    
    
    class DataNode;
#endif

#ifndef __cplusplus
#define nullptr 0

#define true 1
#define false 0

typedef        char           bool;
typedef        unsigned short wchar_t;
#endif





typedef struct WString WString;

struct WString
{
	wchar_t* Data;
	int Capacity, Length;
};

void     gfWString_Reserve    (WString* irString, int iNewCapacity);
WString* gfWString_Create     (int iInitialCapacity);
void     gfWString_Destroy    (WString* irString);
void     gfWString_Reserve    (WString* irString, int iNewCapacity);

int      gfWString_Write      (WString* irString, wchar_t* iSrcBuffer, int iSrcLength);
int      gfWString_WriteLine  (WString* irString, wchar_t* iSrcBuffer, int iSrcLength, int iIndent);




typedef struct DataNodeStruct DataNodeStruct;
typedef struct DataNodeCollection DataNodeCollection;

///typedef struct DLSyntaxNode;
typedef struct DLContext DLContext;
typedef struct DLSyntaxNode DLSyntaxNode;


#define dDataNodeNameMaxLength 64
#define dDataNodeValueMaxLength sizeof(void*) / sizeof(wchar_t)
#define dDataNodeCollectionCapacityMultiplier 1.2f

typedef enum DataNodeType
{
	DN_T_Null,
	DN_T_Undefined,
	DN_T_Element,
	DN_T_Attribute,
	
}
DataNodeType;

//
//typedef enum DataNodeNameType
//{
//	DN_NT_String,
//	DN_NT_StringPtr,
//}
//DataNodeNameType;

typedef enum DataNodeValueType
{
	DN_VT_Null,
	DN_VT_Undefined,
	DN_VT_NoValue,
	
	DN_VT_NumInvalid,
	DN_VT_String,
	DN_VT_StringPtr,
	DN_VT_WideString,
	DN_VT_WideStringPtr,
	DN_VT_Int32,
	DN_VT_UInt32,
	DN_VT_Int64,
	DN_VT_UInt64,
	DN_VT_Float32,
	DN_VT_Float64,
}
DataNodeValueType;


void                 gfDNChildren_Reserve  (DataNodeStruct* irParentNode, int iCapacity);
DataNodeCollection*  gfDNChildren_Create   (int iCapacity);
void                 gfDNChildren_Destroy  (DataNodeStruct* irParentNode);
DataNodeStruct*      gfDNChildren_Allocate (DataNodeStruct* irParentNode, int iCount);
DataNodeStruct*      gfDNChildren_Pop      (DataNodeStruct* irParentNode);
DataNodeStruct*      gfDNChildren_Peek     (DataNodeStruct* irParentNode);
void                 gfDNChildren_Reserve  (DataNodeStruct* irParentNode, int iCapacity);
void                 gfDNChildren_Clear    (DataNodeStruct* irParentNode);
void                 gfDNChildren_Init     (DataNodeStruct* irParentNode);

struct DataNodeStruct
{
	wchar_t Name[dDataNodeNameMaxLength];
	wchar_t Value[dDataNodeValueMaxLength];
	
	DataNodeType      Type;
	DataNodeValueType NameType;
	DataNodeValueType ValueType;
	
	DataNodeStruct*   Parent;
	DataNodeStruct*   Children; int ChildCount, ChildCapacity;
	
	
	#ifdef __cplusplus
		
		
		DataNodeStruct(char*    iValue);
		DataNodeStruct(wchar_t* iValue);
		DataNodeStruct(int      iValue);
		DataNodeStruct(float    iValue);
		DataNodeStruct(double   iValue);

		operator DataNode ();
		operator char*    ();
		operator wchar_t* ();
		operator int      ();
		operator float    ();
		operator double   ();
		
		DataNodeStruct& operator= (const DataNodeStruct& iNode);
	#endif
};

DataNodeStruct* gfDataNode_Create();
void            gfDataNode_Destroy(DataNodeStruct* irNode);

void gfDataNode_Init_1(DataNodeStruct* irNode);
void gfDataNode_Init_2(DataNodeStruct* irNode, DataNodeType iNodeType);
void gfDataNode_Init_3(DataNodeStruct* irNode, DataNodeType iNodeType, DataNodeValueType iValueType);

void gfDataNode_InitStatic();


void gfDataNode_GetId   (DataNodeStruct* iNode, wchar_t* oId);
void gfDataNode_GetPath (DataNodeStruct* iNode, wchar_t* oPath);




void gfDataNode_FromDLSyntaxNode (DataNodeStruct* irNode, DLSyntaxNode* iSyntaxNode, DLContext* iDLC);
void gfDataNode_ToDLSyntaxNode   (DataNodeStruct* iNode,  DLSyntaxNode* irSyntaxNode);


DataNodeStruct* gfDataNode_GetNodeByPath(DataNodeStruct* irNode, wchar_t* iPath, bool iDoAllowSubPath, bool iDoCreateMissing);
DataNodeStruct* gfDataNode_SetNodeByPath(DataNodeStruct* irNode, wchar_t* iPath, DataNodeStruct* iNewNode, bool iDoAllowSubPath, bool iDoCreateMissing);

DataNodeStruct* gfDataNode_GetChildByNameAndIndex(DataNodeStruct* iNode, char* iName, int iIndex);
DataNodeStruct* gfDataNode_GetChildByNameAndIndex_2(DataNodeStruct* iNode, wchar_t* iName, int iIndex);


///DataNodeStruct* gfDataNode_GetChild(DataNodeStruct* iNode, wchar_t* iPath);

void gfDataNode_Begin(DataNodeStruct** irNode, wchar_t* iPath);
void gfDataNode_End(DataNodeStruct** irNode);


void gfDataNode_SetName     (DataNodeStruct* iNode, wchar_t* iName);
void gfDataNode_SetValueSZ  (DataNodeStruct* iNode, wchar_t* iPath, char*    iValue);
void gfDataNode_SetValueWSZ (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iValue);
void gfDataNode_SetValueI32 (DataNodeStruct* iNode, wchar_t* iPath, int      iValue);
void gfDataNode_SetValueF32 (DataNodeStruct* iNode, wchar_t* iPath, float    iValue);


wchar_t* gfDataNode_GetName     (DataNodeStruct* iNode);
char*    gfDataNode_GetValueSZ  (DataNodeStruct* iNode, wchar_t* iPath);
wchar_t* gfDataNode_GetValueWSZ (DataNodeStruct* iNode, wchar_t* iPath);
int      gfDataNode_GetValueI32 (DataNodeStruct* iNode, wchar_t* iPath);
float    gfDataNode_GetValueF32 (DataNodeStruct* iNode, wchar_t* iPath);

wchar_t* gfDataNode_GetNameOr     (DataNodeStruct* iNode, wchar_t* iName, wchar_t* iOrName);
char*    gfDataNode_GetValueSZOr  (DataNodeStruct* iNode, wchar_t* iPath, char*    iOrValue);
wchar_t* gfDataNode_GetValueWSZOr (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iOrValue);
int      gfDataNode_GetValueI32Or (DataNodeStruct* iNode, wchar_t* iPath, int      iOrValue);
float    gfDataNode_GetValueF32Or (DataNodeStruct* iNode, wchar_t* iPath, float    iOrValue);

///int gfDataNode_ToString(DataNodeStruct* iNode, wchar_t** irBuffer, int iIndent);
///int gfDataNode_ToString(DataNodeStruct* iNode, wchar_t** irBuffer, int iBufferCapacity, int iBufferSize, int iIndent);
int gfDataNode_ToString(DataNodeStruct* iNode, WString* irString, int iIndent);


extern DataNodeStruct gDataNodeStruct_Null;


///~~ C++ part;

#ifdef __cplusplus

//
//extern "C++"
//{
class DataNode;


class DataNode
{
	public:
	//
	///wchar_t Name[dDataNodeNameMaxLength];
	///wchar_t Value[dDataNodeValueMaxLength];
	//
	DataNodeStruct* Ref;
	
	 DataNode();
	/// DataNode(DataNodeType iNodeType);
	 ///DataNode(DLSyntaxNode* iSyntaxNode);
	~DataNode();
	
	//DataNode(DataNodeStruct iValue);
	DataNode(DataNodeStruct& iValue);
	DataNode(DataNodeStruct* iValue);
	
	///DataNode(DataNodeStruct iValue);
	DataNode(char*    iValue);
	DataNode(wchar_t* iValue);
	DataNode(int      iValue);
	DataNode(float    iValue);
	DataNode(double   iValue);
	
	operator DataNodeStruct& ();
	operator DataNodeStruct* ();
	
	///operator DataNodeStruct ();
	operator char*    ();
	operator wchar_t* ();
	operator int      ();
	operator float    ();
	operator double   ();
	
	///bool             operator= ();
	bool             operator! ();


	///      DataNodeStruct*  operator[](wchar_t* iPath);
	///const DataNodeStruct*  operator[](wchar_t* iPath) const;
	
			DataNodeStruct&  operator[](char* iPath);
	const DataNodeStruct&  operator[](char* iPath) const;
		   DataNodeStruct&  operator[](wchar_t* iPath);
	const DataNodeStruct&  operator[](wchar_t* iPath) const;

	DataNodeStruct& Create(wchar_t* iPath);
	
	void Load(wchar_t* iPath);
	void Save(wchar_t* iPath);
	
	int  ToStringPreview(wchar_t* irBuffer, int iBufferInitialSize);
	int  ToStringPreview(wchar_t** irBuffer);
	//DataNode& GetChildByNameAndIndex(char* iName, int iIndex);
	//DataNode& GetChildByNameAndIndex(wchar_t* iName, int iIndex);

	//void GetId   (wchar_t*& oId);
	//void GetPath (wchar_t*& oPath);
	
	//private:
	//void Init();
	//void Init(DataNodeType iNodeType);
	//void Init(DataNodeType iNodeType, DataNodeValueType iValueType);
	//void InitChildren();
	//
	
	///static DataNode Null;
	//void InitStatic();
	
	static void FromFile(wchar_t* iPath);
};
//}

extern DataNode gDataNode_Null;


#endif

#ifdef __cplusplus
    }
#endif


///~~ include guard;
#endif

