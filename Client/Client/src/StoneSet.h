#pragma once

#include "TableData.h"

#define STONE_EQUIP_MAX 3 // ×î¶àÈý¸ö±¦Ê¯Î»ÖÃ

class CStoneInfo : public CRawDataInfo {
public:
	CStoneInfo()
		: nItemID(0), nType(0) {
		memset(nEquipPos, 0, sizeof(nEquipPos));
		memset(szHintFunc, 0, sizeof(szHintFunc));
	}

	int nItemID;
	int nEquipPos[STONE_EQUIP_MAX];
	int nType;			 // ±¦Ê¯ÀàÐÍ
	char szHintFunc[64]; // Ð§¹ûÏÔÊ¾½Å±¾
	DWORD nItemRgb;

	bool IsEquip(int pos) { // µ±Ç°Î»ÖÃÊÇ·ñ¿ÉÒÔ×°±¸±¦Ê¯,´«Èë:1,2,3
		if (pos <= 1 || pos >= 3)
			return false;

		for (int i = 0; i < STONE_EQUIP_MAX; i++)
			if (nEquipPos[i] == pos)
				return true;

		return false;
	}
};

class CStoneSet : public CRawDataSet {
public:
	static CStoneSet* I() { return _Instance; }

	CStoneSet(int nIDStart, int nIDCnt)
		: CRawDataSet(nIDStart, nIDCnt) {
		_Instance = this;
		_Init();
	}

protected:
	static CStoneSet* _Instance; // Ïàµ±ÓÚµ¥¼ü, °Ñ×Ô¼º¼Ç×¡

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) {
		return new CStoneInfo[nCnt];
	}

	virtual void _DeleteRawDataArray() {
		delete[](CStoneInfo*) _RawDataArray;
	}

	virtual int _GetRawDataInfoSize() {
		return sizeof(CStoneInfo);
	}

	virtual void* _CreateNewRawData(CRawDataInfo* pInfo) {
		return NULL;
	}

	virtual void _DeleteRawData(CRawDataInfo* pInfo) {
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo* pRawDataInfo, vector<string>& ParamList) {
		// if(ParamList.size()==0) return FALSE;

		CStoneInfo* pInfo = (CStoneInfo*)pRawDataInfo;

		int n, m = 0;
		string strList[10], strLine;

		pInfo->nItemID = Str2Int(ParamList[m++]);

		strLine = ParamList[m++];
		n = Util_ResolveTextLine(strLine.c_str(), strList, 10, ',');
		n = n > STONE_EQUIP_MAX ? STONE_EQUIP_MAX : n;
		for (int i = 0; i < n; i++)
			pInfo->nEquipPos[i] = Str2Int(strList[i]);

		pInfo->nType = Str2Int(ParamList[m++]);

		strncpy(pInfo->szHintFunc, ParamList[m++].c_str(), sizeof(pInfo->szHintFunc));

		pInfo->nItemRgb = strtoul(ParamList[m++].c_str(), NULL, 16); //Str2Int(ParamList[m++]);
		return TRUE;
	}
};

inline CStoneInfo* GetStoneInfo(int nTypeID) {
	return (CStoneInfo*)CStoneSet::I()->GetRawDataInfo(nTypeID);
}
