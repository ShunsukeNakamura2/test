#include<stdio.h>

/*
�Z�ۑ�5
char,short,int,long�̔z��ƃ|�C���^���`���A
���ꂼ��A�z��̓Y����1�i�߂��ꍇ��
�|�C���^���C���N�������g�����ꍇ�ɁA
�A�h���X�����o�C�g�i�ނ��\�����Ă��������B
*/
int main(int argc, char *argv[])
{
    char char_array[2];
    short short_array[2];
    int int_array[2];
    long long_array[2];
    char *char_pointer = char_array;
    short *short_pointer = short_array;
    int *int_pointer = int_array;
    long *long_pointer = long_array;

    printf("char  �z��  =%d, short  �z��  =%d, int  �z��  =%d, long  �z��  =%d\n" 
                                    , (int)&char_array[1] - (int)&char_array[0]
                                    , (int)&short_array[1] - (int)&short_array[0]
                                    , (int)&int_array[1] - (int)&int_array[0]
                                    , (int)&long_array[1] - (int)&long_array[0]);
    printf("char�|�C���^=%d, short�|�C���^=%d, int�|�C���^=%d, long�|�C���^=%d\n"
                                    , (int)(char_pointer + 1) - (int)char_pointer
                                    , (int)(short_pointer + 1) - (int)short_pointer
                                    , (int)(int_pointer + 1) - (int)int_pointer
                                    , (int)(long_pointer + 1) - (int)long_pointer);
    return 0;
}