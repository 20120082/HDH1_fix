#include "syscall.h"
#define MAX_ARRAY_SIZE 100
#define SORT_ASCENDING 1
#define SORT_DESCENDING 2


int main() 
{
    int n, a[MAX_ARRAY_SIZE + 1], sortOrder, temp, i, j;
    do {
        PrintString("Enter the number n of integers to sort (0 <= n <= 100): ");
        n = ReadNum();
        if (n < 0 || n > 100)
            PrintString("Please enter n with 0 <= n <= 100:\n");
    } while (n < 0 || n > 100);

    do {
        PrintString("Enter sort order (1: ascending, 2: descending): ");
        sortOrder = ReadNum();
        if (sortOrder != 1 && sortOrder != 2)
            PrintString(" Please choose 1 or 2\n");
    } while (sortOrder != 1 && sortOrder != 2);

    for (i = 0; i < n; i++) {
        PrintString("Enter a[");
        PrintNum(i);
        PrintString("]: ");
        a[i] = ReadNum();
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n - 1; j++) {
            if (sortOrder == SORT_ASCENDING) {
                if (a[j] > a[j + 1]) {
                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                }
            } else if (sortOrder == SORT_DESCENDING) {
                if (a[j] < a[j + 1]) {
                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                }
            }
        }
    }

    PrintString("Sorted array: ");
    for (i = 0; i < n; i++) {
        PrintNum(a[i]);
        PrintChar(' ');
    }
}