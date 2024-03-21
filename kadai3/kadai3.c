#include <stdio.h>
#include <string.h>

/*
〇課題3
任意の文字列をcharの配列に読み込み、その配列のサイズ(バイト数)
と文字列長を表示してください。
またその文字列の最初の1文字と最後の1文字を表示してください。
*/
int main(int argc, char *argv[])
{
    char str[] = "ShunsukeNakamura";
    printf("配列サイズ=%d 文字列長=%d\n", sizeof(str), strlen(str));
    printf("1文字目は%c、最後の文字は%cです。\n", str[0], str[strlen(str) - 1]);
    return 0;
}