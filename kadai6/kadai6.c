#include<stdio.h>
#include<stddef.h>

/*
〇課題6
構造体「SAMPLE_t」のサイズ、各メンバのサイズの合計、
各メンバの先頭からのオフセットとサイズを表示してください。
*/

struct SAMPLE_t
{
    char m1;
    short m2;
    char m3;
    long m4;
    char m5;
    void *m6;
};

int main(int argc, char *argv[])
{
    struct SAMPLE_t sample;
    /*
    gccコンパイル時の文字化け対策で、造の前に\を入れている
    https://www.novell.com/ja-jp/support/kb/tids/jp/00006500_/00006574.htm
    */
    printf("構\造体のサイズ=%d メンバの合計サイズ=%d\n", sizeof(sample), sizeof(sample.m1) + sizeof(sample.m2)
                                                                     + sizeof(sample.m3) + sizeof(sample.m4)
                                                                     + sizeof(sample.m5) + sizeof(sample.m6));
    printf("char  m1オフセット=%d サイズ=%d\n", offsetof(struct SAMPLE_t, m1), sizeof(sample.m1));
    printf("short m2オフセット=%d サイズ=%d\n", offsetof(struct SAMPLE_t, m2), sizeof(sample.m2));
    printf("char  m3オフセット=%d サイズ=%d\n", offsetof(struct SAMPLE_t, m3), sizeof(sample.m3));
    printf("long  m4オフセット=%d サイズ=%d\n", offsetof(struct SAMPLE_t, m4), sizeof(sample.m4));
    printf("char  m5オフセット=%d サイズ=%d\n", offsetof(struct SAMPLE_t, m5), sizeof(sample.m5));
    printf("void *m6オフセット=%d サイズ=%d\n", offsetof(struct SAMPLE_t, m6), sizeof(sample.m6));
    return 0;
}