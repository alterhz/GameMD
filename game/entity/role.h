//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _ROLE_H_
#define _ROLE_H_

#include "sprite.h"

class CRole : public ISprite
{
public:
	CRole();
	~CRole();

	// ��ʼ��
	bool Init();

public:
	// ��ȡ�����������
	virtual ESpriteType GetSpriteType() const { return ESpriteType_Role; }

};


#endif