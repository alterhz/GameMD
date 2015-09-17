//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "memoryleak.h"
#include <string>
#include "action.h"
#include "attribute.h"

class IScene;

enum ESpriteType
{
	ESpriteType_Null = 0,
	ESpriteType_Role,
	ESpriteType_Monster,
};

// ���о��飨�����еĻ���壩�Ļ���
class ISprite
{
public:
	ISprite();
	virtual ~ISprite();

public:	//base
	int GetId() const { return m_nId; }
	std::string GetName() const { return m_strName; }
	void SetName(std::string strName) { m_strName = strName; }

	// ���õ�ǰѪ��
	void SetHP(int nHP);
	// �������Ѫ��
	void SetMaxHP(int nMaxHP);
	
public:	//base
	// ��ȡ�����������
	virtual ESpriteType GetSpriteType() const = 0;

public:
	// ���볡��
	virtual bool DoEnterScene();
	virtual bool DoLeaveScene();

public:
	// �߼�ѭ��
	virtual bool DoTick(MSTIME msNow);
	// ��ȡ��ɫ��ǰ����λ��
	virtual Vector2 GetCurrentPosition(MSTIME msNow) const;
	// �ƶ���ĳ��λ��(˲��վ��)���˺�������ʹ��
	virtual bool MoveTo(Vector2 &v2);
	// ��
	virtual bool RunTo(VtPath &vtPath, MSTIME msNow);

public:
	virtual bool OnEnterScene(IScene *pScene);
	virtual bool OnLeaveScene(IScene *pScene);


protected:
	// �߼�ѭ���¼�
	virtual bool OnTick(MSTIME msNow);
	// ״̬�Զ��л�
	virtual void OnActionAutoSwitch(MSTIME msNow);

protected:
	IAction * GetCurrentAction() { return m_pCurrentAction; }
	// �ı䵱ǰ״̬
	bool ChangeAction(IAction *pAction);

private:
	// ����Action������Action
	void DestroyAction(IAction *pAction);

protected:
	CAttribute m_attr;

private:
	int m_nId;
	std::string m_strName;

	IAction *m_pCurrentAction;

	int m_nWaitEnterSceneId;

	// ��ǰ���ڳ���
	IScene *m_pCurrentScene;
};

#endif