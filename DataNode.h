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

WString* gfWString_Create     (int iInitialCapacity);
void     gfWString_Destroy    (WString* iString);
void     gfWString_Reserve    (WString* iString, int iNewCapacity);

int      gfWString_Write      (WString* iString, wchar_t* iSrcBuffer, int iSrcLength);
int      gfWString_WriteLine  (WString* iString, wchar_t* iSrcBuffer, int iSrcLength, int iIndent);



typedef struct DataNodeValue      DataNodeValue;
typedef struct DataNodeTypeList   DataNodeTypeList;

typedef struct DataNodeStruct     DataNodeStruct;
typedef struct DataNodeCollection DataNodeCollection;

//typedef struct ULSyntaxNode;
typedef struct ULContext ULContext;
typedef struct ULSyntaxNode ULSyntaxNode;


#define dDataNodeNameMaxLength 64
#define dDataNodeValueMaxLength sizeof(void*) / sizeof(wchar_t)
#define dDataNodeCollectionCapacityMultiplier 1.2f

typedef enum DataNodeType
{
	DN_T_Null,
	DN_T_Undefined,
	
	DN_T_Element,
	DN_T_Element_Missing,
	DN_T_Attribute,
	DN_T_Attribute_Missing,
	
	DN_T_Test,
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
	DN_VT_WString,
	DN_VT_WStringPtr,
	DN_VT_Int32,
	DN_VT_UInt32,
	DN_VT_Int64,
	DN_VT_UInt64,
	DN_VT_Float32,
	DN_VT_Float64,
	
	DN_VT_ValueList,
}
DataNodeValueType;



void                 gfDNChildren_Init     (DataNodeStruct* iParentNode);
void                 gfDNChildren_Reserve  (DataNodeStruct* iParentNode, int iCapacity);
DataNodeStruct*      gfDNChildren_Allocate (DataNodeStruct* iParentNode, int iCount);
DataNodeStruct*      gfDNChildren_Pop      (DataNodeStruct* iParentNode);
DataNodeStruct*      gfDNChildren_Peek     (DataNodeStruct* iParentNode);
void                 gfDNChildren_Clear    (DataNodeStruct* iParentNode);

//
////DataNodeCollection*  gfDNChildren_Create   (int iCapacity);
//void                 gfDNChildren_Destroy  (DataNodeStruct* iParentNode);

void                 gfDNAttributes_Init     (DataNodeStruct* iParentNode);
void                 gfDNAttributes_Reserve  (DataNodeStruct* iParentNode, int iCapacity);
DataNodeStruct*      gfDNAttributes_Allocate (DataNodeStruct* iParentNode, int iCount);
DataNodeStruct*      gfDNAttributes_Pop      (DataNodeStruct* iParentNode);
DataNodeStruct*      gfDNAttributes_Peek     (DataNodeStruct* iParentNode);
void                 gfDNAttributes_Clear    (DataNodeStruct* iParentNode);


struct DataNodeValue
{
	wchar_t Data[dDataNodeValueMaxLength];
	DataNodeValueType Type;
	
	DataNodeValue* NextPtr;
	
	
	
	#ifdef __cplusplus
		DataNodeValue();
	
		DataNodeValue(char*    iValue);
		DataNodeValue(wchar_t* iValue);
		DataNodeValue(int      iValue);
		DataNodeValue(float    iValue);
		DataNodeValue(double   iValue);

		//operator DataNode ();
		operator char*    ();
		operator wchar_t* ();
		operator int      ();
		operator float    ();
		operator double   ();
		
		//DataNodeValue& operator= (const DataNodeValue& iNode);
	#endif
};

DataNodeValue* gfDNValue_AllocNext (DataNodeValue* iValue);
void           gfDNValue_ClearAll  (DataNodeValue* iValue);
DataNodeValue* gfDNValue_GetNext   (DataNodeValue* iValue);


int    gfDNValue_GetI32  (DataNodeValue* iValue);
float  gfDNValue_GetF32  (DataNodeValue* iValue);
double gfDNValue_GetF64  (DataNodeValue* iValue);

void   gfDNValue_GetStr  (DataNodeValue* iValue, char*    iDstBuf, size_t iMaxLen);
void   gfDNValue_GetWStr (DataNodeValue* iValue, wchar_t* iDstBuf, size_t iMaxLen);

char*    gfDNValue_GetStrPtr  (DataNodeValue* iValue);
wchar_t* gfDNValue_GetWStrPtr (DataNodeValue* iValue);



struct DataNodeTypeList
{
	DataNodeValueType* Items;
	int Count,Capacity;
};

DataNodeTypeList* gfDNTypeList_Create(wchar_t* iTypeListString);
void              gfDNTypeList_Destroy(DataNodeTypeList* iTypeList);





struct DataNodeStruct
{
	wchar_t Name[dDataNodeNameMaxLength];
	//wchar_t Value[dDataNodeValueMaxLength];
	
	DataNodeType      Type;
	DataNodeValueType NameType;
	DataNodeValue     Value;
	//DataNodeValueType ValueType;
	
	DataNodeStruct*   Parent;
	DataNodeStruct*   Attributes; int AttrCount,  AttrCapacity;
	DataNodeStruct*   Children;   int ChildCount, ChildCapacity;
	
	
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
void            gfDataNode_Destroy(DataNodeStruct* iNode);

void gfDataNode_Init_1(DataNodeStruct* iNode);
void gfDataNode_Init_2(DataNodeStruct* iNode, DataNodeType iNodeType);
void gfDataNode_Init_3(DataNodeStruct* iNode, DataNodeType iNodeType, DataNodeValueType iValueType);

void gfDataNode_InitStatic();

void gfDataNode_UpdateState(DataNodeStruct* iNode, bool iIsMissing);

void gfDataNode_FromULSyntaxNode (DataNodeStruct* iNode, ULSyntaxNode* iSyntaxNode, ULContext* iULC);///, DataNodeType iNodeType)
void gfDataNode_ToULSyntaxNode   (DataNodeStruct* iNode,  ULSyntaxNode* iSyntaxNode);
DataNodeStruct* gfDataNode_FromString(wchar_t* iBuffer);
int gfDataNode_ToString(DataNodeStruct* iNode, WString* iString, int iIndent);



DataNodeStruct* gfDataNode_GetNodeByPath(DataNodeStruct* iNode, wchar_t* iPath, bool iDoAllowSubPath, bool iDoCreateMissing);
DataNodeStruct* gfDataNode_SetNodeByPath(DataNodeStruct* iNode, wchar_t* iPath, DataNodeStruct* iNewNode, bool iDoAllowSubPath, bool iDoCreateMissing);

DataNodeStruct* gfDataNode_GetChildByNameAndIndex(DataNodeStruct* iNode, char* iName, int iIndex);
DataNodeStruct* gfDataNode_GetChildByNameAndIndex_2(DataNodeStruct* iNode, wchar_t* iName, int iIndex);


//DataNodeStruct* gfDataNode_GetChild(DataNodeStruct* iNode, wchar_t* iPath);

void gfDataNode_Begin(DataNodeStruct** irNode, wchar_t* iPath);
void gfDataNode_End(DataNodeStruct** irNode);


void gfDataNode_SetName     (DataNodeStruct* iNode, wchar_t* iName);
void gfDataNode_SetValueSZ  (DataNodeStruct* iNode, wchar_t* iPath, char*    iValue);
void gfDataNode_SetValueWSZ (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iValue);
void gfDataNode_SetValueI32 (DataNodeStruct* iNode, wchar_t* iPath, int      iValue);
void gfDataNode_SetValueF32 (DataNodeStruct* iNode, wchar_t* iPath, float    iValue);
void gfDataNode_SetValueF64 (DataNodeStruct* iNode, wchar_t* iPath, double   iValue);


wchar_t* gfDataNode_GetName     (DataNodeStruct* iNode);

void     gfDataNode_GetValuesVA (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iFormat, void* iVArgs);
void     gfDataNode_GetValues   (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iFormat, ...);


char*    gfDataNode_GetValueSZ  (DataNodeStruct* iNode, wchar_t* iPath);
wchar_t* gfDataNode_GetValueWSZ (DataNodeStruct* iNode, wchar_t* iPath);
int      gfDataNode_GetValueI32 (DataNodeStruct* iNode, wchar_t* iPath);
float    gfDataNode_GetValueF32 (DataNodeStruct* iNode, wchar_t* iPath);
double   gfDataNode_GetValueF64 (DataNodeStruct* iNode, wchar_t* iPath);

wchar_t* gfDataNode_GetNameOr     (DataNodeStruct* iNode, wchar_t* iName, wchar_t* iOrName);
char*    gfDataNode_GetValueSZOr  (DataNodeStruct* iNode, wchar_t* iPath, char*    iOrValue);
wchar_t* gfDataNode_GetValueWSZOr (DataNodeStruct* iNode, wchar_t* iPath, wchar_t* iOrValue);
int      gfDataNode_GetValueI32Or (DataNodeStruct* iNode, wchar_t* iPath, int      iOrValue);
float    gfDataNode_GetValueF32Or (DataNodeStruct* iNode, wchar_t* iPath, float    iOrValue);
double   gfDataNode_GetValueF64Or (DataNodeStruct* iNode, wchar_t* iPath, double   iOrValue);

extern DataNodeStruct gDataNodeStruct_Null;


#ifdef __cplusplus

//
//extern "C++"
//{
class DataNode;


class DataNode
{
	public:
	DataNodeStruct* Ref;
	
	 DataNode();
	~DataNode();
	
	DataNode(DataNodeStruct& iValue);
	DataNode(DataNodeStruct* iValue);
	
	DataNode(char*    iValue);
	DataNode(wchar_t* iValue);
	DataNode(int      iValue);
	DataNode(float    iValue);
	DataNode(double   iValue);
	
	operator DataNodeStruct& ();
	operator DataNodeStruct* ();
	
	operator char*    ();
	operator wchar_t* ();
	operator int      ();
	operator float    ();
	operator double   ();
	
	//bool             operator= ();
	bool             operator! ();

	//      DataNodeStruct*  operator[](wchar_t* iPath);
	//const DataNodeStruct*  operator[](wchar_t* iPath) const;
	
			DataNodeStruct&  operator[](char* iPath);
	const DataNodeStruct&  operator[](char* iPath) const;
		   DataNodeStruct&  operator[](wchar_t* iPath);
	const DataNodeStruct&  operator[](wchar_t* iPath) const;
	
	DataNodeValue&  GetValue(int iIndex);
	//int             GetValueI32(int iIndex);
	//float           GetValueF32(int iIndex);
	//double          GetValueF64(int iIndex);
	void            SetValue(int iIndex, DataNodeValue& iValue);
	
	DataNodeStruct& Create(wchar_t* iPath);
	
	bool Contains(wchar_t* iPath);
	void Get(wchar_t* iPath, wchar_t* iFormat, ...);
	
	void Load(wchar_t* iPath);
	void Save(wchar_t* iPath);
	
	int  ToStringPreview(wchar_t* iBuffer, int iBufferInitialSize);
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
	
	static DataNode* FromString (wchar_t* iString);
	static DataNode* FromFile   (wchar_t* iPath);
};
//}

extern DataNode gDataNode_Null;


#endif

#ifdef __cplusplus
    }
#endif

#endif

