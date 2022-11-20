#include "syscall.h"

#define MODE_READWRITE 0


int main()
{
    char str[100];
    int id = Open("hello.txt", MODE_READWRITE);
    Read(str, 50, id);
    PrintString(str);
    PrintString("\n");
    Close(id);
    Halt();

}
