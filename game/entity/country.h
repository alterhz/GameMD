// ���ң����ӣ�

#ifndef _COUNTRY_H_
#define _COUNTRY_H_

class ICountry
{
public:
	ICountry() {}
	virtual ~ICountry() {}

public:

private:

};

class CClient;

class CCountry : public ICountry
{
public:
	CCountry();
	~CCountry();

	int GetIndexId() const { return m_nIndexId; }

	CClient * GetClient() const { return m_pClient; }
	
public:

public:
	int m_nIndexId;
	// ͨ�Ŷ���
	CClient *m_pClient;
};

#endif