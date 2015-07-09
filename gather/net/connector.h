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

public:
	// ����������־
	bool GatherBagExtend(int nRoleId, std::string strRoleName, int nRoleSN, int nType, int nExtendNum);
	// �����������������־
	bool GatherBuyInstance(int nRoleId, std::string strRoleName, int nRoleSN, 
		int nRoleLevel, int nVipLevel, int nInstanceSN, int nBuyCount);
	// ��Ϸ�ҹ�����־
	bool GatherBuyCoin(int nRoleId, std::string strRoleName, int nRoleSN, 
		int nRoleLevel, int nVipLevel, int nSpendDiamond, int nCoin);
	// ��ʯ����������
	bool GatherDiamond(std::string strAccount, int nRoleId, std::string strRoleName, int nRoleLevel, 
		int nType, int nSubType, int nChangeDiamond, int nTotalDiamond);
	// ��Ҳ���������
	bool GatherCoin(int nRoleId, std::string strRoleName, int nRoleLevel, 
		int nType, int nSubType, int nChangeCoin, int nTotalCoin);
	// ���߲�����������־
	bool GatherItem(int nItemSN, int nItemType, int nNum, int nType, int nWay, 
		int nRoleId, std::string strRoleName, int nRoleLevel, gproto::gather::INFO_Equipment &infoEquipMent);
	// װ��������־
	bool GatherEquip(int nRoleId, std::string strRoleName, int nEquipSN, int nEquipId, int nMakeType, 
		gproto::gather::INFO_Equipment &equipMakeBefore, gproto::gather::INFO_Equipment &equipMakeAfter);
	// ������־
	bool GatherRelive(int nRoleId, std::string strRoleName, int nRoleLevel, int nInstanceSN, int nWasteDiamond);
	// ������־
	bool GatherInstance(int nInstanceSN, int nResult, int nPassTime, int nDeadTimes, std::vector<int> vtRoleId);
	// ��������־
	bool GatherArena(int nRoleId, std::string strRoleName, int nRoleLevel, int nResult);


public:
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
	int m_nTimerId;
};

#endif