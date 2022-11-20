#include "syscall.h"

int main() {
    char fileName[256];
    int size;
    int success;

    PrintString("nhap so luong ki tu co trong file name (co tinh .txt): ");
    size=ReadNum();
    PrintString("nhap ten file : ");
    ReadString(fileName, size);
    success=Remove(fileName);

    if(success==1) 
    PrintString("Xoa file thanh cong");
    else
    PrintString("Xoa file that bai");
    Halt();

}
