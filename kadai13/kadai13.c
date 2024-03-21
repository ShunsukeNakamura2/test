#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
〇課題13
構造体DAT_tの配列をソートし表示してください。
ソート順は、nの値で昇順、nが同一の値の場合は
sの文字コードで昇順とします。
ソート処理にはC標準関数qsort()を使ってください。
*/

typedef struct DAT_t {
    int n;
    const char *s;
}DAT;

int compare_dat(const void *dat1, const void *dat2);

int main(int argc, char *argv[])
{
    DAT dat[] = {
        {9, "A"},
        {2, "A"},
        {10, "testB"},
        {5, "AB"},
        {10, "testA"},
        {5, "AA"}
    };
    int dat_length = sizeof(dat) / sizeof(DAT);

    /*
    gccコンパイル時の文字化け対策でソの後に\を入れている
    */
    printf("ソ\ート前\n");
    for(int i = 0; i < dat_length; i++) {
        printf("dat[%d]:%d,%s\n", i, dat[i].n, dat[i].s);
    }

    qsort(dat, dat_length, sizeof(DAT), compare_dat);

    printf("ソ\ート後\n");
    for(int i = 0; i < dat_length; i++) {
        printf("dat[%d]:%d,%s\n", i, dat[i].n, dat[i].s);
    }

    return 0;
}

int compare_dat(const void *n1, const void *n2)
{
    const DAT *dat1 = (DAT*)n1;
    const DAT *dat2 = (DAT*)n2;

    if(dat1->n > dat2->n) {
        return 1;
    } else if(dat1->n < dat2->n) {
        return -1;
    } else if(strcmp(dat1->s, dat2->s) > 0) {
        return 1;
    } else if(strcmp(dat1->s, dat2->s) < 0) {
        return -1;
    }
    return 0;
}