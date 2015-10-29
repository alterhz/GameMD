#include "testclient.h"
#include "debug.h"

CTestClient::CTestClient()
	: m_nId(0)
	, m_pNetConnector(nullptr)
	, m_pNetSocket(nullptr)
	, m_tBegin(0)
	, m_nCountryIndexId(0)
{
	static int g_nID = 0;
	m_nId = (++g_nID);
}

CTestClient::~CTestClient()
{

}

bool CTestClient::Init(INetConnector *pNetConnector)
{
	if (nullptr == pNetConnector)
	{
		return false;
	}

	m_pNetConnector = pNetConnector;

	return m_pNetConnector->Connect(this, "127.0.0.1", 8000);
}

bool CTestClient::SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage)
{
	char szSendBuffer[MAX_SENDBUFFER_LENGTH] = {0};

	// Э����
	unsigned short *pMessageLength = reinterpret_cast<unsigned short *>(szSendBuffer);
	*pMessageLength = wProtocolId;

	void *pData = reinterpret_cast<void *>(szSendBuffer + sizeof(unsigned short));

	// Э�����ݱ���
	if (!pMessage->SerializePartialToArray(pData, MAX_SENDBUFFER_LENGTH-sizeof(unsigned short)))
	{
		LOGPrint("Э��[" + wProtocolId + "]����ʧ�ܡ�");
		return false;
	}

	if (nullptr == m_pNetSocket)
	{
		return false;
	}

	return m_pNetSocket->DoSend(szSendBuffer, sizeof(unsigned short) + pMessage->ByteSize());
}

void CTestClient::Disconnect()
{
	if (m_pNetSocket)
	{
		m_pNetSocket->DoClose();
	}
}

bool CTestClient::OnConnected(INetSocket *pNetSocket)
{
	if (nullptr == pNetSocket)
	{
		return false;
	}

	m_pNetSocket = pNetSocket;

	LOGInfo("Test�ͻ��������Ϸ�����[localIP:" + m_pNetSocket->GetLocalIP() + ",localPort:"
		+ m_pNetSocket->GetLocalPort() + ", remoteIP:" + m_pNetSocket->GetRemoteIP() + ", remotePort:"
		+ m_pNetSocket->GetRemotePort() + "]��");

	//// ���Ͳ�������
	//gproto::MSG_C2G_HeartBeat msgHeartBeat;
	//SendMessage(gproto::CSID_C2G_HeartBeat, &msgHeartBeat);

	//gproto::MSG_C2G_StartGame msgStartGame;
	//msgStartGame.set_rolename("����һ��");
	//SendMessage(gproto::CSID_C2G_StartGame, &msgStartGame);

	SendPrepare();

	return true;
}

bool CTestClient::OnDisconnect()
{
	LOGInfo("Test�ͻ�����������Ͽ��������ӡ�");

	m_pNetSocket = nullptr;

	return true;
}

bool CTestClient::OnRecvPacket(const char *pPacket, unsigned short wLength)
{
	// Э�����
	const unsigned short *pProtocolId = reinterpret_cast<const unsigned short *>(pPacket);
	unsigned short wProtocolId = *pProtocolId;

	const char *pMessage = pPacket + sizeof(unsigned short);

	int nMessageLength = wLength - sizeof(unsigned short);

	switch (wProtocolId)
	{
	case gproto::CSID_G2C_HeartBeat:
		{
			gproto::MSG_G2C_HeartBeat msgHeartBeat;
			if (!msgHeartBeat.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("Э��CSID_G2C_HeartBeat����ʧ�ܡ�");
				return false;
			}

			LOGDebug("�ط�����CSID_G2C_HeartBeat");
		}
		break;
	case gproto::CSID_G2C_PrepareGround:
		{
			gproto::MSG_G2C_PrepareGround msg;
			if (!msg.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("Э��CSID_G2C_PrepareGround����ʧ�ܡ�");
				return false;
			}
			
			m_nCountryIndexId = msg.countryindexid();

			LOGDebug("[" + m_nId + "]CountryIndexId[" + m_nCountryIndexId + "]ƥ��ɹ���׼��������");

			// ׼������
			SendEnterGround();
		}
		break;
	case gproto::CSID_G2C_EnterGround:
		{
			gproto::MSG_G2C_EnterGround msgEnterGround;
			if (!msgEnterGround.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("Э��CSID_G2C_EnterGround����ʧ�ܡ�");
				return false;
			}

			// ���볡���ɹ�
			if (gproto::MSG_G2C_EnterGround_EResult_ERR == msgEnterGround.ret())
			{
				LOGDebug("[" + m_nId + "]���볡��ʧ�ܡ�");
			}
			else
			{
				SendGetGroundInfo();
			}
		}
		break;
	case gproto::CSID_G2C_GetGroundInfo:
		{
			gproto::MSG_G2C_GetGroundInfo msgGetGroundInfo;
			if (!msgGetGroundInfo.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("Э��CSID_G2C_GetGroundInfo����ʧ�ܡ�");
				return false;
			}

			LOGPrint("[" + m_nId + "]�����ߴ�W:" + msgGetGroundInfo.wgcount() + ",H:" + msgGetGroundInfo.hgcount());

			/*
			for (int i=0; i<msgGetGroundInfo.grids_size(); ++i)
			{
				const gproto::Info_Grid &infoGrid = msgGetGroundInfo.grids(i);

				LOGPrint("SN:" + infoGrid.sn() + ",X:" + infoGrid.x() + ",Y:" + infoGrid.y());
			}
			*/

			for (int i=0; i<msgGetGroundInfo.gobjects_size(); ++i)
			{
				const gproto::Info_GObject &infoGObject = msgGetGroundInfo.gobjects(i);

				if (gproto::Info_GObject_EType_Still == infoGObject.type())
				{
					LOGPrint("[" + m_nId + "]Still -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
						+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp() 
						+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
				}
				else
				{
					LOGPrint("[" + m_nId + "]Walkable -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
						+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp()
						+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
				}
			}
		}
		break;
	case gproto::CSID_G2C_GObjectEnterGround:
		{
			gproto::MSG_G2C_GObjectEnterGround msgGObjectEnterGround;
			if (!msgGObjectEnterGround.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("Э��CSID_G2C_GObjectEnterGround����ʧ�ܡ�");
				return false;
			}

			const gproto::Info_GObject &infoGObject = msgGObjectEnterGround.gobject();

			if (gproto::Info_GObject_EType_Still == infoGObject.type())
			{
				LOGPrint("[" + m_nId + "]Still EnterGround -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
					+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp() 
					+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
			}
			else
			{
				LOGPrint("[" + m_nId + "]Walkable EnterGround -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
					+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp()
					+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
			}
		}
		break;
	case gproto::CSID_G2C_BattlePrepare:
		{
			// ׼��
			SendPrepareFinish();
		}
		break;
	case gproto::CSID_G2C_BattleStart:
		{
			// ս����ʼ
			LOGPrint("[" + m_nId + "]ս����ʼ");
		}
		break;
	case gproto::CSID_G2C_BattleReward:
		{
			// ����
			gproto::MSG_G2C_BattleReward msg;
			if (!msg.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("Э��CSID_G2C_BattleReward����ʧ�ܡ�");
				return false;
			}

			LOGDebug("[" + m_nId + "]ʤ����Country��" + msg.wincountryindexid());
		}
		break;
	case gproto::CSID_G2C_BattleFight:
		{
			// ս������
			gproto::MSG_G2C_BattleFight msg;
			if (!msg.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("Э��CSID_G2C_BattleFight����ʧ�ܡ�");
				return false;
			}

			if (msg.countryindexid() == m_nCountryIndexId)
			{
				// �ֵ��Լ��Ļغ�
				LOGDebug("[" + m_nId + "]Country[" + msg.countryindexid() + "]�Լ��Ļغϣ����Թ�����");

				{
					// �����߼�
				}

				static int n = 0;
				++n;
				if (n < 10)
				{
					// �������
					SendBattleFightEnd();
				}
			}
			else
			{
				LOGDebug("[" + m_nId + "]Country[" + msg.countryindexid() + "]�����˵Ļغϣ��ȴ���");
			}
		}
		break;
	default:
		break;
	}

	return true;
}

bool CTestClient::OnError(int nErrCode)
{
	LOGInfo("���ӷ�����ʧ�ܡ�");

	return true;
}

bool CTestClient::SendPrepare()
{
	gproto::MSG_C2G_Prepare msg;
	return SendMessage(gproto::CSID_C2G_Prepare, &msg);
}

bool CTestClient::SendEnterGround()
{
	gproto::MSG_C2G_EnterGround msg;
	return SendMessage(gproto::CSID_C2G_EnterGround, &msg);
}

bool CTestClient::SendGetGroundInfo()
{
	gproto::MSG_C2G_GetGroundInfo msg;
	return SendMessage(gproto::CSID_C2G_GetGroundInfo, &msg);
}

bool CTestClient::SendPrepareFinish()
{
	gproto::MSG_C2G_PrepareFinish msg;
	return SendMessage(gproto::CSID_C2G_PrepareFinish, &msg);
}

bool CTestClient::SendBattleFightEnd()
{
	gproto::MSG_C2G_BattleFightEnd msg;
	return SendMessage(gproto::CSID_C2G_BattleFightEnd, &msg);
}


CTestClientManager::CTestClientManager()
	: m_pTestClientA(new CTestClient)
	, m_pTestClientB(new CTestClient)
{
}

CTestClientManager::~CTestClientManager()
{
	delete m_pTestClientA;
	m_pTestClientA = nullptr;

	delete m_pTestClientB;
	m_pTestClientB = nullptr;
}

void CTestClientManager::Init(NS_IO::INetService *pNetService)
{
	if (nullptr == pNetService)
	{
		LOGError("nullptr == pNetService");
		return ;
	}

	if (nullptr == m_pTestClientA || nullptr == m_pTestClientB)
	{
		LOGError("nullptr == m_pTestClientA || nullptr == m_pTestClientB");
		return ;
	}

	INetConnector *pNetConnectorA = pNetService->CreateConnector();
	if (pNetConnectorA)
	{
		m_pTestClientA->Init(pNetConnectorA);
	}

	INetConnector *pNetConnectorB = pNetService->CreateConnector();
	if (pNetConnectorB)
	{
		m_pTestClientB->Init(pNetConnectorB);
	}
}
