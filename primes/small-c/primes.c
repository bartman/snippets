#define B "+-----------------------------+"
#include <malloc.h>
int main(int a,char*v[]){
int r=atoi(v[1])+1,p=2,i,c=0,s=r*(log(r)+log(log(r)));
char*t=malloc(s);
puts(B"\n|	Count		Prime |\n"B);
while(++c<r){i=p;printf("|%11d%17d |\n",c,p);
while((i+=p)<s)t[i]=1;while(t[++p]);}
puts(B);}
