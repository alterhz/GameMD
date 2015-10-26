// ����ս��

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include <map>

#include "event.h"
#include "utilityinc.h"

class ICountry;

class IBattleGround
{
	enum EStatus
	{
		EStatus_None = 0,
		EStatus_Waiting,
		EStatus_Run,
		EStatus_Pause,
		EStatus_Finish,
	};

public:
	IBattleGround();
	virtual ~IBattleGround();

	int GetIndexId() const { return m_nIndexId; }

	EStatus GetCurrentStatus() const { return m_eStatus; }
	bool ChangeStatus(EStatus eStatus);

public:
	bool Init();
	bool DoTick();

public:
	// ��ȡ��������
	virtual void GetGroundInfo(ICountry *pCountry);


protected:
	virtual bool OnInit() { return true; }
	virtual bool OnTick() { return true; }
	virtual bool OnGoRun() { return true; }
	virtual bool OnGoFinish() { return true; }

protected:
	EStatus m_eStatus;

private:
	int m_nIndexId;
};

class ICountry;
class IGround;

// ս��(���ս)
class CFrontBattleGround : public IBattleGround
{
	typedef std::map<int, ICountry *> MapCountry;

public:
	CFrontBattleGround();
	~CFrontBattleGround();

public:
	// ��Ӷ�ս˫��
	bool InitTwoCountry(ICountry *pCountryA, ICountry *pCountryB);

public:
	// ��ȡ��������
	virtual void GetGroundInfo(ICountry *pCountry);

protected:
	// ��ʼ��
	virtual bool OnInit();
	virtual bool OnTick();

private:
	// ����
	IGround * m_pGround;
	// ���ң����ӣ��б�
	MapCountry m_mapCountry;
};

class CBattleGroundManager : public Singleton<CBattleGroundManager>, public NS_IO::ITimerEvent
{
	typedef std::map<int, IBattleGround *> MapBattleGround;
public:
	CBattleGroundManager();
	~CBattleGroundManager();

public:
	bool Init();

	// ����һ�����ս����
	CFrontBattleGround* CreateFrontBattleGround();

protected:
	// ����true:����������false:��ֹ
	virtual bool OnTimerEvent(int nTimerId);


private:
	int m_nDoTickTimerId;
	MapBattleGround m_mapBattleGround;
};

#endif