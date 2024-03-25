#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
* 〇課題17
* パラメータで渡されたサイズ分メモリを確保する
* プログラムを作成してください
* (パラメータsizeの値が格納するデータ型の
* 範囲外の値かチェックする必要はありません)
*/

#define MEGA 1048576
#define KILO 1024

int main(int argc, char* argv[])
{
    int param_length;
    int size = 0;
    char last_char;
    void *memory;
    int rep_num;

    if (argc < 2) {
        printf("usage: kadai17 size[M|K]\noptions: size[M | K]  サイズ(M = メガ, K = キロ)");
        return 0;
    }

    param_length = strlen(argv[1]);
    rep_num = param_length - 1;
    for(int i = 0; i < rep_num; i++) {
        size *= 10;
        char buff = argv[1][i];
        if (!isdigit(buff)) {
            printf("usage: kadai17 size[M|K]\noptions: size[M | K]  サイズ(M = メガ, K = キロ)");
            return 0;
        }
        size += buff - '0';
    }

    last_char = argv[1][param_length - 1];
    if(isdigit(last_char)) {
        size = size * 10 + (last_char - '0');
    } else if (last_char == 'M') {
        size *= MEGA;
    } else if (last_char == 'K') {
        size *= KILO;
    } else {
        printf("usage: kadai17 size[M|K]\noptions: size[M | K]  サイズ(M = メガ, K = キロ)");
        return 0;
    }

    memory = malloc(size);
    free(memory);

    return 0;
}
