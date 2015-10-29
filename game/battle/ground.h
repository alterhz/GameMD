// ����

#ifndef _GROUND_H_
#define _GROUND_H_

#include <map>
#include "macrosdef.h"
#include "configread.h"

class IGObject;

// ����
class IGrid
{
	typedef std::map<int, IGObject *> MapGObject;

public:
	IGrid(int x, int y);
	virtual ~IGrid() {}

public:
	int GetSN() const { return m_nSN; }
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	bool Init(int nSN);

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
	int m_nSN;
	const CXmlData_Ground *m_pXmlData_Ground;
	int m_nX;
	int m_nY;
	// �����ϰ󶨵ĳ����������2������������ս����λ������ǳ�ǽ������ǽ���ݻٺ󣬿��Ժ�ս����λһ�𹲴档
	MapGObject m_mapGObject;
};

// ��ͨ����
class CGrid : public IGrid
{
public:
	CGrid(int x, int y);
	~CGrid() {}
};

// ����
class IGround
{
public:
	typedef std::map<int, IGrid *> MapGrid;

public:
	IGround();
	virtual ~IGround();

	int GetWGCount() const { return m_nWGCount; }
	int GetHGCount() const { return m_nHGCount; }

public:
	IGrid * GetGrid(int x, int y);
	IGrid * GetGrid(int x, int y) const;
	const MapGrid & GetMapGrid() const { return m_mapGrid; }
	MapGrid & GetMapGrid() { return m_mapGrid; }

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

#endif