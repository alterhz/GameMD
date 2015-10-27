#include "gamedispatcher.h"
#include "debug.h"

#include "battleground.h"
#include "client.h"
#include "country.h"

using namespace NS_IO;

void CGameDispatcher::OnInit()
{
	RegMessage(gproto::CSID_C2G_HeartBeat, &CGameDispatcher::OnHeartBeat, this);
	RegMessage(gproto::CSID_C2G_Prepare, &CGameDispatcher::OnPrepare, this);
	RegMessage(gproto::CSID_C2G_EnterGround, &CGameDispatcher::OnEnterGround, this);
	RegMessage(gproto::CSID_C2G_LeaveGround, &CGameDispatcher::OnLeaveGround, this);
	RegMessage(gproto::CSID_C2G_GetGroundInfo, &CGameDispatcher::OnGetGroundInfo, this);
}

void CGameDispatcher::OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	LOGDebug("CGameDispatcher������");
}

void CGameDispatcher::OnPrepare(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	LOGDebug("׼����");

	if (nullptr == pClient->GetCountry())
	{
		// �������Ҳ���
		ICountry *pCountry = CCountryManager::getMe().CreateCountry();
		if (nullptr == pCountry)
		{
			LOGError("nullptr == pCountry");
			return;
		}

		pClient->SetCountry(pCountry);
		// �������ҵ����������¼�
		pCountry->OnConnected(pClient);
	}

	if (pClient->GetCountry())
	{
		// ��ӵ�׼������
		if (CCountryManager::getMe().PushPrepareList(pClient->GetCountry()))
		{
			// ׼���ɹ�
			pClient->SendPrepare(true);
		}
		else
		{
			// ׼��ʧ��
			pClient->SendPrepare(false);
		}
	}
	else
	{
		LOGError("׼��ʱ��û��Country����");
		// ׼��ʧ��
		pClient->SendPrepare(false);
	}
}

void CGameDispatcher::OnEnterGround(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// ���볡��
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	pCountry->EnterGround();
}

void CGameDispatcher::OnLeaveGround(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// �뿪����
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	pCountry->LeaveGround();
}

void CGameDispatcher::OnGetGroundInfo(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// �뿪����
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	IBattleGround *pBattleGround = pCountry->GetBattleGround();
	if (nullptr == pBattleGround)
	{
		LOGError("nullptr == pBattleGround");
		return ;
	}

	pBattleGround->GetGroundInfo(pCountry);
}
