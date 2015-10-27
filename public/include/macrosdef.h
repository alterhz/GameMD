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

enum EGroundType
{
	EGroundType_None = 0,
	EGroundType_Land = 1,	//����
	EGroundType_Sand = 2,	//ɳ��
	EGroundType_River = 3,	//����
	EGroundType_Lawn = 4,	//�ݵ�
	EGroundType_Snow = 5,	//ѩ��
};

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
	GObjectType_Still,
	GObjectType_Walkable,
};

// ս������
class IGObject;
typedef std::vector<IGObject *> VtGObject;
typedef std::map<int, IGObject *> MapGObject;

class IGrid;
typedef std::vector<IGrid *> VtGrid;
typedef std::map<int, IGrid *> MapGrid;

class ICountry;
typedef std::vector<ICountry *> VtCountry;
typedef std::map<int, ICountry *> MapCountry;



#endif