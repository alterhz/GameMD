#include "gamedispatcher.h"
#include "debug.h"

#include "battleground.h"
#include "client.h"
#include "country.h"
#include "gobject.h"

using namespace NS_IO;

// �궨��Э����������ٴ�����
#define DecodeProtoBuf(proto_msg, msg) proto_msg msg; \
	if (!msg.ParseFromArray(pMessage, nLength)) \
{ \
	LOGPrint("Э��[" + nProtoId + "]����ʧ�ܡ�"); \
	return ; \
}

void CGameDispatcher::OnInit()
{
	RegMessage(gproto::CSID_C2G_HeartBeat, &CGameDispatcher::OnHeartBeat, this);
	RegMessage(gproto::CSID_C2G_Prepare, &CGameDispatcher::OnPrepare, this);
	RegMessage(gproto::CSID_C2G_EnterGround, &CGameDispatcher::OnEnterGround, this);
	RegMessage(gproto::CSID_C2G_LeaveGround, &CGameDispatcher::OnLeaveGround, this);
	//RegMessage(gproto::CSID_C2G_GetGroundInfo, &CGameDispatcher::OnGetGroundInfo, this);
	RegMessage(gproto::CSID_C2G_PrepareFinish, &CGameDispatcher::OnPrepareFinish, this);
	RegMessage(gproto::CSID_C2G_BattleFightEnd, &CGameDispatcher::OnBattleFightEnd, this);
	RegMessage(gproto::CSID_C2G_MoveFight, &CGameDispatcher::OnMoveFight, this);
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
			int nCountryIndexId = pClient->GetCountry()->GetIndexId();
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

void CGameDispatcher::OnPrepareFinish(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// ׼�����
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

	pBattleGround->PrepareFinish(pCountry);
}

void CGameDispatcher::OnBattleFightEnd(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// ׼�����
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

	pBattleGround->BattleBoutFinish(pCountry);
}

void CGameDispatcher::OnMoveFight(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// ׼�����
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	DecodeProtoBuf(gproto::MSG_C2G_MoveFight, msg);

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

	int nGObjectIndexId = msg.gobjectindexid();
	VtCoor2 vtCoor2;
	for (int i=0; i<msg.path_size(); ++i)
	{
		COOR2 coor2(msg.path(i).x(), msg.path(i).y());

		vtCoor2.push_back(coor2);
	}

	IGObject *pMoveGObject = pBattleGround->FindGObject(nGObjectIndexId);
	if (nullptr == pMoveGObject)
	{
		LOGError("nullptr == pMoveGObject");
		return ;
	}

	if (GObjectType_Walkable != pMoveGObject->GetType())
	{
		return ;
	}

	CWalkableObject *pWalkableObject = pMoveGObject->GetWalkableObject();
	if (nullptr == pWalkableObject)
	{
		LOGError("nullptr == pWalkableObject");
		return ;
	}

	if (!pWalkableObject->Move(vtCoor2))
	{
		// ����ʧ��
		pCountry->SendMoveFight(false, nGObjectIndexId);
	}

	// ����
	int nTargetGObjectIndexId = msg.targetgobjectindexid();
	if (0 == nTargetGObjectIndexId)
	{
		// û�й���Ŀ��
		return ;
	}

	// ��������
	
}
