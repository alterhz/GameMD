// ����ս��

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include <map>

class IBattleGround
{
public:
	IBattleGround() {}
	virtual ~IBattleGround() {}

public:
	virtual bool Init() { return true; }

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
	virtual bool Init();


private:
	int m_nIndexId;
	// ���ң����ӣ��б�
	MapCountry m_mapCountry;
	// ����
	IGround * m_pGround;
};


#endif