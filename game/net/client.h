//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "netio.h"
#include "msginc.h"
using namespace NS_IO;

class ICountry;

class CClient : public INetClient
{
public:
	CClient();
	virtual ~CClient() {}

	void SetCountry(ICountry *pCountry) { m_pCountry = pCountry; }
	ICountry * GetCountry() { return m_pCountry; }

public:
	// ���������¼�
	virtual bool OnConnected(INetSocket *pNetSocket);
	// ����Ͽ��¼�
	virtual bool OnDisconnect();
	// ��������
	virtual bool OnRecvPacket(const char *pPacket, unsigned short wLength);
	// �����쳣
	virtual bool OnError(int nErrCode) { return true; }

public:
	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

public:
	bool SendPrepare(bool bOk);

private:
	INetSocket *m_pNetSocket;
	// ���Ҳ���
	ICountry *m_pCountry;
};

#endif