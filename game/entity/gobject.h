// �������

#ifndef _GROUNDOBJECT_H_
#define _GROUNDOBJECT_H_

#include "macrosdef.h"
#include "xmldata.h"

class IGrid;
class IGround;

class CStillObject;
class CWalkableObject;


// ��������
class IGObject
{
public:
	IGObject();
	virtual ~IGObject() {}

	int GetIndexId() const { return m_nIndexId; }

	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	IGrid * GetGrid() const { return m_pGrid; }
	void BindGrid(IGrid *pGrid) { m_pGrid = pGrid; }

	// ��Ӫ
	void SetCampId(int nCampId) { m_nCampId = nCampId; }
	int GetCampId() const { return m_nCampId; }


public:
	// ��ʼ������
	bool Init(int nSN);
	// ���볡��
	bool EnterGround(int x, int y, IGround *pGround);

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
	// �Ƿ���Թ���
	virtual bool IsCanAttack() const { return true; }
	virtual int GetHP() const { return 0; }
	virtual int GetSP() const { return 0; }
	virtual int GetAtt() const { return 0; }


private:
	int m_nIndexId;

protected:
	int m_nSN;
	// λ������
	int m_nX;
	int m_nY;
	// ��������
	IGround *m_pGround;

	// ����
	IGrid *m_pGrid;
	// ��ӪId
	int m_nCampId;
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
	virtual bool IsCanAttack() const;

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
	virtual bool IsCanAttack() const { return true; }
	

public:
	// ����
	bool Move(int x, int y);
	// ��ȡ�ƶ�����
	int GetWalkLength() const;

protected:
	const CXmlData_Walkable *m_pXmlData_Walkable;
};


#endif