#include "syscall.h"

#define MODE_READWRITE 0


int main()
{
    char a[256];
    char b[256];
    char space;
    int size;
    char str[100];
    int id;
    int len;

    PrintString("nhap so luong ki tu co trong file name 1(co tinh .txt): ");
    size=ReadNum();
    PrintString("nhap ten file 1: ");
    ReadString(a, size);
    ReadChar(space);
    PrintString("nhap so luong ki tu co trong file name 2(co tinh .txt): ");
    size=ReadNum();
    PrintString("nhap ten file 2: ");
    ReadString(b, size);
    ReadChar(space);


    
    id = Open(a, MODE_READWRITE);
    Read(str, 50, id);
    len=0;
    while (str[len] != '\0') ++len;
    Close(id);


    id = Open(b, MODE_READWRITE);
    Write(str, len, id);
    Close(id);
    Halt();

}
