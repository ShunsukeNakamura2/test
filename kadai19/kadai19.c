#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
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
#define MSG_FILE_ERROR "file %s error\nerror:%s (code:%d)\n"
#define RETURN_NOMAL_END 0
#define RETURN_USAGE 1
#define RETURN_FAIL_OPEN 2
#define RETURN_FAIL_CLOSE 3
#define RETURN_FAIL_SEEK 4
#define RETURN_FAIL_WRITE 5
#define RETURN_FAIL_SEEK_CLOSE 6
#define RETURN_FAIL_WRITE_CLOSE 7

int main(int argc, char *argv[])
{
    int param_length;
    int size = 0;
    char last_char;
    int rep_num;
    char *request_file_name;
    char *request_size_str;
    int fd;
    int rc;
    int main_rc = RETURN_NOMAL_END;
    

    if (argc < 3) {
        printf(MSG_USAGE);
        return RETURN_USAGE;
    }
    request_file_name = argv[1];
    request_size_str = argv[2];

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

    fd = open(request_file_name, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0644);
    if(fd == -1) {
        printf(MSG_FILE_ERROR, "open", strerror(errno), errno);
        return RETURN_FAIL_OPEN;
    }

    rc = lseek(fd, size - 1, SEEK_SET);
    if(rc == -1) {
        printf(MSG_FILE_ERROR, "seek", strerror(errno), errno);
        main_rc = RETURN_FAIL_SEEK;
    } else {
        rc = write(fd, "\0", 1);
        if(rc == -1) {
            printf(MSG_FILE_ERROR, "write", strerror(errno), errno);
            main_rc = RETURN_FAIL_WRITE;
        }
    }
    
    rc = close(fd);
    if(rc == -1) {
        printf(MSG_FILE_ERROR, "close", strerror(errno), errno);
        if(main_rc == RETURN_FAIL_SEEK) {
            main_rc = RETURN_FAIL_SEEK_CLOSE;
        } else if(main_rc == RETURN_FAIL_WRITE) {
            main_rc = RETURN_FAIL_WRITE_CLOSE;
        } else {
            main_rc = RETURN_FAIL_CLOSE;
        }
    }
    return main_rc;
}
