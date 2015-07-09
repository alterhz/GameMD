#include "dbconnection.h"
#include "debug.h"

CDbConnectionManager::CDbConnectionManager()
	: m_strServer("127.0.0.1")
	, m_strUser("root")
	, m_strPwd("")
	, m_strDb("gamedb_gather")
	, m_nPort(3306)
{
}

CDbConnectionManager::~CDbConnectionManager()
{
	for (mysqlpp::Connection *pDbConnection : m_vtDbConnection)
	{
		if (pDbConnection)
		{
			pDbConnection->disconnect();

			delete pDbConnection;
		}
	}

	m_vtDbConnection.clear();
}

mysqlpp::Connection * CDbConnectionManager::Fetch()
{
	mysqlpp::Connection *pDbConnection = nullptr;

	m_mtx.lock();
	if (m_vtDbConnection.size() > 0)
	{
		pDbConnection = m_vtDbConnection.back();
		m_vtDbConnection.pop_back();
	}
	m_mtx.unlock();

	if (nullptr == pDbConnection)
	{
		pDbConnection = new mysqlpp::Connection(false);
		if (nullptr == pDbConnection)
		{
			// ����
			return nullptr;
		}

		//�ǳ���Ҫ������Ҫ�����ַ������������Ļ�������
		pDbConnection->set_option(new mysqlpp::SetCharsetNameOption("utf8"));
		//�ǳ���Ҫ���������ã�����洢�����޷�ִ��
		pDbConnection->set_option(new mysqlpp::MultiStatementsOption(true));
		pDbConnection->set_option(new mysqlpp::MultiResultsOption(true));
		// �����Զ�����
		pDbConnection->set_option(new mysqlpp::ReconnectOption(true));

		// û�����ӻ��߶Ͽ������ӣ�����������
		if (!pDbConnection->connect(m_strDb.c_str(), m_strServer.c_str(), m_strUser.c_str(), m_strPwd.c_str(), m_nPort))
		{
			// ����ʧ��
			LOGPrint("����Db[Server:" + m_strServer.c_str() + ", User:" + m_strUser.c_str() + ", Pwd:" + m_strPwd.c_str() 
				+ ", Db:" + m_strDb.c_str() + ", Port:" + m_nPort + "]ʧ�ܡ�");

			delete pDbConnection;
			pDbConnection = nullptr;

			return nullptr;
		}
	}

	return pDbConnection;
}

void CDbConnectionManager::Recycle(mysqlpp::Connection *pDbConnection)
{
	if (nullptr == pDbConnection)
	{
		return ;
	}

	m_mtx.lock();
	m_vtDbConnection.push_back(pDbConnection);
	m_mtx.unlock();
}



