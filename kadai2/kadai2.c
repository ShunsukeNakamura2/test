#include<stdio.h>

/*
〇課題2
char,shot,int,long,float,double型の変数の
サイズ(バイト数)を表示してください。
またサイズは5桁表示とし、5桁に満たない場合
は0で詰めてください。
*/

int main(){
    printf("char=%05d short=%05d int=%05d ", sizeof(char), sizeof(short), sizeof(int));
    printf("long=%05d float=%05d double=%05d\n", sizeof(long), sizeof(float), sizeof(double));
    return 0;
}