#include <stdio.h>

/*
* 〇課題18
* パラメータで渡されたファイルの16進ダンプを
* 表示するプログラムを作成してください。
*/

int main(int argc, char *argv[])
{
    FILE *fp;
    int error;

    error = fopen_s(&fp, argv[1], "rb");    
    if(error != 0) {
        printf("file open error\nerror code:%d", error);
        return 0;
    }
    
    printf("ADDRESS  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");

    fclose(fp);
    return 0;
}
