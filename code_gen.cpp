// virus.cpp : Defines the entry point for the console application.
//afx.h"

#include "code_gen.h"
#include "stdafx.h"
using namespace std;


int (*fn)();
char* ptrChar;
int* ptrStart;
int* ptrStop;
void* ptrStarter;

proc stProc;
proc* ptrProc=(proc*)&stProc;


/*
Get random number
*/


int getRand(int num){
	if(num == NULL)
	{
		num=ptrProc->szNewCode-1;
	}
	srand(time(0));
	int alloc = (int)(rand()%(num)+1);
	return alloc;
}

int getBit(){
	srand(time(0));
	return (int)(rand()%2);
}

void mutation(proc* st){
	if((st->szNewCode*8)<st->runTimes){
		resize(st);
	}
	flipBit(st);
	st->runTimes++;
}

void flipBit(proc* st){
	int pos = getRand(st->szNewCode);
	ptrChar = (char*)((int)st->ptrNewCode+pos);
	*ptrChar = *ptrChar ^ getBit();
}

void resize(proc* st){
	if(st->szNewCode>0){
		st->szOldCode=st->szNewCode;
		st->szNewCode=st->szOldCode*2;
		st->ptrOldCode=st->ptrNewCode;
	}
	else{
		st->szNewCode=st->szOldCode;
	}
	st->ptrNewCode = VirtualAlloc(NULL,st->szNewCode,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	makecopy(st);
	VirtualFree(st->ptrOldCode,st->szOldCode,MEM_RELEASE);

}

/*
Genes
*/

int sequences(){
	_asm{
		here:
		call addr
		addr:
		pop eax
		mov ptrStart,eax
		call dead_end
		big_return:
		add ptrStarter,0dh
		jmp ptrStarter
		mov eax,ebx;
		mov ebx,esp;
		mov ecx,eax;
		mov edx,eax;
	}
	_asm{
		mov [ebx],eax;
		inc eax;
		dec eax;
		add eax,4;
		add eax,ebx;
		sub eax,4;
		sub eax,ebx;
		shl eax,cl;
		shr eax,cl;
	}
	_asm{
		mov eax, fs:[0x30];
		mov eax, [eax+0x0c];
		mov eax, [eax+0x1c];
		mov eax, [eax];
	}

	_asm{
		xor eax,eax;
		xor ecx,ecx;
	}

	_asm{
		push 0;
		ret;
	}
	_asm{
		dead_end:
		call motherfucker
		motherfucker:
		pop eax
		mov ptrStop,eax
		jmp big_return
	}
	return 0;
}

/*
Copying original to pointers from struct
*/

void makecopy(proc* st){
	//ptrProc->ptrNewCode = malloc(ptrProc->szOldCode*2);
	memset(st->ptrNewCode,(char)0x90,st->szNewCode);
	if(st->ptrNewCode!=0 && st->ptrOldCode!=0 && st->szOldCode > 0){
		memcpy(st->ptrNewCode,st->ptrOldCode,st->szOldCode);
	}

}

/*
	Running sandbox
*/

void sandbox(){
	__try{
		fn=(int(*)())ptrProc->ptrNewCode;
		DumpMem();
		
		if(IsAdmin()){
			exit(0);
		}
		create_process();
		
	}	
	__except(EXCEPTION_EXECUTE_HANDLER || EXCEPTION_WRITE_FAULT || EXCEPTION_ACCESS_VIOLATION){
		//LPCSTR str = "Dohajzlu";
		//VirtualFree(ptrProc->ptrNewCode,ptrProc->szNewCode,MEM_RELEASE);
		//MessageBoxA(NULL,str,"LaLA",MB_OK);
		create_process();
	}


}

/*
	Dump of created process
*/

void DumpMem(){
			ofstream f;
			f.open("mem.dump");
			ptrChar = (char*)ptrProc->ptrNewCode;
			for(int i=0;i<ptrProc->szNewCode;i++)
			{
				f << *(ptrChar+i);
			}
			f.close();

}

/*
	Is administrator?
*/

BOOL IsAdmin()
{
    HANDLE          hToken = NULL;
    PSID            pAdminSid = NULL;
    BYTE            buffer[BUFF_SIZE];
    PTOKEN_GROUPS   pGroups = (PTOKEN_GROUPS)buffer; 
    DWORD           dwSize;             // buffer size
    DWORD           i;
    BOOL            bSuccess;
    SID_IDENTIFIER_AUTHORITY siaNtAuth = SECURITY_NT_AUTHORITY;

    // get token handle
    if ( !OpenProcessToken ( GetCurrentProcess ( ), TOKEN_QUERY, &hToken ) )
        return false;
    
    bSuccess = GetTokenInformation ( hToken, TokenGroups, 
                                    (LPVOID)pGroups, BUFF_SIZE, 
                                    &dwSize );
    CloseHandle ( hToken );
    if ( !bSuccess )
        return false;
    if ( !AllocateAndInitializeSid ( &siaNtAuth, 2, 
                                     SECURITY_BUILTIN_DOMAIN_RID,
                                     DOMAIN_ALIAS_RID_ADMINS,
                                     0,0,0,0,0,0, &pAdminSid ) )
        return false;

    bSuccess = FALSE;
	for ( i = 0; (i < pGroups->GroupCount) && !bSuccess; i++ )
    {
        if ( EqualSid ( pAdminSid, pGroups->Groups[i].Sid ) )
            bSuccess = TRUE;
    }
    FreeSid ( pAdminSid );
    
    return bSuccess;
}

/*
	Creating process
*/

void create_process(){
	mutation(ptrProc);
	sandbox();
}




int _tmain(int argc, _TCHAR* argv[])
{
	/*
	Linking method to ptrCalls structures
	*/


	/*
	ASM code to get pointers
	*/

	__asm{
	call some_return
	hehe:
	call sequences
	some_return:
	pop ptrStarter
	jmp hehe
	}
	ptrProc->ptrOldCode = ptrStart;
	ptrProc->szOldCode = (int)ptrStop-(int)ptrStart;
	
	resize(ptrProc);
	
	create_process();
	//printf("%d %08x %x %x",size,*inst_ptr,old_code,ptrChar);
	printf("0x%08x\n %p\n %p\n %p\n %p\n %p\n %p\n %p\n %p\n %p\n %p\n",ptrProc->ptrNewCode);
	int i=0;
	while(i<=3000000){}
	return 0;
}

