#include "syscall.h"

int main()
{
    int i;

    for(i=33;i<=126;i++)
    {
    PrintNum(i);
    PrintChar(':');
    PrintChar(i);
    PrintChar('\n');
    }

    Halt();
}