#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
* 〇課題17
* パラメータで渡されたサイズ分メモリを確保する
* プログラムを作成してください
* (パラメータsizeの値が格納するデータ型の
* 範囲外の値かチェックする必要はありません)
*/

#define MEGA 1048576;
#define KILO 1024;

int main(int argc, char *argv[])
{
    int param_length;
    int size;
    char *size_char;
    char prefix;
    void *memory;
    
    param_length = strlen(argv[1]);
    prefix = argv[1][param_length - 1];

    size_char = malloc(param_length);
    strncpy(size_char, argv[1], param_length - 1);
    size = atoi(size_char);
    free(size_char);

    if(prefix == 'M') {
        size *= MEGA;
    } else if(prefix == 'K') {
        size *= KILO;
    }

    memory = malloc(size);

    return 0;
}
