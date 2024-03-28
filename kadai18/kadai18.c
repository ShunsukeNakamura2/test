#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

/*
* 〇課題18
* パラメータで渡されたファイルの16進ダンプを
* 表示するプログラムを作成してください。
*/

#define LINE_NUM 16                     /* 1行あたりLINE_NUMバイトずつ表示 */
#define OUTPUT_NUM (LINE_NUM * 3 + 1)   /* 1行あたりのデータ部の出力文字数 */
#define MSG_USAGE "usage: kadai18 filename\noptions: filename ファイル名\n"
#define MSG_FILE_ERROR "\nfile %s error\nerror:%s (code:%d)\n"
#define RETURN_NOMAL_END 0
#define RTURN_USAGE 1
#define RETURN_FAIL_OPEN 2
#define RETURN_FAIL_CLOSE 3
#define RETURN_FAIL_READ 4
#define RETURN_FAIL_READ_CLOSE 5

int main(int argc, char *argv[])
{
    int fd;
    int rc;
    int main_rc = RETURN_NOMAL_END;
    int line_count = 0;
    int read_size;
    unsigned char line[LINE_NUM];

    if(argc < 2) {
        printf(MSG_USAGE);
        return RTURN_USAGE;
    }
    
    fd = open(argv[1], O_RDONLY | O_BINARY);
    if(fd == -1) {
        printf(MSG_FILE_ERROR, "open", strerror(errno), errno);
        return RETURN_FAIL_OPEN;
    }
    
    printf("ADDRESS  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    while(1) {
        read_size = read(fd, line, sizeof(line));
        if(read_size == -1) {
            printf(MSG_FILE_ERROR, "read", strerror(errno), errno);
            main_rc = RETURN_FAIL_READ;
            break;
        } else if(read_size == 0) {
            break;
        }
        unsigned char output[OUTPUT_NUM] = {0};
        for(int i = 0; i < read_size; i++) {
            sprintf_s(&output[i * 3], 4, " %02X", line[i]);
        }

        printf("%07X0%s\n", line_count, output);
        
        if(read_size < LINE_NUM) {
            break;
        }
        line_count++;
    }
    
    rc = close(fd);
    if(rc == -1) {
        printf(MSG_FILE_ERROR, "close", strerror(errno), errno);
        if(main_rc == RETURN_FAIL_READ) {
            main_rc = RETURN_FAIL_READ_CLOSE;
        } else {
            main_rc = RETURN_FAIL_CLOSE;
        }
    }
    return main_rc;
}
