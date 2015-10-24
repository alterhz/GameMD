//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _TESTCLIENT_H_
#define _TESTCLIENT_H_

#include "netio.h"
#include "msginc.h"
#include "utilityinc.h"
using namespace NS_IO;

#include <ctime>

class CTestClient : public INetClient
{
public:
	CTestClient();
	~CTestClient();

	bool Init(INetConnector *pNetConnector);

	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

	void Disconnect();

public:
	// ׼��
	bool SendPrepare();

protected:
	// ���������¼�
	virtual bool OnConnected(INetSocket *pNetSocket);
	// ����Ͽ��¼�(�˺������ú�INetSocket��������)
	virtual bool OnDisconnect();
	// ��������
	virtual bool OnRecvPacket(const char *pPacket, unsigned short wLength);
	// �����쳣
	virtual bool OnError(int nErrCode);


private:
	INetConnector *m_pNetConnector;
	INetSocket *m_pNetSocket;

	std::clock_t m_tBegin;
};

class CTestClientManager : public Singleton<CTestClientManager>
{
public:
	CTestClientManager();
	~CTestClientManager();

public:
	void Init(NS_IO::INetService *pNetService);


private:
	CTestClient *m_pTestClientA;
	CTestClient *m_pTestClientB;
};


#endif