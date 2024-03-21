#include<stdio.h>

/*
Z‰Û‘è10
intŒ^‚Ì2‚Â‚Ì•Ï”‚Ì’l‚ğ“ü‚ê‘Ö‚¦‚éŠÖ”‚ğ
ì¬‚µ‚Ä‚­‚¾‚³‚¢B
*/

void swap_int(int *a, int *b);

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;

    printf("“ü‚ê‘Ö‚¦‘O\na=%d b=%d\n", a, b);
    swap_int(&a, &b);
    printf("“ü‚ê‘Ö‚¦Œã\na=%d b=%d\n", a, b);

    return 0;
}

void swap_int(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}