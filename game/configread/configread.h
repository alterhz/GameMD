//-------------------------------------------
// author : alterhz
// date : 8/19/2015
// description : 

#ifndef _CONFIGREAD_H_
#define _CONFIGREAD_H_

#include "tinyxml2.h"
#include <string>
#include <map>
#include "xmldata.h"
#include "utilityinc.h"

template<typename T>
class CConfigRead
{
	typedef std::map<int, T *> MapT;

public:
	CConfigRead();
	~CConfigRead();


public:
	bool LoadConfigData(std::string strFileName);
	// ��������
	const T * GetRecord(int nId) const
	{
		MapT::const_iterator it = m_mapRecord.find(nId);
		if (it != m_mapRecord.end())
		{
			return it->second;
		}

		return nullptr;
	}

protected:
	// ������¼����
	void OnRead(const tinyxml2::XMLElement *pEleRecord);

private:
	// xml��¼�ڵ�����
	std::string m_strRecordNodeName;
	// ��������
	std::string m_strKeyName;
	// ���ݼ�
	MapT m_mapRecord;
};

class CConfigReadManager : public Singleton<CConfigReadManager>
{
public:
	CConfigReadManager();
	~CConfigReadManager();

	// ��������
	bool LoadConfigData();

	// ȫ������ID
	int GetGlobalConfigId() const { return m_nGlobalConfigId; }

public:
	int m_nGlobalConfigId;
	// ȫ������
	CConfigRead<CXmlData_Config> xdConfig;
	
};


#endif