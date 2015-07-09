//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _MSGDISPATCH_H_
#define _MSGDISPATCH_H_

#include "msginc.h"
#include "UtilityInc.h"
#include "event.h"
using namespace NS_IO;

class CClient;
class CProtocolFactory;

class CMsgDispatch : public Singleton<CMsgDispatch>
{
public:
	CMsgDispatch();
	~CMsgDispatch();
	void SetCurrentClient(CClient *pClient) { m_pCurrentClient = pClient; }

public:
	bool Dispatch(unsigned short wProtocolID, const char *pBuffer, int nLength);

private:
	bool OnMsgC2GHeartBeat(gproto::gather::MSG_C2G_HeartBeat *pMsg);
	bool OnMsgG2CHeartBeat(gproto::gather::MSG_G2C_HeartBeat *pMsg);

	// ��ֵ
	bool OnMsgO2GDeposit(gproto::gather::MSG_O2G_Deposit *pMsg);
	// �˺�ע��
	bool OnMsgO2GAccountReg(gproto::gather::MSG_O2G_AccountReg *pMsg);
	// ��ɫ����
	bool OnMsgO2GRoleCreate(gproto::gather::MSG_O2G_RoleCreate *pMsg);
	// ��½��־
	bool OnMsgO2GLogin(gproto::gather::MSG_O2G_Login *pMsg);
	// ������չ��־
	bool OnMsgO2GBagExtend(gproto::gather::MSG_O2G_BagExtend *pMsg);
	// �����������������־
	bool OnMsgO2GBuyInstance(gproto::gather::MSG_O2G_BuyInstance *pMsg);
	// ��Ϸ�ҹ�����־
	bool OnMsgO2GBuyCoin(gproto::gather::MSG_O2G_BuyCoin *pMsg);
	// ��ʯ����������
	bool OnMsgO2GDiamond(gproto::gather::MSG_O2G_Diamond *pMsg);
	// ��Ҳ���������
	bool OnMsgO2GCoin(gproto::gather::MSG_O2G_Coin *pMsg);
	// ���߲���������
	bool OnMsgO2GItem(gproto::gather::MSG_O2G_Item *pMsg);
	// װ��������־
	bool OnMsgO2GEquip(gproto::gather::MSG_O2G_Equip *pMsg);
	// ������־
	bool OnMsgO2GRelive(gproto::gather::MSG_O2G_Relive *pMsg);
	// ������־
	bool OnMsgO2GInstance(gproto::gather::MSG_O2G_Instance *pMsg);
	// ��������־
	bool OnMsgO2GArena(gproto::gather::MSG_O2G_Arena *pMsg);

private:
	CClient *m_pCurrentClient;
	CProtocolFactory *m_pProtocolFactory;
};

#endif