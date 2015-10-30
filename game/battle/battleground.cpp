#include "battleground.h"

#include "debug.h"

#include "app.h"
#include "country.h"
#include "gobject.h"
#include "ground.h"
#include "mymath.h"


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
	if (OnInit())
	{
		ChangeStatus(EStatus_Run);
	

		return true;
	}
	else
	{
		return false;
	}
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
				OnGoRun();

				m_eStatus = EStatus_Run;
				return true;
			}
		}
		break;
	case IBattleGround::EStatus_Run:
		{
			if (EStatus_Finish == eStatus)
			{
				OnGoFinish();

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
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	pCountry->SendGetGroundInfo(m_nWGCount, m_nHGCount, m_mapGrid, m_mapGObject);
}

ICountry * IBattleGround::FindCountry(int nIndexId)
{
	MapCountry::iterator itCountry = m_mapCountry.find(nIndexId);
	if (itCountry != m_mapCountry.end())
	{
		return itCountry->second;
	}

	return nullptr;
}

ICountry * IBattleGround::FindCountry(int nIndexId) const
{
	MapCountry::const_iterator itCountry = m_mapCountry.find(nIndexId);
	if (itCountry != m_mapCountry.end())
	{
		return itCountry->second;
	}

	return nullptr;
}

bool IBattleGround::Enter(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	if (!FindCountry(pCountry->GetIndexId()))
	{
		// ����ս�����
		m_mapCountry.insert(std::make_pair(pCountry->GetIndexId(), pCountry));
		pCountry->SetBattleGround(this);
	}

	return OnEnter(pCountry);
}

bool IBattleGround::Leave(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	int nCountryIndexId = pCountry->GetIndexId();

	if (FindCountry(nCountryIndexId))
	{
		pCountry->SetBattleGround(nullptr);

		m_mapCountry.erase(nCountryIndexId);

		OnLeave(pCountry);
	}

	return true;
}

bool IBattleGround::GObjectEnter(IGObject *pGObject, int x, int y)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	int nGObjectIndexId = pGObject->GetIndexId();

	if (FindGObject(nGObjectIndexId))
	{
		LOGError("�������Ѿ����ڵ�ǰGObject[" + nGObjectIndexId + "]��");
		return false;
	}

	IGrid *pGrid = GetGrid(x, y);
	if (nullptr == pGrid)
	{
		LOGError("nullptr == pGrid");
		return false;
	}

	if (!pGrid->IsWalkable(EToWard_Both))
	{
		LOGDebug("�������ߣ����Բ���ֱ����ӵ������С�");
		return false;
	}

	// ���Ӱ�GObject
	pGrid->AddGObject(pGObject);

	pGObject->SetBattleGround(this);
	pGObject->SetXY(x, y);

	m_mapGObject.insert(std::make_pair(pGObject->GetIndexId(), pGObject));

	OnGObjectEnter(pGObject);

	return true;
}

bool IBattleGround::GObjectLeave(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	int nGObjectIndexId = pGObject->GetIndexId();

	if (!FindGObject(nGObjectIndexId))
	{
		LOGError("�����в����ڵ�ǰGObject[" + nGObjectIndexId + "]��");
		return false;
	}

	OnGObjectLeave(pGObject);

	m_mapGObject.erase(nGObjectIndexId);

	pGObject->SetBattleGround(nullptr);

	int nX = pGObject->GetX();
	int nY = pGObject->GetY();

	IGrid *pGrid = GetGrid(nX, nY);
	if (pGrid)
	{
		pGrid->DelGObject(pGObject);
	}

	return true;
}

void IBattleGround::PrepareFinish(ICountry *pCountry)
{
	LOGDebug("׼�����");
}

IGObject * IBattleGround::FindGObject(int nIndexId)
{
	MapGObject::iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

IGObject * IBattleGround::FindGObject(int nIndexId) const
{
	MapGObject::const_iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

bool IBattleGround::OnEnter(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	pCountry->SendGetGroundInfo(m_nWGCount, m_nHGCount, m_mapGrid, m_mapGObject);

	return true;
}

bool IBattleGround::OnGObjectEnter(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	// ֪ͨ�����еĹ��ң������һ��GObject
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendGObjectEnterGround(pGObject->GetType(), pGObject->GetIndexId(), 
				pGObject->GetSN(), pGObject->GetX(), pGObject->GetY(), pGObject->GetHP(), 
				pGObject->GetMaxHP(), pGObject->GetSP(), pGObject->GetLevel(), pGObject->GetCampId());
		}
	}

	return true;
}

void IBattleGround::BattleBoutFinish(ICountry *pCountry)
{
	LOGDebug("[" + pCountry->GetIndexId() + "]ս���غϽ�����");
}

bool IBattleGround::PathIsOK(const VtCoor2 &vtCoor2)
{
	// ��֤·�����Ƿ������ڸ���
	if (vtCoor2.size() < 2)
	{
		// ֻ��һ�����޷�����
		return false;
	}

	COOR2 coor2Prev = vtCoor2[0];

	for (int i=1; i<static_cast<int>(vtCoor2.size()); ++i)
	{
		COOR2 coor2 = vtCoor2[i];

		// ·�����Ƿ񶼿����ƶ�
		IGrid *pGrid = GetGrid(coor2.x, coor2.y);
		if (nullptr == pGrid)
		{
			LOGError("nullptr == pGrid");
			return false;
		}

		if (!pGrid->IsWalkable(EToWard_Both))
		{
			// ·���㣬���ڲ��������ߵ�
			return false;
		}

		if (COOR2::Length(coor2, coor2Prev) != 1)
		{
			// ����·�����������
			return false;
		}

		coor2Prev = coor2;
	}

	return true;
}

bool IBattleGround::GObjectMove(IGObject *pGObject, const VtCoor2 &vtCoor2)
{
	LOGDebug("GObjectMove");
	return true;
}

bool IBattleGround::GObjectUseSkill(IGObject *pGObject, int nSkillSN, int nTargetGObjectIndexId)
{
	LOGDebug("Ŀ���ͷż���");
	return true;
}

const int G_nDemoWidthCount = 20;
const int G_nDemoHeigthCount = 30;

CFrontBattleGround::CFrontBattleGround()
	: m_eBattleStatus(EBattleStatus_Waiting)
	, m_nBoutIndex(0)
	, m_nBoutCountryIndexId(0)
	, m_nWinCountryIndexId(0)
{
}

CFrontBattleGround::~CFrontBattleGround()
{
}

bool CFrontBattleGround::OnInit()
{
	int nHGCount = G_nDemoHeigthCount;
	int nWGCount = G_nDemoWidthCount;

	// ��ʼ��������ͼ
	InitGroundSize(nWGCount, nHGCount);

	// ��ʼ����������
	for (int nY=1; nY<=nHGCount; ++nY)
	{
		for (int nX=1; nX<=nWGCount; ++nX)
		{
			IGrid *pNewGrid = new CGrid(nX, nY);
			if (nullptr == pNewGrid)
			{
				LOGError("nullptr == pNewGrid");
				continue;
			}

			int nSN = 1;

			if (!pNewGrid->Init(nSN))
			{
				LOGError("��ʼ����ͼ����[SN:" + nSN + "]ʧ�ܡ�");
				continue;
			}

			SetGrid(nX, nY, pNewGrid);
		}
	}

	// ���س������̶�����λ������������ľ����ǽ������

	// ��ͼ���ķ���3������
	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(1);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(10, 15, this);
	}

	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(2);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(8, 15, this);
	}

	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(2);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(12, 15, this);
	}


	// ���س������ƶ���������λ
	{
		CWalkableObject *pNewDogFace = new CWalkableObject();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init(1);
		pNewDogFace->SetCampId(0);
		pNewDogFace->SetLevel(1);

		pNewDogFace->EnterGround(1, 15, this);
	}

	{
		CWalkableObject *pNewDogFace = new CWalkableObject();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init(1);
		pNewDogFace->SetCampId(0);
		pNewDogFace->SetLevel(1);

		pNewDogFace->EnterGround(18, 15, this);
	}

	return true;
}

bool CFrontBattleGround::OnTick()
{
	//LOGDebug("���սDoTick");
	return true;
}

bool CFrontBattleGround::OnEnter(ICountry *pCountry)
{
	IBattleGround::OnEnter(pCountry);

	// ���������������ң���ʼ
	if (m_mapCountry.size() >= 2)
	{
		// �л�����״̬
		ChangeBattleStatus(EBattleStatus_Prepare);
	}

	return true;
}

bool CFrontBattleGround::OnLeave(ICountry *pCountry)
{
	// һ����ɫ�뿪����������һ����ɫ���ʤ��
	EBattleStatus eBattleStatus = GetCurrentBattleStatus();
	if (EBattleStatus_Prepare == eBattleStatus
		|| EBattleStatus_Battle == eBattleStatus)
	{
		if (m_mapCountry.size() == 1)
		{
			auto itCountry = m_mapCountry.begin();

			if (itCountry->second)
			{
				// ��ʤ��CountryIndexId
				m_nWinCountryIndexId = itCountry->second->GetIndexId();

				ChangeBattleStatus(EBattleStatus_Reward);
			}
		}
	}

	return false;
}

void CFrontBattleGround::ChangeBattleStatus(EBattleStatus eBattleStatus)
{
	switch (m_eBattleStatus)
	{
	case CFrontBattleGround::EBattleStatus_Waiting:
		{
			if (EBattleStatus_Prepare == eBattleStatus)
			{
				OnGoPrepare();

				m_eBattleStatus = eBattleStatus;
			}
		}
		break;
	case CFrontBattleGround::EBattleStatus_Prepare:
		{
			if (EBattleStatus_Battle == eBattleStatus)
			{
				OnGoBattle();

				m_eBattleStatus = eBattleStatus;
			}
			else if (EBattleStatus_Reward == eBattleStatus)
			{
				OnGoReward();

				m_eBattleStatus = eBattleStatus;
			}
		}
		break;
	case CFrontBattleGround::EBattleStatus_Battle:
		{
			if (EBattleStatus_Reward == eBattleStatus)
			{
				OnGoReward();

				m_eBattleStatus = eBattleStatus;
			}
		}
		break;
	case CFrontBattleGround::EBattleStatus_Reward:
		break;
	default:
		break;
	}
}

// ս��׼��
void CFrontBattleGround::OnGoPrepare()
{
	// ���Խ׶Σ��ͻ��˵��ű�����ֱ���ɷ������Զ��ű���Ȼ��ͻ��˷���׼����ɼ��ɡ�
	{
		if (m_mapCountry.size() == 2)
		{
			auto itCountry = m_mapCountry.begin();

			ICountry *pCountryA = itCountry->second;
			if (pCountryA)
			{
				const VtGObject &vtGObject = pCountryA->GetPrepareGObject();

				int nSetX = 10 - vtGObject.size() / 2;
				for (IGObject *pGObject : vtGObject)
				{
					if (pGObject)
					{
						if (GObjectType_Still == pGObject->GetType())
						{
							CStillObject *pStillObject = pGObject->GetStillObject();
							if (nullptr == pStillObject)
							{
								LOGError("nullptr == pStillObject");
								continue;
							}

							if (pStillObject->IsFlag())
							{
								// ����
								pStillObject->EnterGround(10, 1, this);
								continue;
							}
						}

						if (!pGObject->EnterGround(nSetX++, 2, this))
						{
							LOGError("GObject[" + pGObject->GetIndexId() + "]���볡��ʧ�ܡ�");
						}
					}
				}
			}

			++itCountry;
			ICountry *pCountryB = itCountry->second;
			if (pCountryB)
			{
				const VtGObject &vtGObject = pCountryB->GetPrepareGObject();

				int nSetX = 10 - vtGObject.size() / 2;
				for (IGObject *pGObject : vtGObject)
				{
					if (pGObject)
					{
						if (GObjectType_Still == pGObject->GetType())
						{
							CStillObject *pStillObject = pGObject->GetStillObject();
							if (nullptr == pStillObject)
							{
								LOGError("nullptr == pStillObject");
								continue;
							}

							if (pStillObject->IsFlag())
							{
								// ����
								pStillObject->EnterGround(10, 30, this);
								continue;
							}
						}

						if (!pGObject->EnterGround(nSetX++, 29, this))
						{
							LOGError("GObject[" + pGObject->GetIndexId() + "]���볡��ʧ�ܡ�");
						}
					}
				}
			}

		}
	}

	// ֪ͨ�ͻ���׼��
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendBattlePrepare();
		}
	}
}

// ��ʼս��
void CFrontBattleGround::OnGoBattle()
{
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendBattleStart();
		}
	}

	// ��������ȿ�ʼ�غ�(bout)�Ľ�ɫ
	int nRandInt = NS_IO::Random();

	if (m_vtPrepareFinishCountryIndexId.size() >= 2)
	{
		m_nBoutIndex = nRandInt % m_vtPrepareFinishCountryIndexId.size();

		m_nBoutCountryIndexId = m_vtPrepareFinishCountryIndexId[m_nBoutIndex];
	}
	else
	{
		LOGError("ս����ʼ�������н�ɫ���㡣");
		return ;
	}

	// ֪ͨ�ͻ��ˣ���ǰս���غϵ�Country
	auto itCountrySend = m_mapCountry.begin();
	for (; itCountrySend!= m_mapCountry.end(); ++itCountrySend)
	{
		ICountry *pCountry = itCountrySend->second;

		if (pCountry)
		{
			pCountry->SendBattleFight(m_nBoutCountryIndexId);
		}
	}
}

// ս������������
void CFrontBattleGround::OnGoReward()
{
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendBattleReward(m_nWinCountryIndexId);
		}
	}	
}

void CFrontBattleGround::PrepareFinish(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	if (!FindCountry(pCountry->GetIndexId()))
	{
		LOGError("���ǵ�ǰ������Country������׼����");
		return ;
	}

	for (int nCountryId : m_vtPrepareFinishCountryIndexId)
	{
		if (pCountry->GetIndexId() == nCountryId)
		{
			return ;
		}
	}

	m_vtPrepareFinishCountryIndexId.push_back(pCountry->GetIndexId());

	if (m_vtPrepareFinishCountryIndexId.size() == m_mapCountry.size())
	{
		// ׼�������볡��������ȣ������׼����ɣ���ʼս��
		ChangeBattleStatus(EBattleStatus_Battle);
	}
}

void CFrontBattleGround::BattleBoutFinish(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	if (pCountry->GetIndexId() != m_nBoutCountryIndexId)
	{
		LOGDebug("�����Լ�[" + pCountry->GetIndexId() + "]�Ļغ�[" + m_nBoutCountryIndexId + "]�����ܽ�����");
		return ;
	}

	++m_nBoutIndex;

	if (m_vtPrepareFinishCountryIndexId.size() > 0)
	{
		m_nBoutIndex = m_nBoutIndex % m_vtPrepareFinishCountryIndexId.size();

		m_nBoutCountryIndexId = m_vtPrepareFinishCountryIndexId[m_nBoutIndex];
	}
	else
	{
		LOGError("������country[" + m_vtPrepareFinishCountryIndexId.size() + "]��������");
		return ;
	}

	// �����ƶ�ս���б�
	m_vtBoutMoveFightGObjectIndexId.clear();

	// ֪ͨ�ͻ��ˣ���ǰս���غϵ�Country
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;

		if (pCountry)
		{
			pCountry->SendBattleFight(m_nBoutCountryIndexId);
		}
	}
}

bool CFrontBattleGround::GObjectMove(IGObject *pGObject, const VtCoor2 &vtCoor2)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	// �Ƿ��Ѿ��ƶ�ս����
	if (IsMoveFight(pGObject->GetIndexId()))
	{
		return false;
	}

	// ��֤·��ͨ��
	if (!PathIsOK(vtCoor2))
	{
		return false;
	}

	IGrid *pOldGrid = pGObject->GetGrid();
	if (pOldGrid)
	{
		pOldGrid->DelGObject(pGObject);
	}

	if (vtCoor2.size() <= 2)
	{
		LOGError("·��������[" + vtCoor2.size() + "]����������");
		return false;
	}

	COOR2 coor2MoveTo = vtCoor2[vtCoor2.size() - 1];
	// ��������
	IGrid *pNewGrid = GetGrid(coor2MoveTo.x, coor2MoveTo.y);
	if (pNewGrid)
	{
		pNewGrid->AddGObject(pGObject);
	}

	// ����λ������
	pGObject->SetXY(coor2MoveTo.x, coor2MoveTo.y);

	// ���߹㲥
	auto itCountry = m_mapCountry.begin();
	for (; itCountry!= m_mapCountry.end(); ++itCountry)
	{
		ICountry *pCountry = itCountry->second;
		if (pCountry)
		{
			pCountry->SendMove(pGObject->GetIndexId(), vtCoor2);
		}
	}

	m_vtBoutMoveFightGObjectIndexId.push_back(pGObject->GetIndexId());

	return true;
}

bool CFrontBattleGround::IsMoveFight(int nGObjectIndexId) const
{
	for (const int nHaveMoveFightGObjectIndexId : m_vtBoutMoveFightGObjectIndexId)
	{
		if (nHaveMoveFightGObjectIndexId == nGObjectIndexId)
		{
			return true;
		}
	}

	return false;
}

bool CFrontBattleGround::GObjectUseSkill(IGObject *pGObject, int nSkillSN, int nTargetGObjectIndexId)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	const CXmlData_Skill *pXmlData_Skill = CConfigReadManager::getMe().xdSkill.GetRecord(nSkillSN);
	if (nullptr == pXmlData_Skill)
	{
		LOGError("nullptr == pXmlData_Skill");
		return false;
	}

	if (CXmlData_Skill::ERange_Single == pXmlData_Skill->eRange)
	{
		if (CXmlData_Skill::ETarget_Enemy == pXmlData_Skill->eTarget)
		{
			IGObject *pTargetGObject = FindGObject(nTargetGObjectIndexId);
			if (nullptr == pTargetGObject)
			{
				LOGError("û���ҵ�����[" + nSkillSN + "]�ͷ�Ŀ��[" + nTargetGObjectIndexId + "]��");
				return false;
			}

			// ������Ŀ���Ƿ����ս��
			if (!pTargetGObject->IsCanFight())
			{
				LOGDebug("�������߲�����ս���������ͷż��ܡ�");
				return false;
			}

			if (pTargetGObject->GetCampId() == pGObject->GetCampId())
			{
				LOGDebug("Ŀ��[" + nTargetGObjectIndexId + "]���ѷ���λ�����Լ������ܹ�����");
				return false;
			}

			// ���㹥������
			const COOR2 &coor2Target = pTargetGObject->GetCoor2();
			const COOR2 &coor2Self = pGObject->GetCoor2();

			int nLength = COOR2::Length(coor2Self, coor2Target);

			if (nLength < pXmlData_Skill->nMinAttackLength
				|| nLength > pXmlData_Skill->nMaxAttackLength)
			{
				LOGDebug("����������Χ[nLength:" + nLength + ", minLength:" + pXmlData_Skill->nMinAttackLength 
					+ ", nMaxLength:" + pXmlData_Skill->nMaxAttackLength + "]��");
				return false;
			}

			// �˺�����
			int nAtt = pGObject->GetAtt();

			int nRand = NS_IO::Random() % 21;

			int nDamageValue = static_cast<int>(nAtt * (90 + nRand) / 100.0f) + pXmlData_Skill->nAddDamage;

			int nLostHP = pTargetGObject->LostHP(nDamageValue);
			int nRemainHP = pTargetGObject->GetHP();

			// �����˺��㲥
			auto itCountry = m_mapCountry.begin();
			for (; itCountry!= m_mapCountry.end(); ++itCountry)
			{
				ICountry *pCountry = itCountry->second;
				if (pCountry)
				{
					pCountry->SendFight(nSkillSN, pGObject->GetIndexId(), pTargetGObject->GetIndexId(), nLostHP, nRemainHP);
				}
			}
		}
		else if (CXmlData_Skill::ETarget_Self == pXmlData_Skill->eTarget)
		{

		}
		else if (CXmlData_Skill::ETarget_Friend == pXmlData_Skill->eTarget)
		{

		}
	}
	else if (CXmlData_Skill::ERange_Multi == pXmlData_Skill->eRange)
	{
		if (CXmlData_Skill::ETarget_Self == pXmlData_Skill->eTarget)
		{
			// ������Ϊ�����ͷż���

		}
		else if (CXmlData_Skill::ETarget_Friend == pXmlData_Skill->eTarget)
		{

		}
		else if (CXmlData_Skill::ETarget_Enemy == pXmlData_Skill->eTarget)
		{

		}
	}
	else if (CXmlData_Skill::ERange_All == pXmlData_Skill->eRange)
	{

	}
	else
	{
		LOGError("���ܷ�Χ����[" + pXmlData_Skill->eRange + "]�쳣��");
	}


	return true;
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

IBattleGround * CBattleGroundManager::FindBattleGround(int nBattleGroundIndexId)
{
	MapBattleGround::iterator itBattleGround = m_mapBattleGround.find(nBattleGroundIndexId);
	if (itBattleGround != m_mapBattleGround.end())
	{
		return itBattleGround->second;
	}

	return nullptr;
}

IBattleGround * CBattleGroundManager::FindBattleGround(int nBattleGroundIndexId) const
{
	MapBattleGround::const_iterator itBattleGround = m_mapBattleGround.find(nBattleGroundIndexId);
	if (itBattleGround != m_mapBattleGround.end())
	{
		return itBattleGround->second;
	}

	return nullptr;
}
