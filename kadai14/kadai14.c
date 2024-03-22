#include <stdio.h>

/**
 * 〇課題14
 * コマンドラインで指定されたテキストファイルの
 * 内容を表示するプログラムを作成してください。
 * (文字コード以外のコードが含まれていた場合
 * のことを考慮する必要はありません)
*/

#define MAX_LEN 100 /* テキストファイルの1行の最大文字数 */

int main(int argc, char *argv[])
{
    FILE *fp;
    int error;
    char str[MAX_LEN];

    if((error = fopen_s(&fp, argv[1], "r")) == 0) {
        while(fgets(str, MAX_LEN, fp) != NULL) {
            printf("%s", str);
        }
        fclose(fp);
    } else {
        printf("file open error\nerror code:%d", error);
    }

    return 0;
}
