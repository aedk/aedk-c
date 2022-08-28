#include "DataNode.h"
#include "UL.h"
#include "AE.h"

#include <stdlib.h>
#include <string.h>

#include <wchar.h>
#include <assert.h>
#include <memory.h>
#include <stdarg.h>


#define nullptr 0
size_t _msize(void* iPtr);


DataNode gDataNode_Null;

DataNode::DataNode()
{
	///this->Init(DN_T_Undefined, DN_VT_Null);
	
	this->Ref = nullptr;
}

DataNode::~DataNode()
{
	if(this->Ref != nullptr && this->Ref->Parent == nullptr)
	{
		gfDataNode_Destroy(this->Ref);
		this->Ref = nullptr;
	}
	///throw "NI";
	HERE;
};

DataNode::DataNode(DataNodeStruct* iValue)
{
	this->Ref = iValue;
	///throw "NI";
};
DataNode::DataNode(DataNodeStruct& iValue)
{
	this->Ref = &iValue;
	///throw "NI";
};
DataNode::DataNode(char*    iValue)
{
	throw "NI";
};
DataNode::DataNode(wchar_t* iValue)
{
	throw "NI";
};
DataNode::DataNode(int      iValue)
{
	throw "NI";
};
DataNode::DataNode(float    iValue)
{
	throw "NI";
};
DataNode::DataNode(double   iValue)
{
	throw "NI";
};

DataNode::operator DataNodeStruct*   ()
{
	throw "NI";
};
DataNode::operator char*    ()
{
	throw "NI";
};
DataNode::operator wchar_t* ()
{
	throw "NI";
};
DataNode::operator int      ()
{
	throw "NI";
};
DataNode::operator float    ()
{
	throw "NI";
};
DataNode::operator double   ()
{
	return gfDataNode_GetValueF64(this->Ref, nullptr);
};



bool             DataNode::operator!() 
{
	throw "NI";
}

//DataNode&        DataNode::operator[](int iChildIndex) 
//{
//	throw "NI";
//}
//const DataNode&  DataNode::operator[](int iChildIndex) const
//{
//	throw "NI";
//}

DataNodeValue::DataNodeValue()
{
	HERE;
};
DataNodeValue::operator double()
{
	double oValue = gfDNValue_GetF64(this);///*((double*)&_TgtNode->Value.Data);
	HERE;
	return oValue;
};

      DataNodeStruct&  DataNode::operator[](char* iPath)
{
	throw "NI";
}


const DataNodeStruct&  DataNode::operator[](char* iPath) const
{
	throw "NI";
}

      DataNodeStruct&  DataNode::operator[](wchar_t* iPath)
{
	if(iPath[0] == '@')
	{
		HERE;
	}
	if(this->Ref == nullptr) this->Ref = gfDataNode_Create();
	
	DataNodeStruct* _DNPtr = gfDataNode_GetNodeByPath(this->Ref, iPath, true, true);
	
	if(_DNPtr != nullptr)
	{
		return *_DNPtr;
	}
	else
	{
		HERE;
		return gDataNodeStruct_Null;
	}
}


const DataNodeStruct&  DataNode::operator[](wchar_t* iPath) const
{
	throw "NI";
}

DataNodeStruct::DataNodeStruct(char*    iValue)
{
	gfDataNode_Init_1(this);
	throw "NI";
};
DataNodeStruct::DataNodeStruct(wchar_t* iValue)
{
	gfDataNode_Init_1(this);
	gfDataNode_SetValueWSZ(this, nullptr, iValue);
	//throw "NI";
};
DataNodeStruct::DataNodeStruct(int      iValue)
{
	gfDataNode_Init_1(this);
	gfDataNode_SetValueI32(this, nullptr, iValue);
};
DataNodeStruct::DataNodeStruct(float    iValue)
{
	gfDataNode_Init_1(this);
	gfDataNode_SetValueF32(this, nullptr, iValue);
};
DataNodeStruct::DataNodeStruct(double   iValue)
{
	gfDataNode_Init_1(this);
	gfDataNode_SetValueF64(this, nullptr, iValue);
};

//DataNodeStruct::operator DataNodeStruct*   ()
//{
//	throw "NI";
//};
DataNodeStruct::operator char*    ()
{
	throw "NI";
};
DataNodeStruct::operator wchar_t* ()
{
	wchar_t* oValue = gfDNValue_GetWStrPtr(&this->Value);
	
	HERE;
	return oValue;
};
DataNodeStruct::operator int      ()
{
	throw "NI";
};
DataNodeStruct::operator float    ()
{
	float oValue = gfDataNode_GetValueF32(this, nullptr);
	return oValue;
};
DataNodeStruct::operator double   ()
{
	double oValue = gfDataNode_GetValueF64(this, nullptr);
	return oValue;
};

DataNodeStruct& DataNodeStruct::operator= (const DataNodeStruct& iNode)
{
	this->Value.Type = iNode.Value.Type;

	memcpy(&this->Value,&iNode.Value, sizeof(this->Value) * sizeof(wchar_t));
	//memcpy(&this->Value,iNode.Value, sizeof(this->Value) * sizeof(wchar_t));
	
	return *this;
}



DataNodeValue&  DataNode::GetValue(int iIndex)
{
	DataNodeValue* oValue = nullptr;
	{
		DataNodeValue* cValue = &this->Ref->Value; int cVi = 0; while(cValue != nullptr)
		{
			if(cVi == iIndex) {oValue = cValue; break;}
			cValue = gfDNValue_GetNext(cValue);
			cVi ++;
		}
	}
	assert(oValue != nullptr);
	
	return *oValue;
}
DataNodeStruct&  DataNode::Create(wchar_t* iPath)
{
	DataNodeStruct& oNode = (*this)[iPath];
	{
		gfDataNode_UpdateState(&oNode, false);
	}
	return oNode;
}
bool DataNode::Contains(wchar_t* iPath)
{
	DataNodeStruct* _TgtNode = gfDataNode_GetNodeByPath(this->Ref, iPath, true, false);
	
	return _TgtNode != nullptr;
}
void DataNode::Get(wchar_t* iPath, wchar_t* iFormat, ...)
{
	va_list _VArgs;
	va_start(_VArgs, iFormat);
	
	gfDataNode_GetValuesVA(this->Ref, iPath, iFormat, _VArgs);
	
	va_end(_VArgs);
}
void DataNode::Save(wchar_t* iPath)
{
	throw "NI";
}
void DataNode::Load(wchar_t* iPath)
{
	throw "NI";
}


int DataNode::ToStringPreview(wchar_t* iBuffer, int iBufferSize)
{
	WString* _WStr = gfWString_Create(iBufferSize);
	gfDataNode_ToString(this->Ref, _WStr, 0);
	
	if(iBuffer      == nullptr)       throw "pre-allocated buffer only";
	if(iBufferSize  <= _WStr->Length) throw "buffer size";
	
	wcsncpy_s(iBuffer, _WStr->Length + 1, _WStr->Data, _WStr->Length);
	
	gfWString_Destroy(_WStr);
	
	return _WStr->Length;
}

int DataNode::ToStringPreview(wchar_t** irBuffer)
{
	assert(*irBuffer == nullptr);
	
	WString* _WStr = gfWString_Create(0);
	gfDataNode_ToString(this->Ref, _WStr, 0);
	
	int oBufferLength = _WStr->Length + 1;
	*irBuffer = new wchar_t[oBufferLength];
	wcsncpy_s(*irBuffer, oBufferLength, _WStr->Data, _WStr->Length);
	
	HERE;
	return oBufferLength;
}
DataNode* DataNode::FromString(wchar_t* iString)
{
	DataNodeStruct* _Struct = gfDataNode_FromString(iString);
	return new DataNode(_Struct);
}
DataNode* DataNode::FromFile(wchar_t* iPath)
{
	throw "NI";
}