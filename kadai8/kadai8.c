#include<stdio.h>

/*
* �Z�ۑ�8
* �v���O�������N�����Ă���Ăяo�����т�
* 1�C���N�������g�����l��Ԃ��֐����쐬���Ă��������B
* �����l��0�Ƃ��܂��B
* (int�̍ő�l�𒴂����ꍇ�̓�����l������K�v�͂���܂���)
*/

static int static_counter();

int main(int argc, char *argv[])
{
    for(int i = 0; i < 10; i++) {
        int count = static_counter();
        printf("%d\n", count);
    }
    return 0;
}

/**
 * @brief �Ăяo����邽�т�1�C���N�������g���ꂽ�l��Ԃ�
 * 
 * @return �Ăяo���ꂽ��
*/
static int static_counter()
{
    static int count = 0;
    return ++count;
}
