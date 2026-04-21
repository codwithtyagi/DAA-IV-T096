#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// clear screen
void clrscr()
{
    system("clear");
}

// press enter to continue
void pauseScr()
{
    printf("\nPress Enter to continue...");
    getchar();
}

// print dashed line
void printSep()
{
    printf("--------------------------------------------------\n");
}
