#include "syscall.h"

int main() {
    // information team
    PrintString("\t           _.oO OUR TEAM Oo._         \n");
    PrintString("\t.=====================================.\n");
    PrintString("\t||     ID\t||       Name\t     ||\n");
    PrintString("\t||==============||===================||\n");
    PrintString("\t||  20120082\t|| Nguyen Thanh Hieu ||\n");
    PrintString("\t||==============||===================||\n");
    PrintString("\t||  20120438\t||    Dao Van Canh   ||\n");
    PrintString("\t'====================================='\n");
    PrintString("\n>ASCII: at directory code, run build.linux/nachos -x test/ascii to print the ASCII table\n");
    PrintString(">Sort: \tat directory code, run build.linux/nachos -x test/bubblesort to start the sort program\n");
    PrintString("\t- Enter n (the length of the array, 0 <= n <= 100)\n");
    PrintString("\t- Enter n elements of the array\n");
    PrintString("\t- Enter the order you want to sort the array with (1: increasing, 2: decreasing)\n");
    PrintString("\t- The program will print out the sorted array and then exit\n");
    Halt();
}
