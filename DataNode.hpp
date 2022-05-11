#pragma once

struct DLContext;
struct DLSyntaxNode;


#define dDataNodeNameMaxLength 64
#define dDataNodeValueMaxLength sizeof(void*) / sizeof(wchar_t)
#define dDataNodeCollectionCapacityMultiplier 1.2f

enum DataNodeType
{
	DN_T_Null,
	DN_T_Undefined,
	DN_T_Element,
	DN_T_Attribute,
	
};
enum DataNodeValueType
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
};
class DataNode;
class DataNodeCollection
{
	public:
	DataNode* Owner;
	DataNode* Items;
	int Count,Capacity;
	
	 ///DataNodeCollection(DataNode* iOwner);
	 /*DataNodeCollection();
	~DataNodeCollection();*/
	
	///DataNode* Allocate(int iCount);
};
class DataNode
{
	public:
	
	wchar_t Name[dDataNodeNameMaxLength];
	wchar_t Value[dDataNodeValueMaxLength];
	
	DataNodeType         Type;
	DataNodeValueType    ValueType;
	
	DataNode*            Parent;
	DataNodeCollection*  Children;
	
	 DataNode();
	 DataNode(DataNodeType iNodeType);
	 ///DataNode(DLSyntaxNode* iSyntaxNode);
	~DataNode();
	
	DataNode(char*    iValue);
	DataNode(wchar_t* iValue);
	DataNode(int      iValue);
	DataNode(float    iValue);
	DataNode(double   iValue);

	operator char*    ();
	operator wchar_t* ();
	operator int      ();
	operator float    ();
	operator double   ();

		
		///value_t& operator[](std::size_t idx)       { return mVector[idx]; }
	///const value_t& operator[](std::size_t idx) const { return mVector[idx]; }
	
	

	bool             operator! ();

	DataNode&        operator[](int iChildIndex);
	const DataNode&  operator[](int iChildIndex) const;
	//DataNode&        operator[](char* iPath);
	//const DataNode&  operator[](char* iPath) const;
	DataNode&        operator[](wchar_t* iPath);
	const DataNode&  operator[](wchar_t* iPath) const;
	
	void FromDLSyntaxNode (DLSyntaxNode* iSyntaxNode, DLContext* iDLC);
	void ToDLSyntaxNode   (DLSyntaxNode* irSyntaxNode);

	DataNode& GetChildByNameAndIndex(char* iName, int iIndex);
	DataNode& GetChildByNameAndIndex(wchar_t* iName, int iIndex);

	void GetId   (wchar_t*& oId);
	void GetPath (wchar_t*& oPath);
	
	private:
	void Init();
	void Init(DataNodeType iNodeType);
	void Init(DataNodeType iNodeType, DataNodeValueType iValueType);
	void InitChildren();
	
	
	static DataNode Null;
	void InitStatic();
	
	
};