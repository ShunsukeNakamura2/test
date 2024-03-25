#include <stdio.h>

/*
 * 〇課題14
 * コマンドラインで指定されたテキストファイルの
 * 内容を表示するプログラムを作成してください。
 * (文字コード以外のコードが含まれていた場合
 * のことを考慮する必要はありません)
*/

int main(int argc, char *argv[])
{
    FILE *fp;
    int fopen_rc;
    int fclose_rc;
    int buff;

    if(argc < 2) {
        printf("usage:kadai14 filename");
        return 0;
    }

    fopen_rc = fopen_s(&fp, argv[1], "r");
    if(fopen_rc != 0) {
        printf("file open error\nerror code:%d\n", fopen_rc);
        return 0;
    }
    
    while(1) {
        buff = fgetc(fp);
        if(buff == EOF) {
            break;
        }
        printf("%c", buff);
    }
    
    fclose_rc = fclose(fp);
    if(fclose_rc != 0) {
        printf("\nfile close error\nerror code:%d\n", fclose_rc);
    }
    return 0;
}
