#include "gamedispatcher.h"
#include "debug.h"

#include "client.h"
#include "country.h"

using namespace NS_IO;

void CGameDispatcher::OnInit()
{
	RegMessage(gproto::CSID_C2G_HeartBeat, &CGameDispatcher::OnHeartBeat, this);
	RegMessage(gproto::CSID_C2G_Prepare, &CGameDispatcher::OnPrepare, this);
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
