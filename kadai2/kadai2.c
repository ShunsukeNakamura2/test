#include<stdio.h>

/*
* char,short,int,long,float,double�^��
* �ϐ��̃T�C�Y(�o�C�g��)��\�����Ă��������B
* �܂��T�C�Y��5���\���Ƃ��A5���ɖ����Ȃ�
* �ꍇ��0�ŋl�߂Ă��������B
*/

int main(int argc, char *argv[])
{
    printf("char=%05d short=%05d int=%05d long=%05d float=%05d double=%05d\n", 
                                                sizeof(char), sizeof(short), 
                                                sizeof(int), sizeof(long), 
                                                sizeof(float), sizeof(double));
    return 0;
}