//--------------------------------------------------------------------
// auther: hanzhe
// datetime: 2015-10-28
// description:

#ifndef _MYTIME_H_
#define _MYTIME_H_

#include <time.h>

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

	// ��ȡӦ�ó�������ʱ���
	long GetClock();

	// ��ȡϵͳʱ��(ms)
	unsigned int GetMSTime();

	// ��ȡ��׼0ʱ��ʱ���
	time_t GetTime();

NS_IO_Footer

#endif