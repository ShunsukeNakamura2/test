#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
* 〇課題19
* 指定したサイズのファイルを作成する
* プログラムを作成してください。
* (パラメータsizeの値が格納するデータ型の
* 範囲外の値かチェックする必要はありません)
*/

#define MEGA 1048576
#define KILO 1024
#define MSG_USAGE "usage: kadai19 filename size[M|K]\noptions: filename  ファイル名size[M|K]\n             サイズ(M=メガ,K=キロ)\n"
#define RETURN_USAGE 1
#define RETURN_FAIL_FILE_OPEN 2

int main(int argc, char *argv[])
{
    int param_length;
    int size = 0;
    char last_char;
    int rep_num;
    char *request_file_name = argv[1];
    char *request_size_str = argv[2];
    FILE *fp;
    int rc;

    if (argc < 3) {
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

    rc = fopen_s(&fp, request_file_name, "wb");
    if(rc != 0) {
        char *error_message = strerror(rc);
        printf("file open error\nerror:%s (code:%d)\n", error_message, rc);
        return RETURN_FAIL_FILE_OPEN;
    }

    for(int i = 0; i < size; i++) {
        fputc('\0', fp);
    }

    rc = fclose(fp);
     if(rc != 0) {
        char *error_message = strerror(rc);
        printf("file close error\nerror:%s (code:%d)\n", error_message, rc);
        return RETURN_FAIL_FILE_OPEN;
    }
    return 0;
}
