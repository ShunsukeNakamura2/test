#include<stdio.h>

/*
* �Z�ۑ�4
* int�̃|�C���^�ϐ�p��int�^�̕ϐ�i�̃A�h���X���Z�b�g���A
* i�̃A�h���X�ƒl�Ap�̃A�h���X�ƒl(=i�̃A�h���X)��
* �w����̒l(=i�̒l)��\�����Ă��������B
*/
int main(int argc, char *argv[])
{
    int i = 20;
    int *p = &i;
    printf("i:�A�h���X=%#x �l=%d\n", &i, i);
    printf("p:�A�h���X=%#x �l=%#x �w����̒l=%d\n", &p, p, *p);
    return 0;
}