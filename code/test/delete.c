#include "syscall.h"

int main() {
    char fileName[256];
    int size;

    PrintString("nhap so luong ki tu co trong file name (co tinh .txt): ");
    size=ReadNum();
    PrintString("nhap ten file : ");
    ReadString(fileName, size);
    Remove(fileName);

}
