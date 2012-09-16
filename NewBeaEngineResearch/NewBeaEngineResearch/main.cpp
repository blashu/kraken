#pragma comment(lib, "BeaEngine.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <BeaEngine\BeaEngine.h>

#include "InstructionContainer.h"
#include "CodeChunk.h"

/* ============================= Init datas */
DISASM MyDisasm;
int len;
int i;
int FileSize;
bool Error = 0;
std::vector<unsigned char> pBuffer;
int  (*pSourceCode) (void);     /* function pointer */
FILE *FileHandle;

int RVA2OFFSET(int Address, unsigned char *pBuff);

/* ===============================================================================*/
/*                              
/*          eliminate JUMPS  and reorder instructions  
/*                              
/* ===============================================================================*/

CodeChunk DisassembleCodeFilter(unsigned char *StartCodeSection, 
								unsigned char *EndCodeSection, 
								int (Virtual_Address))
{
	Error = 0;
	MyDisasm.EIP = (int) StartCodeSection;
	MyDisasm.VirtualAddr = (long long) Virtual_Address;
	MyDisasm.Archi = 0;

	CodeChunk codeChunk;

	/* ============================= Loop for Disasm */
	while (!Error)
	{
		MyDisasm.SecurityBlock = (int) EndCodeSection - MyDisasm.EIP;
		len = Disasm(&MyDisasm);

		if ((len != OUT_OF_BLOCK) && (len != UNKNOWN_OPCODE))
		{
			if (MyDisasm.Instruction.BranchType == JmpType
				&& MyDisasm.Instruction.AddrValue != 0)
			{
				MyDisasm.EIP = RVA2OFFSET((int) MyDisasm.Instruction.AddrValue - 0x400000, &(*pBuffer.begin()));
				MyDisasm.VirtualAddr = MyDisasm.Instruction.AddrValue;
			}
			else
			{
				codeChunk.add_to_chunk(MyDisasm);

				MyDisasm.EIP = MyDisasm.EIP+len;
				MyDisasm.VirtualAddr = MyDisasm.VirtualAddr+len;
			}

			if (MyDisasm.EIP >= (long) EndCodeSection)
			{
				Error = 1;
			}
		}
		else
		{
			Error = 1;
		}
	}

	return codeChunk;
}
/* ===============================================================================*/
/*                              
/*      Convert Relative Virtual Address to offset in the file               
/*          works fine even in naughty binaries             
/*          BeatriX manufacture :)              
/*                              
/* ===============================================================================*/

// Dark magic?
int RVA2OFFSET(int RVA, unsigned char *pBuff)
{
	int RawSize,
		VirtualBorneInf,
		RawBorneInf,
		SectionHeader,
		OffsetNtHeaders,
		OffsetSectionHeaders,
		NumberOfSections,
		SizeOfOptionalHeaders,
		VirtualAddress;

	// Calculating offset to get through section headers
	OffsetNtHeaders = (int) *((int*) (pBuff + 0x3c));
	SizeOfOptionalHeaders = (int) *((unsigned short*) (pBuff + OffsetNtHeaders + 0x14));
	OffsetSectionHeaders = OffsetNtHeaders + SizeOfOptionalHeaders + 0x18;

	NumberOfSections = (int) *((unsigned short*) (pBuff + OffsetNtHeaders + 6)); // Wtf do we need this for? Can't find any use of this variable.

	VirtualBorneInf = 0;
	RawBorneInf = 0;
	VirtualAddress = 0;
	SectionHeader = 0;

	while (VirtualAddress <= RVA)
	{
		if (VirtualAddress != 0)
		{
			VirtualBorneInf = VirtualAddress;
			RawSize = (int) *((unsigned int*) (pBuff + OffsetSectionHeaders + 0x10)); // Why the fuck we are reassigning this shit every time in the cycle?
			RawBorneInf = (int) *((unsigned int*) (pBuff + OffsetSectionHeaders + 0x14));
		}

		VirtualAddress = (int) *((unsigned int*) (pBuff + OffsetSectionHeaders 
			+ SectionHeader*0x28 + 0x0C));
		SectionHeader ++;
	}

	if ((RVA - VirtualBorneInf) > RawSize) 
	{
		return -1;
	}

	RawBorneInf = RawBorneInf >> 8;

	if (RawBorneInf & 1) 
	{ 
		RawBorneInf--;
	}

	RawBorneInf = RawBorneInf << 8;

	return RVA - VirtualBorneInf + RawBorneInf + (int) pBuff;
}
/* ===============================================================================*/
/*                  
/*              MAIN    
/*                          
/* ===============================================================================*/
int main(void)
{
	FileHandle = fopen("researcher.exe", "rb");

	if(!FileHandle)
	{
		return 1;
	}

	(void) fseek(FileHandle, 0, SEEK_END);
	FileSize = ftell(FileHandle);
	(void) rewind(FileHandle);

	//pBuffer = (unsigned char*) malloc(FileSize);
	pBuffer = std::vector<unsigned char>( FileSize );

	(void) fread( &(*pBuffer.begin()), sizeof(*pBuffer.begin()), pBuffer.size(), FileHandle);
	(void) fclose(FileHandle);
	/* ============================= Init the Disasm structure (important !)*/
	(void) memset (&MyDisasm, 0, sizeof(DISASM));

	(void) printf("******************************************************* \n");
	(void) printf("Disassemble code by following jumps\n");
	(void) printf("******************************************************* \n");

	auto codeChunk = DisassembleCodeFilter(&(*pBuffer.begin()) + 0x400, &(*pBuffer.begin()) + 0x430, 0x401000);

	ofstream oFile("pew.txt");

	if(!oFile.is_open())
	{
		std::cout << "error" << std::endl;
	}

	for(auto it = codeChunk.begin(), end = codeChunk.end();
		it != end;
		++it)
	{
		std::cout << it->CompleteInstr << endl;
	}

	return 0;
}

///* ============================= Init datas */
//DISASM MyDisasm;
//int len;
//bool Error = 0;
//void *pBuffer;
//int  (*pSourceCode) (void);     /* function pointer */
//
//void DisassembleThisProgram();
//
//void DisassembleSomeBlockOfThisProgram(UIntPtr endPtr);
//
//void DisassembleCode(
//    char *StartCodeSection,
//    char *EndCodeSection,
//    int (*Virtual_Address)(void));
//
//int main(void)
//{
//    pSourceCode = &main;
// 
//    pBuffer = malloc(500);
//    /* ============================= Let's NOP the buffer */
//    (void) memset (pBuffer, 0x90, 500);
// 
//    /* ============================= Copy 100 bytes in it */
//    (void) memcpy (pBuffer, pSourceCode, 500);
// 
//    /* ============================= Disassemble code located in that buffer */
//    DisassembleCode ((char*) pBuffer, ( (char*) pBuffer ) + 500, pSourceCode);
//    
//    // DisassembleThisProgram();
//
//    // DisassembleSomeBlockOfThisProgram(0x401020);
//    
//    return 0;
//}
//
//void DisassembleThisProgram()
//{
//    /* ============================= Init datas */
//    DISASM MyDisasm;
//    int len, i = 0;
//    int Error = 0;
//     
//    /* ===================== display the version and revision used */
//        (void) printf("Version : %s\n", BeaEngineVersion());
//        (void) printf("Revision : %s\n", BeaEngineRevision());
//    /* ============================= Init the Disasm structure (important !)*/
//    (void) memset (&MyDisasm, 0, sizeof(DISASM));
// 
//    /* ============================= Init EIP */
//    MyDisasm.EIP = (UIntPtr) &main;
// 
//    /* ============================= Loop for Disasm */
//    while ((!Error) && (i<30)){
//        len = Disasm(&MyDisasm);
//        if (len != UNKNOWN_OPCODE) {
//            (void) puts(MyDisasm.CompleteInstr);
//            MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)len;
//            i++;
//        }
//        else {
//            Error = 1;
//        }
//    };
//}
//
//// Never gets past 15 byte block for Intel (reference: http://www.beaengine.org/index.php?option=com_content&view=article&id=7&Itemid=8#Ex2)
//void DisassembleSomeBlockOfThisProgram(UIntPtr endPtr)
//{
//    /* ============================= Init datas */
//    DISASM MyDisasm;
//    int len;
//    int Error = 0;
//    UInt64 EndCodeSection = endPtr;
// 
//    /* ============================= Init the Disasm structure (important !)*/
//    (void) memset (&MyDisasm, 0, sizeof(DISASM));
// 
//    /* ============================= Init EIP */
//    MyDisasm.EIP = (UIntPtr) &main;
// 
//    /* ============================= Loop for Disasm */
//    while (!Error){
//        /* ============================= Fix SecurityBlock */
//        MyDisasm.SecurityBlock = (UIntPtr)EndCodeSection - (UIntPtr)MyDisasm.EIP;
// 
//        len = Disasm(&MyDisasm);
//        if (len == OUT_OF_BLOCK) {
//            (void) printf("disasm engine is not allowed to read more memory \n");
//            Error = 1;
//        }
//        else if (len == UNKNOWN_OPCODE) {
//            (void) printf("unknown opcode");
//            Error = 1;
//        }
//        else {
//            (void) puts(MyDisasm.CompleteInstr);
//            MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)len;
//            if (MyDisasm.EIP >= EndCodeSection) {
//                (void) printf("End of buffer reached ! \n");
//                Error = 1;
//            }
//        }
//    };
//}
//
//void DisassembleCode(
//    char *StartCodeSection,
//    char *EndCodeSection,
//    int (*Virtual_Address)(void))
//{
//    /* ============================= Init the Disasm structure (important !)*/
//    (void) memset (&MyDisasm, 0, sizeof(DISASM));
// 
//    /* ============================= Init EIP */
//    MyDisasm.EIP = (int) StartCodeSection;
//    /* ============================= Init VirtualAddr */
//    MyDisasm.VirtualAddr = (long long) Virtual_Address;
// 
//    /* ============================= set IA-32 architecture */
//    MyDisasm.Archi = 0;
//    /* ============================= Loop for Disasm */
//    while (!Error){
//        /* ============================= Fix SecurityBlock */
//        MyDisasm.SecurityBlock = (int) EndCodeSection - MyDisasm.EIP;
// 
//        len = Disasm(&MyDisasm);
//        if (len == OUT_OF_BLOCK) {
//            (void) printf("disasm engine is not allowed to read more memory \n");
//            Error = 1;
//        }
//        else if (len == UNKNOWN_OPCODE) {
//            (void) printf("unknown opcode");
//            Error = 1;
//        }
//        else {
//                (void) printf("%.8X %s\n",(int) MyDisasm.VirtualAddr, 
//                                                             &MyDisasm.CompleteInstr);
//            MyDisasm.EIP = MyDisasm.EIP + len;
//            MyDisasm.VirtualAddr = MyDisasm.VirtualAddr + len;
//            if (MyDisasm.EIP >= (int) EndCodeSection) {
//                (void) printf("End of buffer reached ! \n");
//                Error = 1;
//            }
//        }
//    };
//    return;
//}