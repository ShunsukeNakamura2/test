#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
* 〇課題13
* 構造体DAT_tの配列をソートし表示してください。
* ソート順は、nの値で昇順、nが同一の値の場合は
* sの文字コードで昇順とします。
* ソート処理にはC標準関数qsort()を使ってください。
*/
typedef struct DAT_t {
    int n;
    const char *s;
}DAT;

static int compare_dat(const void *dat1, const void *dat2);

int main(int argc, char *argv[])
{
    DAT dat[] = {
        {9, "A"},
        {2, "A"},
        {5, "AB"},
        {5, "AA"},
    }; 

    int dat_length = sizeof(dat) / sizeof(DAT);

    printf("ソート前\n");
    for(int i = 0; i < dat_length; i++) {
        printf("dat[%d]:%d,%s\n", i, dat[i].n, dat[i].s);
    }

    qsort(dat, dat_length, sizeof(DAT), compare_dat);

    printf("ソート後\n");
    for(int i = 0; i < dat_length; i++) {
        printf("dat[%d]:%d,%s\n", i, dat[i].n, dat[i].s);
    }

    return 0;
}

/**
 * @brief 構造体DAT_t比較関数
 * 
 * @note 以下の条件で大小を比較する
 *       1.メンバn(int) が大きい方が大
 *       2.メンバnが等しい時、メンバs(char*)の文字コードで比較し大きい方が大
 *       3.メンバn, s が両方等しい時、両引数は等しい
 * 
 * @param[in] n1 比較対象1のDAT_t構造体
 * @param[in] n2 比較対象2のDAT_t構造体
 * 
 * @retval > 0  n1が大
 * @retval < 0  n2が大
 * @retval 0    n1とn2が等しい
*/
static int compare_dat(const void *n1, const void *n2)
{
    const DAT *dat1 = (DAT*)n1;
    const DAT *dat2 = (DAT*)n2;

    if(dat1->n != dat2->n) {
        return dat1->n - dat2->n;
    }
    return strcmp(dat1->s, dat2->s);
}
