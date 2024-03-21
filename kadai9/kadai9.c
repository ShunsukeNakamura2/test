#include<stdio.h>

/*
〇課題9
コマンドラインで渡されたパラメータを
全て表示するプログラムを作成してください。
*/

int main(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++) {
        printf("%d : %s\n", i, argv[i]);
    }
    return 0;
}