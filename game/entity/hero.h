// ս��Ӣ��

#ifndef _HERO_H_
#define _HERO_H_


class IFightObj
{
public:
	IFightObj();
	virtual ~IFightObj();

public:
	virtual void Walk() = 0;

private:
	int m_nIndexId;
};

// С��
class CDogFace : public IFightObj
{
public:
	CDogFace();
	~CDogFace();

};

// Ӣ��
class CHero : public IFightObj
{
public:
	CHero();
	~CHero();

public:

};

// ���죨˾��٣�
class CSirdar : public IFightObj
{
public:
	CSirdar();
	~CSirdar();


};



#endif