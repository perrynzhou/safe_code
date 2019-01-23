/*************************************************************************
  > File Name: array_algorithms.cc
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 二  1/15 14:55:10 2019
 ************************************************************************/

#include <stdio.h>
namespace array_algorithms
{
int MaxSubArray(int *arr, int n)
{
  int curr = 0;
  int max = arr[0];
  int start = 0, end = 0;
  for (int i = 0; i < n; i++)
  {
    if (curr >= 0)
    {
      curr += arr[i];
    }
    else
    {
      curr = arr[i];
      start = i;
    }
    if (curr > max)
    {
      max = curr;
      end = i;
    }
  }
  fprintf(stdout, "max array = {");
  for (int i = start; i <= end; i++)
  {

    if (i == end)
    {
      fprintf(stdout, "%d", arr[i]);
      break;
    }
    fprintf(stdout, "%d,", arr[i]);
  }
  fprintf(stdout, "}\n");

  return max;
}
void MaxSubArrayTest()
{
  int arr[] = {1, -2, 3, 10, -4, 7, 2, -5};
  size_t size = sizeof(arr) / sizeof(arr[0]);

  fprintf(stdout, "origin array = {");
  for (int i = 0; i < size; i++)
  {

    if (i == size - 1)
    {
      fprintf(stdout, "%d", arr[i]);
      break;
    }
    fprintf(stdout, "%d,", arr[i]);
  }
  fprintf(stdout, "}\n");

  int max = MaxSubArray((int *)&arr, size);
  fprintf(stdout, "max = %d\n", max);
}
void QuickSort(int *arr, int l, int r)
{
  int i = l, j = r;
  int cur = arr[l];
  if (i < j)
  {
    while (i < j && arr[j] >= cur)
    {
      j--;
    }
    if (i < j)
    {
      arr[i++] = arr[j];
    }
    while (i < j && arr[i] < cur)
    {
      i++;
    }
    if (i < j)
    {
      arr[j--] = arr[i];
    }
    arr[i] = cur;
    QuickSort(arr, l, i - 1);
    QuickSort(arr, i + 1, r);
  }
}
void SumTwo(int *arr, int len, int sum)
{
  fprintf(stdout, "{");
  for (int i = 0; i < len; i++)
  {
    if (i == (len - 1))
    {
      fprintf(stdout, "%d}\n", arr[i]);
      break;
    }
    fprintf(stdout, "%d,", arr[i]);
  }
  int l = 0, r = len - 1;
  QuickSort(arr, l, r);
  while (l < r)
  {
    int curSum = arr[l] + arr[r];
    if (curSum == sum)
    {
      fprintf(stdout, "left =%d,right=%d,sum = %d\n", l, r, sum);
      break;
    }
    else
    {
      if (curSum > sum)
      {
        r--;
      }
      else
      {
        l++;
      }
    }
  }
}
} // namespace array_algorithms
int main(void)
{
  array_algorithms::MaxSubArrayTest();
  return 0;
}