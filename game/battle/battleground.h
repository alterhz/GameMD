// ����ս��

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include <map>

#include "utilityinc.h"

class IBattleGround
{
public:
	IBattleGround();
	virtual ~IBattleGround();

	int GetIndexId() const { return m_nIndexId; }

public:
	virtual bool Init();

protected:
	virtual bool OnInit() { return true; }

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
	// ��ʼ��
	virtual bool OnInit();

	// ��Ӷ�ս˫��
	bool InitTwoCountry(ICountry *pCountryA, ICountry *pCountryB);


private:
	// ����
	IGround * m_pGround;
	// ���ң����ӣ��б�
	MapCountry m_mapCountry;
};

class CBattleGroundManager : public Singleton<CBattleGroundManager>
{
	typedef std::map<int, IBattleGround *> MapBattleGround;
public:
	CBattleGroundManager();
	~CBattleGroundManager();

public:
	bool Init();

	// ����һ�����ս����
	CFrontBattleGround* CreateFrontBattleGround();


private:
	MapBattleGround m_mapBattleGround;
};

#endif