#include "sprite.h"
#include "debug.h"
using namespace NS_IO;

int g_nSpriteIdAlloc = 0;

ISprite::ISprite()
	: m_nId(++g_nSpriteIdAlloc)
	, m_pCurrentAction(nullptr)
{
	m_attr.SetAttr(EAttrType_Speed, 4.0f);
}

ISprite::~ISprite()
{
	DestroyAction(m_pCurrentAction);
	m_pCurrentAction = nullptr;
}

bool ISprite::DoTick(MSTIME msNow)
{
	// �Զ��л�״̬
	OnActionAutoSwitch(msNow);

	return OnTick(msNow);
}

Vector2 ISprite::GetCurrentPosition( MSTIME msNow ) const
{
	if (m_pCurrentAction)
	{
		return m_pCurrentAction->GetCurrentPosition(msNow);
	}

	return Vector2();
}

bool ISprite::MoveTo( Vector2 &v2 )
{
	CStandAction *pStandAction = new CStandAction(v2);
	if (nullptr == pStandAction)
	{
		return false;
	}

	if (ChangeAction(pStandAction))
	{
		return true;
	}

	DestroyAction(pStandAction);
	return false;
}

bool ISprite::RunTo( VtPath &vtPath, MSTIME msNow)
{
	float fSpeed = m_attr.GetAttr(EAttrType_Speed);

	CRunAction *pRunAction = new CRunAction(vtPath, fSpeed, msNow);

	if (nullptr == pRunAction)
	{
		return false;
	}

	if (ChangeAction(pRunAction))
	{
		return true;
	}

	DestroyAction(pRunAction);
	return false;
}

bool ISprite::OnEnterScene(IScene *pScene)
{
	return true;
}

bool ISprite::OnLeaveScene(IScene *pScene)
{
	return true;
}

bool ISprite::ChangeAction( IAction *pAction )
{
	if (nullptr == pAction)
	{
		LOGError("nullptr == pAction");
		return false;
	}

	if (nullptr == m_pCurrentAction)
	{
		// �״�״̬��ֵ��ֻ����Stand״̬

		if (EActionType_Stand != pAction->GetCurrentActionType())
		{
			LOGError("�״γ�ʼ��״̬[" + pAction->GetCurrentActionType() + "]����");
			return false;
		}

		m_pCurrentAction = pAction;

		return true;
	}
	else
	{
		switch (m_pCurrentAction->GetCurrentActionType())
		{
		case EActionType_Null:
			break;
		case EActionType_Stand:
			{
				// վ��״̬�����л����κ�״̬
				DestroyAction(m_pCurrentAction);
				m_pCurrentAction = nullptr;

				// ������״̬
				m_pCurrentAction = pAction;

				return true;
			}
			break;
		case EActionType_Run:
			{
				// ����״̬�¿����л���վ��״̬
				if (EActionType_Stand == pAction->GetCurrentActionType())
				{
					// վ��״̬�����л����κ�״̬
					DestroyAction(m_pCurrentAction);
					m_pCurrentAction = nullptr;

					// ������״̬
					m_pCurrentAction = pAction;

					return true;
				}
				else if (EActionType_Run == pAction->GetCurrentActionType())
				{
					// վ��״̬�����л����κ�״̬
					DestroyAction(m_pCurrentAction);
					m_pCurrentAction = nullptr;

					// ������״̬
					m_pCurrentAction = pAction;

					return true;
				}
				else if (EActionType_Dead == pAction->GetCurrentActionType())
				{
					// վ��״̬�����л����κ�״̬
					DestroyAction(m_pCurrentAction);
					m_pCurrentAction = nullptr;

					// ������״̬
					m_pCurrentAction = pAction;

					return true;
				}
			}
			break;
		case EActionType_Dead:
			{
				// ����״ֻ̬���л���վ��״̬
				if (EActionType_Stand == pAction->GetCurrentActionType())
				{
					// վ��״̬�����л����κ�״̬
					DestroyAction(m_pCurrentAction);
					m_pCurrentAction = nullptr;

					// ������״̬
					m_pCurrentAction = pAction;

					return true;
				}
			}
			break;
		default:
			break;
		}
	}

	return false;
}

void ISprite::DestroyAction( IAction *pAction )
{
	delete pAction;
	pAction = nullptr;
}

void ISprite::SetHP(int nHP)
{
	if (nHP > m_attr.GetAttr(EAttrType_MaxHP))
	{
		m_attr.SetAttr(EAttrType_HP, m_attr.GetAttr(EAttrType_MaxHP));
	}
	else
	{
		m_attr.SetAttr(EAttrType_HP, nHP);
	}
}

void ISprite::SetMaxHP(int nMaxHP)
{
	if (m_attr.GetAttr(EAttrType_HP) > nMaxHP)
	{
		m_attr.SetAttr(EAttrType_HP, nMaxHP);
	}

	m_attr.SetAttr(EAttrType_MaxHP, nMaxHP);
}

bool ISprite::DoEnterScene()
{


	return true;
}

bool ISprite::DoLeaveScene()
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// �¼�������
//////////////////////////////////////////////////////////////////////////

bool ISprite::OnTick(MSTIME msNow)
{
	// �߼�ѭ��
	return true;
}

void ISprite::OnActionAutoSwitch(MSTIME msNow)
{
	if (m_pCurrentAction)
	{
		if (m_pCurrentAction->NeedSwitch(msNow))
		{
			Vector2 v2 = m_pCurrentAction->GetCurrentPosition(msNow);
			CStandAction *pStandAction = new CStandAction(v2);
			ChangeAction(pStandAction);
		}
	}
}

