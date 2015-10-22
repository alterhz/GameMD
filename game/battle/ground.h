// ����

#ifndef _GROUND_H_
#define _GROUND_H_

#include <map>

// ����
class IGrid
{
public:
	enum EToWard
	{
		EToWard_None = 0,	//��ͨ
		EToWard_X ,	//X����ͨ��
		EToWard_Y,	//Y����ͨ��
		EToWard_Both,	//˫��ͨ��
	};

public:
	IGrid();
	virtual ~IGrid() {}


public:
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	void Set(int nX, int nY) { m_nX = nX; m_nY = nY; }

public:
	// �Ƿ��������
	virtual bool IsWalkable(EToWard eToWard) const { return true; }

private:
	int m_nX;
	int m_nY;
};

// ��ͨ����
class CBaseGrid : public IGrid
{
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
	CBaseGrid(EGroundType eGroundType);
	~CBaseGrid() {}

public:
	void SetGroundType(EGroundType eGroundType) { m_eGroundType = eGroundType; }
	EGroundType GetGroundType() const { return m_eGroundType; }

public:
	virtual bool IsWalkable(EToWard eToWard) const;

private:
	EGroundType m_eGroundType;
};

// ����
class CBridgeGrid : public CBaseGrid
{
public:
	CBridgeGrid(EGroundType eGroundType, EToWard eToWard);
	~CBridgeGrid() {}

public:
	virtual bool IsWalkable(EToWard eToWard) const;

private:
	// ����ͨ���ĳ���
	EToWard m_eToWard;
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

protected:
	bool InitGroundSize(int nWGCount, int nHGCount);

protected:
	int XY2N(int nH, int nW) const;
	int N2X(int n) const;
	int N2Y(int n) const;

	// �޸�Ŀ����ӵ�������
	bool SetGrid(int nX, int nY, IGrid *pGrid);

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

};

#endif