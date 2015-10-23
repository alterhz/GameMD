// ����

#ifndef _GROUND_H_
#define _GROUND_H_

#include <map>
#include "macrosdef.h"

class IGObject;

// ����
class IGrid
{
	typedef std::map<int, IGObject *> MapGObject;

public:
	enum EGroundType
	{
		EGroundType_Null = 0,
		EGroundType_Land,	//����
		EGroundType_Sand,	//ɳ��
		EGroundType_River,	//����
		EGroundType_Lawn,	//�ݵ�
		EGroundType_Snow,	//ѩ��
	};

public:
	IGrid(EGroundType eGroundType, int x, int y);
	virtual ~IGrid() {}


public:
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	EGroundType GetGroundType() const { return m_eGroundType; }

	// ��ӳ����󶨵�λ
	bool AddGObject(IGObject *pGObject);
	bool DelGObject(IGObject *pGObject);

public:
	// �Ƿ��������
	virtual bool IsWalkable(EToWard eToWard) const;

protected:
	virtual void OnAddGObject(IGObject *pGObject);
	virtual void OnDelGObject(IGObject *pGObject);


private:
	IGObject * FindGObject(int nIndexId);
	IGObject * FindGObject(int nIndexId) const;


private:
	int m_nX;
	int m_nY;
	// ��������
	EGroundType m_eGroundType;
	// �����ϰ󶨵ĳ����������2������������ս����λ������ǳ�ǽ������ǽ���ݻٺ󣬿��Ժ�ս����λһ�𹲴档
	MapGObject m_mapGObject;
};

// ��ͨ����
class CGrid : public IGrid
{
public:
	CGrid(EGroundType eGroundType, int x, int y);
	~CGrid() {}
};

const int G_nDemoWidthCount = 20;
const int G_nDemoHeigthCount = 30;

// ����
class IGround
{
public:
	typedef std::map<int, IGrid *> MapGrid;

public:
	IGround();
	virtual ~IGround();

public:
	virtual bool Init();

	IGrid * GetGrid(int x, int y);
	IGrid * GetGrid(int x, int y) const;

protected:
	bool InitGroundSize(int nWGCount, int nHGCount);

protected:
	int XY2N(int x, int y) const;
	int N2X(int n) const;
	int N2Y(int n) const;

	// �޸�Ŀ����ӵ�������
	bool SetGrid(int x, int y, IGrid *pGrid);

private:
	IGrid * Find(int n);
	IGrid * Find(int n) const;

protected:
	MapGrid m_mapGrid;
	int m_nWGCount;	//Width����Grid����
	int m_nHGCount;	//Heigth����Grid����
};

class CDemoGround : public IGround
{
public:
	CDemoGround();
	~CDemoGround();

public:
	virtual bool Init(int nWGCount, int nHGCount);

	// ����
};


#endif