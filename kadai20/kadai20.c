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
#define RETURN_NORMAL_END 0
#define RETURN_USAGE 1
#define RETURN_FAIL_OPEN 2
#define RETURN_FAIL_CLOSE 3
#define RETURN_FAIL_WRITE 4
#define ARGC 7

typedef struct st_args {
    void *number;
    char *type;
    char *filename;
    int size_of_type;
} st_args;

static int analyze_args(int argc, char *argv[], st_args *args_val);

int main(int argc, char *argv[])
{
    int main_rc = RETURN_NORMAL_END;
    int rc;
    int fd;
    st_args params = {NULL, NULL, NULL, 0};
    
    rc = analyze_args(argc, argv, &params);
    if(rc == -1) {
        printf(MSG_USAGE);
        return RETURN_USAGE;
    }

    fd = open(params.filename, O_WRONLY | O_CREAT | O_APPEND | O_BINARY,
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd == -1) {
        printf(MSG_FILE_ERROR, "open", strerror(errno), errno);
        return RETURN_FAIL_OPEN;
    }

    rc = write(fd, params.number, params.size_of_type);
    if(rc == -1) {
        printf(MSG_FILE_ERROR, "write", strerror(errno), errno);
        main_rc = RETURN_FAIL_WRITE;
        goto end;
    }

end:
    rc = close(fd);
    if(rc == -1) {
        printf(MSG_FILE_ERROR, "close", strerror(errno), errno);
        if(main_rc == RETURN_NORMAL_END) {
            main_rc = RETURN_FAIL_CLOSE;
        }
    }
    
    return main_rc;
}

/**
 * @brief コマンドライン引数をチェックしパラメータを構造体st_argsに入れる
 * 
 * @param[in] argc コマンドライン引数の個数
 * @param[in] argv コマンドライン引数
 * @param[in,out] args_val パラメータ格納用構造体
 * 
 * @retval 0   正常終了
 * @retval -1  コマンドライン引数不正
*/
static int analyze_args(int argc, char *argv[], st_args *args_val)
{
    char *param_number_str = NULL;
    short param_number_B2;
    int   param_number_B4;
    float param_number_F4;
    char *endptr = NULL;

    if(argc != ARGC) {
        return -1;
    }

    args_val->type = NULL;
    args_val->filename = NULL;

    /* オプションとパラメータを設定 */
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-n") == 0) {
            /* numberを取得 */
            if(param_number_str != NULL) {
                return -1;
            }
            if(++i == argc) {
                return -1;
            }
            param_number_str = argv[i];
        } else if(strcmp(argv[i], "-t") == 0) {
            /* typeを取得 */
            if(args_val->type != NULL) {
                return -1;
            }
            if(++i == argc) {
                return -1;
            }
            args_val->type = argv[i];
        } else if(strcmp(argv[i], "-f") == 0) {
            /* filenameを取得 */
            if(args_val->filename != NULL) {
                return -1;
            }
            if(++i == argc) {
                return -1;
            }
            args_val->filename = argv[i];
        }
    }

    /* オプションに不足がある場合失敗 */
    if(param_number_str == NULL || args_val->type == NULL || args_val->filename == NULL) {
        return -1;
    }

    /* 対応したtypeの数値を格納 */
    if(strcmp(args_val->type, "B2") == 0) {
        param_number_B2 = (short)strtol(param_number_str, &endptr, 10);
        args_val->number = &param_number_B2;
        args_val->size_of_type = sizeof(param_number_B2);
    } else if(strcmp(args_val->type, "B4") == 0) {
        param_number_B4 = (int)strtol(param_number_str, &endptr, 10);
        args_val->number = &param_number_B4;
        args_val->size_of_type = sizeof(param_number_B4);
    } else if(strcmp(args_val->type, "F4") == 0) {
        param_number_F4 = (float)strtod(param_number_str, &endptr);
        args_val->number = &param_number_F4;
        args_val->size_of_type = sizeof(param_number_F4);
    } else {
        return -1;
    }
    /* 文字列 -> 数値変換失敗 */
    if (*endptr != '\0') {
        return -1;
    }
    return 0;
}
