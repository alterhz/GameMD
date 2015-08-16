//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string>
#include "action.h"


// ���о��飨�����еĻ���壩�Ļ���
class ISprite
{
public:
	ISprite();
	virtual ~ISprite();

	int GetId() const { return m_nId; }
	std::string GetName() const { return m_strName; }
	void SetName(std::string strName) { m_strName = strName; }
	

public:
	// ��ȡ��ɫ��ǰ����λ��
	virtual Vector2 GetCurrentPosition(MSTIME msNow) const;
	// �ƶ���ĳ��λ��(˲��վ��)���˺�������ʹ��
	virtual bool MoveTo(Vector2 &v2);
	// ��
	virtual bool RunTo(VtPath &vtPath, MSTIME msNow);


protected:
	IAction * GetCurrentAction() { return m_pCurrentAction; }
	// �ı䵱ǰ״̬
	bool ChangeAction(IAction *pAction);

	// ����Action������Action
	void DestroyAction(IAction *pAction);

private:
	int m_nId;
	std::string m_strName;
	float m_fSpeed;

	IAction *m_pCurrentAction;
};

#endif