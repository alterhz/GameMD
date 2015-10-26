#include "battleground.h"

#include "debug.h"

#include "app.h"
#include "country.h"
#include "gobject.h"
#include "ground.h"

IBattleGround::IBattleGround()
	: m_nIndexId(0)
	, m_eStatus(EStatus_Waiting)
{
	static int g_nIndexId = 0;
	m_nIndexId = (++g_nIndexId);
}

IBattleGround::~IBattleGround()
{

}

bool IBattleGround::Init()
{
	return OnInit();
}

bool IBattleGround::DoTick()
{
	return OnTick();
}

bool IBattleGround::ChangeStatus(EStatus eStatus)
{
	switch (m_eStatus)
	{
	case IBattleGround::EStatus_None:
		break;
	case IBattleGround::EStatus_Waiting:
		{
			if (EStatus_Run == eStatus)
			{
				m_eStatus = EStatus_Run;
				return true;
			}
		}
		break;
	case IBattleGround::EStatus_Run:
		{
			if (EStatus_Finish == eStatus)
			{
				m_eStatus = EStatus_Finish;
				return true;
			}
		}
		break;
	case IBattleGround::EStatus_Pause:
		break;
	case IBattleGround::EStatus_Finish:
		break;
	default:
		break;
	}

	return false;
}

void IBattleGround::GetGroundInfo(ICountry *pCountry)
{
	LOGDebug("Country[" + pCountry->GetIndexId() + "]��ȡ�������ݡ�");
}


CFrontBattleGround::CFrontBattleGround()
	: m_pGround(nullptr)
{
}

CFrontBattleGround::~CFrontBattleGround()
{
	delete m_pGround;
	m_pGround = nullptr;
}

bool CFrontBattleGround::OnInit()
{
	// ����Ѿ����ڸ������ݣ�ɾ��
	if (m_pGround)
	{
		delete m_pGround;
		m_pGround = nullptr;
	}

	IGround *pGround = new CDemoGround();
	if (nullptr == pGround)
	{
		LOGError("nullptr == pGround");
		return false;
	}

	if (!pGround->Init())
	{
		LOGError("��ʼ����������ʧ�ܣ�");
		return false;
	}

	m_pGround = pGround;

	return true;
}

bool CFrontBattleGround::InitTwoCountry(ICountry *pCountryA, ICountry *pCountryB)
{
	if (nullptr == pCountryA || nullptr == pCountryB)
	{
		LOGError("nullptr == pCountryA || nullptr == pCountryB");
		return false;
	}

	// ���ԣ���˫����Ӳ���
	{
		VtGObject vtGObject;
		// ��ӽ���
		CStillObject*pSirdar = new CStillObject();
		if (pSirdar)
		{
			pSirdar->Init(4);

			vtGObject.push_back(pSirdar);
		}

		// ���Ӣ��
		CWalkableObject *pHeroA = new CWalkableObject();
		if (pHeroA)
		{
			pHeroA->Init(2);

			vtGObject.push_back(pHeroA);
		}

		CWalkableObject *pHeroB = new CWalkableObject();
		if (pHeroB)
		{
			pHeroB->Init(3);

			vtGObject.push_back(pHeroB);
		}

		// ���С��
		CWalkableObject *pDogFaceA = new CWalkableObject();
		if (pDogFaceA)
		{
			pDogFaceA->Init(1);

			vtGObject.push_back(pDogFaceA);
		}

		CWalkableObject *pDogFaceB = new CWalkableObject();
		if (pDogFaceB)
		{
			pDogFaceB->Init(1);

			vtGObject.push_back(pDogFaceB);
		}

		pCountryA->ClearFightGObject();
		pCountryA->AddFightGObject(vtGObject);
	}

	{
		VtGObject vtGObject;
		// ��ӽ���
		CStillObject*pSirdar = new CStillObject();
		if (pSirdar)
		{
			pSirdar->Init(4);

			vtGObject.push_back(pSirdar);
		}

		// ���Ӣ��
		CWalkableObject *pHeroA = new CWalkableObject();
		if (pHeroA)
		{
			pHeroA->Init(2);

			vtGObject.push_back(pHeroA);
		}

		CWalkableObject *pHeroB = new CWalkableObject();
		if (pHeroB)
		{
			pHeroB->Init(3);

			vtGObject.push_back(pHeroB);
		}

		// ���С��
		CWalkableObject *pDogFaceA = new CWalkableObject();
		if (pDogFaceA)
		{
			pDogFaceA->Init(1);

			vtGObject.push_back(pDogFaceA);
		}

		CWalkableObject *pDogFaceB = new CWalkableObject();
		if (pDogFaceB)
		{
			pDogFaceB->Init(1);

			vtGObject.push_back(pDogFaceB);
		}

		pCountryB->ClearFightGObject();
		pCountryB->AddFightGObject(vtGObject);
	}

	m_mapCountry.insert(std::make_pair(pCountryA->GetIndexId(), pCountryA));
	m_mapCountry.insert(std::make_pair(pCountryB->GetIndexId(), pCountryB));

	return true;
}

bool CFrontBattleGround::OnTick()
{

	//LOGDebug("���սDoTick");
	return true;
}

void CFrontBattleGround::GetGroundInfo(ICountry *pCountry)
{

}


CBattleGroundManager::CBattleGroundManager()
	: m_nDoTickTimerId(0)
{
	m_nDoTickTimerId = CApp::getMe().SetTimer(this, 100);
}

CBattleGroundManager::~CBattleGroundManager()
{
	CApp::getMe().KillTimer(m_nDoTickTimerId);
	m_nDoTickTimerId = 0;
}

bool CBattleGroundManager::Init()
{
	return true;
}

CFrontBattleGround* CBattleGroundManager::CreateFrontBattleGround()
{
	CFrontBattleGround *pNewFrontBattleGround = new CFrontBattleGround();
	if (nullptr == pNewFrontBattleGround)
	{
		LOGError("nullptr == pNewFrontBattleGround");
		return nullptr;
	}

	pNewFrontBattleGround->Init();

	m_mapBattleGround.insert(std::make_pair(pNewFrontBattleGround->GetIndexId(), pNewFrontBattleGround));

	return pNewFrontBattleGround;
}

bool CBattleGroundManager::OnTimerEvent(int nTimerId)
{
	if (m_nDoTickTimerId == nTimerId)
	{
		auto itBattleGround = m_mapBattleGround.begin();
		for (; itBattleGround!=m_mapBattleGround.end(); ++itBattleGround)
		{
			itBattleGround->second->DoTick();
		}
	}

	return true;
}
