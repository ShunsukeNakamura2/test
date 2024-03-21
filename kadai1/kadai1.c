#include<stdio.h>

/*
 * 〇課題1
 * int型変数に任意の値をセットし、
 * その値を10進数,16進数,8進数で
 * 表示してください。 
 * (入力処理を実装する必要はありません。)
*/
int main()
{
    int num = 20;
    printf("10進数=%d 16進数=%#x 8進数=%#o\n", num, num, num);
    return 0;
}
