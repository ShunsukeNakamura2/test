#include<stdio.h>

/*
* 〇課題10
* int型の2つの変数の値を入れ替える
* 関数を作成してください。 
*/

static void swap_int(int *a, int *b);

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;

    printf("入れ替え前\na=%d b=%d\n", a, b);
    swap_int(&a, &b);
    printf("入れ替え後\na=%d b=%d\n", a, b);

    return 0;
}

/**
 * @brief 引数a,bの中身を入れ替える
 * 
 * @param[in/out] a 入れ替え対象その1
 *                b 入れ替え対象その2
*/
static void swap_int(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
