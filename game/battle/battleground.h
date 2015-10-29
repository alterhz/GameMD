// ����ս��

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include <map>

#include "event.h"
#include "ground.h"
#include "utilityinc.h"

class ICountry;

class IBattleGround : public IGround
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
	// ����ս��
	virtual bool Enter(ICountry *pCountry);
	// �뿪ս��
	virtual bool Leave(ICountry *pCountry);
	// ����
	virtual bool GObjectEnter(IGObject *pGObject);
	virtual bool GObjectLeave(IGObject *pGObject);
	// ׼�����
	virtual void PrepareFinish(ICountry *pCountry);
	// ��ǰ�غ�ս������
	virtual void BattleBoutFinish(ICountry *pCountry);


protected:
	virtual bool OnInit() { return true; }
	virtual bool OnTick() { return true; }
	virtual bool OnGoRun() { return true; }
	virtual bool OnGoFinish() { return true; }

	virtual bool OnEnter(ICountry *pCountry);
	virtual bool OnLeave(ICountry *pCountry) { return true; }
	virtual bool OnGObjectEnter(IGObject *pGObject);
	virtual bool OnGObjectLeave(IGObject *pGObject) { return true; }

protected:
	ICountry * FindCountry(int nIndexId);
	ICountry * FindCountry(int nIndexId) const;
	IGObject * FindGObject(int nIndexId);
	IGObject * FindGObject(int nIndexId) const;

protected:
	EStatus m_eStatus;
	// ���ң����ӣ��б�
	MapCountry m_mapCountry;
	// ������λ��������������ͽ�ɫ��
	MapGObject m_mapGObject;

private:
	int m_nIndexId;
};

// ս��(���ս)
class CFrontBattleGround : public IBattleGround
{
	enum EBattleStatus
	{
		EBattleStatus_Waiting = 1,	//�ȴ���ɫ����
		EBattleStatus_Prepare,	//׼��
		EBattleStatus_Battle,	//ս��
		EBattleStatus_Reward,	//����
	};

public:
	CFrontBattleGround();
	~CFrontBattleGround();

	void ChangeBattleStatus(EBattleStatus eBattleStatus);
	EBattleStatus GetCurrentBattleStatus() const { return m_eBattleStatus; }

protected:
	// ��ʼ��
	virtual bool OnInit();
	virtual bool OnTick();
	// ��ɫ���볡���¼�
	virtual bool OnEnter(ICountry *pCountry);
	virtual bool OnLeave(ICountry *pCountry);
	// ׼�����
	virtual void PrepareFinish(ICountry *pCountry);
	// ��ǰ�غ�ս������
	virtual void BattleBoutFinish(ICountry *pCountry);

	void OnGoPrepare();
	void OnGoBattle();
	void OnGoReward();

private:
	EBattleStatus m_eBattleStatus;
	VtInt m_vtPrepareFinishCountryIndexId;
	int m_nBoutIndex;	//�غ�����˳��
	int m_nBoutCountryIndexId;	//��ǰ�غϵ�CountryIndexId
	// ʤ����CountryIndexId
	int m_nWinCountryIndexId;
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

	IBattleGround * FindBattleGround(int nBattleGroundIndexId);
	IBattleGround * FindBattleGround(int nBattleGroundIndexId) const;


protected:
	// ����true:����������false:��ֹ
	virtual bool OnTimerEvent(int nTimerId);


private:
	int m_nDoTickTimerId;
	MapBattleGround m_mapBattleGround;
};

#endif