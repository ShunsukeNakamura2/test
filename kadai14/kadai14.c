#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

/*
 * 〇課題14
 * コマンドラインで指定されたテキストファイルの
 * 内容を表示するプログラムを作成してください。
 * (文字コード以外のコードが含まれていた場合
 * のことを考慮する必要はありません)
*/

#define BUFF_SIZE 256

int main(int argc, char *argv[])
{
    int file = 0;
    int rc;
    char buff[BUFF_SIZE];
    ssize_t size;

    if(argc < 2) {
        printf("usage:kadai14 filename");
        return 0;
    }

    file = open(argv[1], O_RDONLY);
    if(file == -1) {
        char *error_message = strerror(errno);
        printf("file open error\nerror:%s (code:%d)\n", error_message, errno);
        return 0;
    }
    
    while(1) {
        size = read(file, buff, sizeof(buff) - 1);
        if(size == -1) {
            char *error_message = strerror(errno);
            printf("\nfile read error\nerror:%s (code:%d)\n", error_message, errno);
            break;
        }
        buff[size] = '\0';
        printf("%s", buff);
        if(size == 0) {
            break;
        }
    }
    
    rc = close(file);
    if(rc == -1) {
        char *error_message = strerror(errno);
        printf("\nfile close error\nerror:%s (code:%d)\n", error_message, errno);
    }
    return 0;
}
