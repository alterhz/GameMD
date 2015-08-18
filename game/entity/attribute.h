//-------------------------------------------
// author : alterhz
// date : 8/17/2015
// description : 

#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

#include <map>

enum EAttrType
{
	EAttrType_Null = 0,
	EAttrType_HP,		//��ǰѪ��
	EAttrType_MaxHP,	//���Ѫ��
	EAttrType_Att,		//������
	EAttrType_Speed,	//�ƶ��ٶ�
};

class CAttribute
{
	typedef std::map<EAttrType, float> MapAttr;
public:
	CAttribute() {}
	~CAttribute() {}

public:
	void SetAttr(EAttrType eAttrType, float f);
	float GetAttr(EAttrType eAttrType);

private:
	MapAttr m_mapAttr;
};

#endif