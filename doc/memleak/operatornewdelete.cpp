#include "operatornewdelete.h"

#include "memorystatistics.h"

void * operator new(unsigned int size, const char *file, int line)
{
	// �������ַ������Դﵽͬ����Ч����������һ�ֱȽϺ�  
	// ��Ϊ������һ�ֿ��Ա���ԭ�е����뷽ʽһ��  
	//void * p = malloc(size);
	void *p = operator new(size);

	std::cout << "[new addr:" << p << " - new size:" << size;
	std::cout << " - file:" << file << " - " << line << "]" << std::endl;

	CMemoryStatistics::getMe().New(p, size, file, line);

	return p;
}

void operator delete(void *p)
{
	std::cout << "[delete addr:" << p  << "]" << std::endl;
	free(p);

	CMemoryStatistics::getMe().Delete(p);
}

void operator delete [](void *p)
{
	std::cout << "[delete [] addr:" << p << "]" << std::endl;
	free(p);

	CMemoryStatistics::getMe().Delete(p);
}

void operator delete(void *p, const char *file, int line)
{
	std::cout << "[delete addr:" << p << " - file:" << file << " - line:" << line << "]" << std::endl;
	free(p);

	CMemoryStatistics::getMe().Delete(p);
}

void operator delete [](void *p, const char *file, int line)
{
	std::cout << "[delete [] addr:" << p << " - file:" << file << " - line:" << line << "]" << std::endl;
	free(p);

	CMemoryStatistics::getMe().Delete(p);
}
