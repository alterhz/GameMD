// �������

#ifndef _GROUNDOBJECT_H_
#define _GROUNDOBJECT_H_

#include "macrosdef.h"
#include "xmldata.h"

class IGrid;
class IBattleGround;

class CStillObject;
class CWalkableObject;

#define CAMP_NEUTRAL	0	//������Ӫ
#define CAMP_EAST		1	//����
#define CAMP_WEST		2	//����

// ��������
class IGObject
{
public:
	IGObject();
	virtual ~IGObject() {}

	int GetIndexId() const { return m_nIndexId; }

	int GetSN() const { return m_nSN; }
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }
	void SetXY(int x, int y) { m_nX = x, m_nY = y; }

	COOR2 GetCoor2() const { return COOR2(m_nX, m_nY); }

	IGrid * GetGrid() const { return m_pGrid; }
	void BindGrid(IGrid *pGrid) { m_pGrid = pGrid; }

	// ��Ӫ
	void SetCampId(int nCampId) { m_nCampId = nCampId; }
	int GetCampId() const { return m_nCampId; }

	void SetBattleGround(IBattleGround *pBattleGround) { m_pBattleGround = pBattleGround; }
	IBattleGround * GetBattleGround() const { return m_pBattleGround; }

public:
	// ��ʼ������
	bool Init(int nSN);
	// ���볡��
	bool EnterGround(int x, int y, IBattleGround *pBattleGround);

protected:
	virtual bool OnInit() { return true; }

public:
	// ��ȡ��������
	virtual GObjectType GetType() const = 0;
	// ��ȡ��ֹ����
	virtual CStillObject * GetStillObject() { return nullptr; }
	// ��ȡ�ƶ�����
	virtual CWalkableObject* GetWalkableObject() { return nullptr; }
	// ���󲻿���ͨ��(һ�����Ӳ��ܴ���������ɫ��λ)
	virtual bool IsWalkable(EToWard eToWard) const = 0;
	
	// ս�����
	// �Ƿ����ս��
	virtual bool IsCanFight() const { return true; }
	// �Ƿ���Թ���
	virtual bool IsCanAttack() const { return true; }

	// �������
	virtual int GetAtt() const { return 1; }
	int GetHP() const { return m_nHP; }
	virtual int GetMaxHP() const = 0;
	// ��Ѫ
	int LostHP(int nHP);
	virtual int OnLostHP(int nHP) { return nHP; }
	virtual void OnDead() { }

	int GetSP() const { return m_nSP; }
	void AddSP(int nSP);
	virtual void OnAddSP(int nSP) {}

	int GetLevel() const { return m_nLevel; }
	void SetLevel(int nLevel) { m_nLevel = nLevel; }


private:
	int m_nIndexId;

protected:
	int m_nSN;
	// λ������
	int m_nX;
	int m_nY;
	// ��������
	IBattleGround *m_pBattleGround;

	// ����
	IGrid *m_pGrid;
	// ��ӪId
	int m_nCampId;

	int m_nHP;
	int m_nSP;
	int m_nLevel;
};

// ��ֹ���������ƶ����ĳ�������
class CStillObject : public IGObject
{
public:
	CStillObject();
	virtual ~CStillObject();

protected:
	// ��������
	virtual GObjectType GetType() const { return GObjectType_Still; }
	// ��ȡ��ֹ����
	virtual CStillObject * GetStillObject() { return this; }
	// ��ʼ��
	virtual bool OnInit();
	// �Ƿ����ͨ��
	virtual bool IsWalkable(EToWard eToWard) const;
	//ս�����
	// �Ƿ����ս��
	virtual bool IsCanFight() const;
	// �Ƿ���Թ���
	virtual bool IsCanAttack() const { return false; }
	// ��ȡ���Ѫ��
	virtual int GetMaxHP() const;
	// �����¼�
	virtual void OnDead();

public:
	// �Ƿ�Ϊ�������ͣ����죩
	bool IsFlag() const;

protected:
	const CXmlData_Still *m_pXmlData_Still;
};

// ��ɫ����
class CWalkableObject : public IGObject
{
public:
	CWalkableObject();
	virtual ~CWalkableObject();

public:
	// �ͷż���
	virtual bool UseSkill(int nTargetGObjectIndexId);

protected:
	// ��������
	virtual GObjectType GetType() const { return GObjectType_Walkable; }
	// ��ȡ�ƶ�����
	virtual CWalkableObject * GetWalkableObject() { return this; }
	// ��ʼ��
	virtual bool OnInit();
	// ���󲻿���ͨ��(һ�����Ӳ��ܴ���������ɫ��λ)
	virtual bool IsWalkable(EToWard eToWard) const;
	// ս�����
	virtual int GetAtt() const;
	// ��ȡ���Ѫ��
	virtual int GetMaxHP() const;

private:
	int GetCommonSkillSN() const;
	int GetSkillSN1() const;
	int GetSkillSN2() const;

public:
	// ����
	bool Move(int x, int y);
	bool Move(const VtCoor2 &vtCoor2);
	// ��ȡ�ƶ�����
	int GetWalkLength() const;

protected:
	const CXmlData_Walkable *m_pXmlData_Walkable;
};


#endif