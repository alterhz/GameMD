#include "logindispatcher.h"
#include "debug.h"
using namespace NS_IO;

// �궨��Э����������ٴ�����
#define DecodeProtoBuf(proto_msg, msg) proto_msg msg; \
	if (!msg.ParseFromArray(pMessage, nLength)) \
			{ \
			LOGPrint("Э��[" + nProtoId + "]����ʧ�ܡ�"); \
			return ; \
			}

void CLoginDispatcher::OnInit()
{
	RegMessage(gproto::CSID_C2G_HeartBeat, &CLoginDispatcher::OnHeartBeat, this);
	RegMessage(gproto::CSID_C2G_StartGame, &CLoginDispatcher::OnStartGame, this);
}

void CLoginDispatcher::OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	DecodeProtoBuf(gproto::MSG_C2G_HeartBeat, msgHeartBeat);

	LOGDebug("CLoginDispatcher������");
}

void CLoginDispatcher::OnStartGame(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	DecodeProtoBuf(gproto::MSG_C2G_StartGame, msgStartGame);

	LOGDebug("��ɫ[" + msgStartGame.rolename() + "]��ʼ��Ϸ��");
}

