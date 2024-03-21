#include<stdio.h>

/*
* char,short,int,long,float,double型の
* 変数のサイズ(バイト数)を表示してください。
* またサイズは5桁表示とし、5桁に満たない
* 場合は0で詰めてください。
*/

int main(int argc, char *argv[])
{
    printf("char=%05d short=%05d int=%05d long=%05d float=%05d double=%05d\n", 
                                                sizeof(char), sizeof(short), 
                                                sizeof(int), sizeof(long), 
                                                sizeof(float), sizeof(double));
    return 0;
}