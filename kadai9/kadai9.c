#include<stdio.h>

/*
�Z�ۑ�9
�R�}���h���C���œn���ꂽ�p�����[�^��
�S�ĕ\������v���O�������쐬���Ă��������B
*/

int main(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++) {
        printf("%d : %s\n", i, argv[i]);
    }
    return 0;
}