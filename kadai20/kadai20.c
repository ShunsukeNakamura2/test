#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
* 〇課題20
* 指定した数値を指定した形式でファイルに追記する
* プログラムを作成してください。
* (パラメータnumberの値が格納するデータ型の
* 範囲外の値かチェックする必要はありません)
*/

#define MSG_USAGE "usage: プログラム名 -n number -t type -f filename\noptions: -n number  数値\n             -t type  B2|B4|F4\n             B2  バイナリ2バイト\n             B4  バイナリ4バイト\n             F4  浮動小数 4バイト\n             -f filename  ファイル名\n"
#define MSG_FILE_ERROR "\nfile %s error\nerror:%s (code:%d)\n"
#define RETURN_NOMAL_END 0
#define RETURN_USAGE 1
#define RETURN_FAIL_OPEN 2
#define RETURN_FAIL_CLOSE 3
#define RETURN_FAIL_WRITE 4
#define RETURN_FAIL_WRITE_CLOSE 5
#define TYPE_B2 0
#define TYPE_B4 1
#define TYPE_F4 2

int main(int argc, char *argv[])
{
    int main_rc = RETURN_NOMAL_END;
    int rc;
    int pos_n = -1; /* パラメータ-nの引数上の位置 */
    int pos_t = -1; /* パラメータ-tの引数上の位置 */
    int pos_f = -1; /* パラメータ-fの引数上の位置 */
    short param_number_B2;
    int   param_number_B4;
    float param_number_F4;
    char *param_type_str;
    int   param_type;
    char *param_filename;
    int   rep_num;
    char *endptr;
    int   fd;

    if(argc != 7) {
        printf(MSG_USAGE);
        return RETURN_USAGE;
    }

    /* オプションの位置を取得 */
    rep_num = argc - 1;
    for(int i = 1; i < rep_num; i++) {
        if(strcmp(argv[i], "-n") == 0) {
            pos_n = i;
        } else if(strcmp(argv[i], "-t") == 0) {
            pos_t = i;
        } else if(strcmp(argv[i], "-f") == 0) {
            pos_f = i;
        }
    }

    if(pos_n == -1 || pos_t == -1 || pos_f == -1) {
        printf(MSG_USAGE);
        return RETURN_USAGE;
    }

    /* パラメータtypeを取得 */
    param_type_str = argv[pos_t + 1];
    if(strcmp(param_type_str, "B2") == 0) {
        param_type = TYPE_B2;
    } else if(strcmp(param_type_str, "B4") == 0) {
        param_type = TYPE_B4;
    } else if(strcmp(param_type_str, "F4") == 0) {
        param_type = TYPE_F4;
    } else {
        printf(MSG_USAGE);
        return RETURN_USAGE;
    }

    /* パラメータnumberを取得 */
    endptr = NULL;
    switch (param_type)
    {
    case TYPE_B2:
        param_number_B2 = (short)strtol(argv[pos_n + 1], &endptr, 10);
        break;
    case TYPE_B4:
        param_number_B4 = (int)strtol(argv[pos_n + 1], &endptr, 10);
        break;
    case TYPE_F4:
        param_number_F4 = (float)strtod(argv[pos_n + 1], &endptr);
        break;
    default:
        break;
    }

    if (*endptr != '\0') {
        printf(MSG_USAGE);
        return RETURN_USAGE;
    }

    param_filename = argv[pos_f + 1];
    fd = open(param_filename, O_WRONLY | O_CREAT | O_APPEND | O_BINARY,
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd == -1) {
        printf(MSG_FILE_ERROR, "open", strerror(errno), errno);
        return RETURN_FAIL_OPEN;
    }

    switch (param_type)
    {
    case TYPE_B2:
        rc = write(fd, &param_number_B2, sizeof(param_number_B2));
        break;
    case TYPE_B4:
        rc = write(fd, &param_number_B4, sizeof(param_number_B4));
        break;
    case TYPE_F4:
        rc = write(fd, &param_number_F4, sizeof(param_number_F4));
        break;
    default:
        break;
    }
    if(rc == -1) {
        printf(MSG_FILE_ERROR, "write", strerror(errno), errno);
        main_rc = RETURN_FAIL_OPEN;
    }

    rc = close(fd);
    if(rc == -1) {
        printf(MSG_FILE_ERROR, "close", strerror(errno), errno);
        if(main_rc == RETURN_FAIL_OPEN) {
            main_rc = RETURN_FAIL_WRITE_CLOSE;
        } else {
            main_rc = RETURN_FAIL_CLOSE;
        }
    }
    
    return main_rc;
}
