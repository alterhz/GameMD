//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _APP_H_
#define _APP_H_

#include "netio.h"
#include "event.h"
#include "utilityinc.h"
#include "thread.h"

using namespace NS_IO;

class CApp : public Singleton<CApp>, public ITimerEvent, public IThread
{
public:
	CApp();
	~CApp();

public:
	// ����
	bool Run();
	// ֹͣ
	void Stop();

	// Ͷ���첽�¼�
	bool PostAsyncEvent(IAsyncEvent *pAsyncEvent);

	// ��ʱ�����ú�ȡ��
	int SetTimer(ITimerEvent *pTimerEvent, int nInterval);
	void KillTimer(int nTimerId);

private:
	bool InitNet();
	bool InitDb();

protected:
	// ��ʼ��
	bool OnInit();
	// �߳�ѭ��
	virtual bool OnThreadRun();
	// ����true:����������false:��ֹ
	virtual bool OnTimerEvent(int nTimerId);

public:
	INetService *m_pNetService;
	INetAcceptor *m_pNetAcceptor;
	ITimerManager *m_pTimerManager;
	IEventManager *m_pEventManager;
	int m_nRunTimerId;
};

#endif