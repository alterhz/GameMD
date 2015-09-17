#include "scene.h"
#include "debug.h"
using namespace NS_IO;

#include "macrosdef.h"
#include "zone.h"
#include "sprite.h"

IScene::IScene(ESceneType eSceneType)
	: m_eSceneType(eSceneType)
	, m_nId(0)
	, m_bDead(false)
	, m_pZoneManager(nullptr)
{
}

IScene::~IScene()
{
}

bool IScene::Init()
{
	if (m_pZoneManager)
	{
		SAFE_DELETE(m_pZoneManager);
	}

	if (!m_pZoneManager->Init())
	{
		LOGError("��ʼ��ZoneManager������ʧ�ܡ�");
		return false;
	}

	return OnInit();
}

bool IScene::DoTick()
{
	return OnTick();
}

bool IScene::SpriteEnter(ISprite *pSprite)
{
	if (nullptr == pSprite)
	{
		LOGError("nullptr == pSprite");
		return false;
	}

	// �ж�Sprite�Ƿ��Ѿ��ڳ�����


	pSprite->OnEnterScene(this);

	OnSpriteEnter(pSprite);

	return true;
}

bool IScene::SpriteLeave(ISprite *pSprite)
{
	return OnSpriteLeave(pSprite);
}

//////////////////////////////////////////////////////////////////////////
// �����¼�
bool IScene::OnInit()
{
	return true;
}

bool IScene::OnTick()
{
	return true;
}

bool IScene::OnSpriteEnter(ISprite *pSprite)
{
	return true;
}

bool IScene::OnSpriteLeave(ISprite *pSprite)
{
	return true;
}
