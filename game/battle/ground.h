// ����

#ifndef _GROUND_H_
#define _GROUND_H_

#include <map>

// ����
class IGrid
{
public:
	IGrid() {}
	virtual ~IGrid() {}


public:
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }


private:
	int m_nX;
	int m_nY;
};


// ����
class IGround
{
	typedef std::map<int, IGrid *> MapGrid;

public:
	IGround();
	virtual ~IGround();

	bool Init(int nWGCount, int nHGCount);

private:
	int HW2N(int nH, int nW) const;
	int N2H(int n) const;
	int N2W(int n) const;

private:
	MapGrid m_mapGrid;
	int m_nWGCount;	//Width����Grid����
	int m_nHGCount;	//Heigth����Grid����
};




#endif