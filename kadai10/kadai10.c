#include<stdio.h>

/*
�Z�ۑ�10
int�^��2�̕ϐ��̒l�����ւ���֐���
�쐬���Ă��������B
*/

void swap_int(int *a, int *b);

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;

    printf("����ւ��O\na=%d b=%d\n", a, b);
    swap_int(&a, &b);
    printf("����ւ���\na=%d b=%d\n", a, b);

    return 0;
}

void swap_int(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}