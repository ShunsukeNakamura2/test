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
#define RETURN_USAGE 1
#define RETURN_FAIL_OPEN 2
#define RETURN_FAIL_CLOSE 3
#define RETURN_FAIL_READ 4
#define RETURN_FAIL_READ_CLOSE 5

int main(int argc, char *argv[])
{
    int fd = 0;
    int rc;
    char buff[BUFF_SIZE];
    ssize_t size;
    int main_rc = 0;

    if(argc < 2) {
        printf("usage:kadai14 filename");
        return RETURN_USAGE;
    }

    fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
        printf("file open error\nerror:%s (code:%d)\n", strerror(errno), errno);
        return RETURN_FAIL_OPEN;
    }
    
    while(1) {
        size = read(fd, buff, sizeof(buff) - 1);
        if(size == -1) {
            printf("\nfile read error\nerror:%s (code:%d)\n", strerror(errno), errno);
            main_rc = RETURN_FAIL_READ;
            break;
        } else if(size == 0) {
            break;
        }
        buff[size] = '\0';
        printf("%s", buff);
    }
    
    rc = close(fd);
    if(rc == -1) {
        printf("\nfile close error\nerror:%s (code:%d)\n", strerror(errno), errno);
        if(main_rc == RETURN_FAIL_READ) {
            main_rc = RETURN_FAIL_READ_CLOSE;
        } else {
            main_rc = RETURN_FAIL_CLOSE;
        }
    }
    return main_rc;
}
