#include "syscall.h"

#define MODE_READWRITE 0
#define MODE_WRITE 1

#define stdin 0
#define stdout 1

int main()
{
    char str[100];//neu dung buffer* thi se in them error 7 occurs tu exception.cc
    int id = Open("hello.txt", 0);
    Read(str, 50, id);
    PrintString(str);
    PrintString("\n");
    Close(id);
    Halt();

}
