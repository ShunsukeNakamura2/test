#include <stdio.h>
#include <string.h>

/*
�Z�ۑ�3
�C�ӂ̕������char�̔z��ɓǂݍ��݁A���̔z��̃T�C�Y(�o�C�g��)
�ƕ����񒷂�\�����Ă��������B
�܂����̕�����̍ŏ���1�����ƍŌ��1������\�����Ă��������B
*/
int main(int argc, char *argv[])
{
    char str[] = "ShunsukeNakamura";
    printf("�z��T�C�Y=%d ������=%d\n", sizeof(str), strlen(str));
    printf("1�����ڂ�%c�A�Ō�̕�����%c�ł��B\n", str[0], str[strlen(str) - 1]);
    return 0;
}