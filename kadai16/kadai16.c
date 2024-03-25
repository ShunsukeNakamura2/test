#include <stdio.h>

/*
* 〇課題16
* 最下位ビットが立っているか調べる
* 関数マクロを作成してください。
*/

/* 最下位ビットが1なら「1」, 0なら「0」を返す */
#define BITON(i) ((i) & 1)

int main(int argc, char *argv[])
{
    printf("%d\n", BITON(11));
    return 0;
}
