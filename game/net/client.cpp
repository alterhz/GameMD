#include "client.h"
#include "debug.h"
#include "clientmanager.h"

CClient::CClient()
	: m_pNetSocket(nullptr)
{
}

bool CClient::OnConnected(INetSocket *pNetSocket)
{
	if (nullptr == pNetSocket)
	{
		return false;
	}

	m_pNetSocket = pNetSocket;

	LOGPrint("��һ���ͻ�������");


	return true;
}

bool CClient::OnDisconnect()
{
	LOGPrint("��һ���ͻ��˶Ͽ�");

	m_pNetSocket = nullptr;

	return true;
}

bool CClient::OnRecvPacket(const char *pPacket, unsigned short wLength)
{
	// Э�����
	const unsigned short *pProtocolId = reinterpret_cast<const unsigned short *>(pPacket);
	unsigned short wProtocolId = *pProtocolId;

	const char *pMessage = pPacket + sizeof(unsigned short);

	int nMessageLength = wLength - sizeof(unsigned short);

	CClientManager::getMe().gDM.RunExcute(wProtocolId, pMessage, nMessageLength, this);
	return true;
}

bool CClient::SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage)
{
	if (nullptr == m_pNetSocket)
	{
		return false;
	}

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

	return m_pNetSocket->DoSend(szSendBuffer, sizeof(unsigned short) + pMessage->ByteSize());
}
