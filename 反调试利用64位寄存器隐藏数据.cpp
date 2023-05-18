#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

char secret[8] = { 0, };
void  go_read();
void  go_write();
int jmp1[6] = { (int)go_write};
int jmp2[6] = { (int)go_read};
int LL;
int L3;
void __declspec(naked) go_read()
{
	__asm {

		__emit 0xb8
		__emit 0x00
		__emit 0x00
		__emit 0x00
		__emit 0x00
		__emit 0x67
		__emit 0x4c
		__emit 0x89
		__emit 0x20

		__emit 0x68
		__emit 0x90
		__emit 0x90
		__emit 0x90
		__emit 0x90

		__emit 0x66
		__emit 0xc7
		__emit 0x44
		__emit 0x24
		__emit 0x04
		__emit 0x23
		__emit 0x00

		retf

	}
}

void __declspec(naked) go_write()
{
	__asm {
		//mov r12, 0x12345678
		__emit 0x49
		__emit 0xc7
		__emit 0xc4
		__emit 0x78
		__emit 0x56
		__emit 0x34
		__emit 0x12
		
		//push rmi32
		__emit 0x68
		__emit 0x90
		__emit 0x90
		__emit 0x90
		__emit 0x90
		
		__emit 0x66
		__emit 0xc7
		__emit 0x44
		__emit 0x24
		__emit 0x04
		__emit 0x23
		__emit 0x00

		retf

	}
}

void main()
{
	//__asm int 3;
	printf("---------%p\n", secret);
	*((PWORD)(jmp1 + 1)) = 0x33;
	*((PWORD)(jmp2 + 1)) = 0x33;
	DWORD dwOld = 0;
	VirtualProtect(go_read, 4, PAGE_EXECUTE_READWRITE, &dwOld);
	VirtualProtect(go_write, 4, PAGE_EXECUTE_READWRITE, &dwOld);
	__asm {
		pushad
		mov eax, offset L1
		mov LL, eax
		mov edx,  go_write
		add edx, 8
		mov [edx], eax

		mov eax, offset L2
		mov L3, eax
		mov edx,  go_read
		mov ecx, edx
		add edx, 10
		mov[edx], eax
		inc ecx
		lea edi, secret
		mov [ecx], edi

		popad

	}
  	printf("go_read: %p\n", go_read);
	printf("go_write: %p\n", go_write);
	__asm {
		jmp fword ptr jmp1
	}
L1:
	printf("write data ok\n");
	system("pause");
	
	__asm {
		jmp fword ptr jmp2
	}
L2: 

	printf("back to x86\n");
	printf("---------%p\n", *(int*)secret);
	system("pause");
}