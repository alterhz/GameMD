//-------------------------------------------
// author : alterhz
// date : 8/13/2015
// description : 

#ifndef _LOGINDISPATCHER_H_
#define _LOGINDISPATCHER_H_

#include "memoryleak.h"
#include "observer.h"
#include "msginc.h"

class CClient;

class CLoginDispatcher : public Observer3::IDispatcher<CLoginDispatcher, const char *, int, CClient *>
{
public:
	virtual void OnInit();

private:
	// ����
	void OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
	// ��ʼ��Ϸ
	void OnStartGame(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
};

#endif