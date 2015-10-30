#include "gobject.h"
#include "debug.h"
#include "ground.h"
#include "battleground.h"

IGObject::IGObject()
	: m_nIndexId(0)
	, m_nX(0)
	, m_nY(0)
	, m_pBattleGround(nullptr)
	, m_nCampId(0)
	, m_nHP(0)
	, m_nSP(0)
	, m_nLevel(1)
{
	static int G_nIndexId = 0;
	m_nIndexId = (++G_nIndexId);
}

bool IGObject::Init(int nSN)
{
	m_nSN = nSN;

	return OnInit();
}

bool IGObject::EnterGround(int x, int y, IBattleGround *pBattleGround)
{
	if (nullptr == pBattleGround)
	{
		LOGError("nullptr == pBattleGround");
		return false;
	}

	// ���볡��
	pBattleGround->GObjectEnter(this, x, y);

	return true;
}

void IGObject::AddSP(int nSP)
{
	if (nSP <= 0)
	{
		return ;
	}

	m_nSP += nSP;

	OnAddSP(nSP);
}

int IGObject::LostHP(int nHP)
{
	if (nHP <= 0)
	{
		LOGError("nHP[" + nHP + "] <= 0");
		return false;
	}

	int nLostHP = OnLostHP(nHP);

	if (nLostHP <= 0)
	{
		LOGError("nLostHP[" + nLostHP + "] <= 0");
		return false;
	}

	if (nLostHP > m_nHP)
	{
		nLostHP = m_nHP;
	}

	m_nHP -= nLostHP;

	return m_nHP;
}


// ��ֹ��������
CStillObject::CStillObject()
	: m_pXmlData_Still(nullptr)
{
}

CStillObject::~CStillObject()
{
}

bool CStillObject::OnInit()
{
	// ���ؽ�ɫ����
	const CXmlData_Still *pXmlData_Still = CConfigReadManager::getMe().xdStill.GetRecord(m_nSN);
	if (nullptr == pXmlData_Still)
	{
		LOGError("����CXmlData_Object����[" + m_nSN + "]ʧ�ܡ�");
		return false;
	}

	m_pXmlData_Still = pXmlData_Still;

	// ��ʼ����ǰѪ��
	m_nHP = m_pXmlData_Still->nHP;

	return true;
}

bool CStillObject::IsWalkable(EToWard eToWard) const
{
	if (nullptr == m_pXmlData_Still)
	{
		LOGError("nullptr == m_pXmlData_Object");
		return false;
	}

	if (EToWard_None == m_pXmlData_Still->eToWard)
	{
		return false;
	}

	if (EToWard_Both == m_pXmlData_Still->eToWard)
	{
		return true;
	}

	if (m_pXmlData_Still->eToWard == eToWard)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CStillObject::IsFlag() const
{
	if (nullptr == m_pXmlData_Still)
	{
		LOGError("nullptr == m_pXmlData_Object");
		return false;
	}

	return m_pXmlData_Still->bFlag;
}

int CStillObject::GetMaxHP() const
{
	if (m_pXmlData_Still)
	{
		return m_pXmlData_Still->nHP;
	}

	return 1;
}

bool CStillObject::IsCanFight() const
{
	if (nullptr == m_pXmlData_Still)
	{
		LOGError("nullptr == m_pXmlData_Object");
		return false;
	}

	return m_pXmlData_Still->bCanDestroy;
}

// ������ɫ
CWalkableObject::CWalkableObject()
	: m_pXmlData_Walkable(nullptr)
{
}

CWalkableObject::~CWalkableObject()
{
}

bool CWalkableObject::OnInit()
{
	// ���ؽ�ɫ����
	const CXmlData_Walkable *pXmlData_Walkable = CConfigReadManager::getMe().xdWalkable.GetRecord(m_nSN);
	if (nullptr == pXmlData_Walkable)
	{
		LOGError("����Charactar����[" + m_nSN + "]ʧ�ܡ�");
		return false;
	}

	m_pXmlData_Walkable = pXmlData_Walkable;

	// ��ʼ����ǰѪ��
	m_nHP = m_pXmlData_Walkable->nHP;

	return true;
}

bool CWalkableObject::Move(int x, int y)
{
	if (nullptr == m_pBattleGround)
	{
		LOGError("nullptr == m_pGround");
		return false;
	}

	IGrid *pGrid = m_pBattleGround->GetGrid(x, y);
	if (nullptr == pGrid)
	{
		return false;
	}

	// ����
	int nStepLength = GetWalkLength();

	// �ж��Ƿ�������߹�ȥ


	// ����λ��
	m_nX = x;
	m_nY = y;

	// �����
	if (m_pGrid)
	{
		m_pGrid->DelGObject(this);
	}

	// �°�
	pGrid->AddGObject(this);

	return true;
}

bool CWalkableObject::Move(const VtCoor2 &vtCoor2)
{
	if (nullptr == m_pBattleGround)
	{
		LOGError("nullptr == m_pGround");
		return false;
	}

	// ��֤·������ʼλ�ú���Զ����
	if (vtCoor2.size() < 2)
	{
		return false;
	}

	const COOR2 &coor2Begin = vtCoor2[0];
	const COOR2 &coor2End = vtCoor2[vtCoor2.size() - 1];

	COOR2 coor2 = GetCoor2();

	if (!(coor2Begin == coor2))
	{
		// ��ʼ������
		return false;
	}

	int nLength = COOR2::Length(coor2Begin, coor2End);

	if (nLength > GetWalkLength())
	{
		// ������Զ���߾���
		return false;
	}

	return m_pBattleGround->GObjectMove(this, vtCoor2);
}

bool CWalkableObject::IsWalkable(EToWard eToWard) const
{
	if (nullptr == m_pXmlData_Walkable)
	{
		LOGError("nullptr == m_pXmlData_Charactar");
		return false;
	}

	if (EToWard_None == m_pXmlData_Walkable->eToWard)
	{
		return false;
	}

	if (EToWard_Both == m_pXmlData_Walkable->eToWard)
	{
		return true;
	}

	if (m_pXmlData_Walkable->eToWard == eToWard)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CWalkableObject::GetWalkLength() const
{
	if (m_pXmlData_Walkable)
	{
		return m_pXmlData_Walkable->nWalkLength;
	}

	return 0;
}

int CWalkableObject::GetMaxHP() const
{
	if (m_pXmlData_Walkable)
	{
		return m_pXmlData_Walkable->nHP;
	}

	return 1;
}

int CWalkableObject::GetAtt() const
{
	if (m_pXmlData_Walkable)
	{
		return m_pXmlData_Walkable->nAtt;
	}

	return 1;
}

bool CWalkableObject::UseSkill(int nTargetGObjectIndexId)
{
	if (m_pBattleGround)
	{
		if (m_nSP >= 3)
		{
			// ����
			int nSkillSN = GetSkillSN1();
			if (m_pBattleGround->GObjectUseSkill(this, nSkillSN, nTargetGObjectIndexId))
			{
				m_nSP = 0;
			}
		}
		else
		{
			// �չ�
			int nSkillSN = GetCommonSkillSN();
			m_pBattleGround->GObjectUseSkill(this, nSkillSN, nTargetGObjectIndexId);
		}
	}

	return true;
}

int CWalkableObject::GetCommonSkillSN() const
{
	if (m_pXmlData_Walkable)
	{
		return m_pXmlData_Walkable->nCommonSkillSN;
	}

	return 0;
}

int CWalkableObject::GetSkillSN1() const
{
	if (m_pXmlData_Walkable)
	{
		return m_pXmlData_Walkable->nSkillSN1;
	}

	return 0;
}

int CWalkableObject::GetSkillSN2() const
{
	if (m_pXmlData_Walkable)
	{
		return m_pXmlData_Walkable->nSkillSN2;
	}

	return 0;
}

