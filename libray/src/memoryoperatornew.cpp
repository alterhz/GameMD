#include "memoryoperatornew.h"
#include "memorystatistics.h"

void * operator new(unsigned int size, const char *file, int line)
{
	// �������ַ������Դﵽͬ����Ч����������һ�ֱȽϺ�  
	// ��Ϊ������һ�ֿ��Ա���ԭ�е����뷽ʽһ��  
	//void * p = malloc(size);
	void *p = operator new(size);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().New(p, size, file, line);
	}

	return p;
}

void operator delete(void *p)
{
	free(p);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().Delete(p);
	}
}

void operator delete [](void *p)
{
	free(p);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().Delete(p);
	}
}

void operator delete(void *p, const char *file, int line)
{
	free(p);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().Delete(p);
	}
}

void operator delete [](void *p, const char *file, int line)
{
	free(p);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().Delete(p);
	}
}

