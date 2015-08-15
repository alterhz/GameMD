//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string>
using namespace std;

// ���о��飨�����еĻ���壩�Ļ���
class ISprite
{
public:
	ISprite() {}
	virtual ~ISprite() {}

	int GetId() const { return m_nId; }
	string GetName() const { return m_strName; }


private:
	int m_nId;
	string m_strName;

};

#endif