
#include "functions.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
/*
* My header file
*/
#define BUFF_SIZE   1024

typedef struct proc {
	void* ptrImg;
	int size;
	void* ptrOldCode;
	int szOldCode;
	void* ptrNewCode;
	char* ptrGen;
	int szNewCode;
	int index;
	double runTimes;
}proc;


void sandbox();
int sequences();
void create_process();
int* find_end(void* addr,int* size);
int getRand(int num);
void makecopy(proc* st);
void flipBit(proc* st);
void mutation(proc* st);
void resize(proc* st);
void DumpMem();
BOOL IsAdmin();