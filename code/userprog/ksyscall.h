/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"
#include <stdint.h>
#include <stdlib.h>

#define LF ((char)10)
#define CR ((char)13)
#define TAB ((char)9)
#define SPACE ((char)' ')
#define MAX_NUM_LENGTH 11

char _numberBuffer[MAX_NUM_LENGTH + 2];//cai 1 neu du va cai 2 luu vi tri blank ket thuc so


char isBlank(char c) { return c == LF || c == CR || c == TAB || c == SPACE; }

/**
 *Doc cho den khi vi tri rong
 **/
void readUntilBlank() {
    memset(_numberBuffer, 0, sizeof(_numberBuffer));
    char c = kernel->synchConsoleIn->GetChar();


    if (isBlank(c)) {
        DEBUG(dbgSys, "Unexpected white-space ");
        return;
    }

    int n = 0;

    while (!(isBlank(c))) {
        _numberBuffer[n++] = c;
        if (n > MAX_NUM_LENGTH) {
            DEBUG(dbgSys, "Number is too long");
            return;
        }
        c = kernel->synchConsoleIn->GetChar();
    }
}

/**
 *Tra ve True khi gia tri int bang voi so duoc bieu hien o string
 **/
bool compareNumAndString(int integer, const char *s) {
    if (integer == 0) return strcmp(s, "0") == 0;

    int len = strlen(s);

    if (integer < 0 && s[0] != '-') return false;

    if (integer < 0) s++, --len, integer = -integer;//tu s[0] sang s[1]

    while (integer > 0) {
        int digit = integer % 10;

        if (s[len - 1] - '0' != digit) return false;//kiem so so cuoi cung cua s viec tru 0 la de tu kieu char sang int ascii

        --len;
        integer /= 10;
    }

    return len == 0;
}



void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}


int SysReadNum() {
    readUntilBlank();

    int len = strlen(_numberBuffer);
    
    if (len == 0) return 0;// xet truong hop so 0

    
    if (strcmp(_numberBuffer, "-2147483648") == 0) return INT32_MIN;

    bool nega = (_numberBuffer[0] == '-');//kiểm tra ký tự đầu tiên có phải là dấu âm hay ko
    int zeros = 0;
    bool is_leading = true;
    int num = 0;
    for (int i = nega; i < len; ++i) {
        char c = _numberBuffer[i];
        if (c == '0' && is_leading)//xet so 0 dung dau !!
            ++zeros;
        else
            is_leading = false;
        if (c < '0' || c > '9') {
            DEBUG(dbgSys, "Khong ton tai so " << _numberBuffer);
            return 0;
        }
        num = num * 10 + (c - '0');//c la 1 ky tu '0' cung la 1 ky tu tuong ung 30 (int)
    }

    // 00            01 or -0
    if (zeros > 1 || (zeros && (num || nega))) {
        DEBUG(dbgSys, "Expected number but " << _numberBuffer << " found");
        return 0;
    }

    if (nega)
        num = -num;

    
    if (len <= MAX_NUM_LENGTH - 2) return num;//neu co 9 chu so tro xuong thi chac chan la num da dung

    
    if (compareNumAndString(num, _numberBuffer))//neu ta cho 3 000 000 000 thi num chi luu mot gia tri khac nen ko bao loi tran bo nho the nen ta can kiem tra xem num co that bang
        return num;
    else
        DEBUG(dbgSys,
              "khong ton tai so int32 la so " << _numberBuffer );

    return 0;
}

void PrintNum(int number)// cai PrintNum nay khac PrintNum o syscall.h (syscall.h chi don gian la thu vien cho .c thoi) con cai PrintNum nay la SystemPrintNum dung cho cai exception.cc
{
    bool negative=false;
    int size_num=0;
    int character=number;
    if (character == INT32_MIN) 
    {
        char* a="-2147483648";
        for(int i=0;i<11;i++)
        kernel->synchConsoleOut->PutChar(a[i]);
        return;
    }
    if (number<0) 
    {
        negative=true;
        character=-character;
    }
    for(int i=character;i!=0;i=i/10)
    size_num++;//tim so chu so trong so nguyen

    char *c=(char*) malloc(size_num);
    for(int i=size_num-1;i>=0;i--)
    {
        c[i]=character%10 +'0';//vi character day la dang int sang ascii. Co the thay '0' thanh 48
        character=character/10;
    }
    if (negative) kernel->synchConsoleOut->PutChar('-');


    for(int i=0;i<size_num;i++)
    {
        kernel->synchConsoleOut->PutChar(c[i]);
    }
    free(c);
    if (number==0) kernel->synchConsoleOut->PutChar('0');



}

int RandomNum()
{
    srand((unsigned int)time(NULL));
    int x;
    x=rand();
    return x;
}



char ReadChar()
{
    return kernel->synchConsoleIn->GetChar();
}
void PrintChar(char x)
{
    kernel->synchConsoleOut->PutChar(x);
}
char* SysReadString(int length) {
    char* buffer = new char[length + 1];
    for (int i = 0; i < length; i++) {
        buffer[i] = ReadChar();
    }
    buffer[length] = '\0';
    return buffer;
}

void SysPrintString(char* buffer, int length) {
    for (int i = 0; i < length; i++) {
        kernel->synchConsoleOut->PutChar(buffer[i]);
    }
}

int SysCreate(char* fileName) {
    int success;
    int length = strlen(fileName);

    if (length == 0) {//Ten file khong duoc de trong
        success = -1;

    } else if (fileName == NULL) {// bo nho khong du de cap ten file
        success = -1;
    } else {
        if (!kernel->fileSystem->Create(fileName)) {//luc bien dich thi duoc vi luc bien dich su dung filesys cua linux chu khong phai nachos bao loi do su dung filesys cua nachos
        //va luc chay vi dung linux nen no van lay filesys_stub (linux!)
            success = -1;
        } else {
            success = 0;
        }
    }

    return success;
}

int SysOpen(char* fileName, int type) {
    if (type != 0 && type != 1) return -1;

    int id = kernel->fileSystem->Open(fileName, type);
    if (id == -1) return -1;
    return id;
}

int SysClose(int id) { return kernel->fileSystem->Close(id); }

int SysRead(char *buffer, int size, int id)
{
    if(id==1) return kernel->synchConsoleOut->PutString(buffer,size);//console out
    return kernel->fileSystem->Read(buffer, size, id);
}

int SysWrite(char *buffer, int size, int id)
{
    if(id==0) return kernel->synchConsoleIn->GetString(buffer,size);//console in
    return kernel->fileSystem->Write(buffer, size, id);
}

int SysSeek(int pos,int id)
{
    return kernel->fileSystem->Seek(pos,id);
}

int SysRemove(char *filename)
{
    return kernel->fileSystem->Remove(filename);
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
