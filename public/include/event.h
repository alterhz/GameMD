//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _EVENT_H_
#define _EVENT_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

class IAsyncEvent
{
public:
	virtual ~IAsyncEvent() {}

public:
	// ���߳�ִ��
	virtual bool OnExcute() = 0;
	// ���߳�ִ��
	virtual bool OnResult() = 0;
};

class IEventManager
{
public:
	virtual ~IEventManager() {}

	// ֹͣ
	virtual void Stop() = 0;

	// Ͷ���첽�¼�
	virtual bool PostAsyncEvent(IAsyncEvent *pAsyncEvent) = 0;
};

class ITimerEvent
{
public:
	virtual ~ITimerEvent() {}

public:
	// ����true:����������false:��ֹ
	virtual bool OnTimerEvent(int nTimerId) = 0;
	// ɾ��
	virtual void OnKilled(int nTimerId) {}
};

#define INVALID_TIMER_ID	(0)

class ITimerManager
{
public:
	virtual ~ITimerManager() {}

	// ����Timer�¼�,����TimerId������ʧ�ܣ�����INVALID_TIMER_ID
	virtual int SetTimer(ITimerEvent *pTimerEvent, int nInterval) = 0;
	// ɾ��Timer�¼�
	virtual void KillTimer(int nTimerId) = 0;
};

NS_IO_Footer

#endif