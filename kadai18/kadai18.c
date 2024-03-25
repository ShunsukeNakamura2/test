#include <stdio.h>

/*
* 〇課題18
* パラメータで渡されたファイルの16進ダンプを
* 表示するプログラムを作成してください。
*/

#define LINE_NUM 16                     /* 1行あたりLINE_NUMバイトずつ表示 */
#define OUTPUT_NUM (LINE_NUM * 3 + 1)   /* 1行あたりのデータ部の出力文字数 */

int main(int argc, char *argv[])
{
    FILE *fp;
    int error;
    int line_count = 0;
    int read_size;
    char line[LINE_NUM];

    error = fopen_s(&fp, argv[1], "rb");    
    if(error != 0) {
        printf("file open error\nerror code:%d", error);
        return 0;
    }
    
    printf("ADDRESS  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    while((read_size = fread(line, 1, LINE_NUM, fp)) != 0) {
        char output[OUTPUT_NUM];

        for(int i = 0; i < read_size; i++) {
            sprintf_s(&output[i * 3], 4, " %02X", line[i]);
        }

        printf("%06x00%s\n", line_count, output);
        
        if(read_size < LINE_NUM) {
            break;
        }
        line_count++;
    }
    
    fclose(fp);
    return 0;
}
