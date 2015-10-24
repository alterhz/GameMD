// �������

#ifndef _GROUNDOBJECT_H_
#define _GROUNDOBJECT_H_

#include "macrosdef.h"

class IGrid;
class IGround;
class IFightGObject;
class IWalkableGObject;


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

public:
	// ��ʼ������
	bool Init();
	// ���볡��
	bool EnterGround(int x, int y, IGround *pGround);

protected:
	virtual bool OnInit() { return true; }

public:
	// ��ȡ��������
	virtual GObjectType GetType() const = 0;
	// ��ȡս������
	virtual IFightGObject * GetFightGObject() { return nullptr; }
	// ��ȡ�����߶���
	virtual IWalkableGObject * GetWalkableGObject() { return nullptr; }

	// ���󲻿���ͨ��(һ�����Ӳ��ܴ���������ɫ��λ)
	virtual bool IsWalkable(EToWard eToWard) const = 0;

private:
	int m_nIndexId;

protected:
	// λ������
	int m_nX;
	int m_nY;
	// ��������
	IGround *m_pGround;

	// ����
	IGrid *m_pGrid;
};

// ����
class CGBridge : public IGObject
{
public:
	CGBridge(EToWard eToWard) 
		: m_eToWard(eToWard)
	{}
	~CGBridge() {}

public:
	virtual GObjectType GetType() const { return GObjectType_Bridge; }
	virtual bool IsWalkable(EToWard eToWard) const;

private:
	// ����ͨ���ĳ���
	EToWard m_eToWard;
};

// ս������
class IFightGObject : public IGObject
{
public:
	IFightGObject();
	virtual ~IFightGObject();

public:
	// ���󲻿���ͨ��(һ�����Ӳ��ܴ���������ɫ��λ)
	virtual bool IsWalkable(EToWard eToWard) const { return false; }
	// ��ȡս������
	virtual IFightGObject * GetFightGObject() { return this; }

protected:
	virtual bool OnInit();

public:
	virtual int GetHP() const { return m_nHP; }
	virtual int GetAtt() const { return m_nAtt; }

protected:
	int m_nHP;
	int m_nAtt;
};

// �����߶���
class IWalkableGObject : public IFightGObject
{
public:
	IWalkableGObject();
	virtual ~IWalkableGObject();

public:
	// ��ȡ�����߶���
	virtual IWalkableGObject * GetWalkableGObject() { return this; }

public:
	// ����
	bool Move(int x, int y);

};

// С��
class CDogFace : public IWalkableGObject 
{
public:
	CDogFace();
	~CDogFace();

public:
	virtual GObjectType GetType() const { return GObjectType_DogFace; }
};

// Ӣ��
class CHero : public IWalkableGObject 
{
public:
	CHero() {}
	~CHero() {}

public:
	virtual GObjectType GetType() const { return GObjectType_Hero; }

};

// ���죨˾��٣�
class CSirdar : public IFightGObject
{
public:
	CSirdar() {}
	~CSirdar() {}

public:
	virtual GObjectType GetType() const { return GObjectType_Sirdar; }

};


#endif