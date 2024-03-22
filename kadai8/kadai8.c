#include<stdio.h>

/*
* 〇課題8
* プログラムを起動してから呼び出すたびに
* 1インクリメントした値を返す関数を作成してください。
* 初期値は0とします。
* (intの最大値を超えた場合の動作を考慮する必要はありません)
*/

static int static_counter();

int main(int argc, char *argv[])
{
    for(int i = 0; i < 10; i++) {
        int count = static_counter();
        printf("%d\n", count);
    }
    return 0;
}

/**
 * @brief 呼び出されるたびに1インクリメントされた値を返す
 * 
 * @return 呼び出された回数
*/
static int static_counter()
{
    static int count = 0;
    return ++count;
}
