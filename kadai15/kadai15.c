#include <stdio.h>

/*
* 〇課題15
* 時分秒から秒数を計算する関数マクロを
* 作成してください。秒数は、
* 時×60×60+分×60＋秒で求めます。
*/

#define GETSEC(h, m, s) ((h) * 60 * 60 + (m) * 60 + (s))

int main(int argc, char *argv[])
{
    printf("%d\n", GETSEC(1, 2, 3));
    return 0;
}
