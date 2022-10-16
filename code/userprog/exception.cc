// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

char* stringUser2System(int addr, int convert_length = -1) {
    int length = 0;
    bool stop = false;
    char* str;

    do {
        int oneChar;
        kernel->machine->ReadMem(addr + length, 1, &oneChar);
        length++;
        // if convert_length == -1, we use '\0' to terminate the process
        // otherwise, we use convert_length to terminate the process
        stop = ((oneChar == '\0' && convert_length == -1) ||
                length == convert_length);
    } while (!stop);

    str = new char[length];
    for (int i = 0; i < length; i++) {
        int oneChar;
        kernel->machine->ReadMem(addr + i, 1,
                                 &oneChar);  // copy characters to kernel space
        str[i] = (unsigned char)oneChar;
    }
    return str;
}

/**
 * Modify program counter
 * Duoc tim thay `../machine/mipssim.cc`, comment: Advance program counters
 **/
void IncreaseProgramCounter() {
    /* set previous programm counter (debugging only)
     * tuong ung: registers[PrevPCReg] = registers[PCReg];*/
    kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

    /* set programm counter to next instruction
     * tuong ung: registers[PCReg] = registers[NextPCReg]*/
    kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(NextPCReg));

    /* set next programm counter for brach execution
     * tuong ung: registers[NextPCReg] = pcAfter;*/
    kernel->machine->WriteRegister( NextPCReg, kernel->machine->ReadRegister(NextPCReg) + 4);
}



/**
 * @brief Convert system string to user string
 * 
 * @param str string to convert
 * @param addr addess of user string
 * @param convert_length set max length of string to convert, leave
 * blank to convert all characters of system string
 * @return void
 */
void StringSys2User(char* str, int addr, int convert_length = -1) {
    int length = (convert_length == -1 ? strlen(str) : convert_length);
    for (int i = 0; i < length; i++) {
        kernel->machine->WriteMem(addr + i, 1,
                                  str[i]);  // copy characters to user space
    }
    kernel->machine->WriteMem(addr + length, 1, '\0');
}

void handle_not_implemented_SC(int type) {
    DEBUG(dbgSys, "Not yet implemented syscall " << type << "\n");
    return IncreaseProgramCounter();
}

#define MAX_READ_STRING_LENGTH 255
void handle_SC_ReadString() {
    int memPtr = kernel->machine->ReadRegister(4);  // read address of C-string
    int length = kernel->machine->ReadRegister(5);  // read length of C-string
    if (length > MAX_READ_STRING_LENGTH) {  // avoid allocating large memory
        DEBUG(dbgSys, "String length exceeds " << MAX_READ_STRING_LENGTH);
        SysHalt();
    }
    char* buffer = SysReadString(length);
    StringSys2User(buffer, memPtr);
    delete[] buffer;
    return IncreaseProgramCounter();
}

void handle_SC_PrintString() {
    int memPtr = kernel->machine->ReadRegister(4);  // read address of C-string
    char* buffer = stringUser2System(memPtr);

    SysPrintString(buffer, strlen(buffer));
    delete[] buffer;
    return IncreaseProgramCounter();
}


void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which)
	{
		case NoException:  // return control to kernel
            		kernel->interrupt->setStatus(SystemMode);
            		DEBUG(dbgSys, "Switch to system mode\n");
            		break;
        	case PageFaultException:
        	case ReadOnlyException:
        	case BusErrorException:
        	case AddressErrorException:
        	case OverflowException:
        	case IllegalInstrException:
        	case NumExceptionTypes:
            		cerr << "Error " << which << " occurs\n";
            		SysHalt();
            		ASSERTNOTREACHED();

		case SyscallException:
		{
			switch(type) 
			{
				case SC_Halt:
				{
					DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
					SysHalt();
					ASSERTNOTREACHED();
				}
				break;

				case SC_Add: //ham add la tra ve int (int add())
				{
					DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
					/* Process SysAdd Systemcall*/
					int result_add;
					result_add = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */(int)kernel->machine->ReadRegister(5));

					DEBUG(dbgSys, "Add returning with " << result_add << "\n");
					/* Prepare Result */
					kernel->machine->WriteRegister(2, (int)result_add);
	
					return IncreaseProgramCounter();
				}

				case SC_ReadNum:
				{
					int result_readNum;
					result_readNum=	SysReadNum();
					DEBUG(dbgSys, "input: " << result_readNum << "\n");
					kernel->machine->WriteRegister(2, result_readNum);//thanh ghi 2 la noi luu tru gia tri cua gia tri tra ve cua ReadNum() o file ReadNum.c

					return IncreaseProgramCounter();
				}

				case SC_PrintNum:
				{
					int num;
					num =(int)kernel->machine->ReadRegister(4);//doc tham so truyen vao(value cac tham so truyen vao cua tat ca cac function se duoc luu vao thanh ghi 4,5,6...)
					PrintNum(num);

					return IncreaseProgramCounter();
				}

				case SC_RandomNum:
				{
					int randomNum = RandomNum();
					DEBUG(dbgSys, "Random: " << randomNum << "\n");
					kernel->machine->WriteRegister(2, randomNum);//thanh ghi 2 la noi luu tru gia tri cua gia tri tra ve cua RandomNum() o file ReadNum.c

					return IncreaseProgramCounter();
				}
	
				case SC_ReadChar:
				{
					char result_readChar;
					result_readChar = ReadChar();
					kernel->machine->WriteRegister(2, (int)result_readChar);

					return IncreaseProgramCounter();
				}

				case SC_PrintChar:
				{
					char char_printChar;
					char_printChar = (char) kernel->machine->ReadRegister(4);
					kernel->synchConsoleOut->PutChar(char_printChar);

					return IncreaseProgramCounter();
				}
				case SC_ReadString:
                    			return handle_SC_ReadString();
                		case SC_PrintString:
                   			 return handle_SC_PrintString();
	
				default:
					cerr << "Unexpected system call " << type << "\n";
				break;
			}
		}
		default:
			cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
    }
    ASSERTNOTREACHED();
}
