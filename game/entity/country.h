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
class IBattleGround;

class ICountry
{
public:
	virtual ~ICountry() {}

protected:
	ICountry();

public:
	int GetIndexId() const { return m_nIndexId; }
	CClient * GetClient() const { return m_pClient; }

	void SetBattleGround(IBattleGround *pBattleGround) { m_pBattleGround = pBattleGround; }
	IBattleGround * GetBattleGround() { return m_pBattleGround; }

	void PrepareGround(int nBattleGroundIndexId);
	void ClearPrepareGround() { m_nPrepareBattleGroundIndexId = 0; }

public:
	// ���볡��
	void EnterGround();
	// �뿪����
	void LeaveGround();

public:
	// ���ս������
	bool AddFightGObject(IGObject *pFightGObject);
	bool AddFightGObject(const VtGObject &vtFightGObject);
	// ���ս������
	void ClearFightGObject() { m_vtFightGObject.clear(); }

	// ��ȡս������
	const VtGObject & GetFightGObject() const { return m_vtFightGObject; }

public:
	void OnConnected(CClient *pClient);
	void OnDisconnect();

public:	// ��Ϣ����
	// ׼������
	bool SendPrepareGround();
	// ������Ϣ��ȡ
	bool SendGetGroundInfo(int nWGCount, int nHGCount, const MapGrid &mapGrid, const MapGObject &mapGObject);
	// ���볡��Ӧ��
	bool SendEnterGround(bool bOk);
	// �뿪����Ӧ��
	bool SendLeaveGround(bool bOk);
	// GObject���볡��
	bool SendGObjectEnterGround(GObjectType gObjectType, int nIndexId, int nSN, int x, int y,
		int nHP, int nMaxHP, int nSP, int nLevel, int nCampId);

private:
	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

private:
	int m_nIndexId;
	// ͨ�Ŷ���
	CClient *m_pClient;
	// �Ͽ�ʱ��
	time_t m_tDisconnect;

	// ����
	VtGObject m_vtFightGObject;

	// �����볡��
	int m_nPrepareBattleGroundIndexId;
	// ������Ϣ
	IBattleGround *m_pBattleGround;
	
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