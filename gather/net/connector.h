//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "netio.h"
#include "msginc.h"
#include "event.h"
using namespace NS_IO;

class CConnectorTest : public INetClient, ITimerEvent
{
public:
	CConnectorTest();
	virtual ~CConnectorTest();

	void Init(INetConnector *pNetConnector);

	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

	void Disconnect();

protected:
	// ���������¼�
	virtual bool OnConnected(INetSocket *pNetSocket);
	// ����Ͽ��¼�(�˺������ú�INetSocket��������)
	virtual bool OnDisconnect();
	// ��������
	virtual bool OnRecvPacket(const char *pPacket, unsigned short wLength);
	// �����쳣
	virtual bool OnError(int nErrCode) { return true; }

	// ����true:����������false:��ֹ
	virtual bool OnTimerEvent(int nTimerId);
	// ɾ��
	virtual void OnKilled(int nTimerId);

private:
	INetConnector *m_pNetConnector;
	INetSocket *m_pNetSocket;
	int m_nTimerId;
};

#endif