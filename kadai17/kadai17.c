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
#define MSG_USAGE "usage: kadai17 size[M|K]\noptions: size[M | K]  サイズ(M = メガ, K = キロ)\n"
#define MSG_FAIL_MALLOC "Failed to allocate memory\n"
#define RETURN_USAGE 1
#define RETURN_FAIL_MALLOC 2

int main(int argc, char* argv[])
{
    int param_length;
    int size = 0;
    char last_char;
    void *memory;
    int rep_num;
    char *request_size_str = argv[1];

    if (argc < 2) {
        printf(MSG_USAGE);
        return RETURN_USAGE;
    }

    param_length = strlen(request_size_str);
    /* 末尾を除く文字を処理 */
    rep_num = param_length - 1;
    for(int i = 0; i < rep_num; i++) {
        size *= 10;
        char buff = request_size_str[i];
        if (!isdigit(buff)) {
            printf(MSG_USAGE);
            return RETURN_USAGE;
        }
        size += buff - '0';
    }

    /* 末尾の文字を処理 */
    last_char = request_size_str[param_length - 1];
    if(isdigit(last_char)) {
        size = size * 10 + (last_char - '0');
    } else if (last_char == 'M') {
        size *= MEGA;
    } else if (last_char == 'K') {
        size *= KILO;
    } else {
        printf(MSG_USAGE);
        return RETURN_USAGE;
    }

    printf("size:%d\n", size);
    memory = malloc(size);
    if(memory == NULL) {
        printf(MSG_FAIL_MALLOC);
        return RETURN_FAIL_MALLOC;
    }
    free(memory);

    return 0;
}
