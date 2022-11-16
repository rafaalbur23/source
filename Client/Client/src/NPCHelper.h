#pragma once
#include "util.h"
#include "TableData.h"

#define NPC_MAXSIZE_NAME			128 // npc���Ƴ���

class NPCData : public CRawDataInfo
{
public:
	char szName[NPC_MAXSIZE_NAME];		// ��ͼ����ʾnpc����
	char szArea[NPC_MAXSIZE_NAME];		// λ�û��ߵȼ�
	DWORD dwxPos0, dwyPos0;				// npcλ����Ϣ
	char szMapName[NPC_MAXSIZE_NAME];	// npc���ڵ�ͼ����
};


class NPCHelper : public CRawDataSet
{
public:

	static NPCHelper* I() { return _Instance; }

	NPCHelper(int nIDStart, int nIDCnt, int nCol = 128)
		: CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

	//const char* ConvertResString(const char* str);

protected:

	static NPCHelper* _Instance; // �൱�ڵ���, ���Լ���ס


	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new NPCData[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[](NPCData*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(NPCData);
	}

	virtual void* _CreateNewRawData(CRawDataInfo* pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo* pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo* pRawDataInfo, vector<string>& ParamList);
};


inline NPCData* GetNPCDataInfo(int nTypeID)
{
	return (NPCData*)NPCHelper::I()->GetRawDataInfo(nTypeID);
}