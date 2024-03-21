#include<stdio.h>

/*
〇課題7
引数に指定した数分再起呼び出しする関数を作成し、
関数内で仮引数のアドレスを表示してください。
(スタックオーバーフローを回避する処理を実装する必要はありません)
*/

void reent(int n);

int main(int argc, char *argv[])
{
    int n;
    printf("再帰呼び出しする数を指定してください：");
    scanf_s("%d", &n);

    if(n > 0) {
        reent(n);
    }
    
    return 0;
}

void reent(int n)
{
    printf("n=%d アドレス=%#x\n", n, &n);
    if(n > 0) {
        reent(n - 1);
    }
}