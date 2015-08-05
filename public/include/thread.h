//-------------------------------------------
// author : alterhz
// date : 6/26/2015
// description : 

#ifndef _THREAD_H_
#define _THREAD_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

namespace boost
{
	class shared_mutex;
	class mutex;

	namespace interprocess
	{
		class interprocess_semaphore;
	}
}

NS_IO_Header

// �߳�˯��
void Sleep(unsigned long nInterval);
// ��ȡ��ǰ�߳�ID
int GetCurThreadId();

// ��
class CMutex
{
public:
	CMutex();
	~CMutex();

	void try_lock();
	void lock();
	void unlock();

private:
	boost::mutex *m_pMtx;
};

// �߳�
class IThread
{
public:
	virtual ~IThread() {}

public:
	virtual bool OnThreadRun() = 0;
};

// �����߳�
bool CreateThread(IThread &thread);

NS_IO_Footer

#endif