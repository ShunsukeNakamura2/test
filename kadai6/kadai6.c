#include<stdio.h>
#include<stddef.h>

/*
�Z�ۑ�6
�\���́uSAMPLE_t�v�̃T�C�Y�A�e�����o�̃T�C�Y�̍��v�A
�e�����o�̐擪����̃I�t�Z�b�g�ƃT�C�Y��\�����Ă��������B
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
    gcc�R���p�C�����̕��������΍�ŁA���̑O��\�����Ă���
    https://www.novell.com/ja-jp/support/kb/tids/jp/00006500_/00006574.htm
    */
    printf("�\\���̂̃T�C�Y=%d �����o�̍��v�T�C�Y=%d\n", sizeof(sample), sizeof(sample.m1) + sizeof(sample.m2)
                                                                     + sizeof(sample.m3) + sizeof(sample.m4)
                                                                     + sizeof(sample.m5) + sizeof(sample.m6));
    printf("char  m1�I�t�Z�b�g=%d �T�C�Y=%d\n", offsetof(struct SAMPLE_t, m1), sizeof(sample.m1));
    printf("short m2�I�t�Z�b�g=%d �T�C�Y=%d\n", offsetof(struct SAMPLE_t, m2), sizeof(sample.m2));
    printf("char  m3�I�t�Z�b�g=%d �T�C�Y=%d\n", offsetof(struct SAMPLE_t, m3), sizeof(sample.m3));
    printf("long  m4�I�t�Z�b�g=%d �T�C�Y=%d\n", offsetof(struct SAMPLE_t, m4), sizeof(sample.m4));
    printf("char  m5�I�t�Z�b�g=%d �T�C�Y=%d\n", offsetof(struct SAMPLE_t, m5), sizeof(sample.m5));
    printf("void *m6�I�t�Z�b�g=%d �T�C�Y=%d\n", offsetof(struct SAMPLE_t, m6), sizeof(sample.m6));
    return 0;
}