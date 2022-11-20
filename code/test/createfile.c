#include "syscall.h"

int main() {
    char* fileName ="hello.txt";


    if (Create(fileName) == 0) {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" da tao thanh cong\n");
    } else
        PrintString("Tao file that bai!\n");
    Halt();
}
