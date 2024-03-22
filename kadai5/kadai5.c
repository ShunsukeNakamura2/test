#include<stdio.h>

/*
* 〇課題5
* char,short,int,longの配列とポインタを定義し、
* それぞれ、配列の添字を1つ進めた場合と
* ポインタをインクリメントした場合に、
* アドレスが何バイト進むか表示してください。
*/
int main(int argc, char *argv[])
{
    char char_array[2];
    short short_array[2];
    int int_array[2];
    long long_array[2];
    char *char_pointer = char_array;
    short *short_pointer = short_array;
    int *int_pointer = int_array;
    long *long_pointer = long_array;

    printf("char 配列=%d short 配列=%d int 配列=%d long 配列=%d\n",
                            (long long)&char_array[1] - (long long)&char_array[0],
                            (long long)&short_array[1] - (long long)&short_array[0],
                            (long long)&int_array[1] - (long long)&int_array[0],
                            (long long)&long_array[1] - (long long)&long_array[0]);
    printf("charポインタ=%d shortポインタ=%d intポインタ=%d longポインタ=%d\n",
                            (long long)(char_pointer + 1) - (long long)char_pointer,
                            (long long)(short_pointer + 1) - (long long)short_pointer,
                            (long long)(int_pointer + 1) - (long long)int_pointer,
                            (long long)(long_pointer + 1) - (long long)long_pointer);
    return 0;
}
