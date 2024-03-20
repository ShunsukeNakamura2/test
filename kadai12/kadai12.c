#include<stdio.h>

/**
 * 〇課題12
 * 指定された領域を16進ダンプし表示する
 * 関数を作成してください。
*/

void dump(const void *p, size_t s);

int main()
{
    unsigned int a = 4294967296 - 2;
    dump(&a, sizeof(a));
    return 0;
}

void dump(const void *p, size_t s)
{
    unsigned char *a = (unsigned char *)p;
    for(int i = 0; i < s; i++){
        printf("%02X ", a[i]);
    }
}