//-------------------------------------------
// author : alterhz
// date : 6/17/2015
// description : 

#ifndef _NETIO_H_
#define _NETIO_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

class INetSocket;
class INetAcceptor;

class INetClient
{
public:
	virtual ~INetClient() {}

public:
	// ���������¼�
	virtual bool OnConnected(INetSocket *pNetSocket) { return true; }
	// ����Ͽ��¼�(�˺������ú�INetSocket��������)
	virtual bool OnDisconnect() { return true; }
	// ��������
	virtual bool OnRecvPacket(const char *pPacket, unsigned short wLength) { return true; }
	// �����쳣
	virtual bool OnError(int nErrCode) { return true; }
};

class INetClientManager
{
public:
	virtual ~INetClientManager() { }

public:	//�¼�
	virtual INetClient * OnAccept() = 0;
	// �ͷ�(�˺�������INetAcceptor��������)
	virtual void OnRelease() {}
};

class INetSocket
{
public:
	virtual ~INetSocket() {}

public:
	virtual bool DoSend(const char *pBuffer, unsigned short wLength) = 0;
	virtual void DoClose() = 0;

	virtual const char * GetRemoteIP() = 0;
	virtual unsigned short GetRemotePort() = 0;
	virtual const char * GetLocalIP() = 0;
	virtual unsigned short GetLocalPort() = 0;
};

class INetConnector
{
public:
	virtual ~INetConnector() {}

public:
	virtual bool Connect(INetClient *pNetClient, const char *pIP, unsigned short wPort) = 0;
	virtual bool ReConnect() = 0;
};

class INetAcceptor
{
public:
	virtual ~INetAcceptor() {}

public:
	virtual bool Listen(INetClientManager* pNetClientManager, unsigned short wPort) = 0;
	virtual void Cancel() = 0;
};

class IEventManager;

class INetService
{
public:
	virtual ~INetService() {}
	
public:
	// �����߼�ѭ��(���������ݴ�����Ϸ���false)
	virtual bool DoTick(int nExcuteCount = 1) = 0;
	// Run(����,ֱ������stop)
	virtual bool Run() = 0;
	// Pool(������)����Ϊ0:ִ�������¼�
	virtual bool Poll(int nExcuteCount = 0) = 0;
	// ���������������
	virtual INetAcceptor * CreateListener() = 0;
	// ��������������
	virtual INetConnector * CreateConnector() = 0;
	// ����EventManager
	virtual IEventManager * CreateEventManager(int nThreadNumber = 1) = 0;
};

INetService * CreateNetService();
void DestoryNetService(INetService *pNetService);

NS_IO_Footer

#endif	//_NETIO_H_