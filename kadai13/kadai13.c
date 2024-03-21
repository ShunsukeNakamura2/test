#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
�Z�ۑ�13
�\����DAT_t�̔z����\�[�g���\�����Ă��������B
�\�[�g���́An�̒l�ŏ����An������̒l�̏ꍇ��
s�̕����R�[�h�ŏ����Ƃ��܂��B
�\�[�g�����ɂ�C�W���֐�qsort()���g���Ă��������B
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
    gcc�R���p�C�����̕��������΍�Ń\�̌��\�����Ă���
    */
    printf("�\\�[�g�O\n");
    for(int i = 0; i < dat_length; i++) {
        printf("dat[%d]:%d,%s\n", i, dat[i].n, dat[i].s);
    }

    qsort(dat, dat_length, sizeof(DAT), compare_dat);

    printf("�\\�[�g��\n");
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