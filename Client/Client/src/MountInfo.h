/******************************************************************
**	Project Satisfy 2021
**	Base Files: PKO/Corsair, Mothanna Files
**	All rights for code changes/edits/mods and Interface Changes to
**					Mothanna And Satisfy
*******************************************************************/
#pragma once
#include "util.h"
#include "TableData.h"

#define Mount_MAXSIZE_NAME			32 // mount name size
#define MaxChatrcterx	4

class MountInfo : public CRawDataInfo
{
public:
	int CharID;	//characterinfo id for future use 
	int ItemID;	//iteminfo id  for future use 
	char szName[Mount_MAXSIZE_NAME];		// mount name
	int mountBone[MaxChatrcterx];
	int MountHeight[MaxChatrcterx];
	int PoseID[MaxChatrcterx];
	int OffSetx[MaxChatrcterx];
	int OffSety[MaxChatrcterx];
	float Mountscale[MaxChatrcterx];
	
};


class MountHelper : public CRawDataSet
{
public:

	static MountHelper* I() { return _Instance; }

	MountHelper(int nIDStart, int nIDCnt, int nCol = 128)
		: CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

	//const char* ConvertResString(const char* str);

protected:

	static MountHelper* _Instance; // 锟洁当锟节碉拷锟斤拷, 锟斤拷锟皆硷拷锟斤拷住


	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new MountInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[](MountInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(MountInfo);
	}

	virtual void* _CreateNewRawData(CRawDataInfo* pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo* pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo* pRawDataInfo, std::vector<std::string>& ParamList);
};


inline MountInfo* GetMountInfo(int nTypeID)
{
	return (MountInfo*)MountHelper::I()->GetRawDataInfo(nTypeID);
}

inline MountInfo* GetMountInfoFromItemID(int ItemID)
{
	MountHelper* helper = MountHelper::I();
	for (int i = 0; i < helper->GetIDCnt(); i++)
		if (MountInfo* pInfo = (MountInfo*)helper->GetRawDataInfo(helper->GetIDStart() + i))
			if (pInfo->ItemID == ItemID)
				return pInfo;
	return NULL;
}

