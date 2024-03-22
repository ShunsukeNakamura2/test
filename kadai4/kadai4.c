#include<stdio.h>

/*
* 〇課題4
* intのポインタ変数pにint型の変数iのアドレスをセットし、
* iのアドレスと値、pのアドレスと値(=iのアドレス)と
* 指す先の値(=iの値)を表示してください。
*/
int main(int argc, char *argv[])
{
    int i = 20;
    int *p = &i;
    printf("i:アドレス=%#x 値=%d\n", &i, i);
    printf("p:アドレス=%#x 値=%#x 指す先の値=%d\n", &p, p, *p);
    return 0;
}