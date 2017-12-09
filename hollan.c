/*************************************************************************
	> File Name: henan_goqi.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月09日 星期六 13时06分52秒
 ************************************************************************/

#include<stdio.h>
static void swap(int *a,int *b)
{
    *a= *a^*b;
    *b= *a^*b;
    *a= *a^*b;
}
static void sort(int *arr,int size)
{
    int begin,cur,end= size-1;
    begin= cur = 0;
    while(begin <= end)
    {
        if(arr[cur]==2)
        {
            swap(&arr[end--],&arr[cur]);
        }else if(arr[cur] == 1)
        {
            cur++;
        }else{
            if(begin == cur){
                begin++;
                cur++;
            }else{
                swap(&arr[cur],&arr[begin++]);
            }
        }
    }
}
int main()
{
    int v[11] = {0,1,0,1,0,0,2,2,1,0,1};
    sort((int *)&v,11);
    for(int i=0;i<11;i++)
    {
        fprintf(stdout,"v[%d]=%d\n",i,v[i]);
    }
    return 0;
}
