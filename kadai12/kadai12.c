#include<stdio.h>

/*
* 〇課題12
* 指定された領域を16進ダンプし表示する
* 関数を作成してください。
*/

static void dump(const void *p, size_t s);

int main(int argc, char *argv[])
{
    unsigned int a = 4294967296 - 2;
    dump(&a, sizeof(a));
    return 0;
}

/**
 * @brief 指定された領域を16進ダンプして表示
 * 
 * @param[in] p ダンプ対象
 * @param[in] s ダンプ対象のサイズ
*/
static void dump(const void *p, size_t s)
{
    unsigned char *data = (unsigned char *)p;
    for(int i = 0; i < s; i++){
        printf("%02X ", data[i]);
    }
}
