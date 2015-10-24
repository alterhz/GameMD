#include "ground.h"
#include "debug.h"
#include "gobject.h"


//����
IGrid::IGrid(EGroundType eGroundType, int x, int y)
	: m_nX(x)
	, m_nY(y)
	, m_eGroundType(eGroundType)
{
}

bool IGrid::IsWalkable(EToWard eToWard) const
{
	switch (m_eGroundType)
	{
	case CGrid::EGroundType_Land:
	case CGrid::EGroundType_Sand:
	case CGrid::EGroundType_Lawn:
	case CGrid::EGroundType_Snow:
		{
			auto itGObject = m_mapGObject.begin();
			for (; itGObject!=m_mapGObject.end(); ++itGObject)
			{
				if (!itGObject->second->IsWalkable(eToWard))
				{
					// �����д����赲���ܹ�ȥ
					return false;
				}
			}

			return true;	//����ͨ��
		}
		break;
	case CGrid::EGroundType_River:
		{
			if (m_mapGObject.size() > 0)
			{
				auto itGObject = m_mapGObject.begin();
				for (; itGObject!=m_mapGObject.end(); ++itGObject)
				{
					if (!itGObject->second->IsWalkable(eToWard))
					{
						// �����д����赲���ܹ�ȥ
						return false;
					}
				}

				return true;	//����ͨ��
			}
			else
			{
				return false;	//����ͨ��
			}
		}
		break;
	default:
		{
			LOGError("��������[" + static_cast<int>(m_eGroundType) + "]����");
			return false;
		}
		break;
	}
}

bool IGrid::AddGObject(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	int nIndexId = pGObject->GetIndexId();

	if (FindGObject(nIndexId))
	{
		LOGError("GObject����[" + pGObject->GetIndexId() + "]�Ѿ��󶨡�");
		return false;
	}

	m_mapGObject.insert(std::make_pair(pGObject->GetIndexId(), pGObject));

	// ��
	pGObject->BindGrid(this);

	OnAddGObject(pGObject);

	return true;
}

bool IGrid::DelGObject(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	OnDelGObject(pGObject);

	int nIndexId = pGObject->GetIndexId();

	if (FindGObject(nIndexId))
	{
		m_mapGObject.erase(pGObject->GetIndexId());
	}

	pGObject->BindGrid(nullptr);

	return true;
}

IGObject * IGrid::FindGObject(int nIndexId)
{
	MapGObject::iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

IGObject * IGrid::FindGObject(int nIndexId) const
{
	MapGObject::const_iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

void IGrid::OnAddGObject(IGObject *pGObject)
{

}

void IGrid::OnDelGObject(IGObject *pGObject)
{

}


// ��ͨ����
CGrid::CGrid(EGroundType eGroundType, int x, int y)
	: IGrid(eGroundType, x, y)
{
}


IGround::IGround()
	: m_nWGCount(0)
	, m_nHGCount(0)
{
}

IGround::~IGround()
{
	// ɾ������
	for (auto itDel : m_mapGrid)
	{
		delete itDel.second;
	}
	m_mapGrid.clear();
}

bool IGround::Init()
{
	InitGroundSize(G_nDemoWidthCount, G_nDemoHeigthCount);

	return true;
}

bool IGround::InitGroundSize( int nWGCount, int nHGCount )
{
	if (nWGCount <=0
		|| nHGCount <= 0)
	{
		return false;
	}

	m_nWGCount = nWGCount;
	m_nHGCount = nHGCount;

	m_mapGrid.clear();

	for (int i=0; i<m_nHGCount; ++i)
	{
		for (int j=0; j<m_nWGCount; ++j)
		{
			int n = XY2N(i, j);

			m_mapGrid.insert(std::make_pair(n, nullptr));
		}
	}

	return true;
}

int IGround::XY2N( int x, int y ) const
{
	int n = m_nWGCount * y + x;
	return n;
}

int IGround::N2X( int n ) const
{
	int x = n % m_nWGCount;

	return x;
}

int IGround::N2Y( int n ) const
{
	int y = n / m_nWGCount;

	return y;
}

bool IGround::SetGrid( int x, int y, IGrid *pGrid )
{
	int n = XY2N(x, y);

	MapGrid::iterator it = m_mapGrid.find(n);
	if (it != m_mapGrid.end())
	{
		IGrid *pOldGrid = it->second;
		if (pOldGrid)
		{
			delete pOldGrid;
			pOldGrid = nullptr;
		}

		it->second = pGrid;

		return true;
	}

	return false;
}

IGrid * IGround::GetGrid(int x, int y)
{
	int n = XY2N(x, y);

	return Find(n);
}

IGrid * IGround::GetGrid(int x, int y) const
{
	int n = XY2N(x, y);

	return Find(n);
}

IGrid * IGround::Find(int n)
{
	MapGrid::iterator itGrid = m_mapGrid.find(n);
	if (itGrid != m_mapGrid.end())
	{
		return itGrid->second;
	}

	return nullptr;
}

IGrid * IGround::Find(int n) const
{
	MapGrid::const_iterator itGrid = m_mapGrid.find(n);
	if (itGrid != m_mapGrid.end())
	{
		return itGrid->second;
	}

	return nullptr;
}

// demo���γ���
CDemoGround::CDemoGround()
{
}

CDemoGround::~CDemoGround()
{

}

bool CDemoGround::Init( int nWGCount, int nHGCount )
{
	// ��ʼ��������ͼ
	InitGroundSize(G_nDemoWidthCount, G_nDemoHeigthCount);

	// ��ʼ����������
	for (int nY=0; nY<nHGCount; ++nY)
	{
		for (int nX=0; nX<nWGCount; ++nX)
		{
			IGrid *pNewGrid = new CGrid(CGrid::EGroundType_Land, nX, nY);
			if (nullptr == pNewGrid)
			{
				LOGError("nullptr == pNewGrid");
				continue;
			}

			SetGrid(nX, nY, pNewGrid);
		}
	}

	// ���س������̶�����λ������������ľ����ǽ������

	// ��ͼ���ķ���3������
	{
		CGBridge *pNewGBridge = new CGBridge(EToWard_Y);
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->EnterGround(10, 15, this);
	}

	{
		CGBridge *pNewGBridge = new CGBridge(EToWard_Y);
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->EnterGround(8, 15, this);
	}

	{
		CGBridge *pNewGBridge = new CGBridge(EToWard_Y);
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->EnterGround(12, 15, this);
	}
	

	// ���س������ƶ���������λ
	{
		CDogFace *pNewDogFace = new CDogFace();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init();
		pNewDogFace->EnterGround(1, 15, this);
	}

	{
		CDogFace *pNewDogFace = new CDogFace();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init();
		pNewDogFace->EnterGround(18, 15, this);
	}

	return true;
}
