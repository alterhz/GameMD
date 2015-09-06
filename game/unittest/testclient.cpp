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

	// Э����
	unsigned short *pMessageLength = reinterpret_cast<unsigned short *>(szSendBuffer);
	*pMessageLength = wProtocolId;

	void *pData = reinterpret_cast<void *>(szSendBuffer + sizeof(unsigned short));

	// Э�����ݱ���
	if (!pMessage->SerializePartialToArray(pData, MAX_SENDBUFFER_LENGTH-sizeof(unsigned short)))
	{
		LOGPrint("Э��[" + wProtocolId + "]����ʧ�ܡ�");
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

	LOGInfo("Test�ͻ��������Ϸ�����[localIP:" + m_pNetSocket->GetLocalIP() + ",localPort:"
		+ m_pNetSocket->GetLocalPort() + ", remoteIP:" + m_pNetSocket->GetRemoteIP() + ", remotePort:"
		+ m_pNetSocket->GetRemotePort() + "]��");

	// ���Ͳ�������
	//gproto::gather::MSG_C2G_HeartBeat msgHeartBeat;
	//SendMessage(gproto::gather::CSID_C2G_HeartBeat, &msgHeartBeat);

	gproto::MSG_C2G_StartGame msgStartGame;
	msgStartGame.set_rolename("����һ��");
	SendMessage(gproto::CSID_C2G_StartGame, &msgStartGame);

	return true;
}

bool CTestClient::OnDisconnect()
{
	LOGInfo("Test�ͻ�����������Ͽ��������ӡ�");

	m_pNetSocket = nullptr;

	return true;
}

bool CTestClient::OnRecvPacket(const char *pPacket, unsigned short wLength)
{
	// Э�����
	const unsigned short *pProtocolId = reinterpret_cast<const unsigned short *>(pPacket);
	unsigned short wProtocolId = *pProtocolId;

	const char *pMessage = pPacket + sizeof(unsigned short);

	int nMessageLength = wLength - sizeof(unsigned short);

	switch (wProtocolId)
	{
	case gproto::CSID_G2C_HeartBeat:
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
	LOGInfo("���ӷ�����ʧ�ܡ�");

	return true;
}