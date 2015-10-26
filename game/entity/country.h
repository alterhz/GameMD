// ���ң����ӣ�

#ifndef _COUNTRY_H_
#define _COUNTRY_H_

#include <map>
#include <vector>
#include <list>

#include "event.h"
#include "msginc.h"
#include "macrosdef.h"
#include "utilityinc.h"

class CClient;


class ICountry
{
public:
	virtual ~ICountry() {}

protected:
	ICountry();

public:
	int GetIndexId() const { return m_nIndexId; }
	CClient * GetClient() const { return m_pClient; }

public:
	// ���ս������
	bool AddFightGObject(IFightGObject *pFightGObject);
	bool AddFightGObject(const VtFightGObject &vtFightGObject);
	// ���ս������
	void ClearFightGObject() { m_vtFightGObject.clear(); }

	// ��ȡս������
	const VtFightGObject & GetFightGObject() const { return m_vtFightGObject; }

public:
	void OnConnected(CClient *pClient);
	void OnDisconnect();

public:	// ��Ϣ����
	//bool SendPrepare(bool bOk);

private:
	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

private:
	int m_nIndexId;
	// ͨ�Ŷ���
	CClient *m_pClient;
	// �Ͽ�ʱ��
	time_t m_tDisconnect;

	// ����
	VtFightGObject m_vtFightGObject;
};

class CCountry : public ICountry
{
public:
	CCountry();
	~CCountry();
	
public:
	
};

// ���ҹ���
class CCountryManager : public Singleton<CCountryManager>, public NS_IO::ITimerEvent
{
	typedef std::map<int, ICountry *> MapCountry;
	typedef std::vector<ICountry *> VtCountry;
	typedef std::list<ICountry *> ListCountry;

public:
	CCountryManager();
	~CCountryManager();

public:
	// �������Ҳ���
	ICountry * CreateCountry();
	// ׼��
	bool PushPrepareList(ICountry *pCountry);


protected:
	virtual bool OnTimerEvent(int nTimerId);
	// ƥ��
	void OnMatched(ICountry *pCountryA, ICountry *pCountryB);

private:
	int m_nTimerId;
	MapCountry m_mapCountry;
	ListCountry m_listCountry;
};

#endif