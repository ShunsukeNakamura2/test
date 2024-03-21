#include<stdio.h>

/**
 * 〇課題10
 * int型の2つの変数の値を入れ替える関数を
 * 作成してください。
*/

void swap_int(int *a, int *b);

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;

    printf("入れ替え前\na=%d b=%d\n", a, b);
    swap_int(&a, &b);
    printf("入れ替え後\na=%d b=%d\n", a, b);

    return 0;
}

void swap_int(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}