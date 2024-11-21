#include <stdio.h>
int main(){char a[64];char b[]={0x2f,0x66,0x6c,0x61,0x67};FILE *c=fopen(b,"r");fgets(a,64,c);printf(a+1);}