#include "syscall.h"

#define MODE_READWRITE 0
#define MODE_READ 1

#define stdin 0
#define stdout 1

int main()
{
    char a[256];
    char b[256];
    char space;
    int size;
    char str[100];
    int id;
    int len;
    //trong c cac bien phai khoi tao xong roi moi duoc thao tac (khi bat dau dung den ham thi khong duoc tao bien!!)
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


    
    id = Open(a, 0);
    Read(str, 50, id);
    len=0;
    while (str[len] != '\0') ++len;
    Close(id);


    id = Open(b, 0);
    Write(str, len, id);
    Close(id);
    Halt();

}
