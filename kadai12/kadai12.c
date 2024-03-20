#include<stdio.h>

/**
 * �Z�ۑ�12
 * �w�肳�ꂽ�̈��16�i�_���v���\������
 * �֐����쐬���Ă��������B
*/

void dump(const void *p, size_t s);

int main()
{
    unsigned int a = 4294967296 - 2;
    dump(&a, sizeof(a));
    return 0;
}

void dump(const void *p, size_t s)
{
    unsigned char *a = (unsigned char *)p;
    for(int i = 0; i < s; i++){
        printf("%02X ", a[i]);
    }
}