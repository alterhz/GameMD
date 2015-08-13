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
	RegMessage(1001, &CLoginDispatcher::OnRecvTest, this);
	RegMessage(gproto::gather::CSID_C2G_HeartBeat, &CLoginDispatcher::OnHeartBeat, this);
}

void CLoginDispatcher::OnRecvTest(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	std::cout << pMessage << nLength << std::endl;
}

void CLoginDispatcher::OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	DecodeProtoBuf(gproto::gather::MSG_C2G_HeartBeat, msgHeartBeat);

}

