#include "testclient.h"
#include "debug.h"

CTestClient::CTestClient()
{

}

CTestClient::~CTestClient()
{

}

bool CTestClient::Init(INetConnector *pNetConnector)
{
	if (nullptr == pNetConnector)
	{
		return false;
	}

	m_pNetConnector = pNetConnector;

	return m_pNetConnector->Connect(this, "127.0.0.1", 8000);
}

bool CTestClient::SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage)
{
	char szSendBuffer[MAX_SENDBUFFER_LENGTH] = {0};

	// 协议编号
	unsigned short *pMessageLength = reinterpret_cast<unsigned short *>(szSendBuffer);
	*pMessageLength = wProtocolId;

	void *pData = reinterpret_cast<void *>(szSendBuffer + sizeof(unsigned short));

	// 协议内容编码
	if (!pMessage->SerializePartialToArray(pData, MAX_SENDBUFFER_LENGTH-sizeof(unsigned short)))
	{
		LOGPrint("协议[" + wProtocolId + "]编码失败。");
		return false;
	}

	if (nullptr == m_pNetSocket)
	{
		return false;
	}

	return m_pNetSocket->DoSend(szSendBuffer, sizeof(unsigned short) + pMessage->ByteSize());
}

void CTestClient::Disconnect()
{
	if (m_pNetSocket)
	{
		m_pNetSocket->DoClose();
	}
}

bool CTestClient::OnConnected(INetSocket *pNetSocket)
{
	if (nullptr == pNetSocket)
	{
		return false;
	}

	m_pNetSocket = pNetSocket;

	LOGInfo("Test客户端连接上服务器[localIP:" + m_pNetSocket->GetLocalIP() + ",localPort:"
		+ m_pNetSocket->GetLocalPort() + ", remoteIP:" + m_pNetSocket->GetRemoteIP() + ", remotePort:"
		+ m_pNetSocket->GetRemotePort() + "]。");

	// 发送测试心跳
	gproto::gather::MSG_C2G_HeartBeat msgHeartBeat;
	SendMessage(gproto::gather::CSID_C2G_HeartBeat, &msgHeartBeat);

	return true;
}

bool CTestClient::OnDisconnect()
{
	LOGInfo("Test客户端与服务器断开网络连接。");

	m_pNetSocket = nullptr;

	return true;
}

bool CTestClient::OnRecvPacket(const char *pPacket, unsigned short wLength)
{
	// 协议解析
	const unsigned short *pProtocolId = reinterpret_cast<const unsigned short *>(pPacket);
	unsigned short wProtocolId = *pProtocolId;

	const char *pMessage = pPacket + sizeof(unsigned short);

	int nMessageLength = wLength - sizeof(unsigned short);

	switch (wProtocolId)
	{
	case gproto::gather::CSID_G2C_HeartBeat:
		{
		}
		break;
	default:
		break;
	}

	return true;
}

bool CTestClient::OnError(int nErrCode)
{
	LOGInfo("连接服务器失败。");

	return true;
}
