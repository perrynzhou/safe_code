/*************************************************************************
   > File Name: bit.c
   > Author: perrynzhou
   > Mail: perrynzhou@gmail.com
   > Created Time: 2018年01月08日 星期一 00时27分06秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
/**
 *
    gcc 内置的bit operation
int __builtin_ffs (unsigned int x)
    返回x的最后一位1的是从后向前第几位，比如7368（1110011001000）返回4。
int __builtin_clz (unsigned int x)
    返回前导的0的个数。
int __builtin_ctz (unsigned int x)
    返回后面的0个个数，和__builtin_clz相对。
int __builtin_popcount (unsigned int x)
    返回二进制表示中1的个数。
int __builtin_parity (unsigned int x)
    返回x的奇偶校验位，也就是x的1的个数模2的结果。
 *
 *
 *
 *
 */
/*
 *设置整数*v的第n位清零
 *
 */
void cls_bit(int* v, int n)
{
    *v &= ~(1 << n);
}
/*
 *获取整数*v的第n位
 */
int get_bit(int* v, int n)
{
    return (*v) & (1 << n);
}
/*
 *设置整数*v的第n位为1
 */
void set_bit(int* v, int n)
{
    *v |= (1 << n);
}
/*
 *获取v的绝对值
 */
int abs(int v)
{
    int y = (v >> 31);
    return (v + y) ^ y;
}
/*
 *判断某个整数是否是2的幂
 */
bool is_power2(int a)
{
    return (a & (a - 1) == 0 && a != 0);
}
/*
 *交换2个整数
 */
void swap(int* a, int* b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *b = *a ^ *b;
}
int opposite(int x)
{
    return ~x + 1;
}
#ifdef BIT_TEST
#include <stdlib.h>
#include <time.h>
int main(void)
{
    int arr[10] = { 0 };
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        arr[i] = rand();
        fprintf(stdout, "------array[%d]=%d-------\n", i, arr[i]);
        fprintf(stdout, "..opposite(%d)=%d\n", arr[i], opposite(arr[i]));
        fprintf(stdout, "..get_bit(%d,%d)=%d\n", arr[i], 5, get_bit(&arr[i], 5));
        fprintf(stdout, "..is_power2(%d)=%d\n", arr[i], is_power2(arr[i]));
        cls_bit(&arr[i], 5);
    }
    return 0;
}
#endif
