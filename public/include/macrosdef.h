//-------------------------------------------
// author : alterhz
// date : 9/9/2015
// description : 

#ifndef _MACROSDEF_H_
#define _MACROSDEF_H_

#include <map>
#include <vector>
#include <list>

#define SAFE_DELETE(p) { delete p; p = nullptr; }

typedef std::vector<int> VtInt;
typedef std::map<int, int> MapInt;
typedef std::list<int> ListInt;

enum EToWard
{
	EToWard_None = 0,	//��ͨ
	EToWard_X ,	//X����ͨ��
	EToWard_Y,	//Y����ͨ��
	EToWard_Both,	//˫��ͨ��
};

enum GObjectType
{
	GObjectType_Base  = 0,	//��������
	GObjectType_Bridge,		//����
	GObjectType_DogFace,	//С��
	GObjectType_Hero,	//Ӣ��
	GObjectType_Sirdar,	//����
};

#endif