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
	const T * GetRecord(int nId) const;

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

class CConfigReadManager
{
public:
	CConfigReadManager();
	~CConfigReadManager();

	bool LoadConfigData();

public:
	CConfigRead<CXmlData_Config> xdConfig;
};


#endif