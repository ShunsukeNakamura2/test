#include<stdio.h>
#include<string.h>

/*
* �Z�ۑ�11
* �������n���Ƌt���ɂ��ĕԂ��֐���
* �쐬���Ă��������B
* (�������̕����o�C�g�������l������
* �K�v�͂���܂���)
*/

void rev_str(char *str);

int main(int argc, char *argv[])
{
    char str[] = "Shunsuke Nakamura test";

    printf("��:%s\n", str);
    rev_str(str);
    printf("�t:%s\n", str);

    return 0;
}

void rev_str(char *str)
{
    int length = strlen(str);
    for(int i = 0; i < length / 2; i++) {
        char tmp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = tmp;
    }
}