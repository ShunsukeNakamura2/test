#include<stdio.h>

/*
�Z�ۑ�7
�����Ɏw�肵�������ċN�Ăяo������֐����쐬���A
�֐����ŉ������̃A�h���X��\�����Ă��������B
(�X�^�b�N�I�[�o�[�t���[��������鏈������������K�v�͂���܂���)
*/

void reent(int n);

int main(int argc, char *argv[])
{
    int n;
    printf("�ċA�Ăяo�����鐔���w�肵�Ă��������F");
    scanf_s("%d", &n);

    if(n > 0) {
        reent(n);
    }
    
    return 0;
}

void reent(int n)
{
    printf("n=%d �A�h���X=%#x\n", n, &n);
    if(n > 0) {
        reent(n - 1);
    }
}