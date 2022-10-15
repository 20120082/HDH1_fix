/*  str.c
 *	Simple program to test whether the systemcall interface works.
 *	Just do a syscall that read a string and print it
 */

#include "syscall.h"

char a[256];

int main() {
    PrintString("String length: (<= 255): ");
    ReadString(a, ReadNum());
    PrintString("\n- The string entered is : ");
    PrintString(a);
    PrintString("\n");
    Halt();
}
