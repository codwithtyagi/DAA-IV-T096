#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// clear screen
void clrscr()
{
    system("clear");
}

// press enter to continue
void pause_scr()
{
    printf("\nPress Enter to continue...");
    getchar();
}

// print dashed line
void print_sep()
{
    printf("--------------------------------------------------\n");
}
