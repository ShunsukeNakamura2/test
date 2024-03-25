#include<stdio.h>
#include<string.h>

/*
* 〇課題11
* 文字列を渡すと逆順にして返す関数を
* 作成してください。
* (漢字等の複数バイト文字を考慮する
* 必要はありません)
*/

static void rev_str(char *str);

int main(int argc, char *argv[])
{
    char str[] = "Shunsuke Nakamura test";

    printf("順:%s\n", str);
    rev_str(str);
    printf("逆:%s\n", str);

    return 0;
}

/**
 * @brief 引数の文字列strを逆順にして返す
 * 
 * @param[in/out] str 逆順にする文字列
*/
static void rev_str(char *str)
{
    int length = strlen(str);
    int rep_num = length / 2;
    for(int i = 0; i < rep_num; i++) {
        char tmp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = tmp;
    }
}
