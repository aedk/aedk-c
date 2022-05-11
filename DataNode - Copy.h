#ifndef DATANODE_H
#define DATANODE_H

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

struct DataNodeCollection
{
	DataNodeStruct* Owner;
	DataNodeStruct* Items;
	int Count,Capacity;
	
	 ///DataNodeCollection(DataNode* iOwner);
	 /*DataNodeCollection();
	~DataNodeCollection();*/
	
	///DataNode* Allocate(int iCount);
};

void                 gfDNCollection_Reserve  (DataNodeCollection* irCollection, int iCapacity);
DataNodeCollection*  gfDNCollection_Create   (int iCapacity);
void                 gfDNCollection_Destroy  (DataNodeCollection* irCollection);
DataNodeStruct*      gfDNCollection_Allocate (DataNodeCollection* irCollection, int iCount);
DataNodeStruct*      gfDNCollection_Pop      (DataNodeCollection* irCollection);
DataNodeStruct*      gfDNCollection_Peek     (DataNodeCollection* irCollection);
void                 gfDNCollection_Reserve  (DataNodeCollection* irCollection, int iCapacity);
void                 gfDNCollection_Clear    (DataNodeCollection* irCollection);


struct DataNodeStruct
{
	wchar_t Name[dDataNodeNameMaxLength];
	wchar_t Value[dDataNodeValueMaxLength];
	
	DataNodeType         Type;
	DataNodeValueType    ValueType;
	
	DataNodeStruct*      Parent;
	DataNodeCollection*  Children;

	
};

DataNodeStruct* gfDN_Create();

void gfDN_Init_1(DataNodeStruct* irNode);
void gfDN_Init_2(DataNodeStruct* irNode, DataNodeType iNodeType);
void gfDN_Init_3(DataNodeStruct* irNode, DataNodeType iNodeType, DataNodeValueType iValueType);
void gfDN_InitChildren(DataNodeStruct* irNode);
void gfDN_InitStatic();


void gfDN_GetId   (DataNodeStruct* iNode, wchar_t* oId);
void gfDN_GetPath (DataNodeStruct* iNode, wchar_t* oPath);




void gfDN_FromDLSyntaxNode (DataNodeStruct* irNode, DLSyntaxNode* iSyntaxNode, DLContext* iDLC);
void gfDN_ToDLSyntaxNode   (DataNodeStruct* iNode,  DLSyntaxNode* irSyntaxNode);


DataNodeStruct* gfDN_GetNodeByPath(DataNodeStruct* irNode, wchar_t* iPath, bool iDoAllowSubPath, bool iDoCreateMissing);
DataNodeStruct* gfDN_SetNodeByPath(DataNodeStruct* irNode, wchar_t* iPath, DataNodeStruct* iNewNode, bool iDoAllowSubPath, bool iDoCreateMissing);

DataNodeStruct* gfDN_GetChildByNameAndIndex(DataNodeStruct* iNode, char* iName, int iIndex);
DataNodeStruct* gfDN_GetChildByNameAndIndex_2(DataNodeStruct* iNode, wchar_t* iName, int iIndex);


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
	
	DataNode(DataNodeStruct* iValue);
	DataNode(char*    iValue);
	DataNode(wchar_t* iValue);
	DataNode(int      iValue);
	DataNode(float    iValue);
	DataNode(double   iValue);
	
	operator DataNodeStruct* ();
	operator char*    ();
	operator wchar_t* ();
	operator int      ();
	operator float    ();
	operator double   ();
	

		
		///value_t& operator[](std::size_t idx)       { return mVector[idx]; }
	///const value_t& operator[](std::size_t idx) const { return mVector[idx]; }
	
	

	bool             operator! ();

	//      DataNodeStruct*  operator[](int iChildIndex);
	//const DataNodeStruct*  operator[](int iChildIndex) const;
	//DataNode&        operator[](char* iPath);
	//const DataNode&  operator[](char* iPath) const;
	      DataNodeStruct*  operator[](wchar_t* iPath);
	const DataNodeStruct*  operator[](wchar_t* iPath) const;

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
};
//}
#endif

#ifdef __cplusplus
    }
#endif


///~~ include guard;
#endif

